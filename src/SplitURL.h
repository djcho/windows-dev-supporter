#pragma once
#pragma comment(lib, "wininet.lib")

#include <Windows.h>
#include <wininet.h>
#include <atldef.h>

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

// Class to wrap the Win32 URL_COMPONENTS structure
class CURLComponents : public URL_COMPONENTS
{
public:	
	CURLComponents(void)
	{
		memset(this, 0, sizeof(URL_COMPONENTS));
		dwStructSize = sizeof(URL_COMPONENTS);
	}
};

// Class used to split a URL into component parts.
// Note: Uses WININET InternetCrackUrl function.
class CSplitURL
{
private:

	INTERNET_SCHEME m_nScheme;
	INTERNET_PORT m_nPort;
	TCHAR m_szExtraInfo    [MAX_PATH];
	TCHAR m_szHostName     [MAX_PATH];
	TCHAR m_szPassword     [MAX_PATH];
	TCHAR m_szScheme       [MAX_PATH];
	TCHAR m_szURLPath      [MAX_PATH * 4];
	TCHAR m_szUserName     [MAX_PATH];


public:	
	CSplitURL(void)
		: m_nScheme(INTERNET_SCHEME_DEFAULT)
		, m_nPort(0)
	{
	}
	
	CSplitURL(tstring strUrl)
		: m_nScheme(INTERNET_SCHEME_DEFAULT)
		, m_nPort(0)
	{
		Split(strUrl);
	}

	~CSplitURL(void)
	{
	}

	// Split a URL into component parts
	bool Split(tstring strUrl)
	{
		if (0 >= strUrl.size())
			return false;

		// Get the URL length
		DWORD dwLength = static_cast<DWORD>(strUrl.size());

		CURLComponents url;		
		// Fill structure
		url.lpszScheme = m_szScheme;
		url.dwSchemeLength = dwLength;
		url.lpszHostName = m_szHostName;
		url.dwHostNameLength = dwLength;
		url.lpszUserName = m_szUserName;
		url.dwUserNameLength = dwLength;
		url.lpszPassword = m_szPassword;
		url.dwPasswordLength = dwLength;
		url.lpszUrlPath = m_szURLPath;
		url.dwUrlPathLength = dwLength;
		url.lpszExtraInfo = m_szExtraInfo;
		url.dwExtraInfoLength = dwLength;
		// Split
		bool bRet = InternetCrackUrl(strUrl.c_str(), 0, 0, &url) != FALSE;
		
		// Get the scheme/port
		m_nScheme = url.nScheme;
		m_nPort = url.nPort;
		// Done
		return bRet;
	}

	
	// Get the scheme number
	inline INTERNET_SCHEME GetScheme(void) const throw() { return m_nScheme; }
	// Get the port number
	inline INTERNET_PORT GetPort(void) const throw() { return m_nPort; }
	// Get the scheme name
	inline LPCTSTR GetSchemeName(void) const throw() { return m_szScheme; }
	// Get the host name
	inline LPCTSTR GetHostName(void) const throw() { return m_szHostName; }
	// Get the user name
	inline LPCTSTR GetUserName(void) const throw() { return m_szUserName; }
	// Get the password
	inline LPCTSTR GetPassword(void) const throw() { return m_szPassword; }
	// Get the URL path
	inline LPCTSTR GetURLPath(void) const throw() { return m_szURLPath; }
	// Get the extra info
	inline LPCTSTR GetExtraInfo(void) const throw() { return m_szExtraInfo; }
	
};
