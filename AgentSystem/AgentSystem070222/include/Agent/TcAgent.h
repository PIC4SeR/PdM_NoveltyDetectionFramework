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
	constexpr static const int8_t kNotifyFails = -2;
	constexpr static const int8_t kNotifySuccess = 0;
	constexpr static const int8_t kRunFails = -3;
	constexpr static const int8_t kRunSuccess = 0;

	IMongoDriverAgentInterface* cmMongoInterface;
	INotifyDriverAgentInterface* cmNotifierInterface;
	string rmDatabaseName;
	string rmCollectionName;
	string rmMachineSerialNumber;
	string rmMachineFamily;
	

public:
	
	TcAgent(string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
	~TcAgent();
	virtual int fRun();
	int fNotifyMessage(string pType, string pTitle, string pBody, string pReferenceID, string pReferenceFamily, string pTestName = "");
	virtual int fGetData(list<string>* pLastTemperatures, string* pAvgTemperature, string* pMaxTemperature, string* pMinTemperature, string* pSumTemperatures, list<string>* pTemperaturesBetween, list<string>* pTemperaturesGreatherThan, list<string>* pTemperaturesLowerThan, string* pAvgTemperatureBetweenValues, string* pAvgTemperatureGreatherThanValue, string* pAvgTemperatureLowerThanValue, string* pMaxTemperatureBetweenValues, string* pMaxTemperatureGreatherThanValue, string* pMaxTemperatureLowerThanValue, string* pMinTemperatureBetweenValues, string* pMinTemperatureGreatherThanValue, string* pMinTemperatureLowerThanValue, string* pSumTemperatureBetweenValues, string* pSumTemperatureGreatherThanValue, string* pSumTemperatureLowerThanValue, string* pCountTemperatureBetweenValues, string* pCountTemperatureGreatherThanValue, string* pCountTemperatureLowerThanValue);
	
	string fGetDatabasename();
	string fGetCollectionName();
	string fGetMachineSerialNumber();
	string fGetMachineFamily();

	void fSetMachineSerialNumber(string pMachineSerialNumber);
	void fSetCollectionName(string pCollectionName);
	void fSetMachineFamily(string pMachineFamily);
	void fSetDatabasename(string pDatabaseName);

};



#endif // AGENT_H