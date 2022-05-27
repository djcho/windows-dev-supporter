#include "stdafx.h"
#include "ISignHttpWeb.h"
#include "..\common\StringUtil.h"

#include <Windows.h>
using namespace eewin_util;

HttpWebException::HttpWebException()
{
	this->errorCode = 0;
}


HttpWebException::~HttpWebException()
{
}


HttpWebException::HttpWebException(IN const RETURN_HTTP_WEB_REQUEST errorCode, IN const unsigned int internalErrorCode )
{
	this->errorCode = errorCode;
	setErrorMessageByErrorCode();
	this->innerException.setErrorCode(internalErrorCode);
}

void HttpWebException::setErrorMessageByErrorCode()
{
	switch (this->errorCode)
	{

	case RETURN_HTTP_WEB_REQUEST_EMPTY_URL:
		this->errorMessage = _T("The server url is not set");
		break;
	case RETURN_HTTP_WEB_REQUEST_OPEN_INTERNET_FAILED:
		this->errorMessage = _T("Failed to open internet");
		break;
	case RETURN_HTTP_WEB_REQUEST_CONNECT_INTERNET_FAILED:
		this->errorMessage = _T("Failed to connect internet");
		break;
	case RETURN_HTTP_WEB_REQUEST_OPEN_HTTP_FAILED:
		this->errorMessage = _T("Failed to open http");
		break;
	case RETURN_HTTP_WEB_REQUEST_SET_OPTION_FAILED:
		this->errorMessage = _T("Failed to set internet option");
		break;
	case RETURN_HTTP_WEB_REQUEST_ADD_HTTP_HEADER_FAILED:
		this->errorMessage = _T("Failed to add http header");
		break;
	case RETURN_HTTP_WEB_REQUEST_SEND_REQUEST_FAILED:
		this->errorMessage = _T("Failed to send request");
		break;
	case RETURN_HTTP_WEB_SET_RESPONSE_FAILED:
		this->errorMessage = _T("Failed to set respons data");
		break;
	case 0:
	default:
		this->errorMessage = _T("");
		break;
	}

	this->whatMessage = StringUtil::unicodeToAnsi(this->errorMessage);
}

tstring HttpWebException::getErrorMessage()
{
	return this->errorMessage;
}


int HttpWebException::getErrorCode()
{
	return this->errorCode;
}

HttpWebInnerException HttpWebException::getInnerException()
{
	return this->innerException;
}


/*
- HttpWebInnerException Class -
*/


HttpWebInnerException::HttpWebInnerException()
{
	errorCode = ERROR_SUCCESS;
}


HttpWebInnerException::HttpWebInnerException(IN const int errorCode)
{
	this->errorCode = errorCode;
	setErrorMessageByErrorCode();

}


void HttpWebInnerException::setErrorMessageByErrorCode()
{

	if (this->errorCode == 0)
		this->errorMessage = _T("");

	TCHAR * messageBuffer = NULL;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, this->errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR *)&messageBuffer, 0, NULL);

	if (!messageBuffer)
	{
		this->errorMessage = getWinInetErrorCodeToString(this->errorCode);
	}
	else
	{
		tstring message(messageBuffer, size);
		this->errorMessage = message;
	}

	this->whatMessage = StringUtil::unicodeToAnsi(this->errorMessage);

	LocalFree(messageBuffer);
}

tstring HttpWebInnerException::getWinInetErrorCodeToString(IN const int errorCode)
{
	switch (errorCode) {
	case 12001:
		return tstring(_T("Out of handles"));
	case 12002:
		return tstring(_T("Timeout"));
	case 12004:
		return tstring(_T("Internal Error"));
	case 12005:
		return tstring(_T("Invalid URL"));
	case 12006:
		return tstring(_T("Unrecognized Scheme : Please check the URL name in 'TVFilmAlerter.ini'."));
	case 12007:
		return tstring(_T("Service Name Not Resolved"));
	case 12008:
		return tstring(_T("Protocol Not Found"));
	case 12013:
		return tstring(_T("Incorrect User Name"));
	case 12014:
		return tstring(_T("Incorrect Password"));
	case 12015:
		return tstring(_T("Login Failure"));
	case 12016:
		return tstring(_T("Invalid Operation"));
	case 12017:
		return tstring(_T("Operation Canceled"));
	case 12020:
		return tstring(_T("Not Proxy Request"));
	case 12023:
		return tstring(_T("No Direct Access"));
	case 12026:
		return tstring(_T("Request Pending"));
	case 12027:
		return tstring(_T("Incorrect Format"));
	case 12028:
		return tstring(_T("Item not found"));
	case 12029:
		return tstring(_T("Cannot connect"));
	case 12030:
		return tstring(_T("Connection Aborted"));
	case 12031:
		return tstring(_T("Connection Reset"));
	case 12033:
		return tstring(_T("Invalid Proxy Request"));
	case 12034:
		return tstring(_T("Need UI"));
	case 12035:
		return tstring(_T("Sec Cert Date Invalid"));
	case 12038:
		return tstring(_T("Sec Cert CN Invalid"));
	case 12044:
		return tstring(_T("Client Auth Cert Needed"));
	case 12045:
		return tstring(_T("Invalid CA Cert"));
	case 12046:
		return tstring(_T("Client Auth Not Setup"));
	case 12150:
		return tstring(_T("HTTP Header Not Found"));
	case 12152:
		return tstring(_T("Invalid HTTP Server Result"));
	case 12153:
		return tstring(_T("Invalid HTTP Header"));
	case 120154:
		return tstring(_T("Invalid Query Request"));
	case 120156:
		return tstring(_T("Redirect Failed"));
	case 120159:
		return tstring(_T("TCP/IP not installed"));
	default:
		return tstring(_T("UnKnown"));
	}
}


void HttpWebInnerException::setErrorCode(IN const int errorCode)
{
	this->errorCode = errorCode;
	setErrorMessageByErrorCode();
}


int HttpWebInnerException::getErrorCode()
{
	return this->errorCode;
}


tstring HttpWebInnerException::getErrorMessage()
{
	return this->errorMessage;
}


HttpWebInnerException::~HttpWebInnerException()
{

}