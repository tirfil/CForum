/*
 * SocketManager.cpp
 *
 *  Created on: 2 juin 2010
 *      Author: user
 */

#include "ApplicationManager.h"
#include "SocketManager.h"


SocketManager::SocketManager(ApplicationManager* am)
{
	//am->RegisterApplication(string("SocketManager"), this );
	am->RegisterMainLoop( this );
	server = new Forum();
	server->OpenServer(9876);
}

SocketManager::~SocketManager()
{
	delete server;
}

void
SocketManager::Execute()
{
	server->HandleNewConnection();
	server->HandleAllConnections();
}
