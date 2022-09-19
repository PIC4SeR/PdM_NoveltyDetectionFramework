#pragma once
#ifndef TCEVENT_H
#define TCEVENT_H

#include <string>
#include <inttypes.h>
#include <chrono>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/builder/stream/document.hpp>



namespace Spea {
    namespace Archimede {
        namespace Data {



            using namespace std;



            //Event, Misure, Dati



            class TcEvent
            {
            public:

                static const string kCollection;



                string _id;
                uint64_t rpEventID;
                string rpEventData;
                string rpTestCell;
                chrono::system_clock::time_point rpTimestamp;
                uint64_t rpTimestampEpoch;
                string rpRemark;

                class TcEventAttributes {
                public:
                    static const string _id;
                    static const string rpEventID;
                    static const string rpEventData;
                    static const string rpTestCell;
                    static const string rpTimestamp;
                    static const string rpTimestampEpoch;
                    static const string TImestampMicroEpoch;
                    static const string rpRemark;
                };

                TcEvent();
                TcEvent(string pId, uint64_t pEventID, string pEventData, string pTestCell, chrono::system_clock::time_point pTimestamp, uint64_t pTimestampEpoch, string pRemark);
                TcEvent(bsoncxx::document::value pBsonDocument);
                TcEvent(bsoncxx::document::view pBsonDocument);

                void fSetTimestamp(chrono::system_clock::time_point pTimestamp) {
                    this->rpTimestamp = pTimestamp;
                    this->rpTimestampEpoch = (uint64_t)chrono::duration_cast<chrono::milliseconds>(pTimestamp.time_since_epoch()).count();
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

        }
    }
}

#endif
