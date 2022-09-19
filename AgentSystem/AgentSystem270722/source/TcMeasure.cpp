#include "../include/TcMeasure.h"
#include <bsoncxx/json.hpp>


using namespace Spea::Archimede::Data;

TcMeasure::TcMeasure() { 
    this->_id = "";
    this->rpReferenceID = "";
    this->rpReferenceType = "";
    this->rpMeasureUnit = "";
    this->rpMeasureType = "";
    this->rpDrawReference = "";
    this->rpRelayProbeType = "";
    this->rpTimestamp = chrono::time_point<chrono::system_clock, chrono::milliseconds>();
    this->rpTimestampEpoch = 0;
    this->rpTimestampMicroEpoch = 0;
    this->rpStressIndex = 0;
    this->rpValue = 0.00;
    this->rpHighLimit = 0.00;
    this->rpLowLimit = 0.00;
    this->rpResult = "";
    this->rpTPGMName = "";
    this->rpProductSN = "";
    this->rpTask = "";
    this->rpTest = "";
    this->rpTestPoint = "";
    this->rpRemark = "";
}
TcMeasure::~TcMeasure() { ; }
TcMeasure::TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, string pDrawReference, string pRelayProbeType, chrono::time_point<chrono::system_clock, chrono::milliseconds> pTimestamp, uint64_t pTimestampEpoch, uint64_t pTimestampMicroEpoch, int pStressIndex, double pValue, double pHighLimit, double pLowLimit, string pResult, string pTPGMName, string pProductSN, string pTask, string pTest, string pTestPoint, string pRemark){
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
TcMeasure::TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, chrono::time_point<chrono::system_clock, chrono::milliseconds> pTimestamp, double pValue, double pHighLimit, double pLowLimit, string pTest){
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
TcMeasure::TcMeasure(bsoncxx::document::value pBsonDocument){
    bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
    this->rpValue = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
    this->rpHighLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
    this->rpLowLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
    this->rpResult = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
    this->rpTest = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
    this->rpTask = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
    this->rpRemark = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
    this->rpTPGMName = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
    this->rpProductSN = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
    this->rpReferenceID = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
    this->rpReferenceType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
    this->rpMeasureType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
    this->rpMeasureUnit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
    this->rpDrawReference = cBsonDocumentView[TcMeasureAttributes::kDrawReference].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
    this->rpRelayProbeType = cBsonDocumentView[TcMeasureAttributes::kRelayProbeType].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
    this->rpTimestamp = cBsonDocumentView[TcMeasureAttributes::kTimestamp].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].type() != bsoncxx::type::k_date ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : (cBsonDocumentView[TcMeasureAttributes::kTimestamp].type() == bsoncxx::type::k_utf8 ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : chrono::time_point<chrono::system_clock, chrono::milliseconds>(cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value));
    this->rpTimestampMicroEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].type() != bsoncxx::type::k_int64 ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
    this->rpTimestampEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
}
TcMeasure::TcMeasure(bsoncxx::document::view pBsonDocumentView){
    this->rpValue = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
    this->rpHighLimit = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].type() != bsoncxx::type::k_double ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
    this->rpLowLimit = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].type() != bsoncxx::type::k_double ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
    this->rpResult = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
    this->rpTest = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
    this->rpTask = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
    this->rpRemark = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
    this->rpTPGMName = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
    this->rpProductSN = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
    this->rpReferenceID = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
    this->rpReferenceType = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
    this->rpMeasureType = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
    this->rpMeasureUnit = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
    this->rpDrawReference = pBsonDocumentView[TcMeasureAttributes::kDrawReference].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
    this->rpRelayProbeType = pBsonDocumentView[TcMeasureAttributes::kRelayProbeType].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
    this->rpTimestamp = pBsonDocumentView[TcMeasureAttributes::kTimestamp].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].type() != bsoncxx::type::k_date ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : (pBsonDocumentView[TcMeasureAttributes::kTimestamp].type() == bsoncxx::type::k_utf8 ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : chrono::time_point<chrono::system_clock, chrono::milliseconds>(pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value));
    this->rpTimestampMicroEpoch = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].type() != bsoncxx::type::k_int64 ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
    this->rpTimestampEpoch = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
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
        TcMeasure cMeasure;
        bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
        cMeasure.rpValue = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
        cMeasure.rpHighLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
        cMeasure.rpLowLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
        cMeasure.rpResult = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
        cMeasure.rpTest = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
        cMeasure.rpTask = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
        cMeasure.rpRemark = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
        cMeasure.rpTPGMName = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
        cMeasure.rpProductSN = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
        cMeasure.rpReferenceID = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
        cMeasure.rpReferenceType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
        cMeasure.rpMeasureType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
        cMeasure.rpMeasureUnit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
        cMeasure.rpDrawReference = cBsonDocumentView[TcMeasureAttributes::kDrawReference].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
        cMeasure.rpRelayProbeType = cBsonDocumentView[TcMeasureAttributes::kRelayProbeType].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
        cMeasure.rpTimestamp = cBsonDocumentView[TcMeasureAttributes::kTimestamp].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].type() != bsoncxx::type::k_date ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : (cBsonDocumentView[TcMeasureAttributes::kTimestamp].type() == bsoncxx::type::k_utf8 ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : chrono::time_point<chrono::system_clock, chrono::milliseconds>(cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value));
        cMeasure.rpTimestampMicroEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].type() != bsoncxx::type::k_int64 ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
        cMeasure.rpTimestampEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].length() == 0 || cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : cBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
        
        return(cMeasure);
    }
    catch (exception e) {
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
        if (pMeasure.rpTimestamp != chrono::time_point<chrono::system_clock, chrono::milliseconds>()) {
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

                
        bsoncxx::document::value cBsonDocument = cBsonDocumentBuilder << bsoncxx::builder::stream::finalize;
        return(cBsonDocument);
    } catch(exception e){
        return(bsoncxx::builder::stream::document{} << "" << "" << bsoncxx::builder::stream::finalize);
    }

}
TcMeasure TcMeasure::fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocumentView){
	try {
        TcMeasure cMeasure;
        cMeasure.rpValue = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
        cMeasure.rpHighLimit = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].type() != bsoncxx::type::k_double ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value;
        cMeasure.rpLowLimit = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].type() != bsoncxx::type::k_double ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value;
        cMeasure.rpResult = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kResult].get_utf8().value.to_string();
        cMeasure.rpTest = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTest].get_utf8().value.to_string();
        cMeasure.rpTask = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTask].get_utf8().value.to_string();
        cMeasure.rpRemark = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRemark].get_utf8().value.to_string();
        cMeasure.rpTPGMName = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTPGMName].get_utf8().value.to_string();
        cMeasure.rpProductSN = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kProductSN].get_utf8().value.to_string();
        cMeasure.rpReferenceID = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
        cMeasure.rpReferenceType = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
        cMeasure.rpMeasureType = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
        cMeasure.rpMeasureUnit = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
        cMeasure.rpDrawReference = pBsonDocumentView[TcMeasureAttributes::kDrawReference].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kDrawReference].get_utf8().value.to_string();
        cMeasure.rpRelayProbeType = pBsonDocumentView[TcMeasureAttributes::kRelayProbeType].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kRelayProbeType].get_utf8().value.to_string();
        cMeasure.rpTimestamp = pBsonDocumentView[TcMeasureAttributes::kTimestamp].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].type() != bsoncxx::type::k_date ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : (pBsonDocumentView[TcMeasureAttributes::kTimestamp].type() == bsoncxx::type::k_utf8 ? chrono::time_point<chrono::system_clock, chrono::milliseconds>() : chrono::time_point<chrono::system_clock, chrono::milliseconds>(pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestamp].get_date().value));
        cMeasure.rpTimestampMicroEpoch = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].type() != bsoncxx::type::k_int64 ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch].get_int64().value;
        cMeasure.rpTimestampEpoch = pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].length() == 0 || pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : pBsonDocumentView[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value;
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
        if (pMeasure.rpTimestamp != chrono::time_point<chrono::system_clock, chrono::milliseconds>()) {
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
        
        bsoncxx::document::view cBsonDocument = cBsonDocumentBuilder << bsoncxx::builder::stream::finalize;
        return(cBsonDocument);
    } catch(exception e){
        return(bsoncxx::document::view());
    }

}
TcMeasure::TcMeasure(const TcMeasure& other) {
    this->_id = other._id;
    this->rpReferenceID = other.rpReferenceID;
    this->rpReferenceType = other.rpReferenceType;
    this->rpMeasureUnit = other.rpMeasureUnit;
    this->rpMeasureType = other.rpMeasureType;
    this->rpDrawReference = other.rpDrawReference;
    this->rpRelayProbeType = other.rpRelayProbeType;
    this->rpTimestamp = other.rpTimestamp;
    this->rpTimestampEpoch = other.rpTimestampEpoch;
    this->rpTimestampMicroEpoch = other.rpTimestampMicroEpoch;
    this->rpStressIndex = other.rpStressIndex;
    this->rpValue = other.rpValue;
    this->rpHighLimit = other.rpHighLimit;
    this->rpLowLimit = other.rpLowLimit;
    this->rpResult = other.rpResult;
    this->rpTPGMName = other.rpTPGMName;
    this->rpProductSN = other.rpProductSN;
    this->rpTask = other.rpTask;
    this->rpTest = other.rpTest;
    this->rpTestPoint = other.rpTestPoint;
    this->rpRemark = other.rpRemark;
}
TcMeasure::TcMeasure(TcMeasure&& other) {
    this->_id = other._id;
    this->rpReferenceID = other.rpReferenceID;
    this->rpReferenceType = other.rpReferenceType;
    this->rpMeasureUnit = other.rpMeasureUnit;
    this->rpMeasureType = other.rpMeasureType;
    this->rpDrawReference = other.rpDrawReference;
    this->rpRelayProbeType = other.rpRelayProbeType;
    this->rpTimestamp = other.rpTimestamp;
    this->rpTimestampEpoch = other.rpTimestampEpoch;
    this->rpTimestampMicroEpoch = other.rpTimestampMicroEpoch;
    this->rpStressIndex = other.rpStressIndex;
    this->rpValue = other.rpValue;
    this->rpHighLimit = other.rpHighLimit;
    this->rpLowLimit = other.rpLowLimit;
    this->rpResult = other.rpResult;
    this->rpTPGMName = other.rpTPGMName;
    this->rpProductSN = other.rpProductSN;
    this->rpTask = other.rpTask;
    this->rpTest = other.rpTest;
    this->rpTestPoint = other.rpTestPoint;
    this->rpRemark = other.rpRemark;

    other._id = "";
    other.rpReferenceID = "";
    other.rpReferenceType = "";
    other.rpMeasureUnit = "";
    other.rpMeasureType = "";
    other.rpDrawReference = "";
    other.rpRelayProbeType = "";
    other.rpTimestamp = chrono::time_point<chrono::system_clock, chrono::milliseconds>();
    other.rpTimestampEpoch = 0;
    other.rpTimestampMicroEpoch = 0;
    other.rpStressIndex = 0;
    other.rpValue = 0;
    other.rpHighLimit = 0;
    other.rpLowLimit = 0;
    other.rpResult = "";
    other.rpTPGMName = "";
    other.rpProductSN = "";
    other.rpTask = "";
    other.rpTest = "";
    other.rpTestPoint = "";
    other.rpRemark = "";


}
TcMeasure& TcMeasure::operator=(const TcMeasure& other) {

    if (this != &other) {
        this->_id = other._id;
        this->rpReferenceID = other.rpReferenceID;
        this->rpReferenceType = other.rpReferenceType;
        this->rpMeasureUnit = other.rpMeasureUnit;
        this->rpMeasureType = other.rpMeasureType;
        this->rpDrawReference = other.rpDrawReference;
        this->rpRelayProbeType = other.rpRelayProbeType;
        this->rpTimestamp = other.rpTimestamp;
        this->rpTimestampEpoch = other.rpTimestampEpoch;
        this->rpTimestampMicroEpoch = other.rpTimestampMicroEpoch;
        this->rpStressIndex = other.rpStressIndex;
        this->rpValue = other.rpValue;
        this->rpHighLimit = other.rpHighLimit;
        this->rpLowLimit = other.rpLowLimit;
        this->rpResult = other.rpResult;
        this->rpTPGMName = other.rpTPGMName;
        this->rpProductSN = other.rpProductSN;
        this->rpTask = other.rpTask;
        this->rpTest = other.rpTest;
        this->rpTestPoint = other.rpTestPoint;
        this->rpRemark = other.rpRemark;
    }

    return(*this);

}
TcMeasure& TcMeasure::operator=(TcMeasure&& other) {

    if (this != &other) {
        this->_id = other._id;
        this->rpReferenceID = other.rpReferenceID;
        this->rpReferenceType = other.rpReferenceType;
        this->rpMeasureUnit = other.rpMeasureUnit;
        this->rpMeasureType = other.rpMeasureType;
        this->rpDrawReference = other.rpDrawReference;
        this->rpRelayProbeType = other.rpRelayProbeType;
        this->rpTimestamp = other.rpTimestamp;
        this->rpTimestampEpoch = other.rpTimestampEpoch;
        this->rpTimestampMicroEpoch = other.rpTimestampMicroEpoch;
        this->rpStressIndex = other.rpStressIndex;
        this->rpValue = other.rpValue;
        this->rpHighLimit = other.rpHighLimit;
        this->rpLowLimit = other.rpLowLimit;
        this->rpResult = other.rpResult;
        this->rpTPGMName = other.rpTPGMName;
        this->rpProductSN = other.rpProductSN;
        this->rpTask = other.rpTask;
        this->rpTest = other.rpTest;
        this->rpTestPoint = other.rpTestPoint;
        this->rpRemark = other.rpRemark;
        
        other._id = "";
        other.rpReferenceID = "";
        other.rpReferenceType = "";
        other.rpMeasureUnit = "";
        other.rpMeasureType = "";
        other.rpDrawReference = "";
        other.rpRelayProbeType = "";
        other.rpTimestamp = chrono::time_point<chrono::system_clock, chrono::milliseconds>();
        other.rpTimestampEpoch = 0;
        other.rpTimestampMicroEpoch = 0;
        other.rpStressIndex = 0;
        other.rpValue = 0;
        other.rpHighLimit = 0;
        other.rpLowLimit = 0;
        other.rpResult = "";
        other.rpTPGMName = "";
        other.rpProductSN = "";
        other.rpTask = "";
        other.rpTest = "";
        other.rpTestPoint = "";
        other.rpRemark = "";
    }

    return(*this);

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
const string TcMeasure::TcMeasureAttributes::kRemark = "rpRemark";
const string TcMeasure::TcMeasureAttributes::kRelayProbeType = "RelayProbeType";
const string TcMeasure::TcMeasureAttributes::kTimestamp = "rpTimestamp";
const string TcMeasure::TcMeasureAttributes::kTimestampEpoch = "rpTimestampEpoch";
const string TcMeasure::TcMeasureAttributes::kTimestampMicroEpoch = "TimestampMicroEpoch";
const string TcMeasure::kCollection = "Measures";

