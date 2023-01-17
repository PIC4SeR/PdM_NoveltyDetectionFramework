
#ifndef TCERRORDEGRADATIONTIMEESTRIMATOR_H
#define TCERRORDEGRADATIONTIMEESTRIMATOR_H

#include <string>
#include <chrono>
#include <vector>

#include "../TcAgent.h"

using namespace std;

class TcErrorDegradationTimeEstimator : public TcAgent
{

private:

	bool rmDBConfigEnable;
	int rmNumSamplesRead;
	double rmMinOperativeThresholdError;
	double rmMaxOperativeThresholdError;
	int rmMinNumOfRegrSamples;

	unsigned int rmPredictor;
	string rmPredictedErrorType;
	double rmPredictedErrorValue;

	chrono::milliseconds rmNotificationPreventionThresholdTime;
	chrono::milliseconds rmLastPredictedTimeToError;
	chrono::system_clock::time_point rmLastPredictedTimeToErrorTime;
	chrono::system_clock::time_point rmLastSampleTime;

	string rmTestResultCollection;
	string rmPredictionResultCollection;
	string rmConfigurationCollection;
	string rmConfigurationFile;


public:

	class TcError{
			public:
				class TcGetLastErrors {
        			public:
            			static const int kSuccess = 0;
						static const int kNoDataAvailable = 1;
						static const int kFail = -1;
						static const int kConnectionLost = -2;
        		};
				class TcNotifyPrediction {
        			public:
            			static const int kSuccess = 0;
						static const int kFail = -1;
						static const int kConnectionLost = -2;
        		};
				class TcMakePrediction {
        			public:
            			static const int kSuccess = 0;
						static const int kInsufficientSamples = 1;
						static const int kFail = -1;
        		};
			};

	static const string kPredictorAttribute;
	static const string kLastTestTimeAttribute;
	static const string kErrorsAttribute;

	static const string kActualErrorTime;
	static const string kAgentStartTime;
	static const string kTrainStartTime;
	static const string kTrainEndTime;
	static const string kPredictEndTime;
	static const string kAgentEndTime;
	static const string kEstimDegradTime;
	static const string kRemainingTime;
	static const string kPredictor;
	static const string kActualError;
	static const string kM;
	static const string kQ;

	static const string kNumSamplesRead;
	static const string kPredictedErrorType;
	static const string kPredictedErrorValue;
	static const string kMinOpeThresholdError;
	static const string kMaxOpeThresholdError;
	static const string kkMinNumRegrSamples;
	static const string kPreventionThresholdTime;
	static const string kTestResultCollection;
	static const string kPredictionResultCollection;
	static const string kDatabaseName;
	static const string kMongoDriverRemoteConnectionType;
	static const string kMongoDriverRemoteConnectionHost;
	static const string kMongoDriverRemoteConnectionPort;
	static const string kAgentId;
	static const string kAgentName;
	static const string kStepRunTime;
	static const string kNextRunTime;
	static const string kPriority;
	static const string kStopped;
	static const string kConfigurationCollection;
	static const string kConfigurationAgents;
	static const string kConfTimestamp;
	static const string kAgentPredictor;



	TcErrorDegradationTimeEstimator(bool pLocalFileConfigEnable, bool pLocalConfigEnable, string pLocalConfigFile, string pDatabaseName,  string pConfigurationCollection, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, int pNumSamplesRead, unsigned int pPredictor, string pPredictedErrorType, double pPredictedErrorValue, double pMinOperativeThresholdError, double pMaxOperativeThresholdError, int pMinNumOfRegrSamples, chrono::milliseconds pPreventionThresholdTime, string pTestResultCollection, string pPredictionResultCollection, string pAgentname, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime = chrono::high_resolution_clock::now(), TcAgent::Priority pPriority = Priority::Medium, bool pStopped = false);
	~TcErrorDegradationTimeEstimator();

	virtual int fRun();
	int fGetLastErrors(list<long long> *pTimes, list<double> *pErrors);
	int fGetLastConfigurationFromDatabase();
	int fGetLastConfigurationFromFile();
	void fMakePrediction(list<long long> pTimes, list<double> pErrors, long long *pPrediction, double *pMcoefficient, double* pQoffset, chrono::system_clock::time_point* pStartTrainTime, chrono::system_clock::time_point* pEndTrainTime, chrono::system_clock::time_point* pEndPredictionTime, chrono::system_clock::time_point* pPredictedTimeOfError, chrono::milliseconds* pPredictedTimeToError);
	int fNotifyPrediction(chrono::system_clock::time_point pAgentStartTime, long long pLastErrorTime, double pLastError, long long pPrediction, double pMcoefficient, double pQoffset, chrono::system_clock::time_point pStartTrainTime, chrono::system_clock::time_point pEndTrainTime, chrono::system_clock::time_point pEndPredictionTime, chrono::system_clock::time_point pPredictedTimeOfError, chrono::milliseconds pPredictedTimeToError);
};





#endif // AGENT_H