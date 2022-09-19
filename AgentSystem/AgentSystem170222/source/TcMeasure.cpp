#include "../include/TcMeasure.h"
#include <bsoncxx/json.hpp>

TcMeasure::TcMeasure() { ; }
TcMeasure::~TcMeasure() { ; }
TcMeasure::TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, string pDrawReference, string pRelayProbeType, chrono::system_clock::time_point pTimestamp, uint64_t pTimestampEpoch, uint64_t pTimestampMicroEpoch, int pStressIndex, double pValue, double pHighLimit, double pLowLimit, string pResult, string pTPGMName, string pProductSN, string pTask, string pTest, string pTestPoint, string pRemark){
    this->_id = pId;
    this->rpReferenceID = pReferenceID;
    this->rpReferenceType = pReferenceType;
    this->rpMeasureUnit = pMeasureUnit;
    this->rpMeasureType = pMeasureType;
    this->rpDrawReference = pDrawReference;
    this->rpRelayProbeType = pRelayProbeType;
    this->rpTimestamp = pTimestamp;
    this->rpTimestampEpoch = pTimestampEpoch;
    this->rpTimestampMicroEpoch = pTimestampMicroEpoch;
    this->rpStressIndex = pStressIndex;
    this->rpValue = pValue;
    this->rpHighLimit = pHighLimit;
    this->rpLowLimit = pLowLimit;
    this->rpResult = pResult;
    this->rpTPGMName = pTPGMName;
    this->rpProductSN = pProductSN;
    this->rpTask = pTask;
    this->rpTest = pTest;
    this->rpTestPoint = pTestPoint;
    this->rpRemark = pRemark; 
 }

 TcMeasure::TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, chrono::system_clock::time_point pTimestamp, double pValue, double pHighLimit, double pLowLimit, string pTest){
    this->_id = pId;
    this->rpReferenceID = pReferenceID;
    this->rpReferenceType = pReferenceType;
    this->rpMeasureUnit = pMeasureUnit;
    this->rpMeasureType = pMeasureType;
    this->rpTimestamp = pTimestamp;
    this->rpTimestampEpoch = (long long)chrono::duration_cast<chrono::milliseconds>(pTimestamp.time_since_epoch()).count();
    this->rpTimestampMicroEpoch = (long long)chrono::duration_cast<chrono::microseconds>(pTimestamp.time_since_epoch()).count();
    this->rpValue = pValue;
    this->rpHighLimit = pHighLimit;
    this->rpLowLimit = pLowLimit;
    this->rpTest = pTest; 
 }




TcMeasure::TcMeasure(bsoncxx::document::value pBsonDocument){/*  */
    bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
    this->rpValue = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].get_double().value;
    this->rpTimestamp = chrono::system_clock::time_point{cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value};
	this->rpHighLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
	this->rpLowLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
    this->rpTimestampMicroEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
    this->rpTimestampEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
    this->rpResult = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
    this->rpTest = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
    this->rpTask = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
    this->rpRemark = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
    this->rpTPGMName = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
    this->rpProductSN = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
    this->rpReferenceID = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
    this->rpReferenceType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
    this->rpMeasureType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
    this->rpMeasureUnit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
    this->rpDrawReference = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
    this->rpRelayProbeType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
}
TcMeasure::TcMeasure(bsoncxx::document::view pBsonDocument){

    this->rpValue = pBsonDocument[TcMeasure::TcMeasureAttributes::kValue].get_double().value;
    this->rpTimestamp = chrono::system_clock::time_point{ pBsonDocument[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value };
	this->rpHighLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
	this->rpLowLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
    this->rpTimestampMicroEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
    this->rpTimestampEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
    this->rpResult = pBsonDocument[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
    this->rpTest = pBsonDocument[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
    this->rpTask = pBsonDocument[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
    this->rpRemark = pBsonDocument[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
    this->rpTPGMName = pBsonDocument[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
    this->rpProductSN = pBsonDocument[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
    this->rpReferenceID = pBsonDocument[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
    this->rpReferenceType = pBsonDocument[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
    this->rpMeasureType = pBsonDocument[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
    this->rpMeasureUnit = pBsonDocument[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
    this->rpDrawReference = pBsonDocument[TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
    this->rpRelayProbeType = pBsonDocument[TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
}
TcMeasure TcMeasure::from_bsonvalue(bsoncxx::document::value pBsonDocument){
    return(TcMeasure::fDeserializeObjectBsonValue(pBsonDocument));
}
TcMeasure TcMeasure::from_bsonview(bsoncxx::document::view pBsonDocument){
    return(TcMeasure::fDeserializeObjectBsonView(pBsonDocument));
}
TcMeasure TcMeasure::from_json(string pJsonDocument){
    return(TcMeasure::fDeserializeObjectBsonValue(bsoncxx::from_json(pJsonDocument)));
}

bsoncxx::document::value TcMeasure::to_bsonvalue(){
    return(TcMeasure::fSerializeObjectBsonValue(*this));
}
bsoncxx::document::view TcMeasure::to_bsonview(){
    return(TcMeasure::fSerializeObjectBsonView(*this));
}
string TcMeasure::to_json(){
    return(bsoncxx::to_json(TcMeasure::fSerializeObjectBsonValue(*this)));
}

TcMeasure TcMeasure::fDeserializeObjectBsonValue (bsoncxx::document::value pBsonDocument) {
	try {
        bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
        TcMeasure cMeasure;
        cMeasure.rpValue = pBsonDocument.view()[TcMeasure::TcMeasureAttributes::kValue].get_double().value;
	    cMeasure.rpTimestamp = chrono::system_clock::time_point{cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value };
	    cMeasure.rpHighLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
	    cMeasure.rpLowLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
        cMeasure.rpTimestampMicroEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
        cMeasure.rpTimestampEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
        cMeasure.rpResult = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
        cMeasure.rpTest = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
        cMeasure.rpTask = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
        cMeasure.rpRemark = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
        cMeasure.rpTPGMName = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
        cMeasure.rpProductSN = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
        cMeasure.rpReferenceID = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
        cMeasure.rpReferenceType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
        cMeasure.rpMeasureType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
        cMeasure.rpMeasureUnit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
        cMeasure.rpDrawReference = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
        cMeasure.rpRelayProbeType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
        return(cMeasure);
    } catch(exception e){
        return(TcMeasure());
    }
}
bsoncxx::document::value TcMeasure::fSerializeObjectBsonValue(TcMeasure pMeasure){
    try {
        
        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};
        
        if (pMeasure.rpReferenceID != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kReferenceID << pMeasure.rpReferenceID;
        }
        if (pMeasure.rpReferenceType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kReferenceType << pMeasure.rpReferenceType;
        }
        if (pMeasure.rpTimestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTimestamp << bsoncxx::types::b_date{pMeasure.rpTimestamp};
        }
        if (pMeasure.rpTimestampEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTimestampEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.rpTimestampEpoch };
        }
        if (pMeasure.rpTimestampMicroEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.rpTimestampMicroEpoch };
        }
        if (pMeasure.rpMeasureType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kMeasureType << pMeasure.rpMeasureType;
        }
        if (pMeasure.rpMeasureUnit != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kMeasureUnit << pMeasure.rpMeasureUnit;
        }
        if (pMeasure.rpDrawReference != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kDrawReference << pMeasure.rpDrawReference;
        }
        if (pMeasure.rpRelayProbeType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kRelayProbeType << pMeasure.rpRelayProbeType;
        }
        if (pMeasure.rpStressIndex != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kStressIndex << pMeasure.rpStressIndex;
        }
        if (pMeasure.rpHighLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kHighLimit << pMeasure.rpHighLimit;
        }
        if (pMeasure.rpLowLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kLowLimit << pMeasure.rpLowLimit;
        }
        if (pMeasure.rpValue != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kValue << pMeasure.rpValue;
        }
        if (pMeasure.rpResult != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kResult << pMeasure.rpResult;
        }
        if (pMeasure.rpTPGMName != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTPGMName << pMeasure.rpTPGMName;
        }
        if (pMeasure.rpProductSN != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kProductSN << pMeasure.rpProductSN;
        }
        if (pMeasure.rpTask != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTask << pMeasure.rpTask;
        }
        if (pMeasure.rpTest != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTest << pMeasure.rpTest;
        }
        if (pMeasure.rpTestPoint != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTestPoint << pMeasure.rpTestPoint;
        }
        if (pMeasure.rpRemark != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kRemark << pMeasure.rpRemark;
        }

        cBsonDocumentBuilder << bsoncxx::builder::stream::finalize;        
        bsoncxx::document::value cBsonDocument = cBsonDocumentBuilder.extract();
        return(cBsonDocument);
    } catch(exception e){
        return(bsoncxx::builder::stream::document{} << "" << "" << bsoncxx::builder::stream::finalize);
    }

}



TcMeasure TcMeasure::fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocument){
	try {
        TcMeasure cMeasure;
        cMeasure.rpValue = pBsonDocument[TcMeasure::TcMeasureAttributes::kValue].get_double().value;
        cMeasure.rpTimestamp = chrono::system_clock::time_point{ pBsonDocument[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value };
	    cMeasure.rpHighLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
	    cMeasure.rpLowLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
        cMeasure.rpTimestampMicroEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
        cMeasure.rpTimestampEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
        cMeasure.rpResult = pBsonDocument[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
        cMeasure.rpTest = pBsonDocument[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
        cMeasure.rpTask = pBsonDocument[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
        cMeasure.rpRemark = pBsonDocument[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
        cMeasure.rpTPGMName = pBsonDocument[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
        cMeasure.rpProductSN = pBsonDocument[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
        cMeasure.rpReferenceID = pBsonDocument[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
        cMeasure.rpReferenceType = pBsonDocument[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
        cMeasure.rpMeasureType = pBsonDocument[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
        cMeasure.rpMeasureUnit = pBsonDocument[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
        cMeasure.rpDrawReference = pBsonDocument[TcMeasure::TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
        cMeasure.rpRelayProbeType = pBsonDocument[TcMeasure::TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
        return(cMeasure);
    } catch(exception e){
        return(TcMeasure());
    }
}
bsoncxx::document::view TcMeasure::fSerializeObjectBsonView(TcMeasure pMeasure){
    try {
        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};

        if (pMeasure.rpReferenceID != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kReferenceID << pMeasure.rpReferenceID;
        }
        if (pMeasure.rpReferenceType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kReferenceType << pMeasure.rpReferenceType;
        }
        if (pMeasure.rpTimestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTimestamp << bsoncxx::types::b_date{ pMeasure.rpTimestamp };
        }
        if (pMeasure.rpTimestampEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTimestampEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.rpTimestampEpoch };
        }
        if (pMeasure.rpTimestampMicroEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.rpTimestampMicroEpoch };
        }
        if (pMeasure.rpMeasureType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kMeasureType << pMeasure.rpMeasureType;
        }
        if (pMeasure.rpMeasureUnit != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kMeasureUnit << pMeasure.rpMeasureUnit;
        }
        if (pMeasure.rpDrawReference != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kDrawReference << pMeasure.rpDrawReference;
        }
        if (pMeasure.rpRelayProbeType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kRelayProbeType << pMeasure.rpRelayProbeType;
        }
        if (pMeasure.rpStressIndex != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kStressIndex << pMeasure.rpStressIndex;
        }
        if (pMeasure.rpHighLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kHighLimit << pMeasure.rpHighLimit;
        }
        if (pMeasure.rpLowLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kLowLimit << pMeasure.rpLowLimit;
        }
        if (pMeasure.rpValue != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kValue << pMeasure.rpValue;
        }
        if (pMeasure.rpResult != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kResult << pMeasure.rpResult;
        }
        if (pMeasure.rpTPGMName != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTPGMName << pMeasure.rpTPGMName;
        }
        if (pMeasure.rpProductSN != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kProductSN << pMeasure.rpProductSN;
        }
        if (pMeasure.rpTask != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTask << pMeasure.rpTask;
        }
        if (pMeasure.rpTest != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTest << pMeasure.rpTest;
        }
        if (pMeasure.rpTestPoint != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kTestPoint << pMeasure.rpTestPoint;
        }
        if (pMeasure.rpRemark != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::kRemark << pMeasure.rpRemark;
        }

        cBsonDocumentBuilder << bsoncxx::builder::stream::finalize;
        
        bsoncxx::document::view cBsonDocument = cBsonDocumentBuilder.extract();
        return(cBsonDocument);
    } catch(exception e){
        return(bsoncxx::document::view());
    }

}

const string TcMeasure::TcMeasureAttributes::_id = "_id";



const string TcMeasure::TcMeasureAttributes::kDrawReference = "DrawReference";
const string TcMeasure::TcMeasureAttributes::kStressIndex = "StressIndex";
const string TcMeasure::TcMeasureAttributes::kMeasureType = "MeasureType";
const string TcMeasure::TcMeasureAttributes::kMeasureUnit = "MeasureUnit";
const string TcMeasure::TcMeasureAttributes::kReferenceID = "ReferenceID";
const string TcMeasure::TcMeasureAttributes::kReferenceType = "ReferenceType";

const string TcMeasure::TcMeasureAttributes::kHighLimit = "HighLimit";
const string TcMeasure::TcMeasureAttributes::kLowLimit = "LowLimit";
const string TcMeasure::TcMeasureAttributes::kValue = "Value";
const string TcMeasure::TcMeasureAttributes::kResult = "Result";
const string TcMeasure::TcMeasureAttributes::kTPGMName = "TPGMName";
const string TcMeasure::TcMeasureAttributes::kProductSN = "ProductSN";
const string TcMeasure::TcMeasureAttributes::kTask = "Task";
const string TcMeasure::TcMeasureAttributes::kTest = "Test";
const string TcMeasure::TcMeasureAttributes::kTestPoint = "TestPoint";
const string TcMeasure::TcMeasureAttributes::kRemark = "Remark";
const string TcMeasure::TcMeasureAttributes::kRelayProbeType = "RelayProbeType";
const string TcMeasure::TcMeasureAttributes::kTimestamp = "Timestamp";
const string TcMeasure::TcMeasureAttributes::kTimestampEpoch = "TimestampEpoch";
const string TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch = "TimestampMicroEpoch";
const string TcMeasure::kCollection = "Measures";

