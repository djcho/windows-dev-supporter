#include "stdafx.h"
#include "IsignHttpWeb.h"
#include "global_util_convert.h"

using namespace std;
using namespace global_util;

HttpWebRequestMapParam::HttpWebRequestMapParam()
{

}

HttpWebRequestMapParam::HttpWebRequestMapParam(IN const std::map<std::string, std::string> paramData)
{
	this->paramMapData = paramData;
	this->paramString = convertUtil::ansi2unicode(makeParamString());
}

HttpWebRequestMapParam::~HttpWebRequestMapParam()
{

}

tstring HttpWebRequestMapParam::getParamString()
{
	return this->paramString;
}

void HttpWebRequestMapParam::clearRequestParam()
{
	this->paramMapData.clear();
}

void HttpWebRequestMapParam::setRequestParam(IN const std::map<std::string, std::string> paramData)
{
	this->paramString = convertUtil::ansi2unicode(makeParamString());
}

HttpWebRequestMapParam & HttpWebRequestMapParam::operator=(const HttpWebRequestMapParam & src)
{
	this->paramMapData =  src.paramMapData;
	this->paramString = src.paramString;

	return *this;
}


string HttpWebRequestMapParam::makeParamString()
{
	std::map<std::string, std::string>::iterator	mapIter = this->paramMapData.begin();
	unsigned int									loopCount = 0;
	string											paramString;

	while (mapIter != this->paramMapData.end())
	{
		paramString += mapIter->first;
		paramString += "=";
		paramString += mapIter->second;

		if (loopCount != this->paramMapData.size() - 1)
			paramString += "&";

		++loopCount;
		++mapIter;
	}

	return paramString;
}