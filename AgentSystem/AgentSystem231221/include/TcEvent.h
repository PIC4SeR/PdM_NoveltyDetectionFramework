#pragma once
#ifndef TCEVENT_H
#define TCEVENT_H

#include <string>
#include <inttypes.h>
#include <chrono>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using namespace std;



//Event, Misure, Dati


class TcEvent
{
    public:
        string _id;
        uint64_t EventID;
        string EventData;
        string TestCell;
        chrono::system_clock::time_point Timestamp;
        uint64_t TimestampEpoch;
        string Remark;

        class TcEventAttributes {
        public:
            static const string _id;
            static const string EventID;
            static const string EventData;
            static const string TestCell;
            static const string Timestamp;
            static const string TimestampEpoch;
            static const string TImestampMicroEpoch;
            static const string Remark;
        };

        TcEvent();
        TcEvent(string pId, uint64_t pEventID, string pEventData, string pTestCell, chrono::system_clock::time_point pTimestamp, uint64_t pTimestampEpoch, string pRemark);
        TcEvent(bsoncxx::document::value pBsonDocument);
        TcEvent(bsoncxx::document::view pBsonDocument);
        
        void fSetTimestamp(chrono::system_clock::time_point pTimestamp){
            this->Timestamp = pTimestamp;
            this->TimestampEpoch =  (uint64_t) chrono::duration_cast<chrono::milliseconds>(pTimestamp.time_since_epoch()).count();
        }

        

        static TcEvent fDeserializeObjectBsonValue(bsoncxx::document::value pBsonDocument);
        static TcEvent fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocument);
        static bsoncxx::document::value fSerializeObjectBsonValue(TcEvent pEvent);
        static bsoncxx::document::view fSerializeObjectBsonView(TcEvent pEvent);

        TcEvent from_bsonvalue(bsoncxx::document::value pBsonDocument);
        TcEvent from_bsonview(bsoncxx::document::view pBsonDocument);
        TcEvent from_json(string pJsonDocument);
        
       
        bsoncxx::document::value to_bsonvalue();
        string to_json();
        bsoncxx::document::view to_bsonview();

     
};



#endif
