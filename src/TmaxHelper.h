#ifndef _EE_WIN_GLOBAL_TMAX_DATA_SENDER_H_
#define _EE_WIN_GLOBAL_TMAX_DATA_SENDER_H_

#include <string>

using namespace std;

typedef enum
{
	REQUEST_TMAX_DATA_ERROR_SUCCESS = 0,
	REQUEST_TMAX_DATA_ERROR_ENV_READ_FAIL = -1010,
	REQUEST_TMAX_DATA_ERROR_TPSTART_FAIL,
	REQUEST_TMAX_DATA_ERROR_TPACALL_FAIL,
	REQUEST_TMAX_DATA_ERROR_TPALLOC_FAIL,
}RequestTmaxDataError;

typedef enum
{
	REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_SUCCESS = 0,
	REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_ENV_READ_FAIL = -1100,
	REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_TPSTART_FAIL,
	REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_REQUEST_BUFFER_MEMORY_ALLOC_FIAL,
	REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_RESPONSE_BUFFER_MEMORY_ALLOC_FIAL,
	REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_TPCALL_FAIL,
}RequestAndResponseTmaxDataError;

class TmaxHelper
{
public:
	static RequestTmaxDataError requestTmaxData(const string &tmaxEnvPath, const string &serviceName, const string &sendData);

	static RequestAndResponseTmaxDataError requestAndresponseTmaxData(const string &tmaxEnvPath,
															const string &serviceName, 
									                        const string &requsetBuffer, 
									                        string &responseBuffer, 
									                        const int responseLength, 
									                        int *errorCode);
};

#endif
