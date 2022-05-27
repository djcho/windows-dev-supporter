#ifndef _EE_WIN_ISIGNCONFIG_REGCONFIG_H_
#define _EE_WIN_ISIGNCONFIG_REGCONFIG_H_

#include <string>
#include <vector>

#ifdef ISIGNCONFIG_EXPORTS
#define ISIGNCONFIG_API __declspec(dllexport)
#elif ISIGNCONFIG_STATIC
#define ISIGNCONFIG_API 
#else
#define ISIGNCONFIG_API __declspec(dllimport)
#endif

#define		MAX_REG_PATH_LEN		255
#define		MAX_REG_VALUE_LEN		(1024 * 5)
#define		MAX_REG_KEY_LEN			255

enum RETURN_REG_CONF
{
	RETURN_REG_CONF_SUCCESS = 0,
	RETURN_REG_CONF_INVALID_NULL_PARAM = -3400,
	RETURN_REG_CONF_INVALID_STRING_EMPTY_PARAM,
	RETURN_REG_CONF_INVALID_BUF_SIZE,
	RETURN_REG_CONF_EMPTY_KEY_PATH,
	RETURN_REG_CONF_ROOT_PATH_OPEN_FAILED,
	RETURN_REG_CONF_DELETE_KEY_FAILED,
	RETURN_REG_CONF_QUERY_VALUE_FAILED,
	RETURN_REG_CONF_DELETE_VALUE_FAILED,
	RETURN_REG_CONF_SET_VALUE_FAILED,
};

enum REG_CONF_HIVE
{
	REG_CONF_HKLM,
	REG_CONF_HKCU,
	REG_CONF_HKCR,
	REG_CONF_HKUR,
};

using namespace std;
typedef basic_string<TCHAR> tstring;

class ISIGNCONFIG_API RegConfig
{
public:

	/// <summary> 읽기 혹은 쓰기할 레지스트리의 경로를 설정하여 객체를 생성하는 생성자</summary> 
	/// <param name = 'rootHive'>읽기 혹은 쓰기할 레지스트리의 하이브 지정  ex) REG_CONF_HKLM, REG_CONF_HKCU </param>
	/// <param name = 'rootPath'>레지스트리 경로가 담긴 버퍼</param>
	RegConfig(IN const REG_CONF_HIVE rootHive, IN const tstring& rootPath);
	
	/// <summary> 객체의 소멸자</summary>  
	~RegConfig();

	/// <summary> 읽기 혹은 쓰기할 레지스트리의 하이브를 설정 혹은 재설정한다.</summary> 
	void setRootHive(IN const REG_CONF_HIVE rootHive);

	/// <summary> 읽기 혹은 쓰기할 레지스트리의 경로를 설정 혹은 재설정한다.</summary> 
	/// <param name = 'rootPath'>레지스트리 경로가 담긴 버퍼</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF setRootPath(IN const tstring& rootPath);

	/// <summary> 레지스트리 키를 생성한다.</summary> 
	/// <param name = 'subKeyPath'>생성할 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF createKey(IN const tstring& subKeyPath);

	/// <summary> 레지스트리 키를 삭제한다.</summary> 
	/// <param name = 'subKeyPath'>삭제할 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF deleteKey(IN const tstring& subKeyPath);

	/// <summary> 레지스트리 값을 삭제한다.</summary> 
	/// <param name = 'subKeyPath'>삭제할 값이 존재하는 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF deleteValue(IN const tstring& subKeyPath, IN const tstring& valueName);

	/// <summary> 입력된 이름과 일치하는 문자열 값을 읽어온다.</summary> 
	/// <param name = 'subKeyPath'>읽어올 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>읽고자 하는 레지스트리의 이름 값</param>
	/// <param name = 'outData'>읽은 레지스트리 값을 담을 문자열 버퍼</param>
	/// <param name = 'outDataBufferSize'>outData 버퍼의 크기(배열 요소의 갯수)</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF RegConfig::readStringValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT tstring& outData);

	/// <summary> 입력된 이름과 일치하는 DWORD(32bit) 값을 읽어온다.</summary> 
	/// <param name = 'subKeyPath'>읽어올 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>읽고자 하는 레지스트리의 이름 값</param>
	/// <param name = 'outData'>읽은 레지스트리 값을 담을 DWORD 변수의 주소 값</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF readDwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned long * outData);

	/// <summary> 입력된 이름과 일치하는 QWORD(64bit) 값을 읽어온다.</summary> 
	/// <param name = 'subKeyPath'>읽어올 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>읽고자 하는 레지스트리의 이름 값</param>
	/// <param name = 'outData'>읽은 레지스트리 값을 담을 QWORD 변수의 주소 값</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF readQwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned __int64 * outData);
	
	/// <summary> 입력된 이름과 일치하는 문자열 값을 읽어온다. 입력된 경로에 키가 존재하지 않을경우 키를 생성한다.</summary> 
	/// <param name = 'subKeyPath'>읽어올 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>읽고자 하는 레지스트리의 이름 값</param>
	/// <param name = 'outData'>읽은 레지스트리 값을 담을 바이트 버퍼</param>
	/// <param name = 'outDataBufferSize'>outData 버퍼의 크기(배열 요소의 갯수)</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF readBinValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned char * outData, IN OUT unsigned long * outDataBufferSize);

	/// <summary> 입력된 경로에 이름과 일치하는 문자열 값을 기록한다. REG_SZ 사용</summary> 
	/// <param name = 'subKeyPath'>기록할 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>쓰고자 하는 레지스트리의 이름 값, NULL 혹은 빈문자열을 입력할 경우 subKeyPath로 입력된 경로에 레지스트리 키만 생성된다.</param>
	/// <param name = 'inputData'>쓰고자 하는 값을 담은 문자열 버퍼</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF writeStringValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const tstring& inputData);

	/// <summary> 입력된 경로에 이름과 일치하는 문자열 값을 기록한다. REG_EXPAND_SZ 사용</summary> 
	/// <param name = 'subKeyPath'>기록할 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>쓰고자 하는 레지스트리의 이름 값, NULL 혹은 빈문자열을 입력할 경우 subKeyPath로 입력된 경로에 레지스트리 키만 생성된다.</param>
	/// <param name = 'inputData'>쓰고자 하는 값을 담은 문자열 버퍼</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF writeStringValueEx(IN const tstring& subKeyPath, IN const tstring& valueName, IN const tstring& inputData);

	/// <summary> 입력된 경로에 이름과 일치하는 문자열 값을 기록한다. 입력된 경로에 키가 존재하지 않을경우 키를 생성한다.</summary> 
	/// <param name = 'subKeyPath'>기록할 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>쓰고자 하는 레지스트리의 이름 값, NULL 혹은 빈문자열을 입력할 경우 subKeyPath로 입력된 경로에 레지스트리 키만 생성된다.</param>
	/// <param name = 'inputData'>쓰고자 하는 값을 담은 DWORD 값</param>
	/// <param name = 'inputDataSize'>inputData 에 담겨있는 데이터의 크기(바이트 수)</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF writeDwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned long inputData);

	/// <summary> 입력된 경로에 이름과 일치하는 문자열 값을 기록한다. 입력된 경로에 키가 존재하지 않을경우 키를 생성한다.</summary> 
	/// <param name = 'subKeyPath'>기록할 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>쓰고자 하는 레지스트리의 이름 값, NULL 혹은 빈문자열을 입력할 경우 subKeyPath로 입력된 경로에 레지스트리 키만 생성된다.</param>
	/// <param name = 'inputData'>쓰고자 하는 값을 담은 DWORD 값</param>
	/// <param name = 'inputDataSize'>inputData 에 담겨있는 데이터의 크기(바이트 수)</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF writeQwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned __int64 inputData);

	/// <summary> 입력된 경로에 이름과 일치하는 문자열 값을 기록한다. 입력된 경로에 키가 존재하지 않을경우 키를 생성한다.</summary> 
	/// <param name = 'subKeyPath'>기록할 레지스트리 경로를 지정한다. 빈 문자열 값 입력할 경우 생성자에서 입력한 rootPath 로 지정된다.</param>
	/// <param name = 'valueName'>쓰고자 하는 레지스트리의 이름 값, NULL 혹은 빈문자열을 입력할 경우 subKeyPath로 입력된 경로에 레지스트리 키만 생성된다.</param>
	/// <param name = 'inputData'>쓰고자 하는 값을 담은 바이트 버퍼</param>
	/// <param name = 'inputDataSize'>inputData 에 담겨있는 데이터의 크기(바이트 수)</param>
	/// <returns>RETURN_REG_CONF 열거자에 정의된 에러코드</returns>
	RETURN_REG_CONF writeBinValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned char * inputData, IN unsigned long inputDataSize);

	RETURN_REG_CONF getAllSubKeyName(IN const tstring& subKeyPath, OUT vector<tstring>& keyList);

	RegConfig & operator = (const RegConfig & src);

private:
	tstring	rootPath;
	HKEY	rootHive;

	/// <summary> RegConfig 에서 사용하는 레지스트리 Hive 형식을 HKEY로 변환하여 반환
	/// </summary> 
	HKEY convertRegConfHiveToHKEY(IN const REG_CONF_HIVE hive);
	RETURN_REG_CONF getRegFullPath(IN const tstring& subKeyPath, OUT tstring& regFullPath);
	RETURN_REG_CONF readData(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned char * outData, IN unsigned long * outDataBufferSize, IN unsigned long regType);
	RETURN_REG_CONF writeData(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned char * inputData, IN const unsigned long inputDataSize, IN const unsigned long regType);
};




#endif // _EE_WIN_ISIGNCONFIG_REGCONFIG_H_

