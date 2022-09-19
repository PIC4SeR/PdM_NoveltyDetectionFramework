#include "../include/TcEvent.h"
#include <bsoncxx/json.hpp>

using namespace Spea::Archimede::Data;

TcEvent::TcEvent() { 
    this->_id = "";
    this->rpEventData = "";
    this->rpEventID = (uint64_t)0;
    this->rpTestCell = "";
    this->rpTimestamp = chrono::time_point<chrono::system_clock, chrono::milliseconds>();
    this->rpTimestampEpoch = (uint64_t)0;
    this->rpRemark = ""; 
}
TcEvent::TcEvent(string pId, uint64_t pEventID, string pEventData, string pTestCell, chrono::system_clock::time_point pTimestamp, uint64_t pTimestampEpoch, string pRemark){
    this->_id = pId;
    this->rpEventData = pEventData;   
	this->rpEventID = pEventID;
    this->rpTestCell = pTestCell;
    this->rpTimestamp = pTimestamp;
    this->rpTimestampEpoch = pTimestampEpoch;
    this->rpRemark = pRemark;
 }
TcEvent::TcEvent(bsoncxx::document::value pBsonDocument){
    bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
    this->rpEventData = cBsonDocumentView[TcEvent::TcEventAttributes::rpEventData].get_utf8().value.to_string();
	this->rpEventID = cBsonDocumentView[TcEvent::TcEventAttributes::rpEventID].get_int64().value;
    this->rpTestCell = cBsonDocumentView[TcEvent::TcEventAttributes::rpTestCell].get_utf8().value.to_string();
    this->rpTimestamp = chrono::system_clock::time_point{ cBsonDocumentView[TcEvent::TcEventAttributes::rpTimestamp].get_date().value };
    this->rpTimestampEpoch = cBsonDocumentView[TcEvent::TcEventAttributes::rpTimestampEpoch].get_int64().value;
    this->rpRemark = cBsonDocumentView[TcEvent::TcEventAttributes::rpRemark].get_utf8().value.to_string();
  
}
TcEvent::TcEvent(bsoncxx::document::view pBsonDocument){
	this->rpEventData = pBsonDocument[TcEvent::TcEventAttributes::rpEventData].get_utf8().value.to_string();
	this->rpEventID = pBsonDocument[TcEvent::TcEventAttributes::rpEventID].get_int64().value;
    this->rpTestCell = pBsonDocument[TcEvent::TcEventAttributes::rpTestCell].get_utf8().value.to_string();
    this->rpTimestamp = chrono::system_clock::time_point{ pBsonDocument[TcEvent::TcEventAttributes::rpTimestamp].get_date().value };
    this->rpTimestampEpoch = pBsonDocument[TcEvent::TcEventAttributes::rpTimestampEpoch].get_int64().value;
    this->rpRemark = pBsonDocument[TcEvent::TcEventAttributes::rpRemark].get_utf8().value.to_string();
   
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
        cEvent.rpEventData = cBsonDocumentView[TcEvent::TcEventAttributes::rpEventData].get_utf8().value.to_string();
	    cEvent.rpEventID = cBsonDocumentView[TcEvent::TcEventAttributes::rpEventID].get_int64().value;
        cEvent.rpTestCell = cBsonDocumentView[TcEvent::TcEventAttributes::rpTestCell].get_utf8().value.to_string();
        cEvent.rpTimestamp = chrono::system_clock::time_point{ cBsonDocumentView[TcEvent::TcEventAttributes::rpTimestamp].get_date().value };
        cEvent.rpTimestampEpoch = cBsonDocumentView[TcEvent::TcEventAttributes::rpTimestampEpoch].get_int64().value;
        cEvent.rpRemark = cBsonDocumentView[TcEvent::TcEventAttributes::rpRemark].get_utf8().value.to_string();
        return(cEvent);
    } catch(exception e){
        return(TcEvent());
    }
}
bsoncxx::document::value TcEvent::fSerializeObjectBsonValue(TcEvent pEvent){
    try {


        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};

        if (pEvent.rpEventID > 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpEventID << (int64_t) pEvent.rpEventID;
        }
        if (pEvent.rpEventData != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpEventData << pEvent.rpEventData;
        }
        if (pEvent.rpTimestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpTimestamp << bsoncxx::types::b_date{ pEvent.rpTimestamp };
        }
        if (pEvent.rpTimestampEpoch != 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpTimestampEpoch << (int64_t) pEvent.rpTimestampEpoch;
        }
        if (pEvent.rpTestCell != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpTestCell << pEvent.rpTestCell;
        }
        if (pEvent.rpRemark != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpRemark << pEvent.rpRemark;
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
        cEvent.rpEventData = pBsonDocument[TcEvent::TcEventAttributes::rpEventData].get_utf8().value.to_string();
	    cEvent.rpEventID = pBsonDocument[TcEvent::TcEventAttributes::rpEventID].get_int64().value;
        cEvent.rpTestCell = pBsonDocument[TcEvent::TcEventAttributes::rpTestCell].get_utf8().value.to_string();
        cEvent.rpTimestamp = chrono::system_clock::time_point{ pBsonDocument[TcEvent::TcEventAttributes::rpTimestamp].get_date().value };
        cEvent.rpTimestampEpoch = pBsonDocument[TcEvent::TcEventAttributes::rpTimestampEpoch].get_int64().value;
        cEvent.rpRemark = pBsonDocument[TcEvent::TcEventAttributes::rpRemark].get_utf8().value.to_string();
        return(cEvent);
    } catch(exception e){
        return(TcEvent());
    }
}
bsoncxx::document::view TcEvent::fSerializeObjectBsonView(TcEvent pEvent){
    try {
        bsoncxx::builder::stream::document cBsonDocumentBuilder = bsoncxx::builder::stream::document{};

        if (pEvent.rpEventID > 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpEventID << bsoncxx::types::b_int64{ (int64_t) pEvent.rpEventID };
        }
        if (pEvent.rpEventData != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpEventData << pEvent.rpEventData;
        }
        if (pEvent.rpTimestamp != chrono::system_clock::time_point()) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpTimestamp << bsoncxx::types::b_date{ pEvent.rpTimestamp };
        }
        if (pEvent.rpTimestampEpoch != 0) {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpTimestampEpoch << bsoncxx::types::b_int64{ (int64_t) pEvent.rpTimestampEpoch };
        }
        if (pEvent.rpTestCell != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpTestCell << pEvent.rpTestCell;
        }
        if (pEvent.rpRemark != "") {
            cBsonDocumentBuilder << TcEvent::TcEventAttributes::rpRemark << pEvent.rpRemark;
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
const string TcEvent::TcEventAttributes::rpEventID = "rpEventID";
const string TcEvent::TcEventAttributes::rpEventData = "rpEventData";
const string TcEvent::TcEventAttributes::rpTestCell = "rpTestCell";
const string TcEvent::TcEventAttributes::rpTimestamp = "rpTimestamp";
const string TcEvent::TcEventAttributes::rpTimestampEpoch = "rpTimestampEpoch";
const string TcEvent::TcEventAttributes::rpRemark = "rpRemark";
const string TcEvent::kCollection = "Events";