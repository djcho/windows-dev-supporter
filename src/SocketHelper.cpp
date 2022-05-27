#include "SocketHelper.h"
#include <string.h>
#include <stdlib.h>

#ifdef  _WINDOWS
#include <winsock2.h>
#define		O_NONBLOCK		0
#define 	close(s)		closesocket(s)
#elif defined _AIX
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#elif defined linux || defined __CYGWIN__
#include <sys/socket.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifndef _WINDOWS
typedef int SOCKET;
#endif

struct in_addr* SocketHelper::sa_atoAddr(char *cpAddress)
{
	struct hostent          *stHost;
	static struct in_addr   stSaddr;

	if (cpAddress == NULL)
		return NULL;

	stSaddr.s_addr = inet_addr(cpAddress);
	if (stSaddr.s_addr != -1)
	{
		return &stSaddr;
	}

	stHost = gethostbyname(cpAddress);
	if (stHost != NULL)
	{
		return (struct in_addr *) *stHost->h_addr_list;
	}

	return NULL;
}

int SocketHelper::socketConnect(int *socketHandle, int port, char *addr, const int timeout)
{
	int        nSock = -1;
	struct in_addr     *stAddr;
	int                 nConnected;
	struct sockaddr_in  stAddress;
	int                 nReuseAddr = 1;

	fd_set fd;
	struct timeval tv;
	int ret = -1;
#ifdef _WINDOWS 

	WSADATA  wsaData;
	WORD wVersionRequested;
	int   nError;
	int io = 1;

	/* Initalizing WinSock */
	wVersionRequested = MAKEWORD(2, 0);
	nError = WSAStartup(wVersionRequested, &wsaData);
	if (nError != 0) 
		return SOCKET_HELPER_CONNECT_ERROR_WINSOCK_INIT_FAILURE;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		WSACleanup();
		return SOCKET_HELPER_CONNECT_ERROR_INVALID_WINSOCK_VERSION;
	}
#else
	int flags;
#endif

	if (port <= 0 || addr == NULL) 
		return SOCKET_HELPER_CONNECT_ERROR_INVALID_PARAMETER;

	stAddr = sa_atoAddr(addr);
	if (stAddr == NULL)
	{
		return SOCKET_HELPER_CONNECT_ERROR_INVALID_ADDRESS;
	}
	memset((char *)&stAddress, 0, sizeof(stAddress));
	stAddress.sin_family = AF_INET;
	stAddress.sin_port = htons((u_short)port);
	stAddress.sin_addr.s_addr = stAddr->s_addr;

	nSock = (int)socket(AF_INET, SOCK_STREAM, 0);
	if(nSock < 0)
		return SOCKET_HELPER_CONNECT_ERROR_FAILURE;

	ret = setsockopt(nSock, SOL_SOCKET, SO_REUSEADDR, (char *)&nReuseAddr, sizeof(nReuseAddr));
	if (ret != 0)
	{
		socketClose(reinterpret_cast<int *>(&nSock));
		return SOCKET_HELPER_CONNECT_ERROR_FAILURE;
	}

	tv.tv_usec = 0;
	tv.tv_sec = timeout;

#ifdef _WINDOWS
	ioctlsocket(nSock, FIONBIO, (u_long*)&io);
#else
	flags = fcntl(nSock, F_GETFL, 0);
	fcntl(nSock, F_SETFL, flags | O_NONBLOCK);
#endif

	nConnected = connect(nSock, (struct sockaddr *) &stAddress, sizeof(stAddress));

	FD_ZERO(&fd);
	FD_SET(nSock, &fd);

#ifdef _WINDOWS
	if (select(1, &fd, &fd, NULL, &tv) == 0)
#else 
	if (select(nSock + 1, &fd, &fd, NULL, &tv) == 0)
#endif
	{
		socketClose(reinterpret_cast<int *>(&nSock));
		return SOCKET_HELPER_CONNECT_ERROR_FAILURE;
	}

	if (FD_ISSET(nSock, &fd))
	{
#ifndef  _WINDOWS
		fcntl(nSock, F_SETFL, flags);
#endif

	}
	else {
		socketClose(reinterpret_cast<int *>(&nSock));
		return SOCKET_HELPER_CONNECT_ERROR_FAILURE;
	}

	*socketHandle = nSock;
	return SOCKET_HELPER_CONNECT_ERROR_SUCCESS;
}

int SocketHelper::sendLengthAndData(const int socket, const char *data, const int dataLen)
{
	fd_set	  wset;
	int       nDataLen;
	int       nReturn;
	
	if (socket < 0 || dataLen <= 0)
		return SOCKET_HELPER_SEND_DATA_ERROR_INVALID_PARAMETER;

	FD_ZERO(&wset);
	FD_SET((unsigned)socket, &wset);

	nDataLen = (int)htonl(dataLen);
	nReturn = send((SOCKET)socket, reinterpret_cast<const char *>(&nDataLen), sizeof(nDataLen), 0);
	if (nReturn <= 0)
	{
		return SOCKET_HELPER_SEND_DATA_ERROR_CONNECT_FAILURE;
	}

	nReturn = send((SOCKET)socket, reinterpret_cast<const char *>(data), dataLen, 0);
	if (nReturn <= 0)
	{
		return SOCKET_HELPER_SEND_DATA_ERROR_CONNECT_FAILURE;
	}

	return SOCKET_HELPER_SEND_DATA_ERROR_SUCCESS;
}

int SocketHelper::receiveLengthAndData(const int socket, char **receiveDataBuf, int *receiveDataLen, const int timeout)
{
	int		ret;
	
	fd_set fdSet;
	struct timeval timaVal;

	FD_ZERO(&fdSet);
	FD_SET((unsigned)socket, &fdSet);

	timaVal.tv_sec = timeout;
	timaVal.tv_usec = 0;


	if (select(socket + 1, &fdSet, NULL, NULL, &timaVal) == 0)
	{
		return SOCKET_HELPER_RECEIVE_DATA_ERROR_TIMEOUT;
	}

	if (FD_ISSET(socket, &fdSet) <= 0)
	{
		return SOCKET_HELPER_RECEIVE_DATA_ERROR_CLOSED_SOCKET;

	}
	
	int dataLen = 0;
	if ((ret = recv(socket, reinterpret_cast<char *>(&dataLen), sizeof(dataLen), MSG_PEEK)) < 0)
	{
		return SOCKET_HELPER_RECEIVE_DATA_ERROR_CLOSED_SOCKET;
	}

	ret = recv(socket, reinterpret_cast<char *>(&dataLen), sizeof(dataLen), 0);
	if (ret <= 0)
	{
		return SOCKET_HELPER_RECEIVE_DATA_ERROR_FAILURE;
	}

	*receiveDataLen = ntohl(dataLen);
	if ((*receiveDataLen <= 0) || (*receiveDataLen > 5 * 1024 * 1024))
	{
		return SOCKET_HELPER_RECEIVE_DATA_ERROR_INVALID_LENGTH;
	}

	char *tmpReceiveData;
	tmpReceiveData = (char *)malloc(*receiveDataLen + 1024);
	int	receivedDataSize = 0;

	while (receivedDataSize < *receiveDataLen)
	{
		ret = recv(socket, tmpReceiveData + receivedDataSize, 1024, MSG_PEEK);
		if (ret <= 0)
		{
#ifdef  _WINDOWS
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
#endif
			break;
		}
		ret = recv(socket, tmpReceiveData + receivedDataSize, ret, O_NONBLOCK);
		if (ret <= 0)
		{
#ifdef  _WINDOWS
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
#endif
			break;
		}
		receivedDataSize = receivedDataSize + ret;
	}

	if (receivedDataSize != *receiveDataLen)
	{
		free(tmpReceiveData);
		return SOCKET_HELPER_RECEIVE_DATA_ERROR_FAILURE;
	}

	*receiveDataBuf = (char *)malloc(*receiveDataLen);
	memcpy(*receiveDataBuf, tmpReceiveData, *receiveDataLen);
	free(tmpReceiveData);

	return SOCKET_HELPER_RECEIVE_DATA_ERROR_SUCCESS;

}

void SocketHelper::socketClose(int *socket)
{
	if ((*socket) != 0) {
		close(*socket);
	}
	*socket = 0;
	return;
}

