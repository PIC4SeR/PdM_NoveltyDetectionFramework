#include <stdlib.h>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include "../../../include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h"
#include "../../../include/TcLinearRegressor.h"





TcErrorDegradationTimeEstimator::TcErrorDegradationTimeEstimator(int pNumSamplesRead, unsigned int pSystemFeatureToControl, double pPredictedTimeError, double pMinOperativeThresholdError, double pMaxOperativeThresholdError, int pMinNumOfRegrSamples, chrono::milliseconds pPreventionThresholdTime, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped){
	
	this->rmNumSamplesRead = pNumSamplesRead;
	this->rmSystemFeatureToControl = pSystemFeatureToControl;

	this->rmMinOperativeThresholdError = pMinOperativeThresholdError;
	this->rmMaxOperativeThresholdError = pMaxOperativeThresholdError;
	this->rmMinNumOfRegrSamples = pMinNumOfRegrSamples;
	
	this->rmPreventionThresholdTime = pPreventionThresholdTime;
	
	this->rmPredictedTimeError = pPredictedTimeError;
	this->rmLastPredictedTimeToError = chrono::milliseconds(0);
	this->rmLastPredictedTimeToErrorTime = chrono::system_clock::now();

}

TcErrorDegradationTimeEstimator::~TcErrorDegradationTimeEstimator() { ; }

int TcErrorDegradationTimeEstimator::fRun() {
	int rResult = 0;

    vector<unsigned long> rTimes;
	vector<double> rErrors;
    unsigned long rPrediction = 0;

	fprintf(stdout, "(%s) Enter in %s \n", __func__, __func__);
	fflush(stdout);

	//Get Last Errors to evaluate 
	rResult = fGetData(&rTimes, &rErrors);
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "(%s) Get data from database fails with error %d"  ANSI_COLOR_RESET "\n",  __func__, rResult);
		fflush(stdout);
		return(kRunFails);
	}

	//Last Error Sample Value lower than a Minimum Error Threshold 
	if(rErrors.size() > 0 && rErrors.back() < this->rmMinOperativeThresholdError){
		fprintf(stdout, ANSI_COLOR_GREEN "(%s) Last Error is lower than Minimum Error Operative Threshold, prediction will be skipped"  ANSI_COLOR_RESET "\n",  __func__);
		fflush(stdout);
	} //Last Error Sample Value is higher than a Maximum Error Threshold 
	else if(rErrors.size() > 0 && rErrors.back() > this->rmMaxOperativeThresholdError){
		fprintf(stdout, ANSI_COLOR_RED "(%s) Last Error is lower than Minimum Error Operative Threshold, prediction will be skipped"  ANSI_COLOR_RESET "\n",  __func__);
		fflush(stdout);
	} else{
		//Time and Samples data have not the same size (no linreg possible) or are both null
		if(rTimes.size() != rErrors.size() || (!rTimes.size() && !rErrors.size())){
			fprintf(stdout, ANSI_COLOR_RED "(%s) Times and Samples Data invalid sizes, prediction will be skipped"  ANSI_COLOR_RESET "\n",  __func__);
			fflush(stdout);
			return(kRunFails);
		} 
		//too few data to making accurate predictions 
		if((rErrors.size() < (size_t) this->rmMinNumOfRegrSamples)){
			fprintf(stdout, ANSI_COLOR_YELLOW "(%s) Samples Data invalid sized, prediction will be skipped"  ANSI_COLOR_RESET "\n",  __func__);
			fflush(stdout);
		} else {

			fprintf(stdout, ANSI_COLOR_YELLOW "(%s) Prediction"  ANSI_COLOR_RESET "\n",  __func__);
			fflush(stdout);

			TcLinearRegressor<double, unsigned long> lr;
			double rError = this->rmPredictedTimeError;
			
			lr.fTrain(rErrors, rTimes);
			lr.fPredict(rError, &rPrediction);
			
			chrono::system_clock::time_point cPredictedTimeOfError = chrono::time_point<chrono::system_clock, chrono::milliseconds>(chrono::milliseconds(rPrediction));
			chrono::milliseconds cPredictedTimeToError = chrono::duration_cast<chrono::milliseconds>(cPredictedTimeOfError - chrono::system_clock::now());		
			chrono::system_clock::time_point cPredictedTimeToErrorTime = chrono::system_clock::now();

			fprintf(stdout, ANSI_COLOR_YELLOW "(%s) Reach error %f next %ldh | %ldm | %lds"  ANSI_COLOR_RESET "\n",  __func__, rError, chrono::duration_cast<chrono::hours>(cPredictedTimeToError).count(), chrono::duration_cast<chrono::minutes>(cPredictedTimeToError).count(), chrono::duration_cast<chrono::seconds>(cPredictedTimeToError).count());
			fprintf(stdout, "(%s) time predicted error %f, predicted time of error = %ld\n", __func__, rError, rPrediction);
			fflush(stdout);

			//Time to Error is over prevention thresholds, notification is needed
			if(cPredictedTimeToError > this->rmPreventionThresholdTime){
				fprintf(stdout, ANSI_COLOR_RED "(%s) Notification for maintainers"  ANSI_COLOR_RESET "\n",  __func__);
				fflush(stdout);
			}


			this->rmLastPredictedTimeToError = cPredictedTimeToError;
			this->rmLastPredictedTimeToErrorTime = cPredictedTimeToErrorTime;

			
			
		}
	}

	fprintf(stdout, "(%s) Exit from %s \n", __func__, __func__);
	fflush(stdout);

	return(kRunSuccess);
}

int TcErrorDegradationTimeEstimator::fGetData(vector<unsigned long>* pTimes, vector<double>* pErrors) {

	vector<unsigned long> rTimes;
	vector<double> rErrors;

	fprintf(stdout, "(%s) Enter in %s \n", __func__, __func__);
	fflush(stdout);

	int rResult = cmMongoInterface->fConnectDriver();
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "(%s) Connection to Mongo Driver fails with error %d\n" ANSI_COLOR_RESET, __func__, rResult);
		fflush(stdout);
		return(kGetDataFails);
	}

	/*
	long long rFromMilliseconds = (long long)chrono::duration_cast<chrono::milliseconds>(cmLastSampleRead.time_since_epoch()).count();
	long long rToMilliseconds = (long long) chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	*/

	list<string> cDataOut;
	if ((rResult = this->cmMongoInterface->fGetData(&cDataOut, this->rmDatabaseName, this->rmCollectionName, "Predicted_Feature", to_string(this->rmSystemFeatureToControl), "Last_Testset_Time", this->rmNumSamplesRead, 0, "", list<string>({"Errors","Last_Testset_Time"}))) < 0) {
		fprintf(stdout, ANSI_COLOR_RED "(%s) Get data from database fails with error %d"  ANSI_COLOR_RESET "\n",  __func__, rResult);
		fflush(stdout);
		cmMongoInterface->fDisconnectDriver();
		return(kGetDataFails);
	}
	
	cmMongoInterface->fDisconnectDriver();

	for(string rStringError : cDataOut) {
		bsoncxx::document::value rError = bsoncxx::from_json(rStringError);
		double rErrorValue = rError.view()["Errors"]["MSE"].get_double().value;
		//auto c = chrono::time_point<chrono::system_clock, chrono::milliseconds>(rError.view()["Last_Testset_Time"].get_date().value);
		unsigned long rErrorTimeMilliseconds = (unsigned long) chrono::time_point<chrono::system_clock, chrono::milliseconds>(rError.view()["Last_Testset_Time"].get_date().value).time_since_epoch().count();
		rErrors.push_back(rErrorValue);
		rTimes.push_back(rErrorTimeMilliseconds);
	}

	*pErrors = rErrors;
	*pTimes = rTimes;

	fprintf(stdout, "(%s) Exit from %s \n", __func__, __func__);
	fflush(stdout);


	return(kGetDataSuccess);
}




/*
int TcThresholdsAgent::fThreasholdCompare(float pValue) {

	if (pValue > this->rmAlarmThresholdHigh) {
		return(TcThresholdsAgent::kMeasureOverAlarmThreshold);
	} else if (pValue > this->rmWarningThresholdHigh) {
		return(TcThresholdsAgent::kMeasureOverWarningThreshold);
	}

	if (pValue < this->rmAlarmThresholdLow) {
		return(TcThresholdsAgent::kMeasureUnderAlarmThreshold);
	} else if (pValue < this->rmWarningThresholdLow) {
		return(TcThresholdsAgent::kMeasureUnderWarningThreshold);
	}

	return(TcThresholdsAgent::kMeasureInRange);
}



void TcThresholdsAgent::fGetNotificationByComparing(int pComparingResult, float pValue, float pAlarmThresholdHigh, float pAlarmThresholdLow, string *pNotificationBody, string* pNotificationType, string* pNotificationTitle) {

	if (pComparingResult != TcThresholdsAgent::kMeasureOverAlarmThreshold && pComparingResult != TcThresholdsAgent::kMeasureOverWarningThreshold && pComparingResult != TcThresholdsAgent::kMeasureUnderWarningThreshold && pComparingResult != TcThresholdsAgent::kMeasureUnderAlarmThreshold) {
		*pNotificationBody = "";
		*pNotificationType = "";
		*pNotificationTitle = "";
		return;
	}

	char aStringRoundedValue[16];
	char aStringRoundedAlarmThresholdLow[16];
	char aStringRoundedAlarmThresholdHigh[16];
	
	snprintf(aStringRoundedValue, 16, "%.1f", pValue);
	snprintf(aStringRoundedAlarmThresholdLow, 16, "%.1f", pAlarmThresholdLow);
	snprintf(aStringRoundedAlarmThresholdHigh, 16, "%.1f", pAlarmThresholdHigh);

	string rRange = "Alarm Thresholds: [" + string(aStringRoundedAlarmThresholdLow) + ", " + string(aStringRoundedAlarmThresholdHigh) + "]";
	string rTailDefaultBody = "Value: " + string(aStringRoundedValue) + "\n" + rRange;

	switch (pComparingResult) {
	case TcThresholdsAgent::kMeasureUnderAlarmThreshold:
		*pNotificationBody = "Under minimum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Alarm";
		*pNotificationTitle = "Alarm ";
		break;
	case TcThresholdsAgent::kMeasureUnderWarningThreshold:
		*pNotificationBody = "Close to minimum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Warning";
		*pNotificationTitle = "Warning ";
		break;
	case TcThresholdsAgent::kMeasureOverAlarmThreshold:
		*pNotificationBody = "Over maximum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Alarm";
		*pNotificationTitle = "Alarm ";
		break;
	case TcThresholdsAgent::kMeasureOverWarningThreshold:
		*pNotificationBody = "Close to maximum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Warning";
		*pNotificationTitle = "Warning ";
		break;
	default:
		*pNotificationBody = "";
		*pNotificationType = "";
		*pNotificationTitle = "";
	}

	return;
}

void TcThresholdsAgent::fAdjustNotificatedThresholds(float* pMeasureValue, float* pMeasureAlarmThresholdHigh, float* pMeasureAlarmThresholdLow) {
	return;
}

*/

