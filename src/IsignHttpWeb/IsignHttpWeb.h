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

	/// <summary>직접적인 Http 통신을 수행하는 객체의 생성자</summary> 
	/// <param name = 'commandCode'>ISign 서버의 OpenAPI 커맨드</param>
	/// <param name = 'url'>ISign 서버의 URL 주소</param>
	/// <param name = 'port'>ISign 서버의 포트 번호</param>
	explicit HttpWebRequest(IN const tstring& url, IN const unsigned int port);

	/// <summary>직접적인 Http 통신을 수행하는 객체의 생성자</summary> 
	/// <param name = 'commandCode'>ISign 서버의 OpenAPI 커맨드</param>
	/// <param name = 'url'>ISign 서버의 URL 주소</param>
	/// <param name = 'port'>ISign 서버의 포트 번호</param>
	/// <param name = 'requestParam'>Http(s) 통신 시 사용할 입력 파라메터 객체</param>
	explicit HttpWebRequest(IN const tstring& url, IN const unsigned int port, IN const tstring requestString);

	/// <summary>직접적인 Http 통신을 수행하는 객체의 생성자</summary> 
	/// <param name = 'commandCode'>ISign 서버의 OpenAPI 커맨드</param>
	/// <param name = 'url'>ISign 서버의 URL 주소</param>
	/// <param name = 'port'>ISign 서버의 포트 번호</param>
	/// <param name = 'requestParam'>Http(s) 통신 시 사용할 입력 파라메터 객체</param>
	explicit HttpWebRequest(IN const tstring& url, IN const unsigned int port, IN const unsigned char * requestData, IN const int requestDataSize );


	/// <summary>Http 통신 시 필요한 Request Data 를 설정 혹은 재설정 한다.</summary> 
	/// <param name = 'commandCode'>ISign 서버의 OpenAPI 커맨드</param>
	/// <param name = 'url'>ISign 서버의 URL 주소</param>
	/// <param name = 'port'>ISign 서버의 포트 번호</param>
	void setRequestData(IN const tstring& url, IN const unsigned int port);


	/// <summary>Http 통신 시 필요한 Request Data 를 설정 혹은 재설정 한다.</summary> 
	/// <param name = 'commandCode'>ISign 서버의 OpenAPI 커맨드</param>
	/// <param name = 'url'>ISign 서버의 URL 주소</param>
	/// <param name = 'port'>ISign 서버의 포트 번호</param>
	/// <param name = 'requestParam'>Http(s) 통신 시 사용할 입력 파라메터 객체</param>
	void setRequestData(IN const tstring& url, IN const unsigned int port, IN const tstring requestString);
	void setRequestData(IN const tstring& url, IN const unsigned int port, IN const unsigned char * requestData, IN const int requestDataSize);

	/// <summary>Spenego 인증을 위해 spnego token을 세팅한다.</summary> 
	/// <param name = 'spnegoToken'>spnego Token</param>
	/// <param name = 'password'>ad user password</param>
	/// <param name = 'domain'>ad domain</param>
	void setSpnegoToken(IN const tstring& spnegoToken);

	/// <summary>생성자에 입력된 url, port, requestParam 을 사용하여 Http 통신을 수행한다. 내부에서 오류 발생 시 HttpWebException 형식으로된 예외가 throw 된다.</summary> 
	/// <param name = 'requestMethod'>WEB_REQUEST_METHOD 정의된 Http 요청 방식</param>
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, IN const std::map<tstring, tstring> &requestHeaderList, OUT std::list<tstring> &responseHeaderList);
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, IN const std::map<tstring, tstring> & requestHeaderList);
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType, OUT std::list<tstring> &responseHeaderList);
	void request(IN const HTTP_WEB_REQUEST_METHOD requestMethod, IN const HTTP_CONTENT_TYPE contentType);

	void setLanguage(IN const tstring& language);

	/// <summary>Http 통신 후 수신 받은 Result data 를 얻어온다.</summary> 
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

	/// <summary>생성자에서 입력된 Win32 에러코드를 반환한다.</summary> 
	/// <param name = 'errorCode'>Win32 api 인 GetLastError() 로 반환된 코드</param>
	explicit HttpWebInnerException(IN const int errorCode);


	/// <summary>생성자에서 입력된 Win32 에러코드를 반환한다.</summary> 
	int getErrorCode();

	/// <summary>생성자에서 입력된 Win32 에러코드를 설정한다.</summary> 
	void setErrorCode(IN const int errorCode);

	/// <summary>생성자에서 입력된 Win32 에러코드의 설명을 반환한다. 내부에서 FormatMessage() 를 호출한다. 에러코드가 설정되어 있지 않을경우에는 빈 문자열을 반환한다.</summary> 
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

	/// <summary>RETURN_WEB_REQUEST enum 값으로 나올 수 있는 예외를 처리하기 위한 객체의 생성자</summary> 
	explicit HttpWebException();

	/// <summary>RETURN_WEB_REQUEST enum 값으로 나올 수 있는 예외를 처리하기 위한 객체의 생성자</summary> 
	/// <param name = 'errorCode'>RETURN_WEB_REQUEST 형식의 에러 코드</param>
	/// <param name = 'internalErrorCode'>Win32 api 인 GetLastError() 로 반환된 코드</param>
	explicit HttpWebException(IN const RETURN_HTTP_WEB_REQUEST errorCode, IN const unsigned int internalErrorCode = 0);
	
	/// <summary>생성자에서 입력된 RETURN_WEB_REQUEST 형식의 에러코드를 반환한다.</summary> 
	int getErrorCode();

	/// <summary>생성자에서 입력된 RETURN_WEB_REQUEST 형식의 에러코드의 설명을 반환한다.</summary> 
	tstring getErrorMessage();

	/// <summary>해당 예외의 원인 예외를 반환한다.</summary> 
	HttpWebInnerException getInnerException();
};


#endif //_EE_WIN_ISIGNHTTPWEB_ISIGNHTTPWEB_H_
