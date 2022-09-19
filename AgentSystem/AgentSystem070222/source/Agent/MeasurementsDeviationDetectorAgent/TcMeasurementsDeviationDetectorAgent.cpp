
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <math.h>
#include <random>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <boost/math/distributions/empirical_cumulative_distribution_function.hpp>
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMathSupporter.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMeasurementsDeviationDetectorAgent.h"




TcMeasurementsDeviationDetectorAgent::TcMeasurementsDeviationDetectorAgent(string pReferenceID, string pReferenceType, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pMachineSerialNumber, pMachineFamily, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped) {
	this->cmLastSampleRead = chrono::system_clock::now();
	this->rmReferenceID = pReferenceID;
	this->rmReferenceType = pReferenceType;
}

TcMeasurementsDeviationDetectorAgent::~TcMeasurementsDeviationDetectorAgent(){ ; }


void TcMeasurementsDeviationDetectorAgent::fSetLastSampleRead(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pLastSampleRead) {
	this->cmLastSampleRead = pLastSampleRead;
}

chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> TcMeasurementsDeviationDetectorAgent::fGetLastSampleRead() {
	return(this->cmLastSampleRead);
}



int TcMeasurementsDeviationDetectorAgent::fRun() {
	int rResult = 0;
	int rWindow = 2;

	vector<unsigned long long> cTimes = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<unsigned long long> cMTimes = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<double> cVals = { 2.00, 2.00, 2.00, 2.00, 4.00, 8.00, 9.00, 9.00, 9.00, 9.00 };
	vector<double> cMVals(9);


	vector<TcStatisticTestMeasures> cTestMeasuresStatistics;
	vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cMathDeviations;

	rResult = fGetData(rWindow, &cTestMeasuresStatistics);
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Get data from database fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
		fflush(stdout);
		return(kRunFails);
	}

	for (int i = 0; i < cTestMeasuresStatistics.size(); i++) {
		vector<double> cMcoefficients;
		TcStatisticTestMeasures cTestMeasuresStatistic = cTestMeasuresStatistics[i];*/



		/*rResult = TcMathSupporter::fDerivate<double, unsigned long long>(cTestMeasuresStatistic.cpTimes, cTestMeasuresStatistic.cpValues, &cMcoefficients);
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Derivate function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		}*/

		rResult = TcMathSupporter::fDerivate<double, unsigned long long>(cTimes, cVals, &cMcoefficients);
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Derivate function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		}
			
		TcMathSupporter::TcMathDeviationDetector cMathDeviationDetector(&fNotifyDeviation);
		/*rResult = cMathDeviationDetector.fDetect(cTestMeasuresStatistic, cMcoefficients, cTestMeasuresStatistic.cpMovingTimes, cTestMeasuresStatistic.cpValues, cTestMeasuresStatistic.cpTimes, &cMathDeviations);*/
		rResult = cMathDeviationDetector.fDetect(cTestMeasuresStatistics[0], 0.1, cMcoefficients, cMTimes, cVals, cTimes, &cMathDeviations);
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Detection function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		} else if(rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kNoDeviationDetected){
			fprintf(stdout, ANSI_COLOR_GREEN "Detection function doesn't find deviations"  ANSI_COLOR_RESET "\n");
			fflush(stdout);
		} else if(rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kDetectedDeviation){
			fprintf(stdout, ANSI_COLOR_YELLOW "Detection function finds some deviations"  ANSI_COLOR_RESET "\n");
			fflush(stdout);
		}

	/*}*/
	

	return(kRunSuccess);
}

int TcMeasurementsDeviationDetectorAgent::fGetData(int pMovingWindow, vector<TcStatisticTestMeasures> *pMeasuredValues) {

	int rResult = cmMongoInterface->fConnectMongoDriver();
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Connection to Mongo Driver fails with error %d\n" ANSI_COLOR_RESET, rResult);
		fflush(stdout);
		return(kGetDataFails);
	}

	list<TcStatisticTestMeasures> cMeasuresPerTestLowerThanValues;
	long long rToMilliseconds = (long long)chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

	if (this->cmMongoInterface->fGetStatisticsOfMeasuresLowerThanValue(&cMeasuresPerTestLowerThanValues, this->rmDatabaseName, this->rmCollectionName, "TimestampEpoch", to_string(rToMilliseconds), pMovingWindow, "Value", "", this->rmReferenceType, "", "TimestampEpoch", "Test", "") < 0) {
		cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}



	this->cmLastSampleRead = chrono::high_resolution_clock::now();
	cmMongoInterface->fDisconnectMongoDriver();

	*pMeasuredValues = vector<TcStatisticTestMeasures>(cMeasuresPerTestLowerThanValues.begin(), cMeasuresPerTestLowerThanValues.end());
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


int TcMeasurementsDeviationDetectorAgent::fNotifyDeviation(TcStatisticTestMeasures pStatisticTestMeasures, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation) {
	
	if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::NoDeviation) {
		return(-1);
	}

	char aStringRoundedDevValue[8];
	char aStringRoundedNormValue[8];
	char aStringRoundedMDeviationValue[8];
	char aStringStartDeviationTime[256];
	char aStringDeviationDurationTime[256];
	char aStringMeasureUnit[256];
	char aStringMeasureType[256];
	char aStringReferenceID[256];
	char aStringReferenceType[256];
	char aStringNDeviationSample[8];
	char aStringNSample[8];
	char aStringDeviationTest[256];


	snprintf(aStringDeviationTest, sizeof(char) * 256, "%s", pStatisticTestMeasures.rpTest);
	snprintf(aStringMeasureUnit, sizeof(char) * 256, "%s", pStatisticTestMeasures.rpMeasureUnit);
	snprintf(aStringMeasureType, sizeof(char) * 256, "%s", pStatisticTestMeasures.rpMeasureType);
	snprintf(aStringReferenceID, sizeof(char) * 256, "%s", pStatisticTestMeasures.rpReferenceID);
	snprintf(aStringReferenceType, sizeof(char) * 256, "%s", pStatisticTestMeasures.rpReferenceType);
	snprintf(aStringNSample, sizeof(char) * 8, "%d", (int) pStatisticTestMeasures.cpValues.size());
	snprintf(aStringNDeviationSample, sizeof(char) * 8, "%d", pDeviation.fGetEndIndex() - pDeviation.fGetStartIndex() + 1);
	snprintf(aStringRoundedDevValue, sizeof(char) * 8, "%.2f", pDeviation.fGetDev());
	snprintf(aStringRoundedNormValue, sizeof(char) * 8, "%.2f", pDeviation.fGetNorm());
	snprintf(aStringRoundedMDeviationValue, sizeof(char) * 8, "%.2f", pDeviation.fGetMLinear());


	chrono::milliseconds cDeviationDuration = chrono::duration_cast<chrono::milliseconds>(pDeviation.fGetEndTime() - pDeviation.fGetStartTime());
	snprintf(aStringDeviationDurationTime, sizeof(char) * 16, "%" PRIi64, cDeviationDuration.count());
	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = pDeviation.fGetStartTime();
	time_t cStartDeviationTime = chrono::high_resolution_clock::to_time_t(cStartTime);
	ctime_r(&cStartDeviationTime, aStringStartDeviationTime);
	aStringStartDeviationTime[62] = '\0';

	string rDeviationTime = "Start at " + string(aStringStartDeviationTime) + " with duration " + string(aStringDeviationDurationTime) + "ms\n";
	

	string rDeviationType = "";
	if(pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drop){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kDrop;
	} else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kGlitch;
	} else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kShift;
	} else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kDrift;
	}

	string rDeviationValues = rDeviationType + " from " + string(aStringRoundedNormValue) + string(aStringMeasureUnit) + " to " + string(aStringRoundedDevValue) + string(aStringMeasureUnit) + " with deviation " + string(aStringRoundedMDeviationValue) + string(aStringMeasureUnit) + "/ms\n";
	string rDeviationTest =  "Testname " + string(aStringDeviationTest) + "\n";
	string rDeviationSamples =  "Total measures " + string(aStringNSample) + " Deviated measures " + string(aStringNDeviationSample) + "\n";
	
	
	string rNotificationTitle = string(aStringMeasureType) + " " + rDeviationType + " on " + this->rmMachineFamily + " SN: " + this->rmMachineSerialNumber;
	string rNotificationBody = rDeviationTest + rDeviationValues + rDeviationTime + rDeviationSamples;
	string rNotificationType = "Info";

	int rResult = this->fNotifyMessage(rNotificationType, rNotificationTitle, rNotificationBody, this->rmReferenceID, this->rmReferenceType, pStatisticTestMeasures.rpTest);
	if (rResult < 0) {
		fprintf(stdout, "Deviation notification fails to sent with error %d\n", rResult);
		fflush(stdout);
		return(kNotifyFails);
	}
	
	return(0);


}













