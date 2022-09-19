#pragma once

#ifndef TCMEASUREMENTSDEVIATIONDETECTORAGENT_H
#define TCMEASUREMENTSDEVIATIONDETECTORAGENT_H


#include <string>
#include <chrono>

#include <AgentSystem/include/Agent/TcAgent.h>
#include <AgentSystem/include/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h>


using namespace std;

class TcMeasurementsDeviationDetectorAgent : public TcAgent {
private:
	chrono::system_clock::time_point cmLastSampleRead;
	string rmMachineFamily;
	string rmEquipmentSN;
	string rmReferenceID;
	string rmReferenceType;
	
public:
	TcMeasurementsDeviationDetectorAgent(string pMachineFamily, string pEquipmentSN, string pReferenceid, string pReferenceType, string pCollectionname, string pDatabasename, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string agentid, string agentname, chrono::microseconds stepruntime, Priority priority = Priority::Medium, bool stopped = false);
	~TcMeasurementsDeviationDetectorAgent();

	void fSetLastSampleRead(chrono::system_clock::time_point pLastSampleRead);
	chrono::system_clock::time_point fGetLastSampleRead();

	virtual int fRun();
	virtual int fGetData(int pMovingWindow, vector<TcStatisticTestMeasures>* pMeasuredValues);
	void fTwoSideCUSUM(vector<double> pValues, double pDecisionLikleyhoodThreshold, double pChangeMagnitude, double pMeanUnchanged, double pVarianceUnchanged, int* pEstimatedNchanged, int* pNdetected);

};


#endif