#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <boost/serialization/serialization.hpp>

#include "../include/IMongoDriverAgentInterface.h"
#include "../include/TcMongoDriver.h"
#include "../include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h"



IMongoDriverAgentInterface::IMongoDriverAgentInterface(string pMongoDriverName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort){
    this->cmMongoDriver = new TcMongoDriver(pMongoDriverName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort);
}

IMongoDriverAgentInterface::IMongoDriverAgentInterface() {
    this->cmMongoDriver = new TcMongoDriver();
}

IMongoDriverAgentInterface::~IMongoDriverAgentInterface(){
    if (this->cmMongoDriver != nullptr) {
        delete this->cmMongoDriver;
        this->cmMongoDriver = nullptr;
    }
}

TcMongoDriver* IMongoDriverAgentInterface::fGetDriver() {
    return(this->cmMongoDriver);
}


int IMongoDriverAgentInterface::fGetLastErrors(list<double> *pErrors, list<long long> *pTimes, string pErrorType, string pDatabase, string pCollection, string pFilterattribute, string pFiltervalue, string pSortattribute, int pLimit, int pSkip, string pGroupattribute, list<string> pProjectionattributes){
    list<string> cOutputList;
    list<long long> rTimes;
	list<double> rErrors;
	int rResult = 0;
    
    string rSortString;
    string rProjectionString;
    string rFilterString;

    fprintf(stdout, "(%s) Enter in %s \n", __func__, __func__);
	fflush(stdout);


    if (pFilterattribute == "") {
        rFilterString = "";
    } else {
        rFilterString = "{ \"" + pFilterattribute + "\" : " + pFiltervalue + " }";
    }

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : -1 }";
    }

    if (pProjectionattributes.size() == 0) {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, ";
        for(string rProjectionattribute : pProjectionattributes){
            rProjectionString = rProjectionString + "\"" + rProjectionattribute + "\" : 1";
            if(rProjectionattribute != pProjectionattributes.back()){
                rProjectionString = rProjectionString + ", "; 
            }
        }
        rProjectionString = rProjectionString + "}";
    }
    
    

	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
                                        0,
										pLimit
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    for(string rStringError : cOutputList) {
		bsoncxx::document::value rError = bsoncxx::from_json(rStringError);
		double rErrorValue = rError.view()[TcErrorDegradationTimeEstimator::kErrorsAttribute][pErrorType].get_double().value;
		rErrors.push_back(rErrorValue);

		rTimes.push_back(rError.view()[TcErrorDegradationTimeEstimator::kLastTestTimeAttribute].get_date().value.count());
	}

	*pErrors = rErrors;
	*pTimes = rTimes;


    return(IMongoDriverAgentInterface::kGetSuccess);
}



int IMongoDriverAgentInterface::fInsertPrediction(string pDatabase, string pCollection, chrono::system_clock::time_point pAgentStartTime, long long pLastErrorTime, double pLastError, long long pPrediction, double pMcoefficient, double pQoffset, chrono::system_clock::time_point pStartTrainTime, chrono::system_clock::time_point pEndTrainTime, chrono::system_clock::time_point pEndPredictionTime, chrono::system_clock::time_point pPredictedTimeOfError, chrono::milliseconds pPredictedTimeToError, int pPredictor){
		bsoncxx::document::view_or_value cBsonDocument = bsoncxx::builder::stream::document{} 
		<< TcErrorDegradationTimeEstimator::kActualErrorTime << bsoncxx::types::b_date{chrono::time_point<chrono::system_clock, chrono::milliseconds>(chrono::milliseconds(pLastErrorTime))}
		<< TcErrorDegradationTimeEstimator::kAgentStartTime << bsoncxx::types::b_date{ pAgentStartTime }
		<< TcErrorDegradationTimeEstimator::kTrainStartTime << bsoncxx::types::b_date{ pStartTrainTime }
		<< TcErrorDegradationTimeEstimator::kTrainEndTime << bsoncxx::types::b_date{ pEndTrainTime }
		<< TcErrorDegradationTimeEstimator::kPredictEndTime << bsoncxx::types::b_date{ pEndPredictionTime }
		<< TcErrorDegradationTimeEstimator::kAgentEndTime << bsoncxx::types::b_date{ chrono::system_clock::now() }
		<< TcErrorDegradationTimeEstimator::kEstimDegradTime << bsoncxx::types::b_date{ pPredictedTimeOfError }
		<< TcErrorDegradationTimeEstimator::kRemainingTime << bsoncxx::types::b_int64{ pPredictedTimeToError.count()}
		<< TcErrorDegradationTimeEstimator::kPredictor << bsoncxx::types::b_int64{pPredictor} 
		<< TcErrorDegradationTimeEstimator::kActualError << bsoncxx::types::b_double{pLastError}
		<< TcErrorDegradationTimeEstimator::kM << bsoncxx::types::b_double{ pMcoefficient} 
		<< TcErrorDegradationTimeEstimator::kQ << bsoncxx::types::b_double{ pQoffset }
		<< bsoncxx::builder::stream::finalize;
			
			
		if(this->cmMongoDriver->fInserDocument(pDatabase, pCollection, cBsonDocument) < 0 ){
			fprintf(stdout, ANSI_COLOR_YELLOW "(%s) Error on prediction insertion" ANSI_COLOR_RESET "\n", __func__);
			fflush(stdout);
			return(kErr_Insert);
		}

        return(kInsert_Ok);
}

int IMongoDriverAgentInterface::fGetLastConfiguration(string pDatabase, string pCollection, string pSortattribute, string pAgentId, int *pNumSamplesRead, unsigned int *pPredictor, string *pPredictedErrorType, double *pPredictedErrorValue, double *pMinOperativeThresholdError, double *pMaxOperativeThresholdError, int *pMinNumOfRegrSamples, chrono::milliseconds *pPreventionThresholdTime, string *pTestResultCollection, string *pPredictionResultCollection, string *pConfigurationCollection, string *pDatabaseName, string *pMongoDriverRemoteConnectionType, string *pMongoDriverRemoteConnectionHost, uint16_t *pMongoDriverRemoteConnectionPort, string *pAgentID, string *pAgentname, chrono::microseconds *pStepRunTime, chrono::time_point<chrono::high_resolution_clock> *pNextRunTime, int *pPriority, atomic<bool> *pStopped)
{
    int rNumSamplesRead = 0;
    unsigned int rPredictor = 0;
    string rPredictedErrorType = "";
    double rPredictedErrorValue = 0.00;
    double rMinOperativeThresholdError = 0.00;
    double rMaxOperativeThresholdError = 0.00;
    int rMinNumOfRegrSamples = 0;
    chrono::milliseconds cPreventionThresholdTime;
    string rTestResultCollection = "";
    string rPredictionResultCollection = "";
    string rDatabaseName = "";
    string rMongoDriverRemoteConnectionType = "";
    string rMongoDriverRemoteConnectionHost = "";
    uint16_t rMongoDriverRemoteConnectionPort;
    string rAgentID = "";
    string rAgentname = "";
    chrono::microseconds cStepRunTime;
    chrono::time_point<chrono::high_resolution_clock> cNextRunTime;
    int rPriority = 0;
    bool rStopped = false;

    list<string> cOutputList;
    list<long long> rTimes;
	list<double> rErrors;
	int rResult = 0;
    
    string rSortString = "";

    fprintf(stdout, "(%s) Enter in %s \n", __func__, __func__);
	fflush(stdout);

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : -1 }";
    }
        

	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    "",
										"",
										rSortString,
                                        0,
										1
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kGetConfigurationFails);
    }

    for(string rConfiguration : cOutputList) {

		bsoncxx::document::value cConfiguration = bsoncxx::from_json(rConfiguration);

        *pTestResultCollection = string(cConfiguration.view()[TcErrorDegradationTimeEstimator::kTestResultCollection].get_utf8().value.to_string());
        *pConfigurationCollection = string(cConfiguration.view()[TcErrorDegradationTimeEstimator::kConfigurationCollection].get_utf8().value.to_string());

        auto cAgentConfiguration = cConfiguration.view()[TcErrorDegradationTimeEstimator::kConfigurationAgents][pAgentId];

        if (cAgentConfiguration){
            *pNumSamplesRead = cAgentConfiguration[TcErrorDegradationTimeEstimator::kNumSamplesRead].get_int32().value;
            
            *pPredictor = (unsigned int) cAgentConfiguration[TcErrorDegradationTimeEstimator::kAgentPredictor].get_int32().value;
            *pPredictedErrorType = string(cAgentConfiguration[TcErrorDegradationTimeEstimator::kPredictedErrorType].get_utf8().value.to_string());
            
            auto m = cAgentConfiguration[TcErrorDegradationTimeEstimator::kMinOpeThresholdError].get_double().value;
                          
            *pMinOperativeThresholdError = (double) cAgentConfiguration[TcErrorDegradationTimeEstimator::kMinOpeThresholdError].get_double().value;
            *pMaxOperativeThresholdError = cAgentConfiguration[TcErrorDegradationTimeEstimator::kMaxOpeThresholdError].get_double().value;
            *pMinNumOfRegrSamples = cAgentConfiguration[TcErrorDegradationTimeEstimator::kkMinNumRegrSamples].get_int32().value;
            *pPredictedErrorValue = cAgentConfiguration[TcErrorDegradationTimeEstimator::kPredictedErrorValue].get_double().value;

            *pPreventionThresholdTime = chrono::milliseconds(cAgentConfiguration[TcErrorDegradationTimeEstimator::kPreventionThresholdTime].get_int64().value);
            *pPredictionResultCollection = string(cAgentConfiguration[TcErrorDegradationTimeEstimator::kPredictionResultCollection].get_utf8().value.to_string());
            *pDatabaseName = string(cAgentConfiguration[TcErrorDegradationTimeEstimator::kDatabaseName].get_utf8().value.to_string());
            *pMongoDriverRemoteConnectionType = string(cAgentConfiguration[TcErrorDegradationTimeEstimator::kMongoDriverRemoteConnectionType].get_utf8().value.to_string());
            *pMongoDriverRemoteConnectionHost = string(cAgentConfiguration[TcErrorDegradationTimeEstimator::kMongoDriverRemoteConnectionHost].get_utf8().value.to_string());
            *pMongoDriverRemoteConnectionPort = cAgentConfiguration[TcErrorDegradationTimeEstimator::kMongoDriverRemoteConnectionPort].get_int32().value;
            *pAgentID = string(cAgentConfiguration[TcErrorDegradationTimeEstimator::kAgentId].get_utf8().value.to_string());
            *pAgentname = string(cAgentConfiguration[TcErrorDegradationTimeEstimator::kAgentName].get_utf8().value.to_string());
            *pStepRunTime = chrono::milliseconds(cAgentConfiguration[TcErrorDegradationTimeEstimator::kStepRunTime].get_int32().value);
            *pNextRunTime = chrono::system_clock::now();
            *pPriority = cAgentConfiguration[TcErrorDegradationTimeEstimator::kPriority].get_int32().value;
            (*pStopped).store(cAgentConfiguration[TcErrorDegradationTimeEstimator::kStopped].get_bool().value);
        }
        else {
            return(IMongoDriverAgentInterface::kGetConfigurationFails);        
        }
	}

    return(IMongoDriverAgentInterface::kGetConfigurationSuccess);
}

int IMongoDriverAgentInterface::fGetDataMaxOf(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMaxattribute, string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"max\" : 1 }";
    }


    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }


	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }


    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataMinOf(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMinattribute, string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"min\" : 1 }";
    }


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

   
	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }


    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}



int IMongoDriverAgentInterface::fGetDataAvgOf(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pAvgattribute,  string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if(pSortattribute == ""){
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";

    if(pProjectionattribute == ""){
        rProjectionString = "";
    } else {
        rProjectionString = "{ \"_id\" : 0, \"average\" : 1 }";
    }
    

    if(pAvgattribute == ""){
        rGroupString = "";
    } else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }



	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);
	
    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}



int IMongoDriverAgentInterface::fGetDataSumOf(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pSumattribute,  string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    } else {
        rProjectionString = "{ \"_id\" : 0, \"sum\" : 1 }";
    }


    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }


	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }


    
    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataBetweenValues(list<string> *pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound,  string pSortattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
    
    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";

    if(pProjectionattribute == ""){
        rProjectionString = "";
    } else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }

	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString, 
                                        rSortString
    );

    if (rResult < 0) {
        pOutput = nullptr;
        return(IMongoDriverAgentInterface::kQueryFails);
    }

	*pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataGreatherThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue,  string pSortattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
    
    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;


    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        rSortString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

	*pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataLowerThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSortattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
   
    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";

    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }

	
	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
                                        rSortString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

	*pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataAvgOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pAvgattribute) {
    list<string> cOutputList;
    int rResult = 0;

   
    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "";


    if (pAvgattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }


    rProjectionString = "{ \"_id\" : 0 }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );


    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataAvgOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pAvgattribute == "") {
        rGroupString = "";
    } else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }


    rProjectionString = "{ \"_id\" : 0 }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetDataAvgOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pAvgattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0 }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetDataMaxOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMaxattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetDataMaxOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}


int IMongoDriverAgentInterface::fGetDataMaxOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";

    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetDataMinOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMinattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null,  \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}


int IMongoDriverAgentInterface::fGetDataMinOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetDataMinOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetDataSumOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pSumattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";


    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        "",
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataSumOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";

    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataSumOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";

    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetDataCountOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;
   

    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";
    rGroupString = "{\"_id\" : null, \"count\" : { \"$sum\" : 1 } }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);



}


int IMongoDriverAgentInterface::fGetDataCountOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";
    rGroupString = "{\"_id\" : null, \"count\" : { \"$sum\" : 1 } }";
    

    rResult = cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );



   
    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataCountOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";
    rGroupString = "{\"_id\" : null, \"count\" : { \"$sum\" : 1} }";
 

    
    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}



int IMongoDriverAgentInterface::fGetDataBetweenTimePoints(list<string>* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from,  string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;
    
    
    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document 
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo)) 
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom)) 
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;
    
    
    
    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    } else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sortattribute == "") {
        cSortDocument = bsoncxx::document::view_or_value();
    } else {
        cSortDocument = bsoncxx::builder::stream::document{} << sortattribute << 1 << "_id" << 1 << bsoncxx::builder::stream::finalize;
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        cSortDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataAfterTimePoint(list<string>* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after,  string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sortattribute == "") {
        cSortDocument = bsoncxx::document::view_or_value();
    }
    else {
        cSortDocument = bsoncxx::builder::stream::document{} << sortattribute << 1 << "_id" << 1 << bsoncxx::builder::stream::finalize;
    }


    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        cSortDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetDataBeforeTimePoint(list<string>* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before,  string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sortattribute == "") {
        cSortDocument = bsoncxx::document::view_or_value();
    }
    else {
        cSortDocument = bsoncxx::builder::stream::document{} << sortattribute << 1 << "_id" << 1 << bsoncxx::builder::stream::finalize;
    }


    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        cSortDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataMinBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string minattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (minattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$min" << "$" + minattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataMinAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string minattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (minattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$min" << "$" + minattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataMinBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string minattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);

    
    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (minattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$min" << "$" + minattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}




int IMongoDriverAgentInterface::fGetDataMaxBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string maxattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (maxattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "max" << bsoncxx::builder::stream::open_document << "$max" << "$" + maxattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataMaxAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string maxattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (after - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (maxattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "max" << bsoncxx::builder::stream::open_document << "$max" << "$" + maxattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataMaxBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string maxattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    } else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (maxattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "max" << bsoncxx::builder::stream::open_document << "$max" << "$" + maxattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataSumBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string sumattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    } else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sumattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    } else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "sum" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataSumAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string sumattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sumattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "sum" << bsoncxx::builder::stream::open_document << "$sum" << "$" + sumattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetDataSumBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string sumattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sumattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "sum" << bsoncxx::builder::stream::open_document << "$sum" << "$" + sumattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetDataAvgBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string avgattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (avgattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "avg" << bsoncxx::builder::stream::open_document << "$avg" << "$" + avgattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetDataAvgAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string avgattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (avgattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "avg" << bsoncxx::builder::stream::open_document << "$avg" << "$" + avgattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetDataAvgBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string avgattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (avgattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "avg" << bsoncxx::builder::stream::open_document << "$avg" << "$" + avgattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}



int IMongoDriverAgentInterface::fGetDataCountBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

   
    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "count" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetDataCountAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

   
    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "count" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
  

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetDataCountBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
















                
                

