
#include <stdio.h>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

#include "../../include/Agent/TcAgent.h"
#include "../../include/TcNotification.h"
#include "../../include/TcMeasure.h"


TcAgent::TcAgent() {
	this->cmMongoInterface = new IMongoDriverAgentInterface();
	this->cmNotifierInterface = new INotifyDriverAgentInterface();
}
TcAgent::TcAgent(string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, IAgent::Priority pPriority, bool pStopped) : IAgent(pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped) {
	this->rmDatabaseName = pDatabaseName;
	this->rmCollectionName = pCollectionName;
	this->cmMongoInterface = new IMongoDriverAgentInterface("Agent DB Application Interface", pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort);
	this->cmNotifierInterface = new INotifyDriverAgentInterface();
}

TcAgent::~TcAgent() {
	if (this->cmMongoInterface == nullptr) {
		delete this->cmMongoInterface;
		this->cmMongoInterface = nullptr;
	}
}

void TcAgent::fSetDatabasename(string pDatabaseName) {
	this->rmDatabaseName = pDatabaseName;
}

string TcAgent::fGetDatabasename() {
	return(this->rmDatabaseName);
}

void TcAgent::fSetCollectionName(string pCollectionName) {
	this->rmCollectionName = pCollectionName;
}

string TcAgent::fGetCollectionName() {
	return(this->rmCollectionName);
}



int TcAgent::fRun() {
	return(kRunSuccess);
}


int TcAgent::fNotifyMessage(string pType, string pTitle, string pBody) {

	if (pType.empty()) {
		return(kNotifyFails);
	}
	if (pTitle.empty()) {
		return(kNotifyFails);
	}
	if (pBody.empty()) {
		return(kNotifyFails);
	}

	TcNotification cNotification(pType, pTitle, pBody);
	int rResult = this->cmNotifierInterface->fSendNotification(cNotification.fGetNotificationString());
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Send notification fails with error %d" ANSI_COLOR_RESET "\n", rResult);
		fflush(stdout);
		return(kNotifyFails);
	}

	return(kNotifySuccess);
}

