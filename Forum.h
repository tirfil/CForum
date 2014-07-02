/*
 * Forum.h
 *
 *  Created on: 3 juin 2010
 *      Author: user
 */

#ifndef TELNETTEST_H_
#define TELNETTEST_H_

#include "TcpServer.h"

#include <map>
#include <string>
#include <set>
using namespace std;

typedef map<SOCKET,string> MessageList ;
typedef map<SOCKET,string> NameList ;
typedef set<SOCKET>	DisconnectSet;

class Forum : public TcpServer
{
    private:
		MessageList m_messageList;
		NameList m_nameList;
		DisconnectSet m_disconnectSet;
		void Disconnect(SOCKET);
	public:
		Forum();
		~Forum();
		void InitializeConnection(SOCKET);
		void ManageConnection(SOCKET);
};

#endif /* TELNETTEST_H_ */
