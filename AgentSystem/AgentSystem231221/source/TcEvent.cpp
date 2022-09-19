#include <iostream>
#include "../include/TcEvent.h"
#include <bsoncxx/json.hpp>

TcEvent::TcEvent() { ; }

TcEvent::TcEvent(string pId, uint64_t pEventID, string pEventData, string pTestCell, chrono::system_clock::time_point pTimestamp, uint64_t pTimestampEpoch, string pRemark){
    this->_id = pId;
    this->EventData = pEventData;   
	this->EventID = pEventID;
    this->TestCell = pTestCell;
    this->Timestamp = pTimestamp;
    this->TimestampEpoch = pTimestampEpoch;
    this->Remark = pRemark;
 }

TcEvent::TcEvent(bsoncxx::document::value pBsonDocument){
    bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
    this->EventData = cBsonDocumentView[TcEvent::TcEventAttributes::EventData].get_utf8().value.to_string();
	this->EventID = cBsonDocumentView[TcEvent::TcEventAttributes::EventID].get_int64().value;
    this->TestCell = cBsonDocumentView[TcEvent::TcEventAttributes::TestCell].get_utf8().value.to_string();
    this->Timestamp = chrono::system_clock::time_point{ cBsonDocumentView[TcEvent::TcEventAttributes::Timestamp].get_date().value };
    this->TimestampEpoch = cBsonDocumentView[TcEvent::TcEventAttributes::TimestampEpoch].get_int64().value;
    this->Remark = cBsonDocumentView[TcEvent::TcEventAttributes::Remark].get_utf8().value.to_string();
  
}
TcEvent::TcEvent(bsoncxx::document::view pBsonDocument){
	this->EventData = pBsonDocument[TcEvent::TcEventAttributes::EventData].get_utf8().value.to_string();
	this->EventID = pBsonDocument[TcEvent::TcEventAttributes::EventID].get_int64().value;
    this->TestCell = pBsonDocument[TcEvent::TcEventAttributes::TestCell].get_utf8().value.to_string();
    this->Timestamp = chrono::system_clock::time_point{ pBsonDocument[TcEvent::TcEventAttributes::Timestamp].get_date().value };
    this->TimestampEpoch = pBsonDocument[TcEvent::TcEventAttributes::TimestampEpoch].get_int64().value;
    this->Remark = pBsonDocument[TcEvent::TcEventAttributes::Remark].get_utf8().value.to_string();
   
}

TcEvent TcEvent::from_bsonvalue(bsoncxx::document::value pBsonDocument){
    return(TcEvent::fDeserializeObjectBsonValue(pBsonDocument));
}

TcEvent TcEvent::from_bsonview(bsoncxx::document::view pBsonDocument){
    return(TcEvent::fDeserializeObjectBsonView(pBsonDocument));
}
TcEvent TcEvent::from_json(string pJsonDocument){
    return(TcEvent::fDeserializeObjectBsonValue(bsoncxx::from_json(pJsonDocument)));
}

bsoncxx::document::value TcEvent::to_bsonvalue(){
    return(TcEvent::fSerializeObjectBsonValue(*this));
}
bsoncxx::document::view TcEvent::to_bsonview(){
    return(TcEvent::fSerializeObjectBsonView(*this));
}

string TcEvent::to_json(){
    return(bsoncxx::to_json(TcEvent::fSerializeObjectBsonValue(*this)));
}

TcEvent TcEvent::fDeserializeObjectBsonValue(bsoncxx::document::value pBsonDocument){
	try {
        TcEvent cEvent;
        bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
        cEvent.EventData = cBsonDocumentView[TcEvent::TcEventAttributes::EventData].get_utf8().value.to_string();
	    cEvent.EventID = cBsonDocumentView[TcEvent::TcEventAttributes::EventID].get_int64().value;
        cEvent.TestCell = cBsonDocumentView[TcEvent::TcEventAttributes::TestCell].get_utf8().value.to_string();
        cEvent.Timestamp = chrono::system_clock::time_point{ cBsonDocumentView[TcEvent::TcEventAttributes::Timestamp].get_date().value };
        cEvent.TimestampEpoch = cBsonDocumentView[TcEvent::TcEventAttributes::TimestampEpoch].get_int64().value;
        cEvent.Remark = cBsonDocumentView[TcEvent::TcEventAttributes::Remark].get_utf8().value.to_string();
        return(cEvent);
    } catch(exception e){
        return(TcEvent());
    }
}

bsoncxx::document::value TcEvent::fSerializeObjectBsonValue(TcEvent pEvent){
    try {


        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};

        if (pEvent.EventID > 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::EventID << (int64_t) pEvent.EventID;
        }
        if (pEvent.EventData != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::EventData << pEvent.EventData;
        }
        if (pEvent.Timestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::Timestamp << bsoncxx::types::b_date{ pEvent.Timestamp };
        }
        if (pEvent.TimestampEpoch != 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::TimestampEpoch << (int64_t) pEvent.TimestampEpoch;
        }
        if (pEvent.TestCell != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::TestCell << pEvent.TestCell;
        }
        if (pEvent.Remark != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::Remark << pEvent.Remark;
        }

        cBsonDocumentBuilder << bsoncxx::builder::stream::finalize;

        bsoncxx::document::value cBsonDocument = cBsonDocumentBuilder.extract();
        return(cBsonDocument);
    } catch(exception e){
        return(bsoncxx::builder::stream::document{} << "" << "" << bsoncxx::builder::stream::finalize);
    }

}
TcEvent TcEvent::fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocument){
	try {
        TcEvent cEvent;
        cEvent.EventData = pBsonDocument[TcEvent::TcEventAttributes::EventData].get_utf8().value.to_string();
	    cEvent.EventID = pBsonDocument[TcEvent::TcEventAttributes::EventID].get_int64().value;
        cEvent.TestCell = pBsonDocument[TcEvent::TcEventAttributes::TestCell].get_utf8().value.to_string();
        cEvent.Timestamp = chrono::system_clock::time_point{ pBsonDocument[TcEvent::TcEventAttributes::Timestamp].get_date().value };
        cEvent.TimestampEpoch = pBsonDocument[TcEvent::TcEventAttributes::TimestampEpoch].get_int64().value;
        cEvent.Remark = pBsonDocument[TcEvent::TcEventAttributes::Remark].get_utf8().value.to_string();
        return(cEvent);
    } catch(exception e){
        return(TcEvent());
    }
}
bsoncxx::document::view TcEvent::fSerializeObjectBsonView(TcEvent pEvent){
    try {
        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};

        if (pEvent.EventID > 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::EventID << bsoncxx::types::b_int64{ (int64_t) pEvent.EventID };
        }
        if (pEvent.EventData != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::EventData << pEvent.EventData;
        }
        if (pEvent.Timestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::Timestamp << bsoncxx::types::b_date{ pEvent.Timestamp };
        }
        if (pEvent.TimestampEpoch != 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::TimestampEpoch << bsoncxx::types::b_int64{ (int64_t) pEvent.TimestampEpoch };
        }
        if (pEvent.TestCell != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::TestCell << pEvent.TestCell;
        }
        if (pEvent.Remark != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::Remark << pEvent.Remark;
        }

        cBsonDocumentBuilder << bsoncxx::builder::stream::finalize;
        bsoncxx::document::value cBsonDocument = cBsonDocumentBuilder.extract();
        return(cBsonDocument.view());
    }
    catch (exception e) {
        return(bsoncxx::document::view());
    }

}



const string TcEvent::TcEventAttributes::_id = "_id";
const string TcEvent::TcEventAttributes::EventID = "EventID";
const string TcEvent::TcEventAttributes::EventData = "EventData";
const string TcEvent::TcEventAttributes::TestCell = "TestCell";
const string TcEvent::TcEventAttributes::Timestamp = "Timestamp";
const string TcEvent::TcEventAttributes::TimestampEpoch = "TimestampEpoch";
const string TcEvent::TcEventAttributes::Remark = "Remark";