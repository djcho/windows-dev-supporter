#include "stdafx.h"
#include <list>
#include "ISignHttpWeb.h"
#include "SplitURL.h"
#include "StringUtil.h"

#include <log4cplus\logger.h>
#include <log4cplus\loggingmacros.h>
#include "..\IsignLogger\IsignLogger.h"

using namespace eewin_util;

#define		SAFETY_INTERNET_CLOSE_HANDLE(handle)\
			if(handle)\
				InternetCloseHandle(handle);

#define		SAFETY_MEMORY_FREE(p)\
			if(p)	\
			{		\
				free(p);\
				p = NULL;\
			}

HttpWebRequest::HttpWebRequest()
{
	this->url = _T("");
	this->port = 80;
	this->acceptLanguage = LANGUAGE_KO_KR;
}

HttpWebRequest::HttpWebRequest(IN const tstring& url, IN const unsigned int port)
{
	this->url = url;
	this->port = port;
	this->acceptLanguage = LANGUAGE_KO_KR;
}

HttpWebRequest::HttpWebRequest(IN const tstring& url, IN const unsigned int port, IN const tstring requestString)
{
	this->url = url;
	this->port = port;
	this->acceptLanguage = LANGUAGE_KO_KR;

	string tmpRequestString = StringUtil::tstringToAnsi(requestString);
	setRequestData(url, port, (unsigned char*)tmpRequestString.c_str(), static_cast<const int>(tmpRequestString.length()));
}

HttpWebRequest::HttpWebRequest(IN const tstring& url, IN const unsigned int port, IN const unsigned char * requestData, IN const int requestDataSize)
{
	this->url = url;
	this->port = port;
	this->acceptLanguage = LANGUAGE_KO_KR;

	setRequestData(url, port, requestData, requestDataSize);
}

void HttpWebRequest::setRequestData(IN const tstring& url, IN const unsigned int port)
{
	this->url = url;
	this->port = port;
}

void HttpWebRequest::setRequestData(IN const tstring& url, IN const unsigned int port, IN const tstring requestString)
{
	this->url = url;
	this->port = port;
	string tmpRequestString = StringUtil::tstringToAnsi(requestString);
	
	return setRequestData(url, port, (unsigned char*)tmpRequestString.c_str(), static_cast<int>(tmpRequestString.length()));
}

void HttpWebRequest::setRequestData(IN const tstring& url, IN const unsigned int port, IN const unsigned char * requestData, IN const int requestDataSize)
{
	this->url = url;
	this->port = port;
	if (this->requestData)
	{
		delete[] this->requestData;
	}

	this->requestDataLen = requestDataSize;
	if (this->requestDataLen > 0)
	{
		this->requestData = new unsigned char[this->requestDataLen];
		memcpy(this->requestData, requestData, requestDataSize);
	}
}

void HttpWebRequest::setSpnegoToken(IN const tstring& spnegoToken)
{
	this->spnegoToken = spnegoToken;
}

void HttpWebRequest::request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, IN const std::map<tstring, tstring> &requestHeaderList, OUT std::list<tstring> &responseHeaderList)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("IsignAuthenticator"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	HINTERNET sessionHandle = NULL;
	HINTERNET connectionHandle = NULL;
	HINTERNET requestHandle = NULL;

	try
	{
		if (url.empty())
		{
			LOG4CPLUS_TRACE_END(logger);
			throw HttpWebException(RETURN_HTTP_WEB_REQUEST_EMPTY_URL);
		}

		tstring header;
		header += _T("User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n");
		auto it = requestHeaderList.begin();
		while (it != requestHeaderList.end())
		{
			if (!it->first.empty() && !it->second.empty())
			{
				header += it->first;
				header += _T(": ");
				header += it->second;
				header += _T("\r\n");
			}
			it++;
		}
		header += _T("Accept-Language: ");
		header += this->acceptLanguage;
		header += _T("\r\n");
		if (contentType == HTTP_CONTENT_TYPE_JSON)
			header += _T("Content-type: application/json\r\n");
		else if (contentType == HTTP_CONTENT_TYPE_URL_ENCODED)
			header += _T("Content-type: application/x-www-form-urlencoded\r\n");
		else if (contentType == HTTP_CONTENT_TYPE_TEXT)
			header += _T("Content-type: text/plain\r\n");
		header += _T("Content-Length: ");
		header += to_tstring(this->requestDataLen).c_str();
		header += _T("\r\n");
		header += _T("\r\n\r\n");

		LOG4CPLUS_DEBUG_FMT(logger, _T("url : %s"), url.c_str());
		LOG4CPLUS_DEBUG_FMT(logger, _T("header : %s"), header.c_str());

		CSplitURL splitUrl(url);
		int retWin32Error = ERROR_SUCCESS;

		sessionHandle = InternetOpen(_T("Request"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (!sessionHandle)
		{
			retWin32Error = GetLastError();
			LOG4CPLUS_DEBUG_FMT(logger, _T("InternetOpen failed, resultCode : %d"), retWin32Error);
			LOG4CPLUS_TRACE_END(logger);
			throw HttpWebException(RETURN_HTTP_WEB_REQUEST_OPEN_INTERNET_FAILED, retWin32Error);
		}

		connectionHandle = InternetConnect(sessionHandle, splitUrl.GetHostName(), this->port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
		if (!connectionHandle)
		{
			retWin32Error = GetLastError();
			LOG4CPLUS_DEBUG_FMT(logger, _T("InternetConnect failed, resultCode : %d"), retWin32Error);
			LOG4CPLUS_TRACE_END(logger);
			throw HttpWebException(RETURN_HTTP_WEB_REQUEST_CONNECT_INTERNET_FAILED, retWin32Error);
		}

		DWORD internetOption = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_COOKIES;
		if (splitUrl.GetScheme() == INTERNET_SCHEME_HTTPS)
			internetOption |= INTERNET_FLAG_SECURE;

		tstring extraInfo(splitUrl.GetURLPath());
		extraInfo.append(splitUrl.GetExtraInfo());

		requestHandle = HttpOpenRequest(connectionHandle,
			requestMethod == HTTP_WEB_REQUEST_METHOD_POST ? _T("POST") : NULL,
			extraInfo.c_str(),
			_T("HTTP/1.1"),
			NULL,
			NULL,
			internetOption,
			0);
		if (!requestHandle)
		{
			retWin32Error = GetLastError();
			LOG4CPLUS_DEBUG_FMT(logger, _T("HttpOpenRequest failed, resultCode : %d"), retWin32Error);
			LOG4CPLUS_TRACE_END(logger);
			throw HttpWebException(RETURN_HTTP_WEB_REQUEST_OPEN_HTTP_FAILED, retWin32Error);
		}

		// 사설 SSL 인증서의 경우 12045 에러 발생
		// 12045 에러 무시 옵션 설정
		if (splitUrl.GetScheme() == INTERNET_SCHEME_HTTPS)
		{
			internetOption = SECURITY_FLAG_IGNORE_UNKNOWN_CA
				| SECURITY_FLAG_IGNORE_REVOCATION
				| SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP
				| SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS
				| SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
				| SECURITY_FLAG_IGNORE_CERT_CN_INVALID;


			if (!InternetSetOption(requestHandle, INTERNET_OPTION_SECURITY_FLAGS, &internetOption, sizeof(internetOption)))
			{
				retWin32Error = GetLastError();
				LOG4CPLUS_DEBUG_FMT(logger, _T("InternetSetOption failed, resultCode : %d"), retWin32Error);
				LOG4CPLUS_TRACE_END(logger);
				throw HttpWebException(RETURN_HTTP_WEB_REQUEST_SET_OPTION_FAILED, retWin32Error);
			}
		}

		internetOption = HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD;
		if (!HttpAddRequestHeaders(requestHandle, header.c_str(), (unsigned long)header.size(), internetOption))
		{
			retWin32Error = GetLastError();
			LOG4CPLUS_DEBUG_FMT(logger, _T("HttpAddRequestHeaders failed, resultCode : %d"), retWin32Error);
			LOG4CPLUS_TRACE_END(logger);
			throw HttpWebException(RETURN_HTTP_WEB_REQUEST_ADD_HTTP_HEADER_FAILED, retWin32Error);
		}

		if (this->spnegoToken.length() > 0)
		{
			tstring authorizationHeader = _T("Authorization: Negotiate ") + spnegoToken;
			DWORD internetOption = HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD;

			if (!HttpAddRequestHeaders(requestHandle, authorizationHeader.c_str(), (unsigned long)authorizationHeader.size(), internetOption))
			{
				retWin32Error = GetLastError();
				LOG4CPLUS_DEBUG_FMT(logger, _T("HttpAddRequestHeaders failed, resultCode : %d"), retWin32Error);
				LOG4CPLUS_TRACE_END(logger);
				throw HttpWebException(RETURN_HTTP_WEB_REQUEST_ADD_HTTP_HEADER_FAILED, retWin32Error);
			}
		}

		if (!HttpSendRequestA(requestHandle, NULL, NULL, (void*)this->requestData, this->requestDataLen))
		{
			retWin32Error = GetLastError();
			/*
			char temp2[4096] = { 0, };
			memcpy(temp2, this->requestData, this->requestDataLen);
			LOG4CPLUS_DEBUG_FMT(logger, _T("3. request requestStringLen : %d"), this->requestDataLen);
			LOG4CPLUS_DEBUG_FMT(logger, _T("3. request requestString: %s"), (StringUtil::ansiToTstring(temp2)).c_str());
			*/
			LOG4CPLUS_DEBUG_FMT(logger, _T("HttpSendRequest failed, resultCode : %d"), retWin32Error);
			LOG4CPLUS_TRACE_END(logger);
			throw HttpWebException(RETURN_HTTP_WEB_REQUEST_SEND_REQUEST_FAILED, retWin32Error);
		}

		// get status code
		char statusCodeBuffer[256] = { 0 };
		DWORD statusCodeBufferLen = sizeof(statusCodeBuffer);

		if (HttpQueryInfoA(requestHandle, HTTP_QUERY_STATUS_CODE, &statusCodeBuffer, &statusCodeBufferLen, NULL) != TRUE)
		{
			retWin32Error = GetLastError();
			LOG4CPLUS_DEBUG_FMT(logger, _T("HttpQueryInfo failed, resultCode : %d"), retWin32Error);
			LOG4CPLUS_TRACE_END(logger);
			throw HttpWebException(RETURN_HTTP_WEB_GET_STATUS_CODE_FAILED, retWin32Error);
		}

		this->httpWebResponse.setStatusCode(StringUtil::ansiToTstring(statusCodeBuffer));
		if (std::stoi(statusCodeBuffer) != HTTP_STATUS_OK)
		{
			//LOG4CPLUS_DEBUG_FMT(logger, _T("ERROR, http status code : %s"), statusCodeBuffer);
			LOG4CPLUS_DEBUG_FMT(logger, _T("Http status code : %s"), (StringUtil::ansiToTstring(statusCodeBuffer)).c_str());
			LOG4CPLUS_TRACE_END(logger);
			//공통묘듈이지만 APC에서 성공이 아닌 경우 200을 내려주지 않기 때문에 예외처리를 지운다.
			//throw HttpWebException(RETURN_HTTP_WEB_INVALID_STATUS_CODE);
		}

		// get custom header list
		auto iter = responseHeaderList.begin();
		for (auto iter = responseHeaderList.begin(); iter != responseHeaderList.end(); iter++)
		{
			if ((*iter).empty())
				continue;

			char customBuffer[8192];
			strcpy(customBuffer, StringUtil::tstringToAnsi(*iter).c_str());
			DWORD customBufferLen = sizeof(customBuffer);

			if (HttpQueryInfoA(requestHandle, HTTP_QUERY_CUSTOM, customBuffer, &customBufferLen, NULL) != TRUE)
			{
				retWin32Error = GetLastError();
				LOG4CPLUS_DEBUG_FMT(logger, _T("HttpQueryInfo failed, resultCode : %d"), retWin32Error);
				LOG4CPLUS_TRACE_END(logger);
			}
			if (strcmp(customBuffer, StringUtil::tstringToAnsi(*iter).c_str()) == 0)
			{
				this->httpWebResponse.setResponseHeader(L"", L"");
			}
			this->httpWebResponse.setResponseHeader(*iter, StringUtil::ansiToTstring(customBuffer));
		}

		// response
		unsigned long availableReadBytes = 0;
		unsigned long totalReadBytes = 0;
		unsigned char *readBuffer = NULL;

		while (InternetQueryDataAvailable(requestHandle, &availableReadBytes, 0, 0)
			&& availableReadBytes > 0)
		{
			readBuffer = (unsigned char*)realloc(readBuffer, sizeof(unsigned char) * (totalReadBytes + availableReadBytes));
			if (!readBuffer)
				throw HttpWebException(RETURN_HTTP_WEB_MEM_ALLOC_FAILED);

			unsigned long readBytes = 0;
			unsigned long remainBytes = availableReadBytes;

			while (remainBytes)
			{
				if (!InternetReadFile(requestHandle, readBuffer + totalReadBytes, remainBytes, &readBytes))
				{
					retWin32Error = GetLastError();
					LOG4CPLUS_DEBUG_FMT(logger, _T("InternetReadFile failed, resultCode : %d"), retWin32Error);
					SAFETY_MEMORY_FREE(readBuffer);
					throw HttpWebException(RETURN_HTTP_WEB_READ_FILE_FAILED, retWin32Error);
				}
				remainBytes -= readBytes;
				totalReadBytes += readBytes;
			}
			availableReadBytes = 0; //reset
		}

		LOG4CPLUS_DEBUG_FMT(logger, _T("totalReadBytes : %d"), totalReadBytes);
		this->httpWebResponse.setResponseData(readBuffer, totalReadBytes);
#if 1 // ocsp response 디버그 로그
		{
			char contentType[256] = { 0 };
			DWORD contentTypeLen = sizeof(contentType);

			if (HttpQueryInfoA(requestHandle, HTTP_QUERY_CONTENT_TYPE, &contentType, &contentTypeLen, NULL) == TRUE)
			{
				LOG4CPLUS_DEBUG_FMT(logger, _T("contentType : %s"), StringUtil::ansiToTstring(contentType).c_str());

				if (strcmp(contentType, "application/ocsp-response") == 0)
				{
					char *logBuffer = (char*)calloc(1, (totalReadBytes * 2) + 1);

					for (unsigned int i = 0; i < totalReadBytes; i++)
					{
						sprintf(logBuffer, "%s%02x", logBuffer, readBuffer[i]);
					}
					LOG4CPLUS_DEBUG_FMT(logger, _T("responseData(hex) : %s"), StringUtil::ansiToTstring(logBuffer).c_str());
				}
			}
		}
#endif
		SAFETY_MEMORY_FREE(readBuffer);
	}
	catch (HttpWebException& e)
	{
		SAFETY_INTERNET_CLOSE_HANDLE(sessionHandle);
		SAFETY_INTERNET_CLOSE_HANDLE(connectionHandle);
		SAFETY_INTERNET_CLOSE_HANDLE(requestHandle);

		LOG4CPLUS_DEBUG_FMT(logger, _T("Occur std::exception, errMessage : %s"), e.getErrorMessage().c_str());
		LOG4CPLUS_TRACE_END(logger);
		throw;
		// write log  , e.what();
	}

	SAFETY_INTERNET_CLOSE_HANDLE(sessionHandle);
	SAFETY_INTERNET_CLOSE_HANDLE(connectionHandle);
	SAFETY_INTERNET_CLOSE_HANDLE(requestHandle);

	LOG4CPLUS_TRACE_END(logger);
}

void HttpWebRequest::request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, IN const std::map<tstring, tstring> &requestHeaderList)
{
	std::list<tstring> responseHeaderList;
	this->request(requestMethod, contentType, requestHeaderList, responseHeaderList);
}

void HttpWebRequest::request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, OUT std::list<tstring> &responseHeaderList)
{
	std::map<tstring, tstring> requestHeaderList;
	this->request(requestMethod, contentType, requestHeaderList, responseHeaderList);
}

void HttpWebRequest::request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType)
{
	std::map<tstring, tstring> requestHeaderList;
	std::list<tstring> responseHeaderList;
	this->request(requestMethod, contentType, requestHeaderList, responseHeaderList);
}

void HttpWebRequest::setLanguage(IN const tstring & language)
{
	this->acceptLanguage = language;
}

HttpWebResponse HttpWebRequest::getResponse()
{
	return this->httpWebResponse;
}

HttpWebRequest::~HttpWebRequest()
{
	if (this->requestData)
		delete[] this->requestData;
}