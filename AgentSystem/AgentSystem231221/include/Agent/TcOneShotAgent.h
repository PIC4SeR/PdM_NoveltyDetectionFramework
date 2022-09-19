#pragma once

#ifndef TCONESHOTAGENT_H
#define TCONESHOTAGENT_H


#include <string>
#include <chrono>

#include <AgentSystem/include/Agent/TcAgent.h>

#define FILEPATH_BUFFER_LENGTH 2048


using namespace std;

class TcOneShotAgent : public TcAgent {
private:

    constexpr static const int8_t kNoRun = 1;

    bool fExistOneShotFile();
    void fDeleteOneShotFile();

public:
	TcOneShotAgent(string pMachineFamily, string pEquipmentSN, string pReferenceid, string pReferenceType, string pCollectionname, string pDatabasename, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string agentid, string agentname, chrono::microseconds stepruntime, Priority priority = Priority::Medium, bool stopped = false);
	~TcOneShotAgent();

	virtual int fRunOneShot();
    int fRun();
	

	

};


#endif