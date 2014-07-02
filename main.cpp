/*
 * main.cpp
 *
 *  Created on: 31 mai 2010
 *      Author: user
 */

#include <iostream>
#include <string>
#include <ctime>
using namespace std;
#include "Interface.h"
#include "ApplicationManager.h"

#include "SocketManager.h"

class Benchmark : public Executable, public Application
{
	public:

		Benchmark(ApplicationManager* am)
		{
			am->RegisterApplication(string("Benchmark"), this );
			am->RegisterMainLoop( this );
			m_applicationManager = am;
		}

		void Execute()
		{
			clock_t current = clock();
			//cout << (unsigned long)current << endl;
			unsigned long delay;
			delay = ((unsigned long)(current - m_last)/(unsigned long)CLOCKS_PER_SEC)*1000UL;
			if ( delay > m_maxDelay)
			{
				m_maxDelay = delay;
				cout << "Max loop delay = " << delay << " ms" << endl;
			}

			m_last = current;
		}
		void Start()
		{
			m_last = clock();
			m_maxDelay = 0UL;
		}

	private:
		ApplicationManager*	m_applicationManager;
		clock_t m_last;
		unsigned long m_maxDelay;
};

int main() {
	cout << "ApplicationManager" << endl; // prints ApplicationManager
	ApplicationManager am;
	Benchmark bench(&am);
	SocketManager sm(&am);
	am.Start();
	return 0;
}
