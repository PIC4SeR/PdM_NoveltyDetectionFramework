#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <boost/serialization/serialization.hpp>
#include "../include/IMongoDriverAgentInterface.h"
#include "../include/TcMongoDriver.h"


IMongoDriverAgentInterface::IMongoDriverAgentInterface(string pMongoDriverName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort){
    this->cmMongoDriver = new TcMongoDriver(pMongoDriverName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort);
}

IMongoDriverAgentInterface::~IMongoDriverAgentInterface(){
    if (this->cmMongoDriver != nullptr) {
        delete this->cmMongoDriver;
        this->cmMongoDriver = nullptr;
    }
}

int IMongoDriverAgentInterface::fConnectMongoDriver() {

    if (this->cmMongoDriver == nullptr) {
        return(kConnectionFails);
    }
    int rResult = this->cmMongoDriver->fConnect();
    if (rResult < 0) {
        return(kConnectionFails);
    }
    return(kConnectionSuccess);
}

void IMongoDriverAgentInterface::fDisconnectMongoDriver() {
    if (this->cmMongoDriver == nullptr) {
        return;
    }
    this->cmMongoDriver->fDisconnect();
}



string IMongoDriverAgentInterface::fComposeFilterByMeasureAttributes(string pReferenceid, string pReferencetype, string pMeasuretype) {
    bool rFilteredBefore = false;
    string rFilter = "";

    if (pReferenceid == "" && pReferencetype == "" && pMeasuretype == "") {
        return(rFilter);
    }
    
    if (pReferenceid != "") {
        rFilter += "\"ReferenceID\" : \"" + pReferenceid + "\"";
        rFilteredBefore = true;
    }
    if (pReferencetype != "") {
        if (rFilteredBefore) {
            rFilter += ",";
        }
        rFilter += "\"ReferenceType\" : \"" + pReferencetype + "\"";
        rFilteredBefore = true;
    }
    if (pMeasuretype != "") {
        if (rFilteredBefore) {
            rFilter += ",";
        }
        rFilter += "\"MeasureType\" : \"" + pMeasuretype + "\"";
    }

    return(rFilter);
}


void IMongoDriverAgentInterface::fAddFictitiousData() {
    for (int i = 0; i < 100; i++) {
        bsoncxx::document::value Doc = bsoncxx::builder::stream::document{} << "ReferenceID" << "000001" << "ReferenceType" << "Sensor" << "Timestamp" << bsoncxx::types::b_date{ chrono::system_clock::now() } << "MeasureUnit" << "T" << "MeasureType" << "Temp" << "DrawReference" << "1" << "RelayProbeType" << "1" << "StressIndex" << "1" << "Value" << 10 << "HighLimit" << 10 << "LowLimit" << 8 << "Result" << "10" << "TPGMName" << "tpgm1" << "ProductSN" << "sn0000000001" << "Task" << "t1" << "Test" << "t1" << "TestPoint" << "t1" << "Remark" << "descr" << bsoncxx::builder::stream::finalize;
        string SDoc = bsoncxx::to_json(Doc);
        this->cmMongoDriver->fInserDocument("SpeaTestDB", "Measures", SDoc);
    }
}


int IMongoDriverAgentInterface::fGetMeasuresInBackWindow(list<string> *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pReferenceid, string pReferencetype, string pMeasuretype, string pGroupattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rProjectionString;
    string rFilterString;


    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }


    rFilterString = "{" + fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype) + "}";


   /* if (pReferencetype != "" && pMeasuretype != "") {
        rFilterString = "{\"ReferenceType\" : \"" + pReferencetype + "\", \"MeasureType\" : \"" + pMeasuretype + "\" }";
    }
    else if (pReferencetype == "" && pMeasuretype != "") {
        rFilterString = "{ \"MeasureType\" : \"" + pMeasuretype + "\" }";
    }
    else if (pReferencetype == "" && pMeasuretype == "") {
        rFilterString = "";
    }
    else if (pReferencetype != "" && pMeasuretype == "") {
        rFilterString = "{ \"ReferenceType\" : \"" + pReferencetype + "\" }";
    }*/

    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
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

	*pOutput = cOutputList;

    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetMaxOfMeasuresInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMaxattribute, string pReferenceid, string pReferencetype, string pMeasuretype, string pGroupattribute, string pProjectionattribute){
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

    rFilterString = "{" + fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype) + "}";


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

int IMongoDriverAgentInterface::fGetMinOfMeasuresInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMinattribute, string pReferenceid, string pReferencetype, string pMeasuretype, string pGroupattribute, string pProjectionattribute){
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

    rFilterString = "{" + fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype) + "}";


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



int IMongoDriverAgentInterface::fGetAvgOfMeasuresInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pAvgattribute, string pReferenceid, string pReferencetype, string pMeasuretype, string pGroupattribute, string pProjectionattribute){
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

    rFilterString = "{" + fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype) + "}";

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



int IMongoDriverAgentInterface::fGetSumOfMeasuresInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pSumattribute, string pReferenceid, string pReferencetype, string pMeasuretype, string pGroupattribute, string pProjectionattribute){
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

    rFilterString = "{" + fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype) + "}";


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

int IMongoDriverAgentInterface::fGetStatisticsOfMeasuresGreatherThanValue(list<TcStatisticTestMeasures>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, int pWindow, string pMovavgattribute, string pReferenceid, string pReferencetype, string pMeasuretype, string pSortattribute, string pGroupattribute, string pProjectionattribute) {
    list<TcStatisticTestMeasures> *cOutputList;
    int rResult = 0;

    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;
    string rGroupString;
    string rAddfieldsString;

    if (pSortattribute == "") {
        rSortString = "";
    }
    else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$gt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";



    if (pProjectionattribute == "") {
        rProjectionString = "{ \"_id\" : 0}";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }

    if (pGroupattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : \"$" + pGroupattribute + "\",\
                 \"Test\": { \"$first\" : \"$" + pGroupattribute + "\"},\
                 \"Measures\" : {\
                                    \"$push\" : \"$$ROOT\"\
                                },\
                 \"Calibration\": {\
                                     \"$push\":{\
                                                    \"Value\":{\"$divide\":[{\"$sum\":[\"$HighLimit\",\"$LowLimit\"]},2]}\
                                               }\
                                 },\
                 \"Error\": {\
                                     \"$push\":{\
                                                    \"Value\":{\"$pow\":[{\"$subtract\":[{\"$divide\":[{\"$sum\":[\"$HighLimit\",\"$LowLimit\"]},2]},\"$Value\"]},2]}\
                                               }\
                             },\
                \"Mean\": { \"$avg\": \"$Measures.Value\" },\
                \"StdDev\" : {\"$stdDevSamp\": \"$Measures.Value\"},\
                \"MSE\" : {\"$avg\": \"$Error.Value\"},\
                \"ReferenceID\": { $arrayElemAt: [ $Measures.ReferenceID, 0 ] },\
                \"ReferenceType\": { $arrayElemAt: [ $Measures.ReferencType, 0 ] }\
                }";
    }

    if (pMovavgattribute == "") {
        rAddfieldsString = "";
    }
    else {
        rAddfieldsString = "{\"Statistics\": {\
                \"$map\": {\
                    \"input\": {\"$range\": [0, { \"$subtract\": [{\"$size\":\"$Measures\"}, " + to_string(pWindow - 1) + "] }] },\
                    \"as\" : \"z\",\
                    \"in\" : {\
                        \"Mean\": { \"$avg\": {\"$slice\": [\"$Measures.Value\", \"$$z\"," + to_string(pWindow) + "] } },\
                        \"Variance\": { \"$pow\": [{ \"$stdDevSamp\": {\"$slice\": [\"$Measures.Value\", \"$$z\"," + to_string(pWindow) + "] } }, 2]},\
                        \"MSE\": { \"$avg\": {\"$slice\": [\"$Error.Value\", \"$$z\"," + to_string(pWindow) + "] } },\
                        \"TimestampEpoch\" : { \"$arrayElemAt\": [\"$Measures.TimestampEpoch\", { \"$add\": [\"$$z\", " + to_string(pWindow - 1) + "] }] }\
                    }\
                }\
            }\
        }";
    }



    rResult = cmMongoDriver->fRunQuery<TcStatisticTestMeasures>(cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        rSortString,
        0,
        0,
        rGroupString,
        rAddfieldsString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetStatisticsOfMeasuresLowerThanValue(list<TcStatisticTestMeasures>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, int pWindow, string pMovavgattribute, string pReferenceid, string pReferencetype, string pMeasuretype, string pSortattribute, string pGroupattribute, string pProjectionattribute) {
    list<TcStatisticTestMeasures> *cOutputList;
    int rResult = 0;

    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;
    string rGroupString;
    string rAddfieldsString;

    if (pSortattribute == "") {
        rSortString = "";
    }
    else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$lt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


    if (pProjectionattribute == "") {
        rProjectionString = "{ \"_id\" : 0}";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }


    if (pGroupattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : \"$" + pGroupattribute + "\",\
                 \"Test\": { \"$first\" : \"$" + pGroupattribute + "\"},\
                 \"Measures\" : {\
                                    \"$push\" : \"$$ROOT\"\
                                },\
                 \"Calibration\": {\
                                     \"$push\":{\
                                                    \"Value\":{\"$divide\":[{\"$sum\":[\"$HighLimit\",\"$LowLimit\"]},2]}\
                                               }\
                                 },\
                 \"Error\": {\
                                     \"$push\":{\
                                                    \"Value\":{\"$pow\":[{\"$subtract\":[{\"$divide\":[{\"$sum\":[\"$HighLimit\",\"$LowLimit\"]},2]},\"$Value\"]},2]}\
                                               }\
                             },\
                \"Mean\": { \"$avg\": \"$Measures.Value\" },\
                \"StdDev\" : {\"$stdDevSamp\": \"$Measures.Value\"},\
                \"MSE\" : {\"$avg\": \"$Error.Value\"},\
                \"ReferenceID\": { $arrayElemAt: [ $Measures.ReferenceID, 0 ] },\
                \"ReferenceType\": { $arrayElemAt: [ $Measures.ReferencType, 0 ] }\
                }";
    }

    if (pMovavgattribute == "") {
        rAddfieldsString = "";
    }
    else {
        rAddfieldsString = "{\"Statistics\": {\
                \"$map\": {\
                    \"input\": {\"$range\": [0, { \"$subtract\": [{\"$size\":\"$Measures\"}, " + to_string(pWindow - 1) + "] }] },\
                    \"as\" : \"index\",\
                    \"in\" : {\
                        \"Mean\": { \"$avg\": {\"$slice\": [\"$Measures.Value\", \"$$index\"," + to_string(pWindow) + "] } },\
                        \"Variance\": { \"$pow\": [{ \"$stdDevSamp\": {\"$slice\": [\"$Measures.Value\", \"$$index\"," + to_string(pWindow) + "] } }, 2]},\
                        \"MSE\": { \"$avg\": {\"$slice\": [\"$Error.Value\", \"$$index\"," + to_string(pWindow) + "] } },\
                        \"TimestampEpoch\" : { \"$arrayElemAt\": [\"$Measures.TimestampEpoch\", { \"$add\": [\"$$index\", " + to_string(pWindow - 1) + "] }] }\
                    }\
                }\
            }\
        }";
    }

       /* rAddfieldsString = "{\"MovingStatistics\": {\
                \"$map\": {\
                    \"input\": {\"$range\": [0, { \"$subtract\": [{\"$size\":\"$Measures\"}, " + to_string(pWindow - 1) + "] }] },\
                    \"as\" : \"z\",\
                    \"in\" : {\
                        \"Mean\": { \"$avg\": {\"$slice\": [\"$Measures.Value\", \"$$z\"," + to_string(pWindow) + "] } },\
                        \"Variance\": { \"$pow\": [{ \"$stdDevSamp\": {\"$slice\": [\"$Measures.Value\", \"$$z\"," + to_string(pWindow) + "] } }, 2]},\
                        \"TimestampEpoch\" : { \"$arrayElemAt\": [\"$Measures.TimestampEpoch\", { \"$add\": [\"$$z\", " + to_string(pWindow - 1) + "] }] }\
                    }\
                }\
            }\
        }";*/

    /* Add Fields Reference LINK: https://www.moschetti.org/rants/mongomavg.html */

    rResult = cmMongoDriver->fRunQuery<TcStatisticTestMeasures>(cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        rSortString,
        0,
        0,
        rGroupString,
        rAddfieldsString
    );

    if (rResult < 0) {
        pOutput = nullptr;
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetStatisticsOfMeasuresBetweenValues(list<TcStatisticTestMeasures>* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, int pWindow, string pMovavgattribute, string pReferenceid, string pReferencetype, string pMeasuretype, string pSortattribute, string pGroupattribute, string pProjectionattribute) {
    list<TcStatisticTestMeasures>* cOutputList;
    int rResult = 0;

    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;
    string rGroupString;
    string rAddfieldsString;

    if (pSortattribute == "") {
        rSortString = "";
    }
    else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "{ ";
    rFilterString += "\"" + pBetweenattribute + "\" : { \"$lt\" : " + pUpperbound + ", \"$gt\" : " + pLowerbound + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


    if (pProjectionattribute == "") {
        rProjectionString = "{ \"_id\" : 0}";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }


    if (pGroupattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : \"$" + pGroupattribute + "\",\
                 \"Test\": { \"$first\" : \"$" + pGroupattribute + "\"},\
                 \"Measures\" : {\
                                    \"$push\" : \"$$ROOT\"\
                                },\
                 \"Calibration\": {\
                                     \"$push\":{\
                                                    \"Value\":{\"$divide\":[{\"$sum\":[\"$HighLimit\",\"$LowLimit\"]},2]}\
                                               }\
                                 },\
                 \"Error\": {\
                                     \"$push\":{\
                                                    \"Value\":{\"$pow\":[{\"$subtract\":[{\"$divide\":[{\"$sum\":[\"$HighLimit\",\"$LowLimit\"]},2]},\"$Value\"]},2]}\
                                               }\
                             },\
                \"Mean\": { \"$avg\": \"$Measures.Value\" },\
                \"StdDev\" : {\"$stdDevSamp\": \"$Measures.Value\"},\
                \"MSE\" : {\"$avg\": \"$Error.Value\"},\
                \"ReferenceID\": { $arrayElemAt: [ $Measures.ReferenceID, 0 ] },\
                \"ReferenceType\": { $arrayElemAt: [ $Measures.ReferencType, 0 ] }\
                }";
    }

    if (pMovavgattribute == "") {
        rAddfieldsString = "";
    }
    else {
        rAddfieldsString = "{\"Statistics\": {\
                \"$map\": {\
                    \"input\": {\"$range\": [0, { \"$subtract\": [{\"$size\":\"$Measures\"}, " + to_string(pWindow - 1) + "] }] },\
                    \"as\" : \"z\",\
                    \"in\" : {\
                        \"Mean\": { \"$avg\": {\"$slice\": [\"$Measures.Value\", \"$$z\"," + to_string(pWindow) + "] } },\
                        \"Variance\": { \"$pow\": [{ \"$stdDevSamp\": {\"$slice\": [\"$Measures.Value\", \"$$z\"," + to_string(pWindow) + "] } }, 2]},\
                        \"MSE\": { \"$avg\": {\"$slice\": [\"$Error.Value\", \"$$z\"," + to_string(pWindow) + "] } },\
                        \"TimestampEpoch\" : { \"$arrayElemAt\": [\"$Measures.TimestampEpoch\", { \"$add\": [\"$$z\", " + to_string(pWindow - 1) + "] }] }\
                    }\
                }\
            }\
        }";
    }

   

    rResult = this->cmMongoDriver->fRunQuery<TcStatisticTestMeasures>(cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        rSortString, 
        0, 
        0, 
        rGroupString, 
        rAddfieldsString
    );

    if (rResult < 0) {
        pOutput = nullptr;
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetMeasuresBetweenValues(list<string> *pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pReferenceid, string pReferencetype, string pMeasuretype, string pSortattribute, string pProjectionattribute){
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

    rFilterString = "{ ";
    rFilterString += "\"" + pBetweenattribute + "\" : { \"$lt\" : " + pUpperbound + ", \"$gt\" : " + pLowerbound + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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


int IMongoDriverAgentInterface::fGetMeasuresGreatherThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid, string pReferencetype, string pMeasuretype, string pSortattribute, string pProjectionattribute){
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

    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$gt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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


int IMongoDriverAgentInterface::fGetMeasuresLowerThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid, string pReferencetype, string pMeasuretype, string pSortattribute, string pProjectionattribute){
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

    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$lt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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


int IMongoDriverAgentInterface::fGetAvgOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pAvgattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;

   
    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "{ ";
    rFilterString += "\"" + pBetweenattribute + "\" : { \"$lt\" : " + pUpperbound + ", \"$gt\" : " + pLowerbound + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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


int IMongoDriverAgentInterface::fGetAvgOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$gt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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

int IMongoDriverAgentInterface::fGetAvgOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$lt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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

int IMongoDriverAgentInterface::fGetMaxOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMaxattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pBetweenattribute + "\" : { \"$lt\" : " + pUpperbound + ", \"$gt\" : " + pLowerbound + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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

int IMongoDriverAgentInterface::fGetMaxOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$gt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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


int IMongoDriverAgentInterface::fGetMaxOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$lt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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

int IMongoDriverAgentInterface::fGetMinOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMinattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pBetweenattribute + "\" : { \"$lt\" : " + pUpperbound + ", \"$gt\" : " + pLowerbound + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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


int IMongoDriverAgentInterface::fGetMinOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$gt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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

int IMongoDriverAgentInterface::fGetMinOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$lt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";


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

int IMongoDriverAgentInterface::fGetSumOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pSumattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "{ ";
    rFilterString += "\"" + pBetweenattribute + "\" : { \"$lt\" : " + pUpperbound + ", \"$gt\" : " + pLowerbound + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";

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

int IMongoDriverAgentInterface::fGetSumOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$gt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";

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

int IMongoDriverAgentInterface::fGetSumOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$lt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";

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

int IMongoDriverAgentInterface::fGetCountOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;
   

    rFilterString = "{ ";
    rFilterString += "\"" + pBetweenattribute + "\" : { \"$lt\" : " + pUpperbound + ", \"$gt\" : " + pLowerbound + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";

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


int IMongoDriverAgentInterface::fGetCountOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$gt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";

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


int IMongoDriverAgentInterface::fGetCountOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid, string pReferencetype, string pMeasuretype) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "{ ";
    rFilterString += "\"" + pCompareattribute + "\" : {\"$lt\" : " + pComparevalue + " }";
    rFilterSubString = fComposeFilterByMeasureAttributes(pReferenceid, pReferencetype, pMeasuretype);
    rFilterString += (rFilterSubString == "" ? "" : "," + rFilterSubString);
    rFilterString += " }";

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


/*
int IMongoDriverAgentInterface::fGetMeasuresBetweenTimePoints(list<string>* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string referencetype, string measuretype, string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2; 
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;
    
    
    cFilter1 = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document 
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo)) 
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom)) 
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;
    
    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    } else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    } else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }
    
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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


    rResult = this->cmMongoDriver->RunQuery(&cOutputList, database, collection,
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

int IMongoDriverAgentInterface::fGetMeasuresAfterTimePoint(list<string>* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string referencetype, string measuretype, string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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


    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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
int IMongoDriverAgentInterface::fGetMeasuresBeforeTimePoint(list<string>* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string referencetype, string measuretype, string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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


    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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


int IMongoDriverAgentInterface::fGetMinMeasureBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string minattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter1 = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = this->cmMongoDriver->RunQuery(&cOutputList, database, collection,
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


int IMongoDriverAgentInterface::fGetMinMeasureAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string minattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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

int IMongoDriverAgentInterface::fGetMinMeasureBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string minattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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




int IMongoDriverAgentInterface::fGetMaxMeasureBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string maxattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter1 = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = this->cmMongoDriver->RunQuery(&cOutputList, database, collection,
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

int IMongoDriverAgentInterface::fGetMaxMeasureAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string maxattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (after - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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

int IMongoDriverAgentInterface::fGetMaxMeasureBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string maxattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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

int IMongoDriverAgentInterface::fGetSumMeasureBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string sumattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter1 = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    } else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    } else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = this->cmMongoDriver->RunQuery(&cOutputList, database, collection,
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

int IMongoDriverAgentInterface::fGetSumMeasureAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string sumattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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
int IMongoDriverAgentInterface::fGetSumMeasureBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string sumattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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

int IMongoDriverAgentInterface::fGetAvgMeasureBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string avgattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter1 = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = this->cmMongoDriver->RunQuery(&cOutputList, database, collection,
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
int IMongoDriverAgentInterface::fGetAvgMeasureAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string avgattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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
int IMongoDriverAgentInterface::fGetAvgMeasureBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string avgattribute, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

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

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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



int IMongoDriverAgentInterface::fGetCountMeasureBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter1 = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

   
    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "count" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;

    rResult = this->cmMongoDriver->RunQuery(&cOutputList, database, collection,
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
int IMongoDriverAgentInterface::fGetCountMeasureAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

   
    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "count" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
  

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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


int IMongoDriverAgentInterface::fGetCountMeasureBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string referencetype, string measuretype, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter1;
    bsoncxx::document::view_or_value cFilter2;
    bsoncxx::builder::basic::document cFilterBuilder;
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter1 = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (referencetype != "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype == "" && measuretype != "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "MeasureType" << measuretype << bsoncxx::builder::stream::finalize;
    }
    else if (referencetype != "" && measuretype == "") {
        cFilter2 = bsoncxx::builder::stream::document{} << "ReferenceType" << referencetype << bsoncxx::builder::stream::finalize;
    }

    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter1.view()));
    cFilterBuilder.append(bsoncxx::builder::concatenate(cFilter2.view()));
    cFilter = cFilterBuilder.extract();

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;

    rResult = cmMongoDriver->RunQuery(&cOutputList, database, collection,
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
}*/
















                
                

