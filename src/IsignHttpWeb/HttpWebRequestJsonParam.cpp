#include "stdafx.h"
#include "IsignHttpWeb.h"
#include "global_util_convert.h"

using namespace std;
using namespace global_util;

HttpWebRequestJsonParam::HttpWebRequestJsonParam()
{
}

HttpWebRequestJsonParam::HttpWebRequestJsonParam(IN const Json::Value& paramData)
{
	this->paramJsonData = paramData;

	Json::FastWriter jsonWriter;
	this->paramString = convertUtil::utf8_decode(jsonWriter.write(this->paramJsonData));
	
}

HttpWebRequestJsonParam::~HttpWebRequestJsonParam()
{
}

void HttpWebRequestJsonParam::setRequestParam(IN const Json::Value& paramData)
{
	this->paramJsonData = paramData;

	Json::FastWriter jsonWriter;
	this->paramString = convertUtil::utf8_decode(jsonWriter.write(this->paramJsonData));
}

tstring HttpWebRequestJsonParam::getParamString()
{
	return this->paramString;
}

void HttpWebRequestJsonParam::clearRequestParam()
{
	this->paramJsonData.clear();
}

HttpWebRequestJsonParam & HttpWebRequestJsonParam::operator=(const HttpWebRequestJsonParam & src)
{
	this->paramString = src.paramString;
	this->paramJsonData = src.paramJsonData;

	return *this;
}
