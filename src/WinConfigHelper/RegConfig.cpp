#include "stdafx.h"
#include <log4cplus\logger.h>
#include <log4cplus\loggingmacros.h>
#include "..\IsignLogger\IsignLogger.h"
#include "RegConfig.h"
#include "LogDefinition.h"

RegConfig::RegConfig(IN const REG_CONF_HIVE rootHive, IN const tstring& rootPath)
{
	this->rootHive = convertRegConfHiveToHKEY(rootHive);
	this->rootPath = rootPath;
}

RegConfig::~RegConfig()
{
}


void RegConfig::setRootHive(IN const REG_CONF_HIVE rootHive)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	this->rootHive = convertRegConfHiveToHKEY(rootHive);

	LOG4CPLUS_TRACE_END(logger);
}


RETURN_REG_CONF RegConfig::setRootPath(IN const tstring& rootPath)
{	
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);
	RET_INVALID_BUF_SIZE(logger, MAX_REG_PATH_LEN, rootPath.length());

	this->rootPath = rootPath;

	LOG4CPLUS_TRACE_END(logger);
	return RETURN_REG_CONF_SUCCESS;
}

RETURN_REG_CONF RegConfig::createKey(IN const tstring& subKeyPath)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	tstring				regFullPath;
	RETURN_REG_CONF		regConfResult = getRegFullPath(subKeyPath, regFullPath);

	if (regConfResult != RETURN_REG_CONF_SUCCESS)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("getRegFullPath failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(),regConfResult);
		LOG4CPLUS_TRACE_END(logger);
		return regConfResult;
	}

	HKEY				createdKey = NULL;
	LRESULT				regResult = RegCreateKeyEx(rootHive, regFullPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &createdKey, NULL);

	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();
		
		LOG4CPLUS_DEBUG_FMT(logger, _T("RegCreateKeyEx failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
		LOG4CPLUS_TRACE_END(logger);

		return RETURN_REG_CONF_ROOT_PATH_OPEN_FAILED;
	}

	RegCloseKey(createdKey);


	LOG4CPLUS_TRACE_END(logger);
	return RETURN_REG_CONF_SUCCESS;
}


RETURN_REG_CONF RegConfig::deleteKey(IN const tstring& subKeyPath)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	tstring				regFullPath;
	RETURN_REG_CONF		regConfResult = getRegFullPath(subKeyPath, regFullPath);

	if (regConfResult != RETURN_REG_CONF_SUCCESS)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("getRegFullPath failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), regConfResult);
		LOG4CPLUS_TRACE_END(logger);
		return regConfResult;
	}

	HKEY				deleteKey = NULL;
	LRESULT				regResult = RegDeleteTree(rootHive, regFullPath.c_str());

	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();

		LOG4CPLUS_DEBUG_FMT(logger, _T("RegDeleteTree failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
		LOG4CPLUS_TRACE_END(logger);

		return RETURN_REG_CONF_DELETE_KEY_FAILED;
	}

	RegCloseKey(deleteKey);

	LOG4CPLUS_TRACE_END(logger);
	return RETURN_REG_CONF_SUCCESS;
}


RETURN_REG_CONF RegConfig::deleteValue(IN const tstring& subKeyPath, IN const tstring& valueName)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	RET_INVALID_STRING_EMPTY_PARAM(logger, valueName);

	tstring				regFullPath;
	RETURN_REG_CONF		regConfResult = getRegFullPath(subKeyPath, regFullPath);

	if (regConfResult != RETURN_REG_CONF_SUCCESS)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("getRegFullPath failed resultCode : %d"), regConfResult);
		LOG4CPLUS_TRACE_END(logger);
		return regConfResult;
	}

	HKEY				openKey = NULL;
	LRESULT				regResult = RegOpenKeyEx(rootHive, regFullPath.c_str(), 0, KEY_WRITE, &openKey);

	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("RegOpenKeyEx failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
		LOG4CPLUS_TRACE_END(logger);
		return RETURN_REG_CONF_ROOT_PATH_OPEN_FAILED;
	}

	regResult = RegDeleteValue(openKey, valueName.c_str());
	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("RegDeleteValue failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
		
		RegCloseKey(openKey);

		LOG4CPLUS_TRACE_END(logger);
		return RETURN_REG_CONF_DELETE_VALUE_FAILED;
	}

	RegCloseKey(openKey);

	LOG4CPLUS_TRACE_END(logger);
	return RETURN_REG_CONF_SUCCESS;
}


RETURN_REG_CONF RegConfig::readStringValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT tstring& outData)
{
	TCHAR			outDataBuffer[MAX_REG_VALUE_LEN + 1] = { 0, };
	unsigned long	outDataBufferSize = MAX_REG_VALUE_LEN;

	RETURN_REG_CONF regConfRet = readData(subKeyPath, valueName, reinterpret_cast<unsigned char*>(outDataBuffer), &outDataBufferSize, REG_SZ);
	
	outData = outDataBuffer;

	return regConfRet;
}


RETURN_REG_CONF RegConfig::readDwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned long * outData)
{
	unsigned long dwordSize = sizeof(DWORD);
	return this->readData(subKeyPath, valueName, (BYTE*)outData, &dwordSize, REG_DWORD);
}

RETURN_REG_CONF RegConfig::readQwordValue(IN const tstring & subKeyPath, IN const tstring & valueName, OUT unsigned __int64 * outData)
{
	unsigned long dwordSize = sizeof(__int64);
	return this->readData(subKeyPath, valueName, (BYTE*)outData, &dwordSize, REG_QWORD);
}

RETURN_REG_CONF RegConfig::readBinValue(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned char * outData, IN OUT unsigned long * outDataBufferSize)
{	
	return this->readData(subKeyPath, valueName, (BYTE*)outData, outDataBufferSize, REG_BINARY);
}

RETURN_REG_CONF RegConfig::readData(IN const tstring& subKeyPath, IN const tstring& valueName, OUT unsigned char * outData, IN unsigned long * outDataBufferSize, IN unsigned long regType)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	RET_INVALID_NULL_PARAM(logger, outData);
	RET_INVALID_STRING_EMPTY_PARAM(logger, valueName);
	RET_INVALID_BUF_SIZE(logger, MAX_REG_PATH_LEN, subKeyPath.length());
	RET_INVALID_BUF_SIZE(logger, MAX_REG_VALUE_LEN, valueName.length())
	
	tstring				regFullPath;
	RETURN_REG_CONF		regConfResult = getRegFullPath(subKeyPath, regFullPath);

	if (regConfResult != RETURN_REG_CONF_SUCCESS)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("getRegFullPath failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), regConfResult);
		LOG4CPLUS_TRACE_END(logger);
		return regConfResult;
	}

	HKEY				readKey = NULL;
	LRESULT				regResult = RegOpenKeyEx(rootHive, regFullPath.c_str(), 0, KEY_READ, &readKey);

	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("RegOpenKeyEx failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
		LOG4CPLUS_TRACE_END(logger);
		return RETURN_REG_CONF_ROOT_PATH_OPEN_FAILED;
	}

	regResult = RegQueryValueEx(readKey, valueName.c_str(), 0, &regType, (BYTE *)outData, outDataBufferSize);
	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("RegQueryValueEx failed resultCode : %d"), errCode);
		RegCloseKey(readKey);
		LOG4CPLUS_TRACE_END(logger);
		return RETURN_REG_CONF_QUERY_VALUE_FAILED;
	}

	RegCloseKey(readKey);
	LOG4CPLUS_TRACE_END(logger);
	return RETURN_REG_CONF_SUCCESS;
}


RETURN_REG_CONF RegConfig::writeStringValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const tstring& inputData)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);
	RET_INVALID_STRING_EMPTY_PARAM(logger, valueName);

	TCHAR		inputDataBuffer[MAX_REG_VALUE_LEN + 1] = { 0, };

	_tcsncpy(inputDataBuffer, inputData.c_str(), inputData.length());

	return this->writeData(subKeyPath, valueName, (BYTE*)inputDataBuffer, (DWORD)_tcslen(inputDataBuffer) * sizeof(TCHAR), REG_SZ);
}

RETURN_REG_CONF RegConfig::writeStringValueEx(IN const tstring& subKeyPath, IN const tstring& valueName, IN const tstring& inputData)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);
	RET_INVALID_STRING_EMPTY_PARAM(logger, valueName);

	TCHAR		inputDataBuffer[MAX_REG_VALUE_LEN + 1] = { 0, };

	_tcsncpy(inputDataBuffer, inputData.c_str(), inputData.length());

	return this->writeData(subKeyPath, valueName, (BYTE*)inputDataBuffer, (DWORD)_tcslen(inputDataBuffer) * sizeof(TCHAR), REG_EXPAND_SZ);
}

RETURN_REG_CONF RegConfig::writeDwordValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned long inputData)
{
	return this->writeData(subKeyPath, valueName, (BYTE*)&inputData, sizeof(DWORD), REG_DWORD);
}

RETURN_REG_CONF RegConfig::writeQwordValue(IN const tstring & subKeyPath, IN const tstring & valueName, IN const unsigned __int64 inputData)
{
	return this->writeData(subKeyPath, valueName, (BYTE*)&inputData, sizeof(__int64), REG_QWORD);
}

RETURN_REG_CONF RegConfig::writeBinValue(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned char * inputData, IN unsigned long inputDataSize)
{
	return this->writeData(subKeyPath, valueName, inputData, inputDataSize, REG_BINARY);
}

RETURN_REG_CONF RegConfig::getAllSubKeyName(IN const tstring & subKeyPath, OUT vector<tstring>& keyList)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	int					keyIndex = 0;
	tstring				regFullPath;

	RETURN_REG_CONF		regConfResult = getRegFullPath(subKeyPath, regFullPath);

	if (regConfResult != RETURN_REG_CONF_SUCCESS)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("getRegFullPath failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), regConfResult);
		LOG4CPLUS_TRACE_END(logger);
		return regConfResult;
	}

	HKEY				readKey = NULL;
	LRESULT				regResult = RegOpenKeyEx(rootHive, regFullPath.c_str(), 0, KEY_READ, &readKey);
	
	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("RegOpenKeyEx failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
		LOG4CPLUS_TRACE_END(logger);
		return RETURN_REG_CONF_ROOT_PATH_OPEN_FAILED;
	}

	TCHAR				readKeyNameBuffer[MAX_REG_KEY_LEN + 1] = { 0, };

	while (RegEnumKey(readKey, keyIndex++, readKeyNameBuffer, MAX_REG_KEY_LEN) != ERROR_NO_MORE_ITEMS)
	{
		keyList.push_back(readKeyNameBuffer);
	}

	LOG4CPLUS_TRACE_END(logger);
	return RETURN_REG_CONF_SUCCESS;
}


RETURN_REG_CONF RegConfig::writeData(IN const tstring& subKeyPath, IN const tstring& valueName, IN const unsigned char * inputData, IN const unsigned long inputDataSize, IN const unsigned long regType)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	RET_INVALID_NULL_PARAM(logger, inputData);
	RET_INVALID_BUF_SIZE(logger,MAX_REG_PATH_LEN, subKeyPath.length());
	RET_INVALID_BUF_SIZE(logger,MAX_REG_VALUE_LEN, valueName.length())

	tstring				regFullPath;
	RETURN_REG_CONF		regConfResult = getRegFullPath(subKeyPath, regFullPath);

	if (regConfResult != RETURN_REG_CONF_SUCCESS)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("getRegFullPath failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), regConfResult);
		LOG4CPLUS_TRACE_END(logger);
		return regConfResult;
	}

	HKEY				writeKey = NULL;
	LRESULT				regResult = RegOpenKeyEx(rootHive, regFullPath.c_str(), 0, KEY_WRITE, &writeKey);

	if (regResult != ERROR_SUCCESS)
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("RegOpenKeyEx failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
		LOG4CPLUS_TRACE_END(logger);
		return RETURN_REG_CONF_ROOT_PATH_OPEN_FAILED;
	}

	if (!valueName.empty())
	{
		regResult = RegSetValueEx(writeKey, valueName.c_str(), 0, regType, inputData, inputDataSize);
		if (regResult != ERROR_SUCCESS)
		{
			int errCode = GetLastError();
			LOG4CPLUS_DEBUG_FMT(logger, _T("RegSetValueEx failed, regFullPath : %s, resultCode : %d"), regFullPath.c_str(), errCode);
			RegCloseKey(writeKey);
			LOG4CPLUS_TRACE_END(logger);
			return RETURN_REG_CONF_SET_VALUE_FAILED;
		}
	}
	
	RegCloseKey(writeKey);

	LOG4CPLUS_TRACE_END(logger);
	return RETURN_REG_CONF_SUCCESS;
}


RegConfig & RegConfig::operator=(const RegConfig & src)
{
	this->rootHive = src.rootHive;
	this->rootPath = src.rootPath;

	return *this;
}


/////////////////////////////////////////////////////private methods////////////////////////////////////////////////////////////////////

RETURN_REG_CONF RegConfig::getRegFullPath(IN const tstring& subKeyPath, OUT tstring& regFullPath)
{
	regFullPath = this->rootPath;

	if (!subKeyPath.empty())
	{
		regFullPath = regFullPath + _T("\\") + subKeyPath;
	}

	return RETURN_REG_CONF_SUCCESS;
}


HKEY RegConfig::convertRegConfHiveToHKEY(IN REG_CONF_HIVE hive)
{
	switch (hive)
	{
	case REG_CONF_HKLM:
		return HKEY_LOCAL_MACHINE;
	case REG_CONF_HKCU:
		return HKEY_CURRENT_USER;
	case REG_CONF_HKCR:
		return HKEY_CLASSES_ROOT;
	case REG_CONF_HKUR:
		return HKEY_USERS;
	default:
		return NULL;
	}
}