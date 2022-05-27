#include "stdafx.h"
#include "ISignHttpWeb.h"

#include "StringUtil.h"

using namespace eewin_util;

HttpWebResponse::HttpWebResponse()
{
	this->responseData = NULL;
}


HttpWebResponse::~HttpWebResponse()
{
}

tstring HttpWebResponse::getResponseString()
{
	wstring tmpResponseString = L"";
	if (this->responseData != NULL)
	{
		tmpResponseString = StringUtil::utf8Decode((char*)this->responseData);
	}

	return StringUtil::unicodeToTstring(tmpResponseString);
}

bool HttpWebResponse::getResponseData(OUT unsigned char *responseDataBuf, IN OUT unsigned int *responseDataBufSize)
{
	if (this->responseData == NULL )
	{
		*responseDataBufSize = 0;
		return false;
	}

	if (this->responseDataLen > *responseDataBufSize || responseDataBuf == NULL)
	{
		*responseDataBufSize = this->responseDataLen;
		return false;
	}

	*responseDataBufSize = this->responseDataLen;
	memcpy(responseDataBuf, this->responseData, this->responseDataLen);
	return true;
}

void HttpWebResponse::setResponseData(IN const unsigned char *responseData, IN const unsigned int responseDataSize)
{
	if (this->responseData)
	{
		delete[] this->responseData;
	}

	this->responseDataLen = responseDataSize;
	this->responseData = new unsigned char[this->responseDataLen];
	memcpy(this->responseData, responseData, this->responseDataLen);
}
tstring HttpWebResponse::getStatusCode()
{
	return this->statusCode;
}

void HttpWebResponse::setStatusCode(IN const tstring &statusCode)
{
	this->statusCode = statusCode;
}

std::map<tstring, tstring> HttpWebResponse::getResponseHeader()
{
	return this->responseHeader;
}

void HttpWebResponse::setResponseHeader(IN const tstring & customKey, IN const tstring & customValue)
{
	this->responseHeader.insert({ customKey, customValue } );
}

HttpWebResponse & HttpWebResponse::operator=(IN const HttpWebResponse &src)
{
	this->setResponseData(src.responseData, src.responseDataLen);
	return *this;
}
