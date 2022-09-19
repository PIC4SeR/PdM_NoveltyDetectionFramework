
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <math.h>
#include <random>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <time.h>
#include "../../../include/TcMathSupporter.h"
#include "../../../include/TcStatisticTestMeasures.h"
#include "../../../include/TcMeasure.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMeasurementsDeviationDetectorAgent.h"


using namespace Spea::Archimede::MultiAgentSystem::Agent;

const string TcMeasurementsDeviationDetectorAgent::kAlgorithm = "Algorithm";
const string TcMeasurementsDeviationDetectorAgent::kAlgorithmParameters = "AlgorithmParameters";
const string TcMeasurementsDeviationDetectorAgent::kSenseFactor = "SenseFactor";
const string TcMeasurementsDeviationDetectorAgent::kDeltaFactor = "DeltaFactor";
const string TcMeasurementsDeviationDetectorAgent::kConstTollerance = "ConstTollerance";
const string TcMeasurementsDeviationDetectorAgent::kLinearTollerance = "LinearTollerance";
const string TcMeasurementsDeviationDetectorAgent::kErrorTollerance = "ErrorTollerance";
const string TcMeasurementsDeviationDetectorAgent::kLevelTollerance = "LevelTollerance";
const string TcMeasurementsDeviationDetectorAgent::kConstVariationPercentage = "ConstVariationPercentage";
const string TcMeasurementsDeviationDetectorAgent::kLinearVariationPercentage = "LinearVariationPercentage";
const string TcMeasurementsDeviationDetectorAgent::kImpulsiveVariationPercentage = "ImpulsiveVariationPercentage";
const string TcMeasurementsDeviationDetectorAgent::kDeviationTime = "DeviationTime";
const string TcMeasurementsDeviationDetectorAgent::kDeviationType = "Deviation";
const string TcMeasurementsDeviationDetectorAgent::kAlgorithmReference = "Measurement Deviation Detector Agent";
const string TcMeasurementsDeviationDetectorAgent::kRegistrationDeviationDB = "DeviationDetectionDB";
const string TcMeasurementsDeviationDetectorAgent::kRegistrationDeviationCollection = "DeviationDetectionComparison";




TcMeasurementsDeviationDetectorAgent::TcMeasurementsDeviationDetectorAgent(){
	this->cmLastSampleRead = chrono::high_resolution_clock::now();
	this->rmReferenceID = "";
	this->rmReferenceType = "";
	this->rmSenseFactor = TcDefault::rpSenseFactor;
	this->rmDeltaFactor = TcDefault::rpDeltaFactor;
	this->rmErrorTollerance = TcDefault::rpErrorTollerance;
	this->rmLinearTollerance = TcDefault::rpLinearTollerance;
	this->rmConstTollerance = TcDefault::rpConstTollerance;
	this->rmLinearVariationPercentage = TcDefault::rpLinearVariationPercentage;
	this->rmConstVariationPercentage = TcDefault::rpConstVariationPercentage;
	this->rmImpulsiveVariationPercentage = TcDefault::rpImpulsiveVariationPercentage;
}


TcMeasurementsDeviationDetectorAgent::TcMeasurementsDeviationDetectorAgent(string pReferenceID, string pReferenceType, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pMachineSerialNumber, pMachineFamily, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped) {
	this->cmLastSampleRead = chrono::high_resolution_clock::now();
	this->rmReferenceID = pReferenceID;
	this->rmReferenceType = pReferenceType;
	this->rmSenseFactor = TcDefault::rpSenseFactor;
	this->rmDeltaFactor = TcDefault::rpDeltaFactor;
	this->rmErrorTollerance = TcDefault::rpErrorTollerance;
	this->rmLinearTollerance = TcDefault::rpLinearTollerance;
	this->rmConstTollerance = TcDefault::rpConstTollerance;
	this->rmLinearVariationPercentage = TcDefault::rpLinearVariationPercentage;
	this->rmConstVariationPercentage = TcDefault::rpConstVariationPercentage;
	this->rmImpulsiveVariationPercentage = TcDefault::rpImpulsiveVariationPercentage;
}


TcMeasurementsDeviationDetectorAgent::TcMeasurementsDeviationDetectorAgent(double pSenseFactor, double pDeltaFactor, double pErrorTollerance, double pLinearTollerance, double pConstTollerance, double pLevelTollerance, double pConstVariationPercentage, double pLinearVariationPercentage, double pImpulsiveVariationPercentage, string pReferenceID, string pReferenceType, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pMachineSerialNumber, pMachineFamily, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped) {
	this->rmSenseFactor = pSenseFactor;
	this->rmDeltaFactor = pDeltaFactor;
	this->rmErrorTollerance = pErrorTollerance;
	this->rmLinearTollerance = pLinearTollerance;
	this->rmConstTollerance = pConstTollerance;
	this->rmLinearVariationPercentage = pLinearVariationPercentage;
	this->rmConstVariationPercentage = pConstVariationPercentage;
	this->rmImpulsiveVariationPercentage = pImpulsiveVariationPercentage;
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
	vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cDeviations;


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
		//vector<double> cVals = { 2.00, 2.00, 2.00, 2.00, 20.00, 2.00, 2.00, 2.00, 2.00, 2.00 };
		vector<double> cVals = { 2.00, 2.00, 2.00, 2.50, 3.00, 3.20, 3.50, 3.49, 3.48, 3.49 };
		//vector<double> cVals = { 2.00, 2.00, 2.00, 2.50, 3.00, 3.20, 3.50, 3.49, 3.55, 3.70 };
		//vector<double> cVals = { 2.00, 2.00, 2.50, 3.00, 3.20, 3.50, 3.55, 3.70, 3.75, 3.65 };
		//vector<double> cVals = { 2.00, 2.00, 2.50, 3.00, 3.20, 3.50, 3.55, 3.70, 3.75, 3.72 };
		vector<double> cMVals(9);
		cTestMeasuresStatistics.resize(1);
	#endif




	for (int i = 0; i < cTestMeasuresStatistics.size(); i++) {
		
		#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
			
			//Skip Test that have less measures thant 3
			if(cTestMeasuresStatistics[i].cpValues.size() < 3){
				continue;
			}
		#endif

		


		#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
			vector<int> cNormalIndex(cTestMeasuresStatistics[i].cpValues.size());
		#else
			vector<int> cNormalIndex(cVals.size());
		#endif


		for(int j = 1; j <= cNormalIndex.size(); j++){
			cNormalIndex[j-1] = j;
		}

		vector<double> cMcoefficients;

		#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
			rResult = TcMathSupporter::fDerivate<double, int>(cNormalIndex, cTestMeasuresStatistics[i].cpValues, &cMcoefficients);
			
		#else
			rResult = TcMathSupporter::fDerivate<double, int>(cNormalIndex, cVals, &cMcoefficients);
		#endif


		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Derivate function fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
			fflush(stdout);
			return(TcAgent::kRunFails);
		}
		

		double rDeltaSenseFactor = this->rmDeltaFactor;
		double rSenseFactor = this->rmSenseFactor;
		double rStartingSenseFactor = rDeltaSenseFactor;

		while (rSenseFactor > rStartingSenseFactor) {
			rSenseFactor -= rDeltaSenseFactor;

			TcMathSupporter::TcMathDeviationDetector cMathDeviationDetector(this->rmSenseFactor, this->rmDeltaFactor, this->rmErrorTollerance, this->rmLinearTollerance, this->rmConstTollerance, this->rmLevelTollerance, this->rmConstVariationPercentage, this->rmLinearVariationPercentage, this->rmImpulsiveVariationPercentage);

			#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
				rResult = cMathDeviationDetector.fDetect(cMcoefficients, cTestMeasuresStatistics[i].cpMovingTimes, cTestMeasuresStatistics[i].cpValues, cTestMeasuresStatistics[i].cpTimes, &cDeviations);
			#else
				rResult = cMathDeviationDetector.fDetect(cMcoefficients, cMTimes, cVals, cTimes, &cDeviations);
			#endif

			if (rResult < 0) {
				fprintf(stdout, ANSI_COLOR_RED "Detection function fails with error %d on Test %s"  ANSI_COLOR_RESET "\n", rResult, cTestMeasuresStatistics[i].rpTest.c_str());
				fflush(stdout);
				return(kRunFails);
			}
			else if (rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kNoDeviationDetected) {
				fprintf(stdout, ANSI_COLOR_GREEN "Detection function doesn't find deviations on Test %s"  ANSI_COLOR_RESET "\n", cTestMeasuresStatistics[i].rpTest.c_str());
				fflush(stdout);
			}
			else if (rResult == TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kDetectedDeviation) {
				fprintf(stdout, ANSI_COLOR_YELLOW "Detection function finds some deviations on Test %s"  ANSI_COLOR_RESET "\n", cTestMeasuresStatistics[i].rpTest.c_str());
				fflush(stdout);
			}

			this->cmLastSampleRead = cMathDeviationDetector.fGetLastSampleRead();

			for (TcMathSupporter::TcMathDeviationDetector::TcMathDeviation cDeviation : cDeviations) {
				#if ! DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT
					#if NOTIFY_ENABLE_TCMEASUREMENTSDEVIATIONDETECTORAGENT
						fNotifyDeviation(cTestMeasuresStatistics[i], cDeviation);	
					#endif
					#if PRINT_ENABLE_TCMEASUREMENTSDEVIATIONDETECTORAGENT
						fPrintDeviation(cTestMeasuresStatistics[i], cDeviation);
					#endif
				#else
					#if PRINT_ENABLE_TCMEASUREMENTSDEVIATIONDETECTORAGENT
						fPrintDebugDeviation(cDeviation);
					#endif
				#endif
				
				#if REGISTERDEVIATION_ENABLE_TCMEASUREMENTSDEVIATIONDETECTORAGENT
					//Register Deviation On DB as formatted Json document.
					rResult = fRegisterDeviationData(TcMeasurementsDeviationDetectorAgent::kAlgorithmReference, cDeviation, cTestMeasuresStatistics[i]);
					if (rResult < 0) {
						fprintf(stdout, ANSI_COLOR_RED "Deviation Registration on DB fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
						fflush(stdout);
						return(TcAgent::kRunFails);
					}
				#endif
			}


			//If at least a deviation is found, interrupt the sensefactor loop, pass to next Test to analize
			if (cDeviations.size() > 0) {
				break;
			}
		}
		
	}

	return(TcAgent::kRunSuccess);
}


int TcMeasurementsDeviationDetectorAgent::fRegisterDeviationData(string pAlgorithm, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation, TcStatisticTestMeasures pStatisticTestMeasures) {
	
	try {


		int rResult = cmMongoInterface->fConnectDriver();
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Connection to Mongo Driver fails with error %d\n" ANSI_COLOR_RESET, rResult);
			fflush(stdout);
			return(kGetDataFails);
		}

		bsoncxx::builder::stream::document data_builder{};
		
		data_builder << TcMeasurementsDeviationDetectorAgent::kAlgorithm << pAlgorithm;
		data_builder << TcMeasurementsDeviationDetectorAgent::kAlgorithmParameters << bsoncxx::builder::stream::open_document 
			<< TcMeasurementsDeviationDetectorAgent::kSenseFactor << this->rmSenseFactor 
			<< TcMeasurementsDeviationDetectorAgent::kDeltaFactor << this->rmDeltaFactor
			<< TcMeasurementsDeviationDetectorAgent::kErrorTollerance << this->rmErrorTollerance
			<< TcMeasurementsDeviationDetectorAgent::kConstTollerance << this->rmConstTollerance
			<< TcMeasurementsDeviationDetectorAgent::kLevelTollerance << this->rmLevelTollerance
			<< TcMeasurementsDeviationDetectorAgent::kLinearTollerance << this->rmLinearTollerance
			<< TcMeasurementsDeviationDetectorAgent::kConstVariationPercentage << this->rmConstVariationPercentage
			<< TcMeasurementsDeviationDetectorAgent::kLinearVariationPercentage << this->rmLinearVariationPercentage
			<< TcMeasurementsDeviationDetectorAgent::kImpulsiveVariationPercentage << this->rmImpulsiveVariationPercentage << bsoncxx::builder::stream::close_document;

		string rDeviationType = "";
		if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drop) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrop;
		}
		else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kGlitch;
		}
		else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kShift;
		}
		else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrift;
		}

		
		data_builder << TcMeasurementsDeviationDetectorAgent::kDeviationType << rDeviationType;
		auto cConvertedDetectionTime = chrono::system_clock::now() + (pDeviation.fGetDetectionTime() - chrono::high_resolution_clock::now());


		data_builder << TcMeasurementsDeviationDetectorAgent::kDeviationTime << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedDetectionTime));

		vector<TcMeasure> cMeasures = pStatisticTestMeasures.cpMeasures;
		auto measures_array_builder = bsoncxx::builder::stream::array{};

		for (TcMeasure cMeasure : cMeasures) {
			measures_array_builder << TcMeasure::fSerializeObjectBsonValue(cMeasure);
		}
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures << measures_array_builder;
		
		bsoncxx::document::view_or_value cBsonDocument = data_builder.extract();

		if (cmMongoInterface->fGetDriver()->fInserDocument(TcMeasurementsDeviationDetectorAgent::kRegistrationDeviationDB, TcMeasurementsDeviationDetectorAgent::kRegistrationDeviationCollection, cBsonDocument) < 0) {
			cmMongoInterface->fDisconnectDriver();
			return(kGetDataFails);
		}

		cmMongoInterface->fDisconnectDriver();

		return(TcMeasurementsDeviationDetectorAgent::TcError::TcRegisterDeviationData::kRegisterDeviationData_Ok);
	}
	catch (exception e) {
		fprintf(stdout, ANSI_COLOR_RED "Catched exception - Message %s\n", e.what());
		fflush(stdout);
		return(TcMeasurementsDeviationDetectorAgent::TcError::TcRegisterDeviationData::kErr_RegisterDeviationData);
	}
}



int TcMeasurementsDeviationDetectorAgent::fGetData(int pMovingWindow, vector<TcStatisticTestMeasures>* pMeasuredValues) {
	try{

		int rResult = cmMongoInterface->fConnectDriver();
		if (rResult < 0) {
			fprintf(stdout, ANSI_COLOR_RED "Connection to Mongo Driver fails with error %d\n" ANSI_COLOR_RESET, rResult);
			fflush(stdout);
			return(kGetDataFails);
		}
		

		list<TcStatisticTestMeasures> cMeasuresPerTestBetweenValues;
		long long rToMilliseconds = (long long) chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
		long long rFromMilliseconds = (long long) chrono::duration_cast<chrono::milliseconds>(cmLastSampleRead.time_since_epoch()).count();

		if (this->cmMongoInterface->fGetStatisticsOfMeasuresLowerThanValue(&cMeasuresPerTestBetweenValues, this->rmDatabaseName, this->rmCollectionName, TcMeasure::TcMeasureAttributes::kTimestampEpoch, to_string(rToMilliseconds), pMovingWindow, TcMeasure::TcMeasureAttributes::kValue, this->rmReferenceID, this->rmReferenceType, "", TcMeasure::TcMeasureAttributes::kTest, "") < 0) {
			cmMongoInterface->fDisconnectDriver();
			return(kGetDataFails);
		}

		/*if (this->cmMongoInterface->fGetStatisticsOfMeasuresBetweenValues(&cMeasuresPerTestBetweenValues, this->rmDatabaseName, this->rmCollectionName, TcMeasure::TcMeasureAttributes::kTimestampEpoch, to_string(rFromMilliseconds), to_string(rToMilliseconds), pMovingWindow, TcMeasure::TcMeasureAttributes::kValue, this->rmReferenceID, this->rmReferenceType, "", TcMeasure::TcMeasureAttributes::kTest, "") < 0) {
			cmMongoInterface->fDisconnectDriver();
			return(kGetDataFails);
		}*/

		if (cMeasuresPerTestBetweenValues.front().cpTimes.size() > 0) {
			this->cmLastSampleRead = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(cMeasuresPerTestBetweenValues.front().cpTimes.back()));
		}
		else {
			this->cmLastSampleRead = chrono::high_resolution_clock::now();
		}
		
		cmMongoInterface->fDisconnectDriver();

		*pMeasuredValues = vector<TcStatisticTestMeasures>(cMeasuresPerTestBetweenValues.begin(), cMeasuresPerTestBetweenValues.end());
		return(TcAgent::kGetDataSuccess);
	} catch (exception e) {
		fprintf(stdout, ANSI_COLOR_RED "Catched exception - Message %s\n", e.what());
		fflush(stdout);
		return(TcAgent::kGetDataFails);
	}
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
		return(TcAgent::kNotifyFails);
	}

	
	char aStringStartDeviationTime[64];
	char aStringMeasureType[64];
	char aStringDeviationTest[64];


	snprintf(aStringDeviationTest, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpTest.c_str());
	snprintf(aStringMeasureType, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpMeasureType.c_str());
	

	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = pDeviation.fGetStartTime();
	auto cConvertedStartTime = chrono::system_clock::now() + (cStartTime - chrono::high_resolution_clock::now());


	chrono::time_point<chrono::system_clock, chrono::milliseconds> cConvertedStartMilliTime = chrono::time_point_cast<chrono::milliseconds>(cConvertedStartTime);
	time_t cStartDeviationTime = chrono::system_clock::to_time_t(cConvertedStartMilliTime);
	ctime_s(aStringStartDeviationTime, sizeof(char) * 64, &cStartDeviationTime);

	string rDeviationTime = "Start at " + string(aStringStartDeviationTime);
	

	string rDeviationType = "";
	if(pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drop){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrop;
	} else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kGlitch;
	} else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kShift;
	} else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift){
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrift;
	}

	
	string rDeviationMeasures =  "Testname " + string(aStringDeviationTest) + " Measure type " + string(aStringMeasureType) + "\n";
	string rNotificationTitle = rDeviationType + " on " + this->rmMachineFamily + " " + this->rmMachineSerialNumber;
	string rNotificationBody = rDeviationMeasures + rDeviationTime;
	string rNotificationType = "Info";

	int rResult = this->fNotifyMessage(rNotificationType, rNotificationTitle, rNotificationBody, this->rmReferenceID, this->rmReferenceType, string(aStringDeviationTest));
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Deviation notification fails to sent with error %d\n" ANSI_COLOR_RESET, rResult);
		fflush(stdout);
		return(TcAgent::kNotifyFails);
	}
	
	return(TcAgent::kNotifySuccess);


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


	snprintf(aStringDeviationTest, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpTest.c_str());
	snprintf(aStringMeasureUnit, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpMeasureUnit.c_str());
	snprintf(aStringMeasureType, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpMeasureType.c_str());
	snprintf(aStringReferenceID, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpReferenceID.c_str());
	snprintf(aStringReferenceType, sizeof(char) * 64, "%s", pStatisticTestMeasures.rpReferenceType.c_str());
	snprintf(aStringNSample, sizeof(char) * 8, "%d", (int)pStatisticTestMeasures.cpValues.size());
	snprintf(aStringNDeviationSample, sizeof(char) * 8, "%d", pDeviation.fGetEndIndex() - pDeviation.fGetStartIndex() + 1);
	snprintf(aStringRoundedDevValue, sizeof(char) * 16, "%.2f", pDeviation.fGetDev());
	snprintf(aStringRoundedNormValue, sizeof(char) * 16, "%.2f", pDeviation.fGetNorm());
	snprintf(aStringRoundedMDeviationValue, sizeof(char) * 16, "%.2f", pDeviation.fGetMLinear());


	chrono::milliseconds cDeviationDuration = chrono::duration_cast<chrono::milliseconds>(pDeviation.fGetEndTime() - pDeviation.fGetStartTime());
	snprintf(aStringDeviationDurationTime, sizeof(char) * 16, "%" PRIi64, cDeviationDuration.count());

	chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = pDeviation.fGetStartTime();
	auto cConvertedStartTime = chrono::system_clock::now() + (cStartTime - chrono::high_resolution_clock::now());


	chrono::time_point<chrono::system_clock, chrono::milliseconds> cConvertedStartMilliTime = chrono::time_point_cast<chrono::milliseconds>(cConvertedStartTime);
	time_t cStartDeviationTime = chrono::system_clock::to_time_t(cConvertedStartMilliTime);
	ctime_s(aStringStartDeviationTime, sizeof(char) *64, &cStartDeviationTime);


	string rStringStartDeviationTime(aStringStartDeviationTime);

	rStringStartDeviationTime.erase(std::remove(rStringStartDeviationTime.begin(), rStringStartDeviationTime.end(), '\n'), rStringStartDeviationTime.end());

	string rDeviationTime = "Start at " + rStringStartDeviationTime + " with duration " + string(aStringDeviationDurationTime) + "ms ";


	string rDeviationType = "";
	if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drop) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrop;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kGlitch;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kShift;
	}
	else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift) {
		rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrift;
	}

	string rDeviationValues = rDeviationType + " from " + string(aStringRoundedNormValue) + string(aStringMeasureUnit) + " to " + string(aStringRoundedDevValue) + string(aStringMeasureUnit) + " with deviation " + string(aStringRoundedMDeviationValue) + string(aStringMeasureUnit) + "/ms ";
	string rDeviationMeasures = "Testname " + string(aStringDeviationTest) + " Measure type " + string(aStringMeasureType) + " ";
	string rDeviationSamples = "Total measures " + string(aStringNSample) + " Deviated measures " + string(aStringNDeviationSample);


	string rNotificationTitle = rDeviationType + " on " + this->rmMachineFamily + " SN: " + this->rmMachineSerialNumber;
	string rNotificationBody = rDeviationMeasures + rDeviationValues + rDeviationTime + rDeviationSamples;

	int rResult = fprintf(stdout, ANSI_COLOR_GREEN "%s %s\n" ANSI_COLOR_RESET, rNotificationTitle.c_str(), rNotificationBody.c_str());
	fflush(stdout);
	return;
}


void TcMeasurementsDeviationDetectorAgent::fPrintDebugDeviation(TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation) {
	try {
		if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::NoDeviation) {
			return;
		}

		char aStringRoundedDevValue[16];
		char aStringRoundedNormValue[16];
		char aStringRoundedMDeviationValue[16];
		char aStringStartDeviationTime[64];
		char aStringDeviationDurationTime[16];
		char aStringNDeviationSample[8];

		snprintf(aStringNDeviationSample, sizeof(char) * 8, "%d", pDeviation.fGetEndIndex() - pDeviation.fGetStartIndex() + 1);
		snprintf(aStringRoundedDevValue, sizeof(char) * 16, "%.2f", pDeviation.fGetDev());
		snprintf(aStringRoundedNormValue, sizeof(char) * 16, "%.2f", pDeviation.fGetNorm());
		snprintf(aStringRoundedMDeviationValue, sizeof(char) * 16, "%.2f", pDeviation.fGetMLinear());


		chrono::milliseconds cDeviationDuration = chrono::duration_cast<chrono::milliseconds>(pDeviation.fGetEndTime() - pDeviation.fGetStartTime());
		snprintf(aStringDeviationDurationTime, sizeof(char) * 16, "%" PRIi64, (long long) cDeviationDuration.count());

		chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = pDeviation.fGetStartTime();
		auto cConvertedStartTime = chrono::system_clock::now() + (cStartTime - chrono::high_resolution_clock::now());


		chrono::time_point<chrono::system_clock, chrono::milliseconds> cConvertedStartMilliTime = chrono::time_point_cast<chrono::milliseconds>(cConvertedStartTime);
		time_t cStartDeviationTime = chrono::system_clock::to_time_t(cConvertedStartMilliTime);
		ctime_s(aStringStartDeviationTime, sizeof(char) * 64, &cStartDeviationTime);
		aStringStartDeviationTime[62] = '\0';

		string rDeviationTime = "Start at " + string(aStringStartDeviationTime) + " with duration " + string(aStringDeviationDurationTime) + "ms\n";


		string rDeviationType = "";
		if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drop) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrop;
		}
		else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kGlitch;
		}
		else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kShift;
		}
		else if (pDeviation.fGetDeviation() == TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift) {
			rDeviationType = TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrift;
		}

		string rDeviationValues = rDeviationType + " from " + string(aStringRoundedNormValue) + " to " + string(aStringRoundedDevValue) + " with deviation " + string(aStringRoundedMDeviationValue) + "ms\n";
		string rDeviationSamples = "Deviated measures " + string(aStringNDeviationSample);




		string rNotificationTitle = rDeviationType + " on " + this->rmMachineFamily + " SN: " + this->rmMachineSerialNumber;
		string rNotificationBody = rDeviationValues + rDeviationTime + rDeviationSamples;

		int rResult = fprintf(stdout, ANSI_COLOR_GREEN "%s\n%s\n" ANSI_COLOR_RESET, rNotificationTitle.c_str(), rNotificationBody.c_str());
		fflush(stdout);
		return;
	}
	catch (exception e) {
		fprintf(stdout, ANSI_COLOR_RED "Catched exception - Message %s\n", e.what());
		fflush(stdout);
		return;
	}
}

















