#include "TmaxHelper.h"
#include "usrinc/tmaxapi.h"

RequestTmaxDataError TmaxHelper :: requestTmaxData(const string &tmaxEnvPath, const string &serviceName, const string &sendData)
{
	int tmaxResult = -1;

	if (tpstart((TPSTART_T *)NULL) < 0)
	{
		if ((tmaxResult = tmaxreadenv(const_cast<char*>(tmaxEnvPath.c_str()), "EE-WIN")) < 0)
		{
			return REQUEST_TMAX_DATA_ERROR_ENV_READ_FAIL;
		}

		if ((tmaxResult = tpstart((TPSTART_T *)NULL)) < 0)
		{
			return REQUEST_TMAX_DATA_ERROR_TPSTART_FAIL;
		}
	}

	setlocale(LC_ALL, "Korean");

	int dataLen = static_cast<int>(sendData.length());
	char *cpTmaxBuf;

	cpTmaxBuf = tpalloc("CARRAY", NULL, dataLen + 2);
	if (cpTmaxBuf == NULL)
		return REQUEST_TMAX_DATA_ERROR_TPALLOC_FAIL;

	memset(cpTmaxBuf, 0x00, dataLen + 2);
	memcpy(cpTmaxBuf, sendData.c_str(), dataLen);

	cpTmaxBuf[dataLen + 2] = '\0';
	tmaxResult = tpacall(const_cast<char*>(serviceName.c_str()), cpTmaxBuf, dataLen + 2, TPNOREPLY);

	tpfree(cpTmaxBuf);
	tpend();

	if (tmaxResult < 0)
	{
		return REQUEST_TMAX_DATA_ERROR_TPACALL_FAIL;
	}

	return REQUEST_TMAX_DATA_ERROR_SUCCESS;

}

RequestAndResponseTmaxDataError TmaxHelper ::requestAndresponseTmaxData(const string &tmaxEnvPath,
	                                                            const string &serviceName, 
									                            const string &requsetBuffer, 
                                                                string &responseBuffer, 
	                                                            const int responseLength, 
	                                                            int *errorCode)
{
	int tmaxResult = -1;

	if (tpstart((TPSTART_T *)NULL) < 0)
	{
		if ((tmaxResult = tmaxreadenv(const_cast<char*>(tmaxEnvPath.c_str()), "EE-WIN")) < 0)
		{
			return REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_ENV_READ_FAIL;
		}

		if ((tmaxResult = tpstart((TPSTART_T *)NULL)) < 0)
		{
			return REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_TPSTART_FAIL;
		}
	}

	int requsetBufferLength = static_cast<int>(requsetBuffer.length());
	char *sendBuffer = tpalloc("CARRAY", NULL, requsetBufferLength + 2);
	if (sendBuffer == NULL)
	{
		return REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_REQUEST_BUFFER_MEMORY_ALLOC_FIAL;
	}
	
	int returnBufferLen = responseLength;
	char *receiveBuffer = tpalloc("CARRAY", NULL, returnBufferLen);
	if (receiveBuffer == NULL)
	{
		return REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_RESPONSE_BUFFER_MEMORY_ALLOC_FIAL;
	}
	
	memcpy(sendBuffer, requsetBuffer.c_str(), requsetBufferLength);
	
	LONG	responseBufferLength = 0;
	//요청을 보내는 버퍼를 응답을 받기 위한 버퍼로도 사용 
	tpset_timeout(7);
	if (tpcall(const_cast<char*>(serviceName.c_str()), sendBuffer, requsetBufferLength, &receiveBuffer, &responseBufferLength, TPNOFLAGS) == -1)
	{
		*errorCode = gettperrno();
		tpfree(sendBuffer);
		tpfree(receiveBuffer);
		tpend();

		return REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_TPCALL_FAIL;
	}

	if (responseBufferLength != responseLength)
	{
		*errorCode = responseBufferLength;
	}

	responseBuffer = string(receiveBuffer);

	tpfree(sendBuffer);
	tpfree(receiveBuffer);
	tpend();

	return REQUEST_AND_RESPONSE_TMAX_DATA_ERROR_SUCCESS;
}