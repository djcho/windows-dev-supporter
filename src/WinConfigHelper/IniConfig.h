#ifndef _EE_WIN_ISIGNCONFIG_INICONFIG_H_
#define _EE_WIN_ISIGNCONFIG_INICONFIG_H_

#include <string>
#include <vector>
using namespace std;

#pragma comment(lib, "shlwapi")

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#ifdef ISIGNCONFIG_EXPORTS
#define ISIGNCONFIG_API __declspec(dllexport)
#elif ISIGNCONFIG_STATIC
#define ISIGNCONFIG_API 
#else
#define ISIGNCONFIG_API __declspec(dllimport)
#endif

typedef enum _IINICONFIG_RESULT
{
	INICONFIG_SUCCESS = 0,
	INICONFIG_INITIALIZE_ERROR = 3100,
	INICONFIG_WRITE_INI_ERROR,
	INICONFIG_READ_INI_ERROR,
	INICONFIG_FILE_NOT_EXIST,
	INICONFIG_UNEXPECTED_ERROR,
}IINICONFIG_RESULT;

class ISIGNCONFIG_API INIConfig
{
private:
	tstring *iniFilePath;

public:
	INIConfig(const tstring& filePath);
	~INIConfig();

	IINICONFIG_RESULT writeData(const tstring&  sectionName, const tstring&  keyName, const tstring&  data);
	IINICONFIG_RESULT readData(const tstring&  sectionName, const tstring&  keyName, tstring& outData);
};

#endif // !_EE_WIN_ISIGNCONFIG_INICONFIG_H_
