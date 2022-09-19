#include <stdlib.h>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>




#include "../../../include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h"
#include "../../../include/TcLinearRegressor.h"

TcErrorDegradationTimeEstimator::TcErrorDegradationTimeEstimator(int pNumSamplesRead, unsigned int pPredictor, string pPredictedErrorType, double pPredictedErrorValue, double pMinOperativeThresholdError, double pMaxOperativeThresholdError, int pMinNumOfRegrSamples, chrono::milliseconds pPreventionThresholdTime, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped){
	
	this->rmNumSamplesRead = pNumSamplesRead;
	this->rmPredictor = pPredictor;

	this->rmPredictedErrorType = pPredictedErrorType;
	this->rmPredictedErrorValue = pPredictedErrorValue;

	this->rmMinOperativeThresholdError = pMinOperativeThresholdError;
	this->rmMaxOperativeThresholdError = pMaxOperativeThresholdError;
	this->rmMinNumOfRegrSamples = pMinNumOfRegrSamples;
	
	this->rmNotificationPreventionThresholdTime = pPreventionThresholdTime;
	
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
			double rError = this->rmPredictedErrorValue;
			
			lr.fTrain(rErrors, rTimes);
			lr.fPredict(rError, &rPrediction);
			
			chrono::system_clock::time_point cPredictedTimeOfError = chrono::time_point<chrono::system_clock, chrono::milliseconds>(chrono::milliseconds(rPrediction));
			chrono::milliseconds cPredictedTimeToError = chrono::duration_cast<chrono::milliseconds>(cPredictedTimeOfError - chrono::system_clock::now());		
			chrono::system_clock::time_point cPredictedTimeToErrorTime = chrono::system_clock::now();



			fprintf(stdout, ANSI_COLOR_YELLOW "(%s) Reachs error %f next %ldh | %ldm | %lds"  ANSI_COLOR_RESET "\n",  __func__, rError, chrono::duration_cast<chrono::hours>(cPredictedTimeToError).count(), chrono::duration_cast<chrono::minutes>(cPredictedTimeToError).count(), chrono::duration_cast<chrono::seconds>(cPredictedTimeToError).count());
			fprintf(stdout, "(%s) time predicted error %f, predicted time of error = %ld\n", __func__, rError, rPrediction);
			fflush(stdout);

			int rResult = cmMongoInterface->fConnectDriver();
			if (rResult < 0) {
				fprintf(stdout, ANSI_COLOR_RED "(%s) Connection to Mongo Driver fails with error %d\n" ANSI_COLOR_RESET, __func__, rResult);
				fflush(stdout);
				return(kGetDataFails);
			}

			bsoncxx::document::view_or_value cBsonDocument = bsoncxx::builder::stream::document{} << "LastSampleTimeValue" << bsoncxx::types::b_date{chrono::time_point<chrono::system_clock, chrono::milliseconds>(chrono::milliseconds(rTimes.back()))} << "PredictedTimeValue" << bsoncxx::types::b_date{cPredictedTimeOfError} << "Predictor" << bsoncxx::types::b_int64{this->rmPredictor} << "Error" << bsoncxx::types::b_double{this->rmPredictedErrorValue} << bsoncxx::builder::stream::finalize;
			if(this->cmMongoInterface->fGetDriver()->fInserDocument(this->rmDatabaseName, "PredictedResult", cBsonDocument) < 0 ){
				fprintf(stdout, ANSI_COLOR_YELLOW "(%s) Error on prediction insertion" ANSI_COLOR_RESET "\n", __func__);
				fflush(stdout);
				cmMongoInterface->fDisconnectDriver();
				return(kRunFails);
			}

			cmMongoInterface->fDisconnectDriver();

			//Time to Error is over prevention thresholds, notification is needed
			if(cPredictedTimeToError > this->rmNotificationPreventionThresholdTime){
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

	list<string> cDataOut;
	if ((rResult = this->cmMongoInterface->fGetData(&cDataOut, this->rmDatabaseName, this->rmCollectionName, TcErrorDegradationTimeEstimator::kPredictorAttribute, to_string(this->rmPredictor), TcErrorDegradationTimeEstimator::kLastTestTimeAttribute, this->rmNumSamplesRead, 0, "", list<string>({TcErrorDegradationTimeEstimator::kErrorsAttribute,TcErrorDegradationTimeEstimator::kLastTestTimeAttribute}))) < 0) {
		fprintf(stdout, ANSI_COLOR_RED "(%s) Get data from database fails with error %d"  ANSI_COLOR_RESET "\n",  __func__, rResult);
		fflush(stdout);
		cmMongoInterface->fDisconnectDriver();
		return(kGetDataFails);
	}
	
	cmMongoInterface->fDisconnectDriver();

	for(string rStringError : cDataOut) {
		bsoncxx::document::value rError = bsoncxx::from_json(rStringError);
		double rErrorValue = rError.view()[TcErrorDegradationTimeEstimator::kErrorsAttribute][this->rmPredictedErrorType].get_double().value;
		unsigned long rErrorTimeMilliseconds = (unsigned long) chrono::time_point<chrono::system_clock, chrono::milliseconds>(rError.view()[TcErrorDegradationTimeEstimator::kLastTestTimeAttribute].get_date().value).time_since_epoch().count();
		rErrors.push_back(rErrorValue);
		rTimes.push_back(rErrorTimeMilliseconds);
	}

	*pErrors = rErrors;
	*pTimes = rTimes;

	fprintf(stdout, "(%s) Exit from %s \n", __func__, __func__);
	fflush(stdout);


	return(kGetDataSuccess);
}


const string TcErrorDegradationTimeEstimator::kPredictorAttribute = "Predicted_Feature";
const string TcErrorDegradationTimeEstimator::kErrorsAttribute = "Errors";
const string TcErrorDegradationTimeEstimator::kLastTestTimeAttribute = "Last_Testset_Time";
