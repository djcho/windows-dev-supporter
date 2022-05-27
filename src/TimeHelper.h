#pragma once
#include <string>

enum RETURN_TIME_HELPER
{
	TIME_HELPER_SUCCESS = 0,
	TIME_HELPER_INVALID_PARAM = -111,
	TIME_HELPER_PARSE_TIME_STRING_FAILED,
};

class TimeHelper
{
public:
	static int calculateLeftMinute(const std::wstring &dateTimeString, int &leftMinute);
};

