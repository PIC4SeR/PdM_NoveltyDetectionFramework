#include <math.h>
#include "../include/TcMeasure.h"
#include "../include/TcStatisticTestMeasures.h"

using namespace Spea::Archimede::MultiAgentSystem::Tools;

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

	this->rpMeasureType = cMeasuredValuesInTest.rpMeasureType;
	this->rpMeasureUnit = cMeasuredValuesInTest.rpMeasureUnit;
	this->rpReferenceID = cMeasuredValuesInTest.rpReferenceID;
	this->rpReferenceType = cMeasuredValuesInTest.rpReferenceType;

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
	this->rpMeasureType = "";
	this->rpMeasureUnit = "";
	this->rpReferenceID = "";
	this->rpReferenceType = "";

	this->rpCP = 0;
	this->rpCPk = 0;

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
	this->rpMeasureType = other.rpMeasureType;
	this->rpMeasureUnit = other.rpMeasureUnit;
	this->rpReferenceID = other.rpReferenceID;
	this->rpReferenceType = other.rpReferenceType;
	this->cpMeasures = other.cpMeasures;


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
	this->rpMeasureType = other.rpMeasureType;
	this->rpMeasureUnit = other.rpMeasureUnit;
	this->rpReferenceID = other.rpReferenceID;
	this->rpReferenceType = other.rpReferenceType;
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
	this->cpMeasures = other.cpMeasures;



	other.rpTest = "";
	other.rpMeasureType = "";
	other.rpMeasureUnit = "";
	other.rpReferenceID = "";
	other.rpReferenceType = "";
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
	other.cpMeasures.clear();

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
	this->cpMeasures.clear();


	

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
		this->rpMeasureType = other.rpMeasureType;
		this->rpMeasureUnit = other.rpMeasureUnit;
		this->rpReferenceID = other.rpReferenceID;
		this->rpReferenceType = other.rpReferenceType;
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
		this->cpMeasures = other.cpMeasures;
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
		this->rpMeasureType = other.rpMeasureType;
		this->rpMeasureUnit = other.rpMeasureUnit;
		this->rpReferenceID = other.rpReferenceID;
		this->rpReferenceType = other.rpReferenceType;
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
		this->cpMeasures = other.cpMeasures;

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
		other.rpMeasureType = "";
		other.rpMeasureUnit = "";
		other.rpReferenceID = "";
		other.rpReferenceType = "";
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
		other.cpMeasures.clear();



	}

	return(*this);
}






void TcStatisticTestMeasures::fDeserializeObjectBsonValue(bsoncxx::document::value pBsonDocument, TcStatisticTestMeasures* pMeasuredValuesInTest){
	try {
		bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
		TcStatisticTestMeasures cMeasuredValuesInTest = *pMeasuredValuesInTest;
		cMeasuredValuesInTest.rpTest = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].length() == 0 || cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].type() != bsoncxx::type::k_utf8 ? "" : cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].get_utf8().value.to_string();


		if (cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cMeasures = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures].get_array().value;
			for (const bsoncxx::array::element& cMeasure : cMeasures) {
				cMeasuredValuesInTest.cpValues.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
				cMeasuredValuesInTest.cpHighThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].type() != bsoncxx::type::k_double ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value);
				cMeasuredValuesInTest.cpLowThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].type() != bsoncxx::type::k_double ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value);
				cMeasuredValuesInTest.cpTimes.push_back((long)cMeasure[TcMeasure::TcMeasureAttributes::kTimestampEpoch].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value);
				cMeasuredValuesInTest.cpMeasures.push_back(TcMeasure(cMeasure.get_document().value));
			}
			cMeasuredValuesInTest.rpMeasureUnit = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
			cMeasuredValuesInTest.rpMeasureType = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
			cMeasuredValuesInTest.rpReferenceType = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
			cMeasuredValuesInTest.rpReferenceID = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
		}

		cMeasuredValuesInTest.rpMean = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].length() == 0 || cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value;
		cMeasuredValuesInTest.rpStdDev = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].length() == 0 || cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].get_double().value;
		cMeasuredValuesInTest.rpWindow = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].length() == 0 || cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].type() != bsoncxx::type::k_int32 ? 0 : cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].get_int32().value;
		cMeasuredValuesInTest.rpMSE = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].length() == 0 || cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].type() != bsoncxx::type::k_double ? 0 : cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value;

		if (cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cCalibrations = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration].get_array().value;
			for (const bsoncxx::array::element& cCalibration : cCalibrations) {
				cMeasuredValuesInTest.cpCalibrations.push_back(cCalibration[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cCalibration[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cCalibration[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
			}
		}

		if (cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cErrors = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors].get_array().value;
			for (const bsoncxx::array::element& cError : cErrors) {
				cMeasuredValuesInTest.cpErrors.push_back(cError[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cError[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cError[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
			}
		}


		if (cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cMovingStatisticValues = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics].get_array().value;
			for (const bsoncxx::array::element& cMovingStatisticValue : cMovingStatisticValues) {
				cMeasuredValuesInTest.cpMovingAverages.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].type() != bsoncxx::type::k_double ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value);
				cMeasuredValuesInTest.cpMovingVariances.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].type() != bsoncxx::type::k_double ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].get_double().value);
				cMeasuredValuesInTest.cpMovingMSEs.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].type() != bsoncxx::type::k_double ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value);
				cMeasuredValuesInTest.cpMovingTimes.push_back((unsigned long long)cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].get_int64().value);
			}
		}


		*pMeasuredValuesInTest = cMeasuredValuesInTest;


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

		for (float i = 0; i < pMeasuredValuesInTest.cpMeasures.size(); i++) {
			measures_array_builder << pMeasuredValuesInTest.cpMeasures[i].to_bsonview();
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
		TcStatisticTestMeasures cMeasuredValuesInTest = *pMeasuredValuesInTest;
		cMeasuredValuesInTest.rpTest = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].length() == 0 || pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].type() != bsoncxx::type::k_utf8 ? "" : pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTest].get_utf8().value.to_string();
		
		
		if (pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cMeasures = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMeasures].get_array().value;
			for (const bsoncxx::array::element& cMeasure : cMeasures) {
				cMeasuredValuesInTest.cpValues.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
				cMeasuredValuesInTest.cpHighThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].type() != bsoncxx::type::k_double ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kHighLimit].get_double().value);
				cMeasuredValuesInTest.cpLowThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].type() != bsoncxx::type::k_double ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kLowLimit].get_double().value);
				cMeasuredValuesInTest.cpTimes.push_back((long)cMeasure[TcMeasure::TcMeasureAttributes::kTimestampEpoch].length() == 0 || cMeasure[TcMeasure::TcMeasureAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : cMeasure[TcMeasure::TcMeasureAttributes::kTimestampEpoch].get_int64().value);
				cMeasuredValuesInTest.cpMeasures.push_back(TcMeasure(cMeasure.get_document().value));
			}
			cMeasuredValuesInTest.rpMeasureUnit = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureUnit].get_utf8().value.to_string();
			cMeasuredValuesInTest.rpMeasureType = cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kMeasureType].get_utf8().value.to_string();
			cMeasuredValuesInTest.rpReferenceType = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceType].get_utf8().value.to_string();
			cMeasuredValuesInTest.rpReferenceID = cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].length() == 0 || cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].type() != bsoncxx::type::k_utf8 ? "" : cMeasures[0][TcMeasure::TcMeasureAttributes::kReferenceID].get_utf8().value.to_string();
		}

		cMeasuredValuesInTest.rpMean = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].length() == 0 || pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].type() != bsoncxx::type::k_double ? 0 : pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value;
		cMeasuredValuesInTest.rpStdDev = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].length() == 0 || pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].type() != bsoncxx::type::k_double ? 0 : pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStdDev].get_double().value;
		cMeasuredValuesInTest.rpWindow = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].length() == 0 || pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].type() != bsoncxx::type::k_int32 ? 0 : pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow].get_int32().value;
		cMeasuredValuesInTest.rpMSE = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].length() == 0 || pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].type() != bsoncxx::type::k_double ? 0 :  pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value;

		if (pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cCalibrations = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kCalibration].get_array().value;
			for (const bsoncxx::array::element& cCalibration : cCalibrations) {
				cMeasuredValuesInTest.cpCalibrations.push_back(cCalibration[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cCalibration[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cCalibration[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
			}
		}

		if (pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cErrors = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kErrors].get_array().value;
			for (const bsoncxx::array::element& cError : cErrors) {
				cMeasuredValuesInTest.cpErrors.push_back(cError[TcMeasure::TcMeasureAttributes::kValue].length() == 0 || cError[TcMeasure::TcMeasureAttributes::kValue].type() != bsoncxx::type::k_double ? 0 : cError[TcMeasure::TcMeasureAttributes::kValue].get_double().value);
			}
		}


		if (pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics].type() == bsoncxx::type::k_array) {
			bsoncxx::array::view cMovingStatisticValues = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kStatistics].get_array().value;
			for (const bsoncxx::array::element& cMovingStatisticValue : cMovingStatisticValues) {
				cMeasuredValuesInTest.cpMovingAverages.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].type() != bsoncxx::type::k_double ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMean].get_double().value);
				cMeasuredValuesInTest.cpMovingVariances.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].type() != bsoncxx::type::k_double ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kVariance].get_double().value);
				cMeasuredValuesInTest.cpMovingMSEs.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].type() != bsoncxx::type::k_double ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kMSE].get_double().value);
				cMeasuredValuesInTest.cpMovingTimes.push_back((unsigned long long)cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].length() == 0 || cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].type() != bsoncxx::type::k_int64 ? 0 : cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch].get_int64().value);
			}
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
			measures_array_builder << measures_array_builder << pMeasuredValuesInTest.cpMeasures[i].to_bsonview();
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
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestamp = "rpTimestamp";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kTimestampEpoch = "rpTimestampEpoch";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::kWindow = "Window";

