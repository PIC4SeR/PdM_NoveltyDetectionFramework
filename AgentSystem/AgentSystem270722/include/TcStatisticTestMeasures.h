#pragma once

#ifndef TCSTATISTICTESTMEASURES_H
#define TCSTATISTICTESTMEASURES_H

#include <string>
#include <vector>
#include <chrono>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/exception/exception.hpp>	
#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include "TcMeasure.h"

namespace Spea {
	namespace Archimede {
		namespace MultiAgentSystem {
			namespace Tools {

				using namespace std;
				using namespace Spea::Archimede::Data;

				class TcStatisticTestMeasures {
				public:

					class TcStatisticTestMeasuresAttributes {
					public:
						static const string _id;
						static const string kMeasures;
						static const string kVariance;
						static const string kErrors;
						static const string kMSE;
						static const string kMoving;
						static const string kStatistics;
						static const string kMean;
						static const string kStdDev;
						static const string kTest;
						static const string kCalibration;
						static const string kSize;
						static const string kTimestamp;
						static const string kTimestampEpoch;
						static const string kTimestampMicroEpoch;
						static const string kWindow;
					};

					int rpWindow;
					string rpTest;
					string rpMeasureUnit;
					string rpMeasureType;
					string rpReferenceType;
					string rpReferenceID;

					vector<double> cpValues;
					vector<double> cpCalibrations;
					vector<double> cpHighThresholds;
					vector<double> cpLowThresholds;
					vector<unsigned long long> cpTimes;
					vector<TcMeasure> cpMeasures;

					vector<double> cpErrors; // Pow(Calibration - Values, 2)
					double rpCP; //CP(Values)
					double rpCPk; //CPk(Values)
					double rpStdDev; // Std(Values)
					double rpMean; // Mean(Values)
					double rpMSE;

					vector<double> cpCPs; //Moving CP
					vector<double> cpCPks; //Moving CPk
					vector<double> cpMovingAverages;  //Moving Average
					vector<double> cpMovingVariances; //Moving Variance
					vector<double> cpMovingMSEs; //Moving Mean Squared Error
					vector<unsigned long long> cpMovingTimes; //time instant for each moving features

					TcStatisticTestMeasures(bsoncxx::document::value pBsonDocument);
					TcStatisticTestMeasures(bsoncxx::document::view pBsonDocument);
					TcStatisticTestMeasures(int pWindow, string pTest, double pMean, double pStdDev);
					TcStatisticTestMeasures();
					TcStatisticTestMeasures(const TcStatisticTestMeasures& pOther);
					TcStatisticTestMeasures(TcStatisticTestMeasures&& pOther);
					~TcStatisticTestMeasures();

					static void fDeserializeObjectBsonValue(bsoncxx::document::value pBsonDocument, TcStatisticTestMeasures* pMeasures);
					static bsoncxx::document::value fSerializeObjectBsonValue(TcStatisticTestMeasures pMeasures);
					static void fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocument, TcStatisticTestMeasures* pMeasures);
					static bsoncxx::document::view fSerializeObjectBsonView(TcStatisticTestMeasures pMeasures);

					TcStatisticTestMeasures& operator=(const TcStatisticTestMeasures& pOther);
					TcStatisticTestMeasures& operator=(TcStatisticTestMeasures&& pOther);
					TcStatisticTestMeasures from_bsonvalue(bsoncxx::document::value pBsonDocument);
					TcStatisticTestMeasures from_bsonview(bsoncxx::document::view pBsonDocument);
					TcStatisticTestMeasures from_json(string pJsonDocument);

					bsoncxx::document::value to_bsonvalue();
					bsoncxx::document::view to_bsonview();
					string to_json();

					friend bool operator<(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures);
					friend bool operator==(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures);
					friend bool operator>(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures);
					friend bool operator<=(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures);
					friend bool operator>=(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures);
				};
			}
		}
	}
}

#endif