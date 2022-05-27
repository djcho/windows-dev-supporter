#include "stdafx.h"
#include <Shlwapi.h>
#include <log4cplus\logger.h>
#include <log4cplus\loggingmacros.h>
#include "..\IsignLogger\IsignLogger.h"
#include "INIConfig.h"
#include "LogDefinition.h"

INIConfig::INIConfig(const tstring& filePath)
	: iniFilePath(nullptr)
{
	if(!filePath.empty())
		iniFilePath = new tstring(filePath);
}

IINICONFIG_RESULT INIConfig::writeData(const tstring& sectionName, const tstring& keyName, const tstring& data)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	IINICONFIG_RESULT result = INICONFIG_UNEXPECTED_ERROR;
	
	if (iniFilePath == nullptr)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("iniFilePath is null."));
		return INICONFIG_INITIALIZE_ERROR;
	}
	
	if (WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), data.c_str(), iniFilePath->c_str()))
		result = INICONFIG_SUCCESS;
	else
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("sectionName : %s, keyName : %s, data : %s, iniFilePath : %s"), sectionName.c_str(), keyName.c_str(), data.c_str(), iniFilePath->c_str());
		LOG4CPLUS_DEBUG_FMT(logger, _T("WritePrivateProfileString failed resultCode : %d"), errCode);
		result = INICONFIG_WRITE_INI_ERROR;
	}

	LOG4CPLUS_TRACE_END(logger);
	return result;
}

IINICONFIG_RESULT INIConfig::readData(const tstring& sectionName, const tstring& keyName, tstring& outData)
{
	log4cplus::Logger logger = IsignLogger::getLogger(_T("WinConfigHelper"));
	LOG4CPLUS_TRACE_BEGIN(logger);

	IINICONFIG_RESULT result = INICONFIG_UNEXPECTED_ERROR;
	if (iniFilePath == nullptr)
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("iniFilePath is null."));
		return INICONFIG_INITIALIZE_ERROR;
	}

	if (!PathFileExists(iniFilePath->c_str()))
	{
		int errCode = GetLastError();
		LOG4CPLUS_DEBUG_FMT(logger, _T("PathFileExists failed resultCode : %d, iniFilePath : %s"), errCode, iniFilePath->c_str());
		return INICONFIG_FILE_NOT_EXIST;
	}

	int bufLength = MAX_PATH;
	outData.resize(bufLength);

	int resultLen = -1, errCode = ERROR_SUCCESS;


	resultLen = GetPrivateProfileString(sectionName.c_str(), keyName.c_str(), NULL, &outData[0], bufLength, iniFilePath->c_str());
	if ((resultLen >= 0) && (errCode = GetLastError()) == ERROR_SUCCESS)
	{
		result = INICONFIG_SUCCESS;
		outData.resize(resultLen);
	}
	else
	{
		LOG4CPLUS_DEBUG_FMT(logger, _T("sectionName : %s, keyName : %s, iniFilePath : %s"), sectionName.c_str(), keyName.c_str(), iniFilePath->c_str());
		LOG4CPLUS_DEBUG_FMT(logger, _T("WritePrivateProfileString failed resultCode : %d"), errCode);
		result = INICONFIG_READ_INI_ERROR;
	}

	LOG4CPLUS_TRACE_END(logger);
	return result;
}

INIConfig::~INIConfig()
{
	if (iniFilePath)
		delete iniFilePath;
}