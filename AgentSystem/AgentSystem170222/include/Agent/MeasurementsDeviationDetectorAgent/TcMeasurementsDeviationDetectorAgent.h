#pragma once

#ifndef TCMEASUREMENTSDEVIATIONDETECTORAGENT_H
#define TCMEASUREMENTSDEVIATIONDETECTORAGENT_H


#include <string>
#include <chrono>

#include "../TcAgent.h"
#include "TcStatisticTestMeasures.h"
#include "TcMathSupporter.h"



using namespace std;

class TcMeasurementsDeviationDetectorAgent : public TcAgent {
private:
	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cmLastSampleRead;
	string rmReferenceID;
	string rmReferenceType;

public:
	TcMeasurementsDeviationDetectorAgent(string pReferenceID, string pReferenceType, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextruntime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
	~TcMeasurementsDeviationDetectorAgent();

	void fSetLastSampleRead(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pLastSampleRead);
	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> fGetLastSampleRead();


	virtual int fRun();
	virtual int fGetData(int pMovingWindow, vector<TcStatisticTestMeasures>* pMeasuredValues);
	void fTwoSideCUSUM(vector<double> pValues, double pDecisionLikleyhoodThreshold, double pChangeMagnitude, double pMeanUnchanged, double pVarianceUnchanged, int* pEstimatedNchanged, int* pNdetected);
	int fNotifyDeviation(TcStatisticTestMeasures pStatisticTestMeasures, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation);
	void fPrintDeviation(TcStatisticTestMeasures pStatisticTestMeasures, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation);
	void fPrintDebugDeviation(TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation);

};

#endif