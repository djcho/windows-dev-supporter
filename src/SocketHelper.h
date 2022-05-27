#ifndef _EE_WIN_SOCKET_HELPER_H_
#define _EE_WIN_SOCKET_HELPER_H_

enum SOCKET_HELPER_CONNECT_ERROR
{
	SOCKET_HELPER_CONNECT_ERROR_SUCCESS = 0,
	SOCKET_HELPER_CONNECT_ERROR_INVALID_PARAMETER = -1,
	SOCKET_HELPER_CONNECT_ERROR_INVALID_ADDRESS = -2,
	SOCKET_HELPER_CONNECT_ERROR_FAILURE = -3,
	SOCKET_HELPER_CONNECT_ERROR_WINSOCK_INIT_FAILURE = -4,
	SOCKET_HELPER_CONNECT_ERROR_INVALID_WINSOCK_VERSION = -5
};

enum SOCKET_HELPER_SEND_DATA_ERROR
{
	SOCKET_HELPER_SEND_DATA_ERROR_SUCCESS = 0,
	SOCKET_HELPER_SEND_DATA_ERROR_INVALID_PARAMETER = -1,
	SOCKET_HELPER_SEND_DATA_ERROR_CONNECT_FAILURE = -2,
};

enum SOCKET_HELPER_RECEIVE_DATA_ERROR
{
	SOCKET_HELPER_RECEIVE_DATA_ERROR_SUCCESS = 0,
	SOCKET_HELPER_RECEIVE_DATA_ERROR_TIMEOUT = -1,
	SOCKET_HELPER_RECEIVE_DATA_ERROR_CLOSED_SOCKET = -2,
	SOCKET_HELPER_RECEIVE_DATA_ERROR_FAILURE = -3,
	SOCKET_HELPER_RECEIVE_DATA_ERROR_INVALID_LENGTH = -4,

};

class SocketHelper
{
public:
	// return SOCKET_HELPER_CONNECT_ERROR
	static int socketConnect(int *socket, int port, char *addr, const int timeout);
	
	//return SOCKET_HELPER_SEND_DATA_ERROR
	static int sendLengthAndData(const int	socket, const char *data, const int dataLen);
	
	//return SOCKET_HELPER_RECEIVE_DATA_ERROR
	static int receiveLengthAndData(const int socket, char **receiveDataBuf, int *receiveDataLen, const int timeout);

	static void socketClose(int *socket);

private:
	static struct in_addr* sa_atoAddr(char *cpAddress);
};






#endif