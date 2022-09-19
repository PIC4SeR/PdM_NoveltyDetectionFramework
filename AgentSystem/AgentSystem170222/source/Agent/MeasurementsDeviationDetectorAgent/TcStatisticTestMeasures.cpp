#include <math.h>
#include "../../../include/TcMeasure.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h"



TcStatisticTestMeasures::TcStatisticTestMeasures(bsoncxx::document::value pBsonDocument) {
	TcStatisticTestMeasures cMeasuredValuesInTest;
	TcStatisticTestMeasures::fDeserializeObjectBsonValue(pBsonDocument, &cMeasuredValuesInTest);

	this->rpWindow = cMeasuredValuesInTest.rpWindow;
	this->rpTest = cMeasuredValuesInTest.rpTest;
	this->rpMean = cMeasuredValuesInTest.rpMean;
	this->rpStdDev = cMeasuredValuesInTest.rpStdDev;
	this->rpCP = cMeasuredValuesInTest.rpCP;
	this->rpCPk = cMeasuredValuesInTest.rpCPk;
	this->rpWindow = cMeasuredValuesInTest.rpWindow;
	this->rpMSE = cMeasuredValuesInTest.rpMSE;

	this->cpErrors = cMeasuredValuesInTest.cpErrors;
	this->cpCalibrations = cMeasuredValuesInTest.cpCalibrations;
	this->cpValues = cMeasuredValuesInTest.cpValues;
	this->cpHighThresholds = cMeasuredValuesInTest.cpHighThresholds;
	this->cpLowThresholds = cMeasuredValuesInTest.cpLowThresholds;
	this->cpTimes = cMeasuredValuesInTest.cpTimes;

	this->cpCPs = cMeasuredValuesInTest.cpCPs;
	this->cpCPks = cMeasuredValuesInTest.cpCPks;
	this->cpMovingAverages = cMeasuredValuesInTest.cpMovingAverages;
	this->cpMovingVariances = cMeasuredValuesInTest.cpMovingVariances;
	this->cpMovingMSEs = cMeasuredValuesInTest.cpMovingMSEs;
	this->cpMovingTimes = cMeasuredValuesInTest.cpMovingTimes;
}

TcStatisticTestMeasures::TcStatisticTestMeasures(bsoncxx::document::view pBsonDocument) {
	TcStatisticTestMeasures cMeasuredValuesInTest;
	TcStatisticTestMeasures::fDeserializeObjectBsonView(pBsonDocument, &cMeasuredValuesInTest);
	
	*this = cMeasuredValuesInTest;
	
}

TcStatisticTestMeasures::TcStatisticTestMeasures() {
	this->rpWindow = 0;
	this->rpTest = "";
	this->rpMean = 0;
	this->rpStdDev = 0;
	this->rpCP = 0;
	this->rpCPk = 0;
	this->rpMSE = 0;

	this->rpCP = 0;
	this->rpCPk = 0;
	/*this->cpECDF = empirical_cumulative_distribution_function<vector<double>>(vector<double>(2), true);*/


	this->cpErrors = vector<double>();
	this->cpCalibrations = vector<double>();
	this->cpValues = vector<double>();
	this->cpHighThresholds = vector<double>();
	this->cpLowThresholds = vector<double>();
	this->cpTimes = vector<unsigned long long>();
	

	this->cpCPs = vector<double>();
	this->cpCPks = vector<double>();
	this->cpMovingAverages = vector<double>();
	this->cpMovingVariances = vector<double>();
	this->cpMovingMSEs = vector<double>();
	this->cpMovingTimes = vector<unsigned long long>();
	this->cpCPs = vector<double>();
	this->cpCPks = vector<double>();

	
}

TcStatisticTestMeasures::TcStatisticTestMeasures(const TcStatisticTestMeasures& other) {
	this->rpWindow = other.rpWindow;
	this->rpTest = other.rpTest;
	this->rpMean = other.rpMean;
	this->rpStdDev = other.rpStdDev;
	this->rpCP = other.rpCP;
	this->rpCPk = other.rpCPk;
	this->rpWindow = other.rpWindow;
	this->rpMSE = other.rpMSE;
	this->cpErrors = other.cpErrors;
	this->cpCalibrations = other.cpCalibrations;
	this->cpValues = other.cpValues;
	this->cpHighThresholds = other.cpHighThresholds;
	this->cpLowThresholds = other.cpLowThresholds;
	this->cpTimes = other.cpTimes;
	this->cpCPs = other.cpCPs;
	this->cpCPks = other.cpCPks;
	this->cpMovingAverages = other.cpMovingAverages;
	this->cpMovingVariances = other.cpMovingVariances;
	this->cpMovingMSEs = other.cpMovingMSEs;
	this->cpMovingTimes = other.cpMovingTimes;
	this->rpCP = other.rpCP;
	this->rpCPk = other.rpCPk;



}
TcStatisticTestMeasures::TcStatisticTestMeasures(TcStatisticTestMeasures&& other) {
	
	this->rpWindow = other.rpWindow;
	this->rpTest = other.rpTest;
	this->rpMean = other.rpMean;
	this->rpStdDev = other.rpStdDev;
	this->rpCP = other.rpCP;
	this->rpCPk = other.rpCPk;
	this->rpWindow = other.rpWindow;
	this->rpMSE = other.rpMSE;
	this->cpErrors = other.cpErrors;
	this->cpCalibrations = other.cpCalibrations;
	this->cpValues = other.cpValues;
	this->cpHighThresholds = other.cpHighThresholds;
	this->cpLowThresholds = other.cpLowThresholds;
	this->cpTimes = other.cpTimes;
	this->cpCPs = other.cpCPs;
	this->cpCPks = other.cpCPks;
	this->cpMovingAverages = other.cpMovingAverages;
	this->cpMovingVariances = other.cpMovingVariances;
	this->cpMovingMSEs = other.cpMovingMSEs;
	this->cpMovingTimes = other.cpMovingTimes;
	this->rpCP = other.rpCP;
	this->rpCPk = other.rpCPk;


	other.rpTest = "";
	other.rpWindow = 0;
	other.rpMean = 0;
	other.rpStdDev = 0;
	other.rpCP = 0;
	other.rpCPk = 0;
	other.rpWindow = 0;
	other.rpMSE = 0;
	other.cpErrors.clear();
	other.cpCalibrations.clear();
	other.cpValues.clear();
	other.cpHighThresholds.clear();
	other.cpLowThresholds.clear();
	other.cpTimes.clear();
	other.cpCPs.clear();
	other.cpCPks.clear();
	other.cpMovingAverages.clear();
	other.cpMovingVariances.clear();
	other.cpMovingMSEs.clear();
	other.cpMovingTimes.clear();

}
TcStatisticTestMeasures::~TcStatisticTestMeasures() {

	this->cpErrors.clear();
	this->cpCalibrations.clear();
	this->cpValues.clear();
	this->cpHighThresholds.clear();
	this->cpLowThresholds.clear();
	this->cpTimes.clear();
	this->cpCPs.clear();
	this->cpCPks.clear();
	this->cpMovingAverages.clear();
	this->cpMovingVariances.clear();
	this->cpMovingMSEs.clear();
	this->cpMovingTimes.clear();


	

}
TcStatisticTestMeasures& TcStatisticTestMeasures::operator=(const TcStatisticTestMeasures& other) {

	if (this != &other) {
		this->rpWindow = other.rpWindow;
		this->rpTest = other.rpTest;
		this->rpMean = other.rpMean;
		this->rpStdDev = other.rpStdDev;
		this->rpCP = other.rpCP;
		this->rpCPk = other.rpCPk;
		this->rpWindow = other.rpWindow;
		this->rpMSE = other.rpMSE;
		this->cpErrors = other.cpErrors;
		this->cpCalibrations = other.cpCalibrations;
		this->cpValues = other.cpValues;
		this->cpHighThresholds = other.cpHighThresholds;
		this->cpLowThresholds = other.cpLowThresholds;
		this->cpTimes = other.cpTimes;
		this->cpCPs = other.cpCPs;
		this->cpCPks = other.cpCPks;
		this->cpMovingAverages = other.cpMovingAverages;
		this->cpMovingVariances = other.cpMovingVariances;
		this->cpMovingMSEs = other.cpMovingMSEs;
		this->cpMovingTimes = other.cpMovingTimes;
		this->rpCP = other.rpCP;
		this->rpCPk = other.rpCPk;
	}

	return(*this);

}
TcStatisticTestMeasures& TcStatisticTestMeasures::operator=(TcStatisticTestMeasures&& other) {

	if (this != &other) {
		this->rpWindow = other.rpWindow;
		this->rpTest = other.rpTest;
		this->rpMean = other.rpMean;
		this->rpStdDev = other.rpStdDev;
		this->rpCP = other.rpCP;
		this->rpCPk = other.rpCPk;
		this->rpWindow = other.rpWindow;
		this->rpMSE = other.rpMSE;
		this->cpErrors = other.cpErrors;
		this->cpCalibrations = other.cpCalibrations;
		this->cpValues = other.cpValues;
		this->cpHighThresholds = other.cpHighThresholds;
		this->cpLowThresholds = other.cpLowThresholds;
		this->cpTimes = other.cpTimes;
		this->cpCPs = other.cpCPs;
		this->cpCPks = other.cpCPks;
		this->cpMovingAverages = other.cpMovingAverages;
		this->cpMovingVariances = other.cpMovingVariances;
		this->cpMovingMSEs = other.cpMovingMSEs;
		this->cpMovingTimes = other.cpMovingTimes;
		this->rpCP = other.rpCP;
		this->rpCPk = other.rpCPk;

		this->rpCP = (this->cpHighThresholds[0] - this->cpLowThresholds[0]) / (6 * this->rpStdDev);
		this->rpCPk = min(((this->cpLowThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)), ((this->cpHighThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)));
		for (int i = 0; i < this->cpMovingAverages.size(); i++) {
			this->cpCPs.push_back((this->cpHighThresholds[i] - this->cpLowThresholds[i]) / (6 * sqrt(this->cpMovingVariances[i])));
			this->cpCPks.push_back(min(((this->cpLowThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))), ((this->cpHighThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i])))));
		}




		other.rpTest = "";
		other.rpWindow = 0;
		other.rpMean = 0;
		other.rpStdDev = 0;
		other.rpCP = 0;
		other.rpCPk = 0;
		other.rpWindow = 0;
		other.rpMSE = 0;
		other.cpErrors.clear();
		other.cpCalibrations.clear();
		other.cpValues.clear();
		other.cpHighThresholds.clear();
		other.cpLowThresholds.clear();
		other.cpTimes.clear();
		other.cpCPs.clear();
		other.cpCPks.clear();
		other.cpMovingAverages.clear();
		other.cpMovingVariances.clear();
		other.cpMovingMSEs.clear();
		other.cpMovingTimes.clear();



	}

	return(*this);
}






void TcStatisticTestMeasures::fDeserializeObjectBsonValue(bsoncxx::document::value pBsonDocument, TcStatisticTestMeasures* pMeasures){
	try {
		bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
		TcStatisticTestMeasures cMeasuredValuesInTest;
		
		cMeasuredValuesInTest.rpTest = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].get_utf8().value.to_string();
		bsoncxx::array::view cMeasures = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures].get_array().value;
		for (const bsoncxx::array::element& cMeasure : cMeasures) {
			cMeasuredValuesInTest.cpValues.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
			cMeasuredValuesInTest.cpHighThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value);
			cMeasuredValuesInTest.cpLowThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value);
			cMeasuredValuesInTest.cpTimes.push_back((unsigned long long)cMeasure[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].get_int64().value);
		}

		cMeasuredValuesInTest.rpMeasureUnit = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpMeasureType = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceType = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceID = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpMean = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value;
		cMeasuredValuesInTest.rpStdDev = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].get_double().value;
		cMeasuredValuesInTest.rpMSE = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value;
		cMeasuredValuesInTest.rpWindow = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].get_int32().value;

		bsoncxx::array::view cCalibrations = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration].get_array().value;
		for (const bsoncxx::array::element& cCalibration : cCalibrations) {
			cMeasuredValuesInTest.cpCalibrations.push_back(cCalibration[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
		}
		bsoncxx::array::view cErrors = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors].get_array().value;
		for (const bsoncxx::array::element& cError : cErrors) {
			cMeasuredValuesInTest.cpErrors.push_back(cError[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
		}
		bsoncxx::array::view cMovingStatisticValues = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics].get_array().value;
		for (const bsoncxx::array::element& cMovingStatisticValue : cMovingStatisticValues) {
			cMeasuredValuesInTest.cpMovingAverages.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value);
			cMeasuredValuesInTest.cpMovingVariances.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].get_double().value);
			cMeasuredValuesInTest.cpMovingMSEs.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value);
			cMeasuredValuesInTest.cpMovingTimes.push_back((unsigned long long)cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].get_int64().value);
		}

		return;
    } catch(exception e){
        return;
    }
}

bsoncxx::document::value TcStatisticTestMeasures::fSerializeObjectBsonValue(TcStatisticTestMeasures pMeasuredValuesInTest){
    try {
		
		bsoncxx::builder::stream::document data_builder{};
		data_builder <<	TcMeasure::TcMeasureAttributes::kMeasureType << pMeasuredValuesInTest.rpMeasureType;
		data_builder << TcMeasure::TcMeasureAttributes::kMeasureUnit << pMeasuredValuesInTest.rpMeasureUnit;
		data_builder << TcMeasure::TcMeasureAttributes::kReferenceType << pMeasuredValuesInTest.rpReferenceType;
		data_builder << TcMeasure::TcMeasureAttributes::kReferenceID << pMeasuredValuesInTest.rpReferenceID;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest << pMeasuredValuesInTest.rpTest;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean << pMeasuredValuesInTest.rpMean;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev << pMeasuredValuesInTest.rpStdDev;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE << pMeasuredValuesInTest.rpMSE;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow << pMeasuredValuesInTest.rpWindow;
		

		auto movingavg_array_builder = bsoncxx::builder::stream::array{};
		auto movingvar_array_builder = bsoncxx::builder::stream::array{};
		auto movingmse_array_builder = bsoncxx::builder::stream::array{};
		auto movingtime_array_builder = bsoncxx::builder::stream::array{};
		auto error_array_builder = bsoncxx::builder::stream::array{};
		auto calibration_array_builder = bsoncxx::builder::stream::array{};
		auto measures_array_builder = bsoncxx::builder::stream::array{};

		for (float i = 0; i < pMeasuredValuesInTest.cpMovingAverages.size(); i++) {
			movingavg_array_builder << pMeasuredValuesInTest.cpMovingAverages[i];
			movingvar_array_builder << pMeasuredValuesInTest.cpMovingVariances[i];
			movingmse_array_builder << pMeasuredValuesInTest.cpMovingMSEs[i];
			movingtime_array_builder << (long long) pMeasuredValuesInTest.cpMovingTimes[i];
		}

		for (float i = 0; i < pMeasuredValuesInTest.cpErrors.size(); i++) {
			TcMeasure cMeasure;
			cMeasure.rpValue = pMeasuredValuesInTest.cpValues[i];
			cMeasure.rpHighLimit = pMeasuredValuesInTest.cpHighThresholds[i];
			cMeasure.rpLowLimit = pMeasuredValuesInTest.cpLowThresholds[i];
			cMeasure.rpMeasureType = pMeasuredValuesInTest.rpMeasureType;
			cMeasure.rpMeasureUnit = pMeasuredValuesInTest.rpMeasureUnit;
			cMeasure.rpReferenceType = pMeasuredValuesInTest.rpReferenceType;
			cMeasure.rpReferenceID = pMeasuredValuesInTest.rpReferenceID;
			cMeasure.rpTest = pMeasuredValuesInTest.rpTest;
			measures_array_builder << cMeasure.to_bsonvalue();
			calibration_array_builder << pMeasuredValuesInTest.cpCalibrations[i];
			error_array_builder << pMeasuredValuesInTest.cpErrors[i];
		}

		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors << error_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration << calibration_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures << measures_array_builder;



		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics << bsoncxx::builder::stream::open_document
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean << movingavg_array_builder
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance << movingvar_array_builder
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE << movingmse_array_builder
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch << movingtime_array_builder
			<< bsoncxx::builder::stream::close_document;

		bsoncxx::document::value cBsonDocument = data_builder << bsoncxx::builder::stream::finalize;
		return(cBsonDocument);
    } catch(exception e){
		return(bsoncxx::document::value(nullptr, 0, 0));
    }

}



void TcStatisticTestMeasures::fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocument, TcStatisticTestMeasures* pMeasuredValuesInTest){
	
	try {
		TcStatisticTestMeasures cMeasuredValuesInTest;
		cMeasuredValuesInTest.rpTest = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].get_utf8().value.to_string();
		bsoncxx::array::view cMeasures = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures].get_array().value;
		for (const bsoncxx::array::element& cMeasure : cMeasures) {
			cMeasuredValuesInTest.cpValues.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
			cMeasuredValuesInTest.cpHighThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value);
			cMeasuredValuesInTest.cpLowThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value);
			cMeasuredValuesInTest.cpTimes.push_back((long)cMeasure[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value);
		}

		cMeasuredValuesInTest.rpMeasureUnit = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpMeasureType = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceType = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceID = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpMean = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value;
		cMeasuredValuesInTest.rpStdDev = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].get_double().value;
		cMeasuredValuesInTest.rpWindow = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].get_int32().value;
		cMeasuredValuesInTest.rpMSE = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value;

		bsoncxx::array::view cCalibrations = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration].get_array().value;
		for (const bsoncxx::array::element& cCalibration : cCalibrations) {
			cMeasuredValuesInTest.cpCalibrations.push_back(cCalibration[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
		}
		bsoncxx::array::view cErrors = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors].get_array().value;
		for (const bsoncxx::array::element& cError : cErrors) {
			cMeasuredValuesInTest.cpErrors.push_back(cError[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
		}

		bsoncxx::array::view cMovingStatisticValues = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics].get_array().value;
		for (const bsoncxx::array::element& cMovingStatisticValue : cMovingStatisticValues) {
			cMeasuredValuesInTest.cpMovingAverages.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value);
			cMeasuredValuesInTest.cpMovingVariances.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].get_double().value);
			cMeasuredValuesInTest.cpMovingMSEs.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value);
			cMeasuredValuesInTest.cpMovingTimes.push_back((unsigned long long)cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].get_int64().value);
		}

		*pMeasuredValuesInTest = cMeasuredValuesInTest;
		return;
    
	} catch(exception e){
		return;
    }
}
bsoncxx::document::view TcStatisticTestMeasures::fSerializeObjectBsonView(TcStatisticTestMeasures pMeasuredValuesInTest){
    try {

  		bsoncxx::builder::stream::document data_builder{};
		  data_builder <<	TcMeasure::TcMeasureAttributes::kMeasureType << pMeasuredValuesInTest.rpMeasureType;
		data_builder << TcMeasure::TcMeasureAttributes::kMeasureUnit << pMeasuredValuesInTest.rpMeasureUnit;
		data_builder << TcMeasure::TcMeasureAttributes::kReferenceType << pMeasuredValuesInTest.rpReferenceType;
		data_builder << TcMeasure::TcMeasureAttributes::kReferenceID << pMeasuredValuesInTest.rpReferenceID;
		data_builder << TcMeasure::TcMeasureAttributes::kTest << pMeasuredValuesInTest.rpTest;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean << pMeasuredValuesInTest.rpMean;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev << pMeasuredValuesInTest.rpStdDev;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE << pMeasuredValuesInTest.rpMSE;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow << pMeasuredValuesInTest.rpWindow;
  		
  		auto movingavg_array_builder = bsoncxx::builder::stream::array{};
		auto movingvar_array_builder = bsoncxx::builder::stream::array{};
		auto movingmse_array_builder = bsoncxx::builder::stream::array{};
		auto movingtime_array_builder = bsoncxx::builder::stream::array{};
		auto error_array_builder = bsoncxx::builder::stream::array{};
		auto calibration_array_builder = bsoncxx::builder::stream::array{};
		auto measures_array_builder = bsoncxx::builder::stream::array{};
		
		for (float i = 0 ; i <  pMeasuredValuesInTest.cpMovingAverages.size(); i++) {
    		movingavg_array_builder << pMeasuredValuesInTest.cpMovingAverages[i];
			movingvar_array_builder << pMeasuredValuesInTest.cpMovingVariances[i];
			movingmse_array_builder << pMeasuredValuesInTest.cpMovingMSEs[i];
			movingtime_array_builder << (long long) pMeasuredValuesInTest.cpMovingTimes[i];
		}

		for (float i = 0 ; i < pMeasuredValuesInTest.cpErrors.size(); i++) {
			TcMeasure cMeasure;
			cMeasure.rpValue = pMeasuredValuesInTest.cpValues[i];
			cMeasure.rpHighLimit = pMeasuredValuesInTest.cpHighThresholds[i];
			cMeasure.rpLowLimit = pMeasuredValuesInTest.cpLowThresholds[i];
			cMeasure.rpMeasureType = pMeasuredValuesInTest.rpMeasureType;
			cMeasure.rpMeasureUnit = pMeasuredValuesInTest.rpMeasureUnit;
			cMeasure.rpReferenceType = pMeasuredValuesInTest.rpReferenceType;
			cMeasure.rpReferenceID = pMeasuredValuesInTest.rpReferenceID;
			cMeasure.rpTest = pMeasuredValuesInTest.rpTest;
			measures_array_builder << cMeasure.to_bsonvalue();
			calibration_array_builder << pMeasuredValuesInTest.cpCalibrations[i];
			error_array_builder << pMeasuredValuesInTest.cpErrors[i];
		}

		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors << error_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration << calibration_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures << measures_array_builder;
		

		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics << bsoncxx::builder::stream::open_document
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean << movingavg_array_builder
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance << movingvar_array_builder
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE << movingmse_array_builder
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch << movingtime_array_builder
		<< bsoncxx::builder::stream::close_document;

		bsoncxx::document::value cBsonDocument = data_builder << bsoncxx::builder::stream::finalize;
        return(cBsonDocument.view());




    } catch(exception e){
        return(bsoncxx::document::view());
    }

}




TcStatisticTestMeasures TcStatisticTestMeasures::from_bsonvalue(bsoncxx::document::value pBsonDocument) {
	TcStatisticTestMeasures cMeasuredValuesInTest;
	TcStatisticTestMeasures::fDeserializeObjectBsonValue(pBsonDocument, &cMeasuredValuesInTest);
	return(cMeasuredValuesInTest);
}
TcStatisticTestMeasures TcStatisticTestMeasures::from_bsonview(bsoncxx::document::view pBsonDocument) {
	TcStatisticTestMeasures cMeasuredValuesInTest;
	TcStatisticTestMeasures::fDeserializeObjectBsonView(pBsonDocument, &cMeasuredValuesInTest);
	return(cMeasuredValuesInTest);
}
TcStatisticTestMeasures TcStatisticTestMeasures::from_json(string pJsonDocument) {
	TcStatisticTestMeasures cMeasuredValuesInTest;
	TcStatisticTestMeasures::fDeserializeObjectBsonValue(bsoncxx::from_json(pJsonDocument), &cMeasuredValuesInTest);
	return(cMeasuredValuesInTest);
}

bsoncxx::document::value TcStatisticTestMeasures::to_bsonvalue() {
	return(TcStatisticTestMeasures::fSerializeObjectBsonValue(*this));
}
bsoncxx::document::view TcStatisticTestMeasures::to_bsonview() {
	return(TcStatisticTestMeasures::fSerializeObjectBsonView(*this));
}
string TcStatisticTestMeasures::to_json() {
	return(bsoncxx::to_json(TcStatisticTestMeasures::fSerializeObjectBsonValue(*this)));
}

bool operator<(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures) {
	return(LStatisticTestMeasures.rpTest < RStatisticTestMeasures.rpTest);
}

bool operator==(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures) {
	return(LStatisticTestMeasures.rpTest == RStatisticTestMeasures.rpTest);
}

bool operator>(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures) {
	return(LStatisticTestMeasures.rpTest > RStatisticTestMeasures.rpTest);
}

bool operator<=(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures) {
	return(LStatisticTestMeasures.rpTest <= RStatisticTestMeasures.rpTest);
}

bool operator>=(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures) {
	return(LStatisticTestMeasures.rpTest >= RStatisticTestMeasures.rpTest);
}

bool operator!=(const TcStatisticTestMeasures& LStatisticTestMeasures, const TcStatisticTestMeasures& RStatisticTestMeasures) {
	return(LStatisticTestMeasures.rpTest != RStatisticTestMeasures.rpTest);
}






const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::_id = "_id";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures = TcMeasure::kCollection;
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance = "Variance";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors = "Error";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE = "MSE";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMoving = "Moving";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics = "Statistics";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean = "Mean";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev = "StdDev";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest = "Test";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration = "Calibration";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestamp = "Timestamp";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch = "TimestampEpoch";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow = "Window";

