#ifndef _EE_WIN_ISIGNHTTPWEB_ISIGNHTTPWEB_H_
#define _EE_WIN_ISIGNHTTPWEB_ISIGNHTTPWEB_H_

#ifndef ISIGNHTTPWEB_API
#ifdef ISIGNHTTPWEB_EXPORTS
#define ISIGNHTTPWEB_API __declspec(dllexport)
#else
#define ISIGNHTTPWEB_API __declspec(dllimport)
#endif
#endif

#include <map>
#include <list>

#define IN
#define OUT

#define LANGUAGE_KO_KR _T("ko-KR")
#define LANGUAGE_EN_US _T("en-US")

/*
- WebResponse Class -
*/


class ISIGNHTTPWEB_API HttpWebResponse
{
public:
	explicit HttpWebResponse();
	~HttpWebResponse();

	tstring getResponseString();
	bool getResponseData(OUT unsigned char *responseDataBuf, IN OUT unsigned int *responseDataBufSize);
	//void setResponseString(IN const tstring& responseString);
	void setResponseData(IN const unsigned char *responseData, IN const unsigned int responseDataSize );
	tstring getStatusCode();
	void setStatusCode(IN const tstring & statusCode);
	std::map<tstring, tstring> getResponseHeader();
	void setResponseHeader(IN const tstring & customKey, IN const tstring & customValue);
	HttpWebResponse & operator = (IN const HttpWebResponse &src);
private:
	tstring statusCode;
	std::map<tstring, tstring> responseHeader;
	unsigned char *responseData = NULL;
	unsigned int responseDataLen = 0;
};


/*
 - WebRequest Class -
*/


enum HTTP_WEB_REQUEST_METHOD
{
	HTTP_WEB_REQUEST_METHOD_GET,
	HTTP_WEB_REQUEST_METHOD_POST
};

enum HTTP_CONTENT_TYPE
{
	HTTP_CONTENT_TYPE_JSON,
	HTTP_CONTENT_TYPE_URL_ENCODED,
	HTTP_CONTENT_TYPE_TEXT,
	HTTP_CONTENT_TYPE_NO_TYPE
};

enum RETURN_HTTP_WEB_REQUEST
{
	RETURN_HTTP_WEB_REQUEST_SUCCESS = 0,
	RETURN_HTTP_WEB_REQUEST_EMPTY_URL = -12000,
	RETURN_HTTP_WEB_REQUEST_OPEN_INTERNET_FAILED,
	RETURN_HTTP_WEB_REQUEST_CONNECT_INTERNET_FAILED,
	RETURN_HTTP_WEB_REQUEST_OPEN_HTTP_FAILED,
	RETURN_HTTP_WEB_REQUEST_SET_OPTION_FAILED,
	RETURN_HTTP_WEB_REQUEST_ADD_HTTP_HEADER_FAILED,
	RETURN_HTTP_WEB_REQUEST_SEND_REQUEST_FAILED,
	RETURN_HTTP_WEB_REQUEST_SET_CUSTOM_HEADER_FAILED,
	RETURN_HTTP_WEB_SET_RESPONSE_FAILED,
	RETURN_HTTP_WEB_SET_STD_RUNTIME_ERROR,
	RETURN_HTTP_WEB_GET_STATUS_CODE_FAILED,
	RETURN_HTTP_WEB_GET_CUSTOM_HEADER_FAILED,
	RETURN_HTTP_WEB_INVALID_STATUS_CODE,
	RETURN_HTTP_WEB_MEM_ALLOC_FAILED,
	RETURN_HTTP_WEB_READ_FILE_FAILED,
};


class ISIGNHTTPWEB_API HttpWebRequest
{
public:
	explicit HttpWebRequest();

	/// <summary>�������� Http ����� �����ϴ� ��ü�� ������</summary> 
	/// <param name = 'commandCode'>ISign ������ OpenAPI Ŀ�ǵ�</param>
	/// <param name = 'url'>ISign ������ URL �ּ�</param>
	/// <param name = 'port'>ISign ������ ��Ʈ ��ȣ</param>
	explicit HttpWebRequest(IN const tstring& url, IN const unsigned int port);

	/// <summary>�������� Http ����� �����ϴ� ��ü�� ������</summary> 
	/// <param name = 'commandCode'>ISign ������ OpenAPI Ŀ�ǵ�</param>
	/// <param name = 'url'>ISign ������ URL �ּ�</param>
	/// <param name = 'port'>ISign ������ ��Ʈ ��ȣ</param>
	/// <param name = 'requestParam'>Http(s) ��� �� ����� �Է� �Ķ���� ��ü</param>
	explicit HttpWebRequest(IN const tstring& url, IN const unsigned int port, IN const tstring requestString);

	/// <summary>�������� Http ����� �����ϴ� ��ü�� ������</summary> 
	/// <param name = 'commandCode'>ISign ������ OpenAPI Ŀ�ǵ�</param>
	/// <param name = 'url'>ISign ������ URL �ּ�</param>
	/// <param name = 'port'>ISign ������ ��Ʈ ��ȣ</param>
	/// <param name = 'requestParam'>Http(s) ��� �� ����� �Է� �Ķ���� ��ü</param>
	explicit HttpWebRequest(IN const tstring& url, IN const unsigned int port, IN const unsigned char * requestData, IN const int requestDataSize );


	/// <summary>Http ��� �� �ʿ��� Request Data �� ���� Ȥ�� �缳�� �Ѵ�.</summary> 
	/// <param name = 'commandCode'>ISign ������ OpenAPI Ŀ�ǵ�</param>
	/// <param name = 'url'>ISign ������ URL �ּ�</param>
	/// <param name = 'port'>ISign ������ ��Ʈ ��ȣ</param>
	void setRequestData(IN const tstring& url, IN const unsigned int port);


	/// <summary>Http ��� �� �ʿ��� Request Data �� ���� Ȥ�� �缳�� �Ѵ�.</summary> 
	/// <param name = 'commandCode'>ISign ������ OpenAPI Ŀ�ǵ�</param>
	/// <param name = 'url'>ISign ������ URL �ּ�</param>
	/// <param name = 'port'>ISign ������ ��Ʈ ��ȣ</param>
	/// <param name = 'requestParam'>Http(s) ��� �� ����� �Է� �Ķ���� ��ü</param>
	void setRequestData(IN const tstring& url, IN const unsigned int port, IN const tstring requestString);
	void setRequestData(IN const tstring& url, IN const unsigned int port, IN const unsigned char * requestData, IN const int requestDataSize);

	/// <summary>Spenego ������ ���� spnego token�� �����Ѵ�.</summary> 
	/// <param name = 'spnegoToken'>spnego Token</param>
	/// <param name = 'password'>ad user password</param>
	/// <param name = 'domain'>ad domain</param>
	void setSpnegoToken(IN const tstring& spnegoToken);

	/// <summary>�����ڿ� �Էµ� url, port, requestParam �� ����Ͽ� Http ����� �����Ѵ�. ���ο��� ���� �߻� �� HttpWebException �������ε� ���ܰ� throw �ȴ�.</summary> 
	/// <param name = 'requestMethod'>WEB_REQUEST_METHOD ���ǵ� Http ��û ���</param>
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, IN const std::map<tstring, tstring> &requestHeaderList, OUT std::list<tstring> &responseHeaderList);
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, IN const std::map<tstring, tstring> & requestHeaderList);
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, OUT std::list<tstring> &responseHeaderList);
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType);

	void setLanguage(IN const tstring& language);

	/// <summary>Http ��� �� ���� ���� Result data �� ���´�.</summary> 
	HttpWebResponse getResponse();
	
	~HttpWebRequest();
private:
	tstring	url;
	unsigned int port;
	//tstring	requestString;
	unsigned char *requestData = nullptr;
	int requestDataLen = 0;
	HttpWebResponse	httpWebResponse;
	tstring				spnegoToken;
	tstring acceptLanguage;
};


/*
- HttpWebInnerException Class -
*/


class ISIGNHTTPWEB_API HttpWebInnerException
{
	int				errorCode;
	tstring			errorMessage;
	std::string		whatMessage;

private:
	void setErrorMessageByErrorCode();
	tstring getWinInetErrorCodeToString(IN const int errorCode);
public:
	~HttpWebInnerException();

	explicit HttpWebInnerException();

	/// <summary>�����ڿ��� �Էµ� Win32 �����ڵ带 ��ȯ�Ѵ�.</summary> 
	/// <param name = 'errorCode'>Win32 api �� GetLastError() �� ��ȯ�� �ڵ�</param>
	explicit HttpWebInnerException(IN const int errorCode);


	/// <summary>�����ڿ��� �Էµ� Win32 �����ڵ带 ��ȯ�Ѵ�.</summary> 
	int getErrorCode();

	/// <summary>�����ڿ��� �Էµ� Win32 �����ڵ带 �����Ѵ�.</summary> 
	void setErrorCode(IN const int errorCode);

	/// <summary>�����ڿ��� �Էµ� Win32 �����ڵ��� ������ ��ȯ�Ѵ�. ���ο��� FormatMessage() �� ȣ���Ѵ�. �����ڵ尡 �����Ǿ� ���� ������쿡�� �� ���ڿ��� ��ȯ�Ѵ�.</summary> 
	tstring getErrorMessage();
};


/*
- HttpWebException Class -
*/


class ISIGNHTTPWEB_API HttpWebException
{
	int								errorCode;
	tstring							errorMessage;
	HttpWebInnerException			innerException;
	std::string						whatMessage;
private:
	void setErrorMessageByErrorCode();

public:
	~HttpWebException();

	/// <summary>RETURN_WEB_REQUEST enum ������ ���� �� �ִ� ���ܸ� ó���ϱ� ���� ��ü�� ������</summary> 
	explicit HttpWebException();

	/// <summary>RETURN_WEB_REQUEST enum ������ ���� �� �ִ� ���ܸ� ó���ϱ� ���� ��ü�� ������</summary> 
	/// <param name = 'errorCode'>RETURN_WEB_REQUEST ������ ���� �ڵ�</param>
	/// <param name = 'internalErrorCode'>Win32 api �� GetLastError() �� ��ȯ�� �ڵ�</param>
	explicit HttpWebException(IN const RETURN_HTTP_WEB_REQUEST errorCode, IN const unsigned int internalErrorCode = 0);
	
	/// <summary>�����ڿ��� �Էµ� RETURN_WEB_REQUEST ������ �����ڵ带 ��ȯ�Ѵ�.</summary> 
	int getErrorCode();

	/// <summary>�����ڿ��� �Էµ� RETURN_WEB_REQUEST ������ �����ڵ��� ������ ��ȯ�Ѵ�.</summary> 
	tstring getErrorMessage();

	/// <summary>�ش� ������ ���� ���ܸ� ��ȯ�Ѵ�.</summary> 
	HttpWebInnerException getInnerException();
};


#endif //_EE_WIN_ISIGNHTTPWEB_ISIGNHTTPWEB_H_
