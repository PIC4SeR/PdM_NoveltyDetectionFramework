#pragma once
#ifndef TCERRORDEGRADATIONTIMEESTRIMATOR_H
#define TCERRORDEGRADATIONTIMEESTRIMATOR_H

#include <string>
#include <chrono>
#include <vector>

#include "../TcAgent.h"

using namespace std;

class TcErrorDegradationTimeEstimator : public TcAgent {

private:
	int rmNumSamplesRead;
	double rmPredictedTimeError;
	double rmMinOperativeThresholdError;
	double rmMaxOperativeThresholdError;
	int rmMinNumOfRegrSamples;
	unsigned int rmSystemFeatureToControl;
	chrono::milliseconds rmPreventionThresholdTime;
	chrono::milliseconds rmLastPredictedTimeToError;
	chrono::system_clock::time_point rmLastPredictedTimeToErrorTime;


public:

	TcErrorDegradationTimeEstimator(int pNumSamplesRead, unsigned int pSystemFeatureToControl, double pPredictedTimeError, double pMinOperativeThresholdError, double pMaxOperativeThresholdError, int pMinNumOfRegrSamples, chrono::milliseconds pPreventionThresholdTime, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, string pAgentname, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
	~TcErrorDegradationTimeEstimator();

	virtual int fRun();
	int fGetData(vector<unsigned long>* pTimes, vector<double>* pSamples);

};
			

#endif // AGENT_H