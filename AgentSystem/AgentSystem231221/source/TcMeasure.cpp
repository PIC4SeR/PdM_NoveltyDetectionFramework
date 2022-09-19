#include "../include/TcMeasure.h"
#include <bsoncxx/json.hpp>

TcMeasure::TcMeasure(){}
TcMeasure::TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, string pDrawReference, string pRelayProbeType, chrono::system_clock::time_point pTimestamp, uint64_t pTimestampEpoch, uint64_t pTimestampMicroEpoch, int pStressIndex, double pValue, double pHighLimit, double pLowLimit, string pResult, string pTPGMName, string pProductSN, string pTask, string pTest, string pTestPoint, string pRemark){
    this->_id = pId;
    this->ReferenceID = pReferenceID;
    this->ReferenceType = pReferenceType;
    this->MeasureUnit = pMeasureUnit;
    this->MeasureType = pMeasureType;
    this->DrawReference = pDrawReference;
    this->RelayProbeType = pRelayProbeType;
    this->Timestamp = pTimestamp;
    this->TimestampEpoch = pTimestampEpoch;
    this->TimestampMicroEpoch = pTimestampMicroEpoch;
    this->StressIndex = pStressIndex;
    this->Value = pValue;
    this->HighLimit = pHighLimit;
    this->LowLimit = pLowLimit;
    this->Result = pResult;
    this->TPGMName = pTPGMName;
    this->ProductSN = pProductSN;
    this->Task = pTask;
    this->Test = pTest;
    this->TestPoint = pTestPoint;
    this->Remark = pRemark; 
 }

 TcMeasure::TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, chrono::system_clock::time_point pTimestamp, double pValue, double pHighLimit, double pLowLimit, string pTest){
    this->_id = pId;
    this->ReferenceID = pReferenceID;
    this->ReferenceType = pReferenceType;
    this->MeasureUnit = pMeasureUnit;
    this->MeasureType = pMeasureType;
    this->Timestamp = pTimestamp;
    this->TimestampEpoch = (long long)chrono::duration_cast<chrono::milliseconds>(pTimestamp.time_since_epoch()).count();
    this->TimestampMicroEpoch = (long long)chrono::duration_cast<chrono::microseconds>(pTimestamp.time_since_epoch()).count();
    this->Value = pValue;
    this->HighLimit = pHighLimit;
    this->LowLimit = pLowLimit;
    this->Test = pTest; 
 }




TcMeasure::TcMeasure(bsoncxx::document::value pBsonDocument){/*  */
    bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
    this->Value = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Value].get_double().value;
    this->Timestamp = chrono::system_clock::time_point{cBsonDocumentView[TcMeasure::TcMeasureAttributes::Timestamp].get_date().value};
	this->HighLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::HighLimit].get_double().value;
	this->LowLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::LowLimit].get_double().value;
    this->TimestampMicroEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::TimestampMicroEpoch].get_int64().value;
    this->TimestampEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::TimestampEpoch].get_int64().value;
    this->Result = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Result].get_utf8().value.to_string();
    this->Test = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Test].get_utf8().value.to_string();
    this->Task = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Task].get_utf8().value.to_string();
    this->Remark = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Remark].get_utf8().value.to_string();
    this->TPGMName = cBsonDocumentView[TcMeasure::TcMeasureAttributes::TPGMName].get_utf8().value.to_string();
    this->ProductSN = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ProductSN].get_utf8().value.to_string();
    this->ReferenceID = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ReferenceID].get_utf8().value.to_string();
    this->ReferenceType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ReferenceType].get_utf8().value.to_string();
    this->MeasureType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::MeasureType].get_utf8().value.to_string();
    this->MeasureUnit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::MeasureUnit].get_utf8().value.to_string();
    this->DrawReference = cBsonDocumentView[TcMeasure::TcMeasureAttributes::DrawReference].get_utf8().value.to_string();
    this->RelayProbeType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::RelayProbeType].get_utf8().value.to_string();
}
TcMeasure::TcMeasure(bsoncxx::document::view pBsonDocument){

    this->Value = pBsonDocument[TcMeasure::TcMeasureAttributes::Value].get_double().value;
    this->Timestamp = chrono::system_clock::time_point{ pBsonDocument[TcMeasure::TcMeasureAttributes::Timestamp].get_date().value };
	this->HighLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::HighLimit].get_double().value;
	this->LowLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::LowLimit].get_double().value;
    this->TimestampMicroEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::TimestampMicroEpoch].get_int64().value;
    this->TimestampEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::TimestampEpoch].get_int64().value;
    this->Result = pBsonDocument[TcMeasure::TcMeasureAttributes::Result].get_utf8().value.to_string();
    this->Test = pBsonDocument[TcMeasure::TcMeasureAttributes::Test].get_utf8().value.to_string();
    this->Task = pBsonDocument[TcMeasure::TcMeasureAttributes::Task].get_utf8().value.to_string();
    this->Remark = pBsonDocument[TcMeasure::TcMeasureAttributes::Remark].get_utf8().value.to_string();
    this->TPGMName = pBsonDocument[TcMeasure::TcMeasureAttributes::TPGMName].get_utf8().value.to_string();
    this->ProductSN = pBsonDocument[TcMeasure::TcMeasureAttributes::ProductSN].get_utf8().value.to_string();
    this->ReferenceID = pBsonDocument[TcMeasure::TcMeasureAttributes::ReferenceID].get_utf8().value.to_string();
    this->ReferenceType = pBsonDocument[TcMeasure::TcMeasureAttributes::ReferenceType].get_utf8().value.to_string();
    this->MeasureType = pBsonDocument[TcMeasure::TcMeasureAttributes::MeasureType].get_utf8().value.to_string();
    this->MeasureUnit = pBsonDocument[TcMeasure::TcMeasureAttributes::MeasureUnit].get_utf8().value.to_string();
    this->DrawReference = pBsonDocument[TcMeasureAttributes::DrawReference].get_utf8().value.to_string();
    this->RelayProbeType = pBsonDocument[TcMeasureAttributes::RelayProbeType].get_utf8().value.to_string();
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
        cMeasure.Value = pBsonDocument.view()[TcMeasure::TcMeasureAttributes::Value].get_double().value;
	    cMeasure.Timestamp = chrono::system_clock::time_point{cBsonDocumentView[TcMeasure::TcMeasureAttributes::Timestamp].get_date().value };
	    cMeasure.HighLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::HighLimit].get_double().value;
	    cMeasure.LowLimit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::LowLimit].get_double().value;
        cMeasure.TimestampMicroEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::TimestampMicroEpoch].get_int64().value;
        cMeasure.TimestampEpoch = cBsonDocumentView[TcMeasure::TcMeasureAttributes::TimestampEpoch].get_int64().value;
        cMeasure.Result = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Result].get_utf8().value.to_string();
        cMeasure.Test = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Test].get_utf8().value.to_string();
        cMeasure.Task = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Task].get_utf8().value.to_string();
        cMeasure.Remark = cBsonDocumentView[TcMeasure::TcMeasureAttributes::Remark].get_utf8().value.to_string();
        cMeasure.TPGMName = cBsonDocumentView[TcMeasure::TcMeasureAttributes::TPGMName].get_utf8().value.to_string();
        cMeasure.ProductSN = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ProductSN].get_utf8().value.to_string();
        cMeasure.ReferenceID = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ReferenceID].get_utf8().value.to_string();
        cMeasure.ReferenceType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ReferenceType].get_utf8().value.to_string();
        cMeasure.MeasureType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::MeasureType].get_utf8().value.to_string();
        cMeasure.MeasureUnit = cBsonDocumentView[TcMeasure::TcMeasureAttributes::MeasureUnit].get_utf8().value.to_string();
        cMeasure.DrawReference = cBsonDocumentView[TcMeasure::TcMeasureAttributes::DrawReference].get_utf8().value.to_string();
        cMeasure.RelayProbeType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::RelayProbeType].get_utf8().value.to_string();
        return(cMeasure);
    } catch(exception e){
        return(TcMeasure());
    }
}
bsoncxx::document::value TcMeasure::fSerializeObjectBsonValue(TcMeasure pMeasure){
    try {
        
        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};
        
        if (pMeasure.ReferenceID != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::ReferenceID << pMeasure.ReferenceID;
        }
        if (pMeasure.ReferenceType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::ReferenceType << pMeasure.ReferenceType;
        }
        if (pMeasure.Timestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Timestamp << bsoncxx::types::b_date{pMeasure.Timestamp};
        }
        if (pMeasure.TimestampEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TimestampEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.TimestampEpoch };
        }
        if (pMeasure.TimestampMicroEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TimestampMicroEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.TimestampMicroEpoch };
        }
        if (pMeasure.MeasureType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::MeasureType << pMeasure.MeasureType;
        }
        if (pMeasure.MeasureUnit != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::MeasureUnit << pMeasure.MeasureUnit;
        }
        if (pMeasure.DrawReference != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::DrawReference << pMeasure.DrawReference;
        }
        if (pMeasure.RelayProbeType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::RelayProbeType << pMeasure.RelayProbeType;
        }
        if (pMeasure.StressIndex != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::StressIndex << pMeasure.StressIndex;
        }
        if (pMeasure.HighLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::HighLimit << pMeasure.HighLimit;
        }
        if (pMeasure.LowLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::LowLimit << pMeasure.LowLimit;
        }
        if (pMeasure.Value != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Value << pMeasure.Value;
        }
        if (pMeasure.Result != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Result << pMeasure.Result;
        }
        if (pMeasure.TPGMName != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TPGMName << pMeasure.TPGMName;
        }
        if (pMeasure.ProductSN != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::ProductSN << pMeasure.ProductSN;
        }
        if (pMeasure.Task != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Task << pMeasure.Task;
        }
        if (pMeasure.Test != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Test << pMeasure.Test;
        }
        if (pMeasure.TestPoint != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TestPoint << pMeasure.TestPoint;
        }
        if (pMeasure.Remark != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Remark << pMeasure.Remark;
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
        cMeasure.Value = pBsonDocument[TcMeasure::TcMeasureAttributes::Value].get_double().value;
        cMeasure.Timestamp = chrono::system_clock::time_point{ pBsonDocument[TcMeasure::TcMeasureAttributes::Timestamp].get_date().value };
	    cMeasure.HighLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::HighLimit].get_double().value;
	    cMeasure.LowLimit = pBsonDocument[TcMeasure::TcMeasureAttributes::LowLimit].get_double().value;
        cMeasure.TimestampMicroEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::TimestampMicroEpoch].get_int64().value;
        cMeasure.TimestampEpoch = pBsonDocument[TcMeasure::TcMeasureAttributes::TimestampEpoch].get_int64().value;
        cMeasure.Result = pBsonDocument[TcMeasure::TcMeasureAttributes::Result].get_utf8().value.to_string();
        cMeasure.Test = pBsonDocument[TcMeasure::TcMeasureAttributes::Test].get_utf8().value.to_string();
        cMeasure.Task = pBsonDocument[TcMeasure::TcMeasureAttributes::Task].get_utf8().value.to_string();
        cMeasure.Remark = pBsonDocument[TcMeasure::TcMeasureAttributes::Remark].get_utf8().value.to_string();
        cMeasure.TPGMName = pBsonDocument[TcMeasure::TcMeasureAttributes::TPGMName].get_utf8().value.to_string();
        cMeasure.ProductSN = pBsonDocument[TcMeasure::TcMeasureAttributes::ProductSN].get_utf8().value.to_string();
        cMeasure.ReferenceID = pBsonDocument[TcMeasure::TcMeasureAttributes::ReferenceID].get_utf8().value.to_string();
        cMeasure.ReferenceType = pBsonDocument[TcMeasure::TcMeasureAttributes::ReferenceType].get_utf8().value.to_string();
        cMeasure.MeasureType = pBsonDocument[TcMeasure::TcMeasureAttributes::MeasureType].get_utf8().value.to_string();
        cMeasure.MeasureUnit = pBsonDocument[TcMeasure::TcMeasureAttributes::MeasureUnit].get_utf8().value.to_string();
        cMeasure.DrawReference = pBsonDocument[TcMeasure::TcMeasureAttributes::DrawReference].get_utf8().value.to_string();
        cMeasure.RelayProbeType = pBsonDocument[TcMeasure::TcMeasureAttributes::RelayProbeType].get_utf8().value.to_string();
        return(cMeasure);
    } catch(exception e){
        return(TcMeasure());
    }
}
bsoncxx::document::view TcMeasure::fSerializeObjectBsonView(TcMeasure pMeasure){
    try {
        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};

        if (pMeasure.ReferenceID != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::ReferenceID << pMeasure.ReferenceID;
        }
        if (pMeasure.ReferenceType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::ReferenceType << pMeasure.ReferenceType;
        }
        if (pMeasure.Timestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Timestamp << bsoncxx::types::b_date{ pMeasure.Timestamp };
        }
        if (pMeasure.TimestampEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TimestampEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.TimestampEpoch };
        }
        if (pMeasure.TimestampMicroEpoch != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TimestampMicroEpoch << bsoncxx::types::b_int64{ (int64_t) pMeasure.TimestampMicroEpoch };
        }
        if (pMeasure.MeasureType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::MeasureType << pMeasure.MeasureType;
        }
        if (pMeasure.MeasureUnit != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::MeasureUnit << pMeasure.MeasureUnit;
        }
        if (pMeasure.DrawReference != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::DrawReference << pMeasure.DrawReference;
        }
        if (pMeasure.RelayProbeType != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::RelayProbeType << pMeasure.RelayProbeType;
        }
        if (pMeasure.StressIndex != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::StressIndex << pMeasure.StressIndex;
        }
        if (pMeasure.HighLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::HighLimit << pMeasure.HighLimit;
        }
        if (pMeasure.LowLimit != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::LowLimit << pMeasure.LowLimit;
        }
        if (pMeasure.Value != 0) {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Value << pMeasure.Value;
        }
        if (pMeasure.Result != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Result << pMeasure.Result;
        }
        if (pMeasure.TPGMName != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TPGMName << pMeasure.TPGMName;
        }
        if (pMeasure.ProductSN != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::ProductSN << pMeasure.ProductSN;
        }
        if (pMeasure.Task != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Task << pMeasure.Task;
        }
        if (pMeasure.Test != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Test << pMeasure.Test;
        }
        if (pMeasure.TestPoint != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::TestPoint << pMeasure.TestPoint;
        }
        if (pMeasure.Remark != "") {
            cBsonDocumentBuilder << TcMeasure::TcMeasureAttributes::Remark << pMeasure.Remark;
        }

        cBsonDocumentBuilder << bsoncxx::builder::stream::finalize;
        
        bsoncxx::document::view cBsonDocument = cBsonDocumentBuilder.extract();
        return(cBsonDocument);
    } catch(exception e){
        return(bsoncxx::document::view());
    }

}

const string TcMeasure::TcMeasureAttributes::_id = "_id";



const string TcMeasure::TcMeasureAttributes::DrawReference = "DrawReference";
const string TcMeasure::TcMeasureAttributes::StressIndex = "StressIndex";
const string TcMeasure::TcMeasureAttributes::MeasureType = "MeasureType";
const string TcMeasure::TcMeasureAttributes::MeasureUnit = "MeasureUnit";
const string TcMeasure::TcMeasureAttributes::ReferenceID = "ReferenceID";
const string TcMeasure::TcMeasureAttributes::ReferenceType = "ReferenceType";

const string TcMeasure::TcMeasureAttributes::HighLimit = "HighLimit";
const string TcMeasure::TcMeasureAttributes::LowLimit = "LowLimit";
const string TcMeasure::TcMeasureAttributes::Value = "Value";
const string TcMeasure::TcMeasureAttributes::Result = "Result";
const string TcMeasure::TcMeasureAttributes::TPGMName = "TPGMName";
const string TcMeasure::TcMeasureAttributes::ProductSN = "ProductSN";
const string TcMeasure::TcMeasureAttributes::Task = "Task";
const string TcMeasure::TcMeasureAttributes::Test = "Test";
const string TcMeasure::TcMeasureAttributes::TestPoint = "TestPoint";
const string TcMeasure::TcMeasureAttributes::Remark = "Remark";
const string TcMeasure::TcMeasureAttributes::RelayProbeType = "RelayProbeType";
const string TcMeasure::TcMeasureAttributes::Timestamp = "Timestamp";
const string TcMeasure::TcMeasureAttributes::TimestampEpoch = "TimestampEpoch";
const string TcMeasure::TcMeasureAttributes::TimestampMicroEpoch = "TimestampMicroEpoch";


