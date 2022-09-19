#pragma once

#ifndef TCMEASUREMENTSDEVIATIONDETECTORAGENT_H
#define TCMEASUREMENTSDEVIATIONDETECTORAGENT_H

#include <string>
#include <chrono>

#include "../TcAgent.h"
#include "../../TcStatisticTestMeasures.h"
#include "../../TcMathSupporter.h"



#define DEBUG_TCMEASUREMENTSDEVIATIONDETECTORAGENT 1
#define NOTIFY_ENABLE_TCMEASUREMENTSDEVIATIONDETECTORAGENT 0
#define PRINT_ENABLE_TCMEASUREMENTSDEVIATIONDETECTORAGENT 1
#define REGISTERDEVIATION_ENABLE_TCMEASUREMENTSDEVIATIONDETECTORAGENT 1

using namespace std;
namespace Spea {
	namespace Archimede {
		namespace MultiAgentSystem {
			namespace Agent {
				class TcMeasurementsDeviationDetectorAgent : public TcAgent {
					private:

						class TcError {
						public:
							class TcRegisterDeviationData {
							public:
								static const int kErr_RegisterDeviationData = -1;
								static const int kRegisterDeviationData_Ok = 0;
							};
						};

						static const string kAlgorithm;
						static const string kAlgorithmParameters;
						static const string kSenseFactor;
						static const string kDeltaFactor;
						static const string kConstTollerance;
						static const string kLinearTollerance;
						static const string kErrorTollerance;
						static const string kLevelTollerance;
						static const string kConstVariationPercentage;
						static const string kLinearVariationPercentage;
						static const string kImpulsiveVariationPercentage;
						static const string kDeviationType;
						static const string kDeviationTime;
						static const string kAlgorithmReference;
						static const string kRegistrationDeviationDB;
						static const string kRegistrationDeviationCollection;


						class TcDefault {
						public:
							constexpr static double rpSenseFactor = 0.00001;
							constexpr static double rpDeltaFactor = 0.0000001;
							constexpr static double rpErrorTollerance = 0.0000001;
							constexpr static double rpLinearTollerance = 0.1;
							constexpr static double rpConstTollerance = 0.0000001;
							constexpr static double rpLevelTollerance = 0.0000001;
							constexpr static double rpConstVariationPercentage = 0.1;
							constexpr static double rpLinearVariationPercentage = 0.001;
							constexpr static double rpImpulsiveVariationPercentage = 0.00001;
						};


						double rmSenseFactor;
						double rmDeltaFactor;
						double rmErrorTollerance;
						double rmLinearTollerance;
						double rmConstTollerance;
						double rmLevelTollerance;
						double rmConstVariationPercentage;
						double rmLinearVariationPercentage;
						double rmImpulsiveVariationPercentage;

						chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cmLastSampleRead;
						string rmReferenceID;
						string rmReferenceType;


					public:
						TcMeasurementsDeviationDetectorAgent(string pReferenceID, string pReferenceType, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextruntime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
						TcMeasurementsDeviationDetectorAgent(double pSenseFactor, double pDeltaFactor, double pErrorTollerance, double pLinearTollerance, double pConstTollerance, double pLevelTollerance, double pConstVariationPercentage, double pLinearVariationPercentage, double pImpulsiveVariationPercentage, string pReferenceID, string pReferenceType, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
						TcMeasurementsDeviationDetectorAgent();
						~TcMeasurementsDeviationDetectorAgent();

						void fSetLastSampleRead(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pLastSampleRead);
						chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> fGetLastSampleRead();


						virtual int fRun();
						virtual int fGetData(int pMovingWindow, vector<TcStatisticTestMeasures>* pMeasuredValues);
						void fTwoSideCUSUM(vector<double> pValues, double pDecisionLikleyhoodThreshold, double pChangeMagnitude, double pMeanUnchanged, double pVarianceUnchanged, int* pEstimatedNchanged, int* pNdetected);
						int fNotifyDeviation(TcStatisticTestMeasures pStatisticTestMeasures, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation);
						void fPrintDeviation(TcStatisticTestMeasures pStatisticTestMeasures, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation);
						void fPrintDebugDeviation(TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation);
						int fRegisterDeviationData(string pAlgorithmName, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation, TcStatisticTestMeasures pStatisticTestMeasures);
				};
			}
		}
	}
}

#endif