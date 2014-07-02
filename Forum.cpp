/*
 * Forum.cpp
 *
 *  Created on: 3 juin 2010
 *      Author: user
 */
#include "Forum.h"
#include <string>
#include <iostream>
#include <cstdio>
using namespace std;

Forum::Forum()
{

}

Forum::~Forum()
{

}

void
Forum::InitializeConnection(SOCKET socket)
{
	struct sockaddr addr;
	socklen_t len;
	len = sizeof(addr);
#ifndef WIN32
	int port;
	char ipstr[INET6_ADDRSTRLEN];

	getpeername(socket, &addr, &len);
	if (addr.sa_family == AF_INET) {
	    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
	    port = ntohs(s->sin_port);
	    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	} else { // AF_INET6
	    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
	    port = ntohs(s->sin6_port);
	    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}
	printf("Peer IP address: %s:%d\n", ipstr, port);
#endif
	string welcome("Welcome, you are connected - type bye to exit");
	welcome = welcome + EOL +"enter your name: ";
	Write(socket,welcome.c_str(), welcome.length());
}

void
Forum::ManageConnection(SOCKET socket)
{
	char buffer[1024];
	string name;
	string message;

	if ( m_disconnectSet.find(socket) != m_disconnectSet.end() )
	{
		m_disconnectSet.erase(socket);
		m_nameList.erase(socket);
		m_messageList.erase(socket);
		CloseConnection(socket);
		return;
	}

	// insert a new name if any
	if ( m_nameList.find(socket) == m_nameList.end())
	{
		int nread = Read(socket,buffer,sizeof(buffer));
		if ( nread == 0 ) Disconnect(socket);
		nread = nread-2;
		if ( nread > 0 )
		{
			NameList::iterator it;
			string online("");
			it = m_nameList.begin();
			if (it != m_nameList.end())
			{
				online = online + it->second;
				it++;
			}
			while (it != m_nameList.end())
			{
				online = online + ", " + it->second;
				it++;
			}
			//for ( it = m_nameList.begin(); it != m_nameList.end(); it++ )
			//{
			//	online = online + it->second + " - ";
			//}
			if ( online.length() != 0 )
			{
				online = "Online: " + online + EOL;
				Write(socket,online.c_str(), online.length());
			}
			m_nameList[socket] = string(buffer,nread);
			m_messageList[socket] = "*** Connected ***";
		}
	}
	else
	{
		// erase my old message
		MessageList::iterator it;
		NameList::iterator itn;
		it = m_messageList.find(socket);
		if ( it != m_messageList.end() )
		{
			m_messageList.erase(it);
		}
		// display message from other parties
		for (it = m_messageList.begin(); it != m_messageList.end(); it++)
		{
			itn = m_nameList.find(it->first);
			if ( itn != m_nameList.end() )
			{
				name = itn->second;
				message = name + ": "+ it->second + EOL;
			}
			Write(socket,message.c_str(), message.length());
		}
		// record my new message + bye message
		int nread = Read(socket,buffer,sizeof(buffer));
		if ( nread == 0 ) Disconnect(socket);
		nread = nread-2;
		if ( nread > 0 )
		{
			if ( (nread == 3) && (strncmp(buffer,"bye",3) == 0))
			{
				Disconnect(socket);
			}
			else
			{
				message = string(buffer,nread);
				m_messageList[socket] = message;
			}
		}
	}
}

void
Forum::Disconnect(SOCKET socket)
{
	m_messageList[socket] = "*** Disconnected ***";
	// delay disconnection for last message display
	m_disconnectSet.insert(socket);
}

