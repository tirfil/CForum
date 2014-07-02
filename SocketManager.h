/*
 * SocketManager.h
 *
 *  Created on: 2 juin 2010
 *      Author: user
 */

#ifndef SOCKETMANAGER_H_
#define SOCKETMANAGER_H_

#include "Forum.h"

class SocketManager : public Executable  //, public Application
{
	public:
		SocketManager(ApplicationManager*);
		~SocketManager();
		void Execute();
	private:
		Forum* server;
};

#endif /* SOCKETMANAGER_H_ */
