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
    string rpReferenceID;
    string rpReferenceType;
    string rpMeasureUnit;
    string rpMeasureType;
    string rpDrawReference;
    string rpRelayProbeType;
    chrono::time_point<chrono::system_clock, chrono::milliseconds> rpTimestamp;
    uint64_t rpTimestampEpoch;
    uint64_t rpTimestampMicroEpoch;
    int rpStressIndex;
    double rpValue;
    double rpHighLimit;
    double rpLowLimit;
    string rpResult;
    string rpTPGMName;
    string rpProductSN;
    string rpTask;
    string rpTest;
    string rpTestPoint;
    string rpRemark;

    class TcMeasureAttributes {
    public:
        static const string _id;
        static const string kReferenceID;
        static const string kReferenceType;
        static const string kMeasureUnit;
        static const string kMeasureType;
        static const string kDrawReference;
        static const string kRelayProbeType;
        static const string kTimestamp;
        static const string kTimestampEpoch;
        static const string kTimestampMicroEpoch;
        static const string kEndTimestamp;
        static const string kEndTimestampEpoch;
        static const string kStressIndex;
        static const string kValue;
        static const string kHighLimit;
        static const string kLowLimit;
        static const string kResult;
        static const string kTPGMName;
        static const string kProductSN;
        static const string kTask;
        static const string kTest;
        static const string kTestPoint;
        static const string kRemark;
    };

    TcMeasure();
    ~TcMeasure();
    TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, string pDrawReference, string pRelayProbeType, chrono::time_point<chrono::system_clock, chrono::milliseconds> pTimestamp, uint64_t pTimestampEpoch, uint64_t pTimestampMicroEpoch, int pStressIndex, double pValue, double pHighLimit, double pLowLimit, string kResult, string pTPGMName, string pProductSN, string pTask, string pTest, string pTestPoint, string pRemark);
    TcMeasure(string pId, string pReferenceID, string pReferenceType, string pMeasureUnit, string pMeasureType, chrono::time_point<chrono::system_clock, chrono::milliseconds> pTimestamp, double pValue, double pHighLimit, double pLowLimit, string pTest);
    TcMeasure(bsoncxx::document::value pBsonDocument);
    TcMeasure(bsoncxx::document::view pBsonDocument);

    TcMeasure(const TcMeasure& other);
    TcMeasure(TcMeasure&& other);
    TcMeasure& operator=(const TcMeasure& other);
    TcMeasure& operator=(TcMeasure&& other);



    void fSetTimestamp(chrono::time_point<chrono::system_clock, chrono::milliseconds> pTimestamp) {
        this->rpTimestamp = pTimestamp;
        this->rpTimestampEpoch = (uint64_t)chrono::duration_cast<chrono::milliseconds>(pTimestamp.time_since_epoch()).count();
        this->rpTimestampMicroEpoch = (uint64_t)chrono::duration_cast<chrono::microseconds>(pTimestamp.time_since_epoch()).count();
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
