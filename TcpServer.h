/*
 * TcpServer.h
 *
 *  Created on: 1 juin 2010
 *      Author: user
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#ifdef WIN32

#include <winsock2.h>
typedef int socklen_t;
#define EOL "\r\n"

#else

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
#include <string.h>
#include <sys/ioctl.h>
//#include <sys/signal.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef int SOCKET;
#define EOL "\n"

#endif

#include <fcntl.h>

#include <list>
#include <vector>

using namespace std;



class TcpServer
{
	private:
		typedef list<SOCKET> SocketList;
		typedef vector<SOCKET> RemoveList;
		SocketList m_socketList;
		RemoveList m_removeList;
		SOCKET m_listenfd;
		SOCKADDR_IN m_servaddr;
	public:
		TcpServer();
		~TcpServer();
		virtual void OpenServer(int port);
		// loop
		virtual void HandleNewConnection();
		virtual void HandleAllConnections();
		virtual size_t Read(SOCKET,char*,size_t);
		virtual size_t Write(SOCKET,const char*,size_t);
	protected:
		virtual void InitializeConnection(SOCKET socket)= 0; 	// pure virtual
		virtual void ManageConnection(SOCKET socket) = 0;		// pure virtual
		virtual void CloseConnection(SOCKET socket);
		virtual int setNonblocking(SOCKET fd);
};

#endif /* TCPSERVER_H_ */
