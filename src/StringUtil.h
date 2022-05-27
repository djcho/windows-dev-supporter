#ifndef _EE_WIN_GLOBAL_UTIL_H_
#define _EE_WIN_GLOBAL_UTIL_H_

#include <string>
#include <vector>
#include <tchar.h>
#include <iostream>

#define IN
#define OUT

using namespace std;

#if defined UNICODE || defined _UNICODE
typedef wstring         tstring;
typedef wstringstream   tstringstream;
#ifndef to_tstring
#define to_tstring std::to_wstring
#endif // !to_tstring
#else
typedef string          tstring;
typedef stringstream    tstringstream;
#ifndef to_tstring
#define to_tstring to_string
#endif // !to_tstring
#endif

namespace eewin_util
{
	class StringUtil
	{
		template<typename Out>
		//static void split(const std::string &s, char delim, Out result);
		static void split(IN const tstring &s, IN TCHAR delim, Out result);
	public:
		StringUtil(void);
		~StringUtil(void);
		
		// Convert a wide Unicode string to an UTF8 string
		static std::string utf8Encode(const std::wstring &wstr);

		// Convert an UTF8 string to a wide Unicode String
		static std::wstring utf8Decode(const std::string &str);

		// Convert an wide Unicode string to ANSI string
		static std::string unicodeToAnsi(const std::wstring &wstr);

		// Convert an ANSI string to a wide Unicode String
		static std::wstring ansiToUnicode(const std::string &str);

		static tstring ansiToTstring(const std::string &str);
		static string tstringToAnsi(const tstring &tstr);
		static tstring unicodeToTstring(const std::wstring &wstr);
		static wstring tstringToUnicode(const tstring &tstr);

		static std::vector<tstring> split(IN const tstring &s, IN TCHAR delim);
		static bool compareNoCase(const tstring &a, const tstring &b);
	};
}
#endif