#pragma once
#ifndef TCMEASURE_H
#define TCMEASURE_H

#include <string>
#include <inttypes.h>
#include <chrono>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/value_context.hpp>


using namespace std;

class TcMeasure
{
    public:
        static const string kCollection;

        string _id;
        string ReferenceID;
        string ReferenceType;
        string MeasureUnit;
        string MeasureType;
        string DrawReference;
        string RelayProbeType;
        chrono::system_clock::time_point Timestamp;
        uint64_t TimestampEpoch;
        uint64_t TimestampMicroEpoch;
        int StressIndex;
        double Value;
        double HighLimit;
        double LowLimit;
        string Result;
        string TPGMName;
        string ProductSN;
        string Task;
        string Test;
        string TestPoint;
        string Remark;

        class TcMeasureAttributes{
        public:
            static const string _id;
            static const string ReferenceID;
            static const string ReferenceType;
            static const string MeasureUnit;
            static const string MeasureType;
            static const string DrawReference;
            static const string RelayProbeType;
            static const string Timestamp;
            static const string TimestampEpoch;
            static const string TimestampMicroEpoch;
            static const string EndTimestamp;
            static const string EndTimestampEpoch;
            static const string StressIndex;
            static const string Value;
            static const string HighLimit;
            static const string LowLimit;
            static const string Result;
            static const string TPGMName;
            static const string ProductSN;
            static const string Task;
            static const string Test;
            static const string TestPoint;
            static const string Remark;
        };

        TcMeasure();
        ~TcMeasure();
        TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, string pDrawReference, string pRelayProbeType, chrono::system_clock::time_point pTimestamp, uint64_t pTimestampEpoch, uint64_t pTimestampMicroEpoch, int pStressIndex, double pValue, double pHighLimit, double pLowLimit, string Result, string pTPGMName, string pProductSN, string pTask, string pTest, string pTestPoint, string pRemark);
        TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, chrono::system_clock::time_point pTimestamp, double pValue, double pHighLimit, double pLowLimit, string pTest);
        TcMeasure(bsoncxx::document::value pBsonDocument);
        TcMeasure(bsoncxx::document::view pBsonDocument);
        
        void fSetTimestamp(chrono::system_clock::time_point pTimestamp){
            this->Timestamp = pTimestamp;
            this->TimestampEpoch =  (uint64_t) chrono::duration_cast<chrono::milliseconds>(pTimestamp.time_since_epoch()).count();
            this->TimestampMicroEpoch =  (uint64_t) chrono::duration_cast<chrono::microseconds>(pTimestamp.time_since_epoch()).count();
        }

        TcMeasure from_bsonvalue(bsoncxx::document::value pBsonDocument);
        TcMeasure from_bsonview(bsoncxx::document::view pBsonDocument);
        TcMeasure from_json(string pJsonDocument);


        bsoncxx::document::value to_bsonvalue();
        bsoncxx::document::view to_bsonview();
        string to_json();


        static TcMeasure fDeserializeObjectBsonValue(bsoncxx::document::value pBsonDocument);
        static TcMeasure fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocument);
        
        static bsoncxx::document::value fSerializeObjectBsonValue(TcMeasure pMeasure);
        static bsoncxx::document::view fSerializeObjectBsonView(TcMeasure pMeasure);








};

#endif
