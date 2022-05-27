#include "StringUtil.h"

#include <codecvt>
#include <sstream>
#include <iterator>

#include <Windows.h>
#include <tchar.h>
#include <algorithm>

using namespace eewin_util;
typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;

StringUtil::StringUtil()
{
}
StringUtil::~StringUtil()
{
}

// Convert a wide Unicode string to an UTF8 string
std::string StringUtil::utf8Encode(const std::wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring StringUtil::utf8Decode(const std::string &str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

// Convert an wide Unicode string to ANSI string
std::string StringUtil::unicodeToAnsi(const std::wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
	std::string strTo(size_needed - 1, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an ANSI string to a wide Unicode String
std::wstring StringUtil::ansiToUnicode(const std::string &str)
{
	int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

tstring StringUtil::ansiToTstring(const std::string & str)
{
#ifdef UNICODE
	return ansiToUnicode(str);
#else
	return str;
#endif
}

string StringUtil::tstringToAnsi(const tstring & tstr)
{
#ifdef UNICODE
	return unicodeToAnsi(tstr);
#else
	return tstr;
#endif
}

tstring StringUtil::unicodeToTstring(const std::wstring & wstr)
{
#ifdef UNICODE
	return wstr;
#else
	return unicodeToAnsi(wstr);
#endif
}

wstring StringUtil::tstringToUnicode(const tstring & tstr)
{
#ifdef UNICODE
	return tstr;	
#else
	return ansiToUnicode(tstr);
#endif
}

template<typename Out>
void StringUtil::split(IN const tstring &s, IN TCHAR delim, Out result)
{
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif	
	
	ss.str(s);
	tstring item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<tstring> StringUtil::split(IN const tstring &s, IN TCHAR delim)
{
	std::vector<tstring> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

bool eewin_util::StringUtil::compareNoCase(const tstring & a, const tstring & b)
{
	return std::equal(a.begin(), a.end(), b.begin(), b.end(),
		[](TCHAR a, TCHAR b) {
		return _totlower(a) == _totlower(b);
	});
}
