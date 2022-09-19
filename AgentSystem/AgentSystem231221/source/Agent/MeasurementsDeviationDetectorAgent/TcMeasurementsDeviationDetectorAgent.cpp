
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <math.h>
#include <random>
#include <algorithm>
#include <boost/math/distributions/empirical_cumulative_distribution_function.hpp>

#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMathSupporter.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMeasurementsDeviationDetectorAgent.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h"

TcMeasurementsDeviationDetectorAgent::TcMeasurementsDeviationDetectorAgent(string pMachineFamily, string pEquipmentSN, string pReferenceid, string pReferenceType, string pCollectionname, string pDatabasename, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string agentid, string agentname, chrono::microseconds stepruntime, Priority priority, bool stopped) : TcAgent(pCollectionname, pDatabasename, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, agentid, agentname, stepruntime, priority, stopped) { 
	this->rmMachineFamily = pMachineFamily;
	this->cmLastSampleRead = chrono::system_clock::now();
	this->rmEquipmentSN = pEquipmentSN;
	this->rmReferenceType = pReferenceType;
	this->rmReferenceID = pReferenceid;
}

TcMeasurementsDeviationDetectorAgent::~TcMeasurementsDeviationDetectorAgent(){ ; }


void TcMeasurementsDeviationDetectorAgent::fSetLastSampleRead(chrono::system_clock::time_point pLastSampleRead) {
	this->cmLastSampleRead = pLastSampleRead;
}

chrono::system_clock::time_point TcMeasurementsDeviationDetectorAgent::fGetLastSampleRead() {
	return(this->cmLastSampleRead);
}



int TcMeasurementsDeviationDetectorAgent::fRun() {
	int rResult = 0;
	int rWindow = 5;

	vector<TcStatisticTestMeasures>* cTestMeasuresStatistics;
	vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation>* cMathDeviations;

	rResult = fGetData(rWindow, cTestMeasuresStatistics);
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Get data from database fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
		fflush(stdout);
		return(kRunFails);
	}

	for (int i = 0; i < cTestMeasuresStatistics->size(); i++) {
		vector<double> cMcoefficients;
		TcStatisticTestMeasures cTestMeasuresStatistic = (*cTestMeasuresStatistics)[i];
		rResult = TcMathSupporter::fDerivate<double, long long>(cTestMeasuresStatistic.cpTimes, cTestMeasuresStatistic.cpValues, &cMcoefficients);
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Derivate function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		}
			

		TcMathSupporter::TcMathDeviationDetector cMathDeviationDetector;
		rResult = cMathDeviationDetector.fDetect(0.1, cMcoefficients, cTestMeasuresStatistic.cpValues, cTestMeasuresStatistic.cpTimes, cMathDeviations);
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Detection function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		} else if(rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kNoDeviationDetected){
			fprintf(stdout, ANSI_COLOR_GREEN "Detection function doesn't find deviations"  ANSI_COLOR_RESET "\n");
			fflush(stdout);
			return(kRunFails);
		} else if(rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kDetectedDeviation){
			fprintf(stdout, ANSI_COLOR_YELLOW "Detection function finds some deviations"  ANSI_COLOR_RESET "\n");
			fflush(stdout);
			return(kRunFails);
		}




	}
	

	return(kRunSuccess);
}

int TcMeasurementsDeviationDetectorAgent::fGetData(int pMovingWindow, vector<TcStatisticTestMeasures> *pMeasuredValues) {

	int rResult = cmMongoInterface->fConnectMongoDriver();
	if (rResult < 0) {
		fprintf(stdout, "Connection to Mongo Driver fails with error %d\n", rResult);
		fflush(stdout);
		return(kGetDataFails);
	}

	list<TcStatisticTestMeasures> *cMeasuresPerTestBetweenValues;
	long long rToMilliseconds = (long long)chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

	if (this->cmMongoInterface->fGetStatisticsOfMeasuresLowerThanValue(cMeasuresPerTestBetweenValues, this->rmDatabasename, this->rmCollectionname, "TimestampEpoch", to_string(rToMilliseconds), pMovingWindow, "Value", this->rmReferenceID, this->rmReferenceType, "", "TimestampEpoch", "Test", "") < 0) {
		cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}



	this->cmLastSampleRead = chrono::system_clock::now();
	cmMongoInterface->fDisconnectMongoDriver();

	pMeasuredValues = new vector<TcStatisticTestMeasures>(cMeasuresPerTestBetweenValues->begin(), cMeasuresPerTestBetweenValues->end());
	return(kGetDataSuccess);
}



void TcMeasurementsDeviationDetectorAgent::fTwoSideCUSUM(vector<double> pValues, double pDecisionLikleyhoodThreshold, double pChangeMagnitude, double pMeanUnchanged, double pVarianceUnchanged, int* pEstimatedNchanged, int* pNdetected){

	
	int rNdetected = 0;
	int rEstimatedNchanged = 0;
	int n = -1;
	vector<double> cSumU;
	vector<double> cSumL;
	vector<double> cCumSumU;
	vector<double> cCumSumL;
	double cCumSumLMin = 0;
	double cCumSumUMin = 0;
	vector<double> cLogLikelyhoodU;
	vector<double> cLogLikelyhoodL;

	cLogLikelyhoodL[n] = 0;
	cLogLikelyhoodU[n] = 0;
	cCumSumL[n] = 0;
	cCumSumU[n] = 0;


	int rNsamples = pValues.size();
	for (n = 0; n < rNsamples; n++) {
		cSumL[n] = (abs(pChangeMagnitude)/pVarianceUnchanged) * (pValues[n] - pMeanUnchanged - abs(pChangeMagnitude) / 2);
		cSumU[n] = (abs(pChangeMagnitude) / pVarianceUnchanged) * (pValues[n] - pMeanUnchanged + abs(pChangeMagnitude) / 2);
		cCumSumL[n] = cCumSumL[n - 1] + cSumL[n];
		cCumSumU[n] = cCumSumU[n - 1] + cSumU[n];	
		cCumSumLMin = (n == 0 || cCumSumLMin > cCumSumL[n] ? cCumSumL[n] : cCumSumLMin);
		cCumSumUMin = (n == 0 || cCumSumUMin > cCumSumU[n] ? cCumSumU[n] : cCumSumUMin);
		cLogLikelyhoodL[n] = cCumSumL[n] - cCumSumLMin;
		cLogLikelyhoodU[n] = cCumSumU[n] - cCumSumUMin;
		
		if (cLogLikelyhoodL[n] > pDecisionLikleyhoodThreshold > 0 || cLogLikelyhoodU[n] > pDecisionLikleyhoodThreshold > 0) {
			rNdetected = n;
			if (cLogLikelyhoodL[n] > pDecisionLikleyhoodThreshold > 0) {
				rEstimatedNchanged = (int) distance(cCumSumL.begin(), min_element(cCumSumL.begin(), cCumSumL.end()));
			}
			else if (cLogLikelyhoodU[n] > pDecisionLikleyhoodThreshold > 0) {
				rEstimatedNchanged = (int) distance(cCumSumU.begin(), min_element(cCumSumU.begin(), cCumSumU.end()));
			}
			*pNdetected = rNdetected;
			*pEstimatedNchanged = rEstimatedNchanged;
			return;
		}
	}

	return;

}










