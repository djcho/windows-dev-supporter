#include "stdafx.h"
#include "IsignHttpWeb.h"
#include "global_util_convert.h"

using namespace std;
using namespace global_util;

HttpWebRequestParam::HttpWebRequestParam()
{
}

HttpWebRequestParam::~HttpWebRequestParam()
{
}

void HttpWebRequestParam::setRequestParam(IN const tstring& param)
{
	this->paramString = param;
}

tstring HttpWebRequestParam::getParamString()
{
	return this->paramString;
}

void HttpWebRequestParam::clearRequestParam()
{
	this->paramString.clear();
}