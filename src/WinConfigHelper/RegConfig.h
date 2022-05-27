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

	/// <summary> �б� Ȥ�� ������ ������Ʈ���� ��θ� �����Ͽ� ��ü�� �����ϴ� ������</summary> 
	/// <param name = 'rootHive'>�б� Ȥ�� ������ ������Ʈ���� ���̺� ����  ex) REG_CONF_HKLM, REG_CONF_HKCU </param>
	/// <param name = 'rootPath'>������Ʈ�� ��ΰ� ��� ����</param>
	RegConfig(IN const REG_CONF_HIVE rootHive, IN const tstring& rootPath);
	
	/// <summary> ��ü�� �Ҹ���</summary>  
	~RegConfig();

	/// <summary> �б� Ȥ�� ������ ������Ʈ���� ���̺긦 ���� Ȥ�� �缳���Ѵ�.</summary> 
	void setRootHive(IN const REG_CONF_HIVE rootHive);

	/// <summary> �б� Ȥ�� ������ ������Ʈ���� ��θ� ���� Ȥ�� �缳���Ѵ�.</summary> 
	/// <param name = 'rootPath'>������Ʈ�� ��ΰ� ��� ����</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF setRootPath(IN const tstring& rootPath);

	/// <summary> ������Ʈ�� Ű�� �����Ѵ�.</summary> 
	/// <param name = 'subKeyPath'>������ ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF createKey(IN const tstring& subKeyPath);

	/// <summary> ������Ʈ�� Ű�� �����Ѵ�.</summary> 
	/// <param name = 'subKeyPath'>������ ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF deleteKey(IN const tstring& subKeyPath);

	/// <summary> ������Ʈ�� ���� �����Ѵ�.</summary> 
	/// <param name = 'subKeyPath'>������ ���� �����ϴ� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF deleteValue(IN const tstring& subKeyPath, IN const tstring& valueName);

	/// <summary> �Էµ� �̸��� ��ġ�ϴ� ���ڿ� ���� �о�´�.</summary> 
	/// <param name = 'subKeyPath'>�о�� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>�а��� �ϴ� ������Ʈ���� �̸� ��</param>
	/// <param name = 'outData'>���� ������Ʈ�� ���� ���� ���ڿ� ����</param>
	/// <param name = 'outDataBufferSize'>outData ������ ũ��(�迭 ����� ����)</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF RegConfig::readStringValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT tstring& outData);

	/// <summary> �Էµ� �̸��� ��ġ�ϴ� DWORD(32bit) ���� �о�´�.</summary> 
	/// <param name = 'subKeyPath'>�о�� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>�а��� �ϴ� ������Ʈ���� �̸� ��</param>
	/// <param name = 'outData'>���� ������Ʈ�� ���� ���� DWORD ������ �ּ� ��</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF readDwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned long * outData);

	/// <summary> �Էµ� �̸��� ��ġ�ϴ� QWORD(64bit) ���� �о�´�.</summary> 
	/// <param name = 'subKeyPath'>�о�� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>�а��� �ϴ� ������Ʈ���� �̸� ��</param>
	/// <param name = 'outData'>���� ������Ʈ�� ���� ���� QWORD ������ �ּ� ��</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF readQwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned __int64 * outData);
	
	/// <summary> �Էµ� �̸��� ��ġ�ϴ� ���ڿ� ���� �о�´�. �Էµ� ��ο� Ű�� �������� ������� Ű�� �����Ѵ�.</summary> 
	/// <param name = 'subKeyPath'>�о�� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>�а��� �ϴ� ������Ʈ���� �̸� ��</param>
	/// <param name = 'outData'>���� ������Ʈ�� ���� ���� ����Ʈ ����</param>
	/// <param name = 'outDataBufferSize'>outData ������ ũ��(�迭 ����� ����)</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF readBinValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned char * outData, IN OUT unsigned long * outDataBufferSize);

	/// <summary> �Էµ� ��ο� �̸��� ��ġ�ϴ� ���ڿ� ���� ����Ѵ�. REG_SZ ���</summary> 
	/// <param name = 'subKeyPath'>����� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>������ �ϴ� ������Ʈ���� �̸� ��, NULL Ȥ�� ���ڿ��� �Է��� ��� subKeyPath�� �Էµ� ��ο� ������Ʈ�� Ű�� �����ȴ�.</param>
	/// <param name = 'inputData'>������ �ϴ� ���� ���� ���ڿ� ����</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF writeStringValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const tstring& inputData);

	/// <summary> �Էµ� ��ο� �̸��� ��ġ�ϴ� ���ڿ� ���� ����Ѵ�. REG_EXPAND_SZ ���</summary> 
	/// <param name = 'subKeyPath'>����� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>������ �ϴ� ������Ʈ���� �̸� ��, NULL Ȥ�� ���ڿ��� �Է��� ��� subKeyPath�� �Էµ� ��ο� ������Ʈ�� Ű�� �����ȴ�.</param>
	/// <param name = 'inputData'>������ �ϴ� ���� ���� ���ڿ� ����</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF writeStringValueEx(IN const tstring& subKeyPath, IN const tstring& valueName, IN const tstring& inputData);

	/// <summary> �Էµ� ��ο� �̸��� ��ġ�ϴ� ���ڿ� ���� ����Ѵ�. �Էµ� ��ο� Ű�� �������� ������� Ű�� �����Ѵ�.</summary> 
	/// <param name = 'subKeyPath'>����� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>������ �ϴ� ������Ʈ���� �̸� ��, NULL Ȥ�� ���ڿ��� �Է��� ��� subKeyPath�� �Էµ� ��ο� ������Ʈ�� Ű�� �����ȴ�.</param>
	/// <param name = 'inputData'>������ �ϴ� ���� ���� DWORD ��</param>
	/// <param name = 'inputDataSize'>inputData �� ����ִ� �������� ũ��(����Ʈ ��)</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF writeDwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned long inputData);

	/// <summary> �Էµ� ��ο� �̸��� ��ġ�ϴ� ���ڿ� ���� ����Ѵ�. �Էµ� ��ο� Ű�� �������� ������� Ű�� �����Ѵ�.</summary> 
	/// <param name = 'subKeyPath'>����� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>������ �ϴ� ������Ʈ���� �̸� ��, NULL Ȥ�� ���ڿ��� �Է��� ��� subKeyPath�� �Էµ� ��ο� ������Ʈ�� Ű�� �����ȴ�.</param>
	/// <param name = 'inputData'>������ �ϴ� ���� ���� DWORD ��</param>
	/// <param name = 'inputDataSize'>inputData �� ����ִ� �������� ũ��(����Ʈ ��)</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF writeQwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned __int64 inputData);

	/// <summary> �Էµ� ��ο� �̸��� ��ġ�ϴ� ���ڿ� ���� ����Ѵ�. �Էµ� ��ο� Ű�� �������� ������� Ű�� �����Ѵ�.</summary> 
	/// <param name = 'subKeyPath'>����� ������Ʈ�� ��θ� �����Ѵ�. �� ���ڿ� �� �Է��� ��� �����ڿ��� �Է��� rootPath �� �����ȴ�.</param>
	/// <param name = 'valueName'>������ �ϴ� ������Ʈ���� �̸� ��, NULL Ȥ�� ���ڿ��� �Է��� ��� subKeyPath�� �Էµ� ��ο� ������Ʈ�� Ű�� �����ȴ�.</param>
	/// <param name = 'inputData'>������ �ϴ� ���� ���� ����Ʈ ����</param>
	/// <param name = 'inputDataSize'>inputData �� ����ִ� �������� ũ��(����Ʈ ��)</param>
	/// <returns>RETURN_REG_CONF �����ڿ� ���ǵ� �����ڵ�</returns>
	RETURN_REG_CONF writeBinValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned char * inputData, IN unsigned long inputDataSize);

	RETURN_REG_CONF getAllSubKeyName(IN const tstring& subKeyPath, OUT vector<tstring>& keyList);

	RegConfig & operator = (const RegConfig & src);

private:
	tstring	rootPath;
	HKEY	rootHive;

	/// <summary> RegConfig ���� ����ϴ� ������Ʈ�� Hive ������ HKEY�� ��ȯ�Ͽ� ��ȯ
	/// </summary> 
	HKEY convertRegConfHiveToHKEY(IN const REG_CONF_HIVE hive);
	RETURN_REG_CONF getRegFullPath(IN const tstring& subKeyPath, OUT tstring& regFullPath);
	RETURN_REG_CONF readData(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned char * outData, IN unsigned long * outDataBufferSize, IN unsigned long regType);
	RETURN_REG_CONF writeData(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned char * inputData, IN const unsigned long inputDataSize, IN const unsigned long regType);
};




#endif // _EE_WIN_ISIGNCONFIG_REGCONFIG_H_

