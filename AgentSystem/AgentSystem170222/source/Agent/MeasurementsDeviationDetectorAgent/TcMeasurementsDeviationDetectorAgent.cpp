
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <math.h>
#include <random>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <time.h>
#include <boost/math/distributions/empirical_cumulative_distribution_function.hpp>
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMathSupporter.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h"
#include "../../../include/TcMeasure.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMeasurementsDeviationDetectorAgent.h"

#define DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT 1





TcMeasurementsDeviationDetectorAgent::TcMeasurementsDeviationDetectorAgent(string pReferenceID, string pReferenceType, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pMachineSerialNumber, pMachineFamily, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped) {
	this->cmLastSampleRead = chrono::high_resolution_clock::now();
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
	vector<TcStatisticTestMeasures> cTestMeasuresStatistics;
	vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cMathDeviations;


	#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
		int rWindow = 2;
		rResult = TcMeasurementsDeviationDetectorAgent::fGetData(rWindow, &cTestMeasuresStatistics);
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Get data from database fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		}
	#else
		vector<unsigned long long> cTimes = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		vector<unsigned long long> cMTimes = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		vector<double> cVals = { 2.00, 2.00, 2.00, 2.00, 20.00, 2.00, 2.00, 2.00, 2.00, 2.00 };
		vector<double> cMVals(9);
		cTestMeasuresStatistics.resize(1);
	#endif




	for (int i = 0; i < cTestMeasuresStatistics.size(); i++) {
		
		#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
			TcStatisticTestMeasures cTestMeasuresStatistic = cTestMeasuresStatistics[i];
			
			//Skip Test that have less measures thant 3
			if(cTestMeasuresStatistic.cpValues.size() < 3){
				continue;
			}
		#endif

		


		#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
			vector<int> cNormalIndex(cTestMeasuresStatistic.cpValues.size());
		#else
			vector<int> cNormalIndex(cVals.size());
		#endif


		for(int j = 1; j <= cNormalIndex.size(); j++){
			cNormalIndex[j-1] = j;
		}


		
		
		vector<double> cMcoefficients;

		#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
			rResult = TcMathSupporter::fDerivate<double, int>(cNormalIndex, cTestMeasuresStatistic[i].cpValues, &cMcoefficients);
			
		#else
			rResult = TcMathSupporter::fDerivate<double, int>(cNormalIndex, cVals, &cMcoefficients);
		#endif


		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Derivate function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		}

		TcMathSupporter::TcMathDeviationDetector cMathDeviationDetector(0.01);

		#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
			rResult = cMathDeviationDetector.fDetect(cMcoefficients, cTestMeasuresStatistic.cpMovingTimes, cTestMeasuresStatistic.cpValues, cTestMeasuresStatistic.cpTimes, &cMathDeviations);
		#else
			rResult = cMathDeviationDetector.fDetect(cMcoefficients, cMTimes, cVals, cTimes, &cMathDeviations);
		#endif

		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Detection function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(kRunFails);
		} else if (rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kNoDeviationDetected) {
			fprintf(stdout, ANSI_COLOR_GREEN "Detection function doesn't find deviations"  ANSI_COLOR_RESET "\n");
			fflush(stdout);
		} else if (rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kDetectedDeviation) {
			fprintf(stdout, ANSI_COLOR_YELLOW "Detection function finds some deviations"  ANSI_COLOR_RESET "\n");
			fflush(stdout);
		}

		this->cmLastSampleRead = cMathDeviationDetector.fGetLastSampleRead();

		for (TcMathSupporter::TcMathDeviationDetector::TcMathDeviation cMathDeviation  : cMathDeviations) {
			#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
				fNotifyDeviation(cTestMeasuresStatistic, cMathDeviation);	
				fPrintDeviation(cTestMeasuresStatistic, cMathDeviation);
			#else
				fPrintDebugDeviation(cMathDeviation);
			#endif	
		}

	}

	return(kRunSuccess);
}

int TcMeasurementsDeviationDetectorAgent::fGetData(int pMovingWindow, vector<TcStatisticTestMeasures>* pMeasuredValues) {

	int rResult = cmMongoInterface->fConnectMongoDriver();
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Connection to Mongo Driver fails with error %d\n" ANSI_COLOR_RESET, rResult);
		fflush(stdout);
		return(kGetDataFails);
	}

	list<TcStatisticTestMeasures> cMeasuresPerTestBetweenValues;
	long long rToMilliseconds = (long long) chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	long long rFromMilliseconds = (long long) chrono::duration_cast<chrono::milliseconds>(cmLastSampleRead.time_since_epoch()).count();

	if (this->cmMongoInterface->fGetStatisticsOfMeasuresBetweenValues(&cMeasuresPerTestBetweenValues, this->rmDatabaseName, this->rmCollectionName, TcMeasure::TcMeasureAttributes::kTimestampEpoch, to_string(rToMilliseconds), to_string(rFromMilliseconds), pMovingWindow, "Value", "", this->rmReferenceType, "", "TimestampEpoch", "Test", "") < 0) {
		cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	this->cmLastSampleRead = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(cMeasuresPerTestBetweenValues.back().cpTimes.back()));
	cmMongoInterface->fDisconnectMongoDriver();

	*pMeasuredValues = vector<TcStatisticTestMeasures>(cMeasuresPerTestBetweenValues.begin(), cMeasuresPerTestBetweenValues.end());
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

	
	char aStringStartDeviationTime[64];
	char aStringMeasureType[64];
	char aStringDeviationTest[64];


	snprintf(aStringDeviationTest, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpTest);
	snprintf(aStringMeasureType, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpMeasureType);
	

	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = pDeviation.fGetStartTime();
	chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
	chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
	auto cConvertedStartTime = cEquality_system + (cStartTime - cEquality_highsystem);


	chrono::time_point<chrono::system_clock, chrono::milliseconds> cConvertedStartMilliTime = chrono::time_point_cast<chrono::milliseconds>(cConvertedStartTime);
	time_t cStartDeviationTime = chrono::system_clock::to_time_t(cConvertedStartMilliTime);
	ctime_s(aStringStartDeviationTime, strlen(aStringStartDeviationTime), &cStartDeviationTime);
	aStringStartDeviationTime[62] = '\0';

	string rDeviationTime = "Start at " + string(aStringStartDeviationTime) + "\n";
	

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

	
	string rDeviationMeasures =  "Testname " + string(aStringDeviationTest) + " Measure type " + string(aStringMeasureType) + "\n";
	string rNotificationTitle = rDeviationType + " on " + this->rmMachineFamily + " " + this->rmMachineSerialNumber;
	string rNotificationBody = rDeviationMeasures + rDeviationTime;
	string rNotificationType = "Info";

	int rResult = this->fNotifyMessage(rNotificationType, rNotificationTitle, rNotificationBody, this->rmReferenceID, this->rmReferenceType, string(aStringDeviationTest));
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Deviation notification fails to sent with error %d\n" ANSI_COLOR_RESET, rResult);
		fflush(stdout);
		return(kNotifyFails);
	}
	
	return(kNotifySuccess);


}

void TcMeasurementsDeviationDetectorAgent::fPrintDeviation(TcStatisticTestMeasures pStatisticTestMeasures, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation) {

	if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::NoDeviation) {
		return;
	}

	char aStringRoundedDevValue[16];
	char aStringRoundedNormValue[16];
	char aStringRoundedMDeviationValue[16];
	char aStringStartDeviationTime[64];
	char aStringDeviationDurationTime[64];
	char aStringMeasureUnit[64];
	char aStringMeasureType[64];
	char aStringReferenceID[64];
	char aStringReferenceType[64];
	char aStringNDeviationSample[8];
	char aStringNSample[8];
	char aStringDeviationTest[64];


	snprintf(aStringDeviationTest, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpTest);
	snprintf(aStringMeasureUnit, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpMeasureUnit);
	snprintf(aStringMeasureType, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpMeasureType);
	snprintf(aStringReferenceID, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpReferenceID);
	snprintf(aStringReferenceType, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpReferenceType);
	snprintf(aStringNSample, sizeof(char) * 8, "%d", (int)pStatisticTestMeasures.cpValues.size());
	snprintf(aStringNDeviationSample, sizeof(char) * 8, "%d", pDeviation.fGetEndIndex() - pDeviation.fGetStartIndex() + 1);
	snprintf(aStringRoundedDevValue, sizeof(char) * 16, "%.2f", pDeviation.fGetDev());
	snprintf(aStringRoundedNormValue, sizeof(char) * 16, "%.2f", pDeviation.fGetNorm());
	snprintf(aStringRoundedMDeviationValue, sizeof(char) * 16, "%.2f", pDeviation.fGetMLinear());


	chrono::milliseconds cDeviationDuration = chrono::duration_cast<chrono::milliseconds>(pDeviation.fGetEndTime() - pDeviation.fGetStartTime());
	snprintf(aStringDeviationDurationTime, sizeof(char) * 16, "%" PRIi64, cDeviationDuration.count());

	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = pDeviation.fGetStartTime();
	chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
	chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
	auto cConvertedStartTime = cEquality_system + (cStartTime - cEquality_highsystem);


	chrono::time_point<chrono::system_clock, chrono::milliseconds> cConvertedStartMilliTime = chrono::time_point_cast<chrono::milliseconds>(cConvertedStartTime);
	time_t cStartDeviationTime = chrono::system_clock::to_time_t(cConvertedStartMilliTime);
	ctime_s(aStringStartDeviationTime, strlen(aStringStartDeviationTime), &cStartDeviationTime);
	aStringStartDeviationTime[62] = '\0';

	string rDeviationTime = "Start at " + string(aStringStartDeviationTime) + " with duration " + string(aStringDeviationDurationTime) + "ms\n";


	string rDeviationType = "";
	if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drop) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kDrop;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kGlitch;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kShift;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kDrift;
	}

	string rDeviationValues = rDeviationType + " from " + string(aStringRoundedNormValue) + string(aStringMeasureUnit) + " to " + string(aStringRoundedDevValue) + string(aStringMeasureUnit) + " with deviation " + string(aStringRoundedMDeviationValue) + string(aStringMeasureUnit) + "/ms\n";
	string rDeviationMeasures = "Testname " + string(aStringDeviationTest) + " Measure type " + string(aStringMeasureType) + "\n";
	string rDeviationSamples = "Total measures " + string(aStringNSample) + " Deviated measures " + string(aStringNDeviationSample);


	string rNotificationTitle = rDeviationType + " on " + this->rmMachineFamily + " SN: " + this->rmMachineSerialNumber;
	string rNotificationBody = rDeviationMeasures + rDeviationValues + rDeviationTime + rDeviationSamples;

	int rResult = fprintf(stdout, ANSI_COLOR_GREEN "%s\n%s\n" ANSI_COLOR_RESET, rNotificationTitle.c_str(), rNotificationBody.c_str());
	fflush(stdout);
	return;
}


void TcMeasurementsDeviationDetectorAgent::fPrintDebugDeviation(TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation) {

	if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::NoDeviation) {
		return;
	}

	char aStringRoundedDevValue[16];
	char aStringRoundedNormValue[16];
	char aStringRoundedMDeviationValue[16];
	char aStringStartDeviationTime[64];
	char aStringDeviationDurationTime[64];
	char aStringNDeviationSample[8];
	



	snprintf(aStringNDeviationSample, sizeof(char) * 8, "%d", pDeviation.fGetEndIndex() - pDeviation.fGetStartIndex() + 1);
	snprintf(aStringRoundedDevValue, sizeof(char) * 16, "%.2f", pDeviation.fGetDev());
	snprintf(aStringRoundedNormValue, sizeof(char) * 16, "%.2f", pDeviation.fGetNorm());
	snprintf(aStringRoundedMDeviationValue, sizeof(char) * 16, "%.2f", pDeviation.fGetMLinear());


	chrono::milliseconds cDeviationDuration = chrono::duration_cast<chrono::milliseconds>(pDeviation.fGetEndTime() - pDeviation.fGetStartTime());
	snprintf(aStringDeviationDurationTime, sizeof(char) * 16, "%" PRIi64, cDeviationDuration.count());

	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = pDeviation.fGetStartTime();
	chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
	chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
	auto cConvertedStartTime = cEquality_system + (cStartTime - cEquality_highsystem);


	chrono::time_point<chrono::system_clock, chrono::milliseconds> cConvertedStartMilliTime = chrono::time_point_cast<chrono::milliseconds>(cConvertedStartTime);
	time_t cStartDeviationTime = chrono::system_clock::to_time_t(cConvertedStartMilliTime);
	ctime_s(aStringStartDeviationTime, strlen(aStringStartDeviationTime), &cStartDeviationTime);
	aStringStartDeviationTime[62] = '\0';

	string rDeviationTime = "Start at " + string(aStringStartDeviationTime) + " with duration " + string(aStringDeviationDurationTime) + "ms\n";


	string rDeviationType = "";
	if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drop) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kDrop;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kGlitch;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kShift;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcDeviationType::kDrift;
	}

	string rDeviationValues = rDeviationType + " from " + string(aStringRoundedNormValue) + " to " + string(aStringRoundedDevValue) + " with deviation " + string(aStringRoundedMDeviationValue) + "/ms\n";
	string rDeviationSamples = "Deviated measures " + string(aStringNDeviationSample);


	string rNotificationTitle = rDeviationType + " on " + this->rmMachineFamily + " SN: " + this->rmMachineSerialNumber;
	string rNotificationBody =  rDeviationValues + rDeviationTime + rDeviationSamples;

	int rResult = fprintf(stdout, ANSI_COLOR_GREEN "%s\n%s\n" ANSI_COLOR_RESET, rNotificationTitle.c_str(), rNotificationBody.c_str());
	fflush(stdout);
	return;
}













