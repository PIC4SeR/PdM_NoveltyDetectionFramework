#pragma once
#ifndef TCAGENT_H
#define TCAGENT_H


#include <string>
#include <chrono>

#include "IAgent.h"
#include "../INotifyDriverAgentInterface.h"
#include "../IMongoDriverAgentInterface.h"




#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


using namespace std;


class TcAgent : public IAgent {
protected:
	constexpr static const int8_t kGetDataFails = -1;
	constexpr static const int8_t kGetDataSuccess = 0;
	constexpr static const int8_t kNotifyFails = -1;
	constexpr static const int8_t kNotifySuccess = 0;
	constexpr static const int8_t kRunFails = -1;
	constexpr static const int8_t kRunSuccess = 0;

	IMongoDriverAgentInterface* cmMongoInterface;
	INotifyDriverAgentInterface* cmNotifierInterface;
	string rmDatabaseName;
	string rmCollectionName;


public:

	TcAgent();
	TcAgent(string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime = chrono::microseconds(0), chrono::time_point<chrono::high_resolution_clock> pNextRunTime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
	~TcAgent();
	virtual int fRun();
	int fNotifyMessage(string pType, string pTitle, string pBody);

	string fGetDatabasename();
	string fGetCollectionName();


	void fSetCollectionName(string pCollectionName);
	void fSetDatabasename(string pDatabaseName);

};

#endif // AGENT_H