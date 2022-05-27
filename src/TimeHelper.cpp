#include "TimeHelper.h"

#include "StringUtil.h"
using namespace eewin_util;

#include <time.h>

int TimeHelper::calculateLeftMinute(const std::wstring &dateTimeString, int &leftMinute)
{
	struct tm *nowDate, *expireDate;
	int year, month, day, hour, minute, second;
	time_t fromDate, untilDate;

	if (dateTimeString.empty())
		return TIME_HELPER_INVALID_PARAM;

	if (sscanf(StringUtil::tstringToAnsi(dateTimeString).c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second) == EOF)
		return TIME_HELPER_PARSE_TIME_STRING_FAILED;

	time_t expireTime;
	time(&expireTime);
	expireDate = gmtime(&expireTime);
	expireDate->tm_year = year - 1900;
	expireDate->tm_mon = month - 1;
	expireDate->tm_mday = day;
	expireDate->tm_hour = hour;
	expireDate->tm_min = minute;
	expireDate->tm_sec = second;
	untilDate = mktime(expireDate);

	time_t nowTime;
	time(&nowTime);
	nowDate = gmtime(&nowTime);
	fromDate = mktime(nowDate);

	double diffTime = difftime(untilDate, fromDate);
	leftMinute = int(diffTime / 60);

	return TIME_HELPER_SUCCESS;
}