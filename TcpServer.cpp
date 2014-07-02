/*
 * TcpServer.cpp
 *
 *  Created on: 1 juin 2010
 *      Author: user
 *
 */

#include "TcpServer.h"

#include <string>
#include <iostream>
#include <cstdio>
using namespace std;

TcpServer::TcpServer()
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

TcpServer::~TcpServer()
{
	SocketList::iterator it;
	cout << "TcpServer Destructor" << endl;
	for ( it = m_socketList.begin(); it != m_socketList.end(); it++)
	{
		CloseConnection(*it);
	}
	m_socketList.clear();
	m_removeList.clear();
	closesocket(m_listenfd);
#ifdef WIN32
    WSACleanup();
#endif
}
/*----------------------------------------------------------------------
 Portable function to set a socket into nonblocking mode.
 Calling this on a socket causes all future read() and write() calls on
 that socket to do only as much as they can immediately, and return
 without waiting.
 If no data can be read or written, they return -1 and set errno
 to EAGAIN (or EWOULDBLOCK).
 Thanks to Bjorn Reese for this code.
----------------------------------------------------------------------*/
int
TcpServer::setNonblocking(SOCKET fd)
{
#ifdef WIN32
	u_long flags;
	flags = 1L;
	return ioctlsocket(fd, FIONBIO, &flags );
#else

    int flags;
    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    //cout << "O_NONBLOCK" << endl;
    /* Fix me: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    //cout << "FIOBIO" << endl;
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif

#endif
}


void
TcpServer::OpenServer(int port)
{
	int yes = 1;
    cout << "Open port: " << port << endl;
    /* Set up to be a daemon listening on port */
    m_listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenfd == INVALID_SOCKET)
    {
    	perror("socket");
    	closesocket(m_listenfd);
    }

    cout << "Listen socket: " << m_listenfd << endl;
    memset(&m_servaddr, 0, sizeof(m_servaddr));
    m_servaddr.sin_family = AF_INET;
    m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_servaddr.sin_port   = htons(port);

#ifdef WIN32
	if(setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(int)) == SOCKET_ERROR)
#else
    if(setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == SOCKET_ERROR)
#endif
    {
        perror("setsockopt");
        closesocket(m_listenfd);
    }

    if(bind(m_listenfd, (SOCKADDR*) &m_servaddr, sizeof(m_servaddr)) == SOCKET_ERROR)
    {
    	perror("bind");
    	//closesocket(m_listenfd);
    }
    /* Force the network socket into nonblocking mode */
    setNonblocking(m_listenfd);

    if(listen(m_listenfd, 10) == SOCKET_ERROR) {
        perror("listen");
        closesocket(m_listenfd);
    }

}

void
TcpServer::HandleNewConnection()
{

	SOCKET socket = accept(m_listenfd, NULL, NULL);
	if (socket != INVALID_SOCKET) {
			/* Add socket to list */
			cout << "New socket: " << socket << endl;
			setNonblocking(socket);
			m_socketList.push_back(socket);
			InitializeConnection(socket);
	}
}

void
TcpServer::HandleAllConnections()
{
	// Remove inactive connection
	if ( !m_removeList.empty() )
	{
		RemoveList::iterator it;
		for ( it = m_removeList.begin(); it != m_removeList.end(); it++)
		{
			 m_socketList.remove(*it);
		}
		m_removeList.clear();
	}
	// Handle each connection
	SocketList::iterator it;
	for( it = m_socketList.begin(); it != m_socketList.end(); it++ )
	{
		SOCKET fd = *it;
		ManageConnection(fd);
	}
}

//void
//TcpServer::InitializeConnection(SOCKET socket) //TO DO : pure virtual
//{
//
//}
//void
//TcpServer::ManageConnection(SOCKET socket) //TO DO : pure virtual
//{
//
//}



void
TcpServer::CloseConnection(SOCKET socket)
{
	/* remove socket from the list */
	/* Cannot remove an element in a list under iteration */
	/* Add to a list to remove it later */
	m_removeList.push_back(socket);
#ifdef WIN32
	shutdown((SOCKET)socket,SD_BOTH);
#else
	shutdown((SOCKET)socket,SHUT_RDWR);
#endif
	closesocket(socket);
}

size_t
TcpServer::Read(SOCKET socket, char* buffer, size_t size)
{
	//return read(socket,buffer,size);
#ifdef WIN32
	int nread;
	nread = recv(socket,buffer,size, MSG_PEEK);
	if ( nread > 0 && buffer[nread-1] != '\n' ) return -1;
	return recv(socket,buffer,size, 0);
#else
	return recv(socket,buffer,size, 0);
#endif
}

size_t
TcpServer::Write(SOCKET socket,const char* buffer, size_t size)
{
	//return write(socket,buffer,size);
	return send(socket,buffer,size, 0);
}



