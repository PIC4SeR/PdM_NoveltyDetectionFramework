#include <AgentSystem/include/TcMeasure.h>
#include <AgentSystem/include/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h>



TcStatisticTestMeasures::TcStatisticTestMeasures(bsoncxx::document::value pBsonDocument) {
	*this = TcStatisticTestMeasures::fDeserializeObjectBsonValue(pBsonDocument);
}

TcStatisticTestMeasures::TcStatisticTestMeasures(bsoncxx::document::view pBsonDocument) {
	*this = TcStatisticTestMeasures::fDeserializeObjectBsonView(pBsonDocument);
}

TcStatisticTestMeasures::TcStatisticTestMeasures() {
	; 
}

TcStatisticTestMeasures::TcStatisticTestMeasures(const TcStatisticTestMeasures& other) {
	this->rpWindow = other.rpWindow;
	this->rpTest = other.rpTest;
	this->rpReferenceID = other.rpReferenceID;
	this->rpReferenceType = other.rpReferenceType;
	this->rpMean = other.rpMean;
	this->rpStdDev = other.rpStdDev;
	this->rpCP = other.rpCP;
	this->rpCPk = other.rpCPk;

	copy(other.cpErrors.begin(), other.cpErrors.end(), this->cpErrors.begin());
	copy(other.cpCalibrations.begin(), other.cpCalibrations.end(), this->cpCalibrations.begin());
	copy(other.cpValues.begin(), other.cpValues.end(), this->cpValues.begin());
	copy(other.cpHighThresholds.begin(), other.cpHighThresholds.end(), this->cpHighThresholds.begin());
	copy(other.cpLowThresholds.begin(), other.cpLowThresholds.end(), this->cpLowThresholds.begin());
	copy(other.cpTimes.begin(), other.cpTimes.end(), this->cpTimes.begin());
	copy(other.cpCPs.begin(), other.cpCPs.end(), this->cpCPs.begin());
	copy(other.cpCPks.begin(), other.cpCPks.end(), this->cpCPks.begin());
	copy(other.cpMovingAverages.begin(), other.cpMovingAverages.end(), this->cpMovingAverages.begin());
	copy(other.cpMovingVariances.begin(), other.cpMovingVariances.end(), this->cpMovingVariances.begin());
	copy(other.cpMovingMSEs.begin(), other.cpMovingMSEs.end(), this->cpMovingMSEs.begin());
	copy(other.cpMovingTimes.begin(), other.cpMovingTimes.end(), this->cpMovingTimes.begin());

	this->cpECDF = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues)), true);
	for (int i = 0; i < other.cpValues.size() - other.rpWindow + 1; i++) {
		this->cpMovingECDFs[i] = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues.begin() + i, other.cpValues.begin() + i + other.rpWindow)), true);
	}

	this->rpCP = (this->cpHighThresholds[0] - this->cpLowThresholds[0]) / (6 * this->rpStdDev);
	this->rpCPk = min(((this->cpLowThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)), ((this->cpHighThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)));
	for (int i = 0; i < this->cpMovingAverages.size(); i++) {
		this->cpCPs[i] = (this->cpHighThresholds[i] - this->cpLowThresholds[i]) / (6 * sqrt(this->cpMovingVariances[i]));
		this->cpCPks[i] = min(((this->cpLowThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))), ((this->cpHighThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))));
	}



}
TcStatisticTestMeasures::TcStatisticTestMeasures(TcStatisticTestMeasures&& other) {
	this->rpWindow = other.rpWindow;
	this->rpTest = other.rpTest;
	this->rpReferenceID = other.rpReferenceID;
	this->rpReferenceType = other.rpReferenceType;
	this->rpMean = other.rpMean;
	this->rpStdDev = other.rpStdDev;
	this->rpCP = other.rpCP;
	this->rpCPk = other.rpCPk;

	copy(other.cpErrors.begin(), other.cpErrors.end(), this->cpErrors.begin());
	copy(other.cpCalibrations.begin(), other.cpCalibrations.end(), this->cpCalibrations.begin());
	copy(other.cpValues.begin(), other.cpValues.end(), this->cpValues.begin());
	copy(other.cpHighThresholds.begin(), other.cpHighThresholds.end(), this->cpHighThresholds.begin());
	copy(other.cpLowThresholds.begin(), other.cpLowThresholds.end(), this->cpLowThresholds.begin());
	copy(other.cpTimes.begin(), other.cpTimes.end(), this->cpTimes.begin());
	copy(other.cpCPs.begin(), other.cpCPs.end(), this->cpCPs.begin());
	copy(other.cpCPks.begin(), other.cpCPks.end(), this->cpCPks.begin());
	copy(other.cpMovingAverages.begin(), other.cpMovingAverages.end(), this->cpMovingAverages.begin());
	copy(other.cpMovingVariances.begin(), other.cpMovingVariances.end(), this->cpMovingVariances.begin());
	copy(other.cpMovingMSEs.begin(), other.cpMovingMSEs.end(), this->cpMovingMSEs.begin());
	copy(other.cpMovingTimes.begin(), other.cpMovingTimes.end(), this->cpMovingTimes.begin());

	this->cpECDF = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues)), true);
	for (int i = 0; i < other.cpValues.size() - other.rpWindow + 1; i++) {
		this->cpMovingECDFs[i] = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues.begin() + i, other.cpValues.begin() + i + other.rpWindow)), true);
	}

	this->rpCP = (this->cpHighThresholds[0] - this->cpLowThresholds[0]) / (6 * this->rpStdDev);
	this->rpCPk = min(((this->cpLowThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)), ((this->cpHighThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)));
	for (int i = 0; i < this->cpMovingAverages.size(); i++) {
		this->cpCPs[i] = (this->cpHighThresholds[i] - this->cpLowThresholds[i]) / (6 * sqrt(this->cpMovingVariances[i]));
		this->cpCPks[i] = min(((this->cpLowThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))), ((this->cpHighThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))));
	}


	other.rpTest = "";
	other.rpReferenceID = "";
	other.rpReferenceType = "";
	other.rpWindow = 0;
	other.rpMean = 0;
	other.rpStdDev = 0;
	other.rpCP = 0;
	other.rpCPk = 0;
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
	other.cpMovingECDFs.clear();
	other.cpMovingTimes.clear();

}
TcStatisticTestMeasures::~TcStatisticTestMeasures() {
	
	
	if (this->cpECDF != nullptr) {
		delete this->cpECDF;
	}

	for (int i = 0; i < this->cpMovingAverages.size(); i++) {
		if (this->cpMovingECDFs[i] != nullptr) {
			delete this->cpMovingECDFs[i];
		}
	}

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
	this->cpMovingECDFs.clear();
	this->cpMovingTimes.clear();


	

}
TcStatisticTestMeasures& TcStatisticTestMeasures::operator=(const TcStatisticTestMeasures& other) {

	if (this != &other) {
		this->rpWindow = other.rpWindow;
		this->rpTest = other.rpTest;
		this->rpReferenceID = other.rpReferenceID;
		this->rpReferenceType = other.rpReferenceType;
		this->rpMean = other.rpMean;
		this->rpStdDev = other.rpStdDev;
		this->rpCP = other.rpCP;
		this->rpCPk = other.rpCPk;
		copy(other.cpErrors.begin(), other.cpErrors.end(), this->cpErrors.begin());
		copy(other.cpCalibrations.begin(), other.cpCalibrations.end(), this->cpCalibrations.begin());
		copy(other.cpValues.begin(), other.cpValues.end(), this->cpValues.begin());
		copy(other.cpHighThresholds.begin(), other.cpHighThresholds.end(), this->cpHighThresholds.begin());
		copy(other.cpLowThresholds.begin(), other.cpLowThresholds.end(), this->cpLowThresholds.begin());
		copy(other.cpTimes.begin(), other.cpTimes.end(), this->cpTimes.begin());
		copy(other.cpCPs.begin(), other.cpCPs.end(), this->cpCPs.begin());
		copy(other.cpCPks.begin(), other.cpCPks.end(), this->cpCPks.begin());
		copy(other.cpMovingAverages.begin(), other.cpMovingAverages.end(), this->cpMovingAverages.begin());
		copy(other.cpMovingVariances.begin(), other.cpMovingVariances.end(), this->cpMovingVariances.begin());
		copy(other.cpMovingMSEs.begin(), other.cpMovingMSEs.end(), this->cpMovingMSEs.begin());
		copy(other.cpMovingTimes.begin(), other.cpMovingTimes.end(), this->cpMovingTimes.begin());

		this->cpECDF = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues)), true);
		for (int i = 0; i < other.cpValues.size() - other.rpWindow + 1; i++) {
			this->cpMovingECDFs[i] = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues.begin() + i, other.cpValues.begin() + i + other.rpWindow)), true);
		}

		this->rpCP = (this->cpHighThresholds[0] - this->cpLowThresholds[0]) / (6 * this->rpStdDev);
		this->rpCPk = min(((this->cpLowThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)), ((this->cpHighThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)));
		for (int i = 0; i < this->cpMovingAverages.size(); i++) {
			this->cpCPs[i] = (this->cpHighThresholds[i] - this->cpLowThresholds[i]) / (6 * sqrt(this->cpMovingVariances[i]));
			this->cpCPks[i] = min(((this->cpLowThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))), ((this->cpHighThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))));
		}


	}

	return(*this);

}
TcStatisticTestMeasures& TcStatisticTestMeasures::operator=(TcStatisticTestMeasures&& other) {

	if (this != &other) {
		this->rpWindow = other.rpWindow;
		this->rpTest = other.rpTest;
		this->rpReferenceID = other.rpReferenceID;
		this->rpReferenceType = other.rpReferenceType;
		this->rpMean = other.rpMean;
		this->rpStdDev = other.rpStdDev;
		this->rpCP = other.rpCP;
		this->rpCPk = other.rpCPk;
		copy(other.cpErrors.begin(), other.cpErrors.end(), this->cpErrors.begin());
		copy(other.cpCalibrations.begin(), other.cpCalibrations.end(), this->cpCalibrations.begin());
		copy(other.cpValues.begin(), other.cpValues.end(), this->cpValues.begin());
		copy(other.cpHighThresholds.begin(), other.cpHighThresholds.end(), this->cpHighThresholds.begin());
		copy(other.cpLowThresholds.begin(), other.cpLowThresholds.end(), this->cpLowThresholds.begin());
		copy(other.cpTimes.begin(), other.cpTimes.end(), this->cpTimes.begin());
		copy(other.cpCPs.begin(), other.cpCPs.end(), this->cpCPs.begin());
		copy(other.cpCPks.begin(), other.cpCPks.end(), this->cpCPks.begin());
		copy(other.cpMovingAverages.begin(), other.cpMovingAverages.end(), this->cpMovingAverages.begin());
		copy(other.cpMovingVariances.begin(), other.cpMovingVariances.end(), this->cpMovingVariances.begin());
		copy(other.cpMovingMSEs.begin(), other.cpMovingMSEs.end(), this->cpMovingMSEs.begin());
		copy(other.cpMovingTimes.begin(), other.cpMovingTimes.end(), this->cpMovingTimes.begin());

		this->cpECDF = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues)), true);
		for (int i = 0; i < other.cpValues.size() - other.rpWindow + 1; i++) {
			this->cpMovingECDFs[i] = new empirical_cumulative_distribution_function(move(vector<double>(other.cpValues.begin() + i, other.cpValues.begin() + i + other.rpWindow)), true);
		}
		
		this->rpCP = (this->cpHighThresholds[0] - this->cpLowThresholds[0]) / (6 * this->rpStdDev);
		this->rpCPk = min(((this->cpLowThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)), ((this->cpHighThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)));
		for (int i = 0; i < this->cpMovingAverages.size(); i++) {
			this->cpCPs[i] = (this->cpHighThresholds[i] - this->cpLowThresholds[i]) / (6 * sqrt(this->cpMovingVariances[i]));
			this->cpCPks[i] = min(((this->cpLowThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))), ((this->cpHighThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))));
		}



		other.rpTest = "";
		other.rpReferenceID = "";
		other.rpReferenceType = "";
		other.rpWindow = 0;
		other.rpMean = 0;
		other.rpStdDev = 0;
		other.rpCP = 0;
		other.rpCPk = 0;
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
		other.cpMovingECDFs.clear();
		other.cpMovingTimes.clear();



	}

	return(*this);
}

void TcStatisticTestMeasures::operator()() {
	
	this->cpECDF = new empirical_cumulative_distribution_function(move(vector<double>(this->cpValues)), true);
    for (int i = 0; i < this->cpValues.size() - this->rpWindow + 1; i++) {
		this->cpMovingECDFs[i] = new empirical_cumulative_distribution_function(move(vector<double>(this->cpValues.begin() + i, this->cpValues.begin() + i + this->rpWindow)),true);
	}

	this->rpCP = (this->cpHighThresholds[0] - this->cpLowThresholds[0]) / (6 * this->rpStdDev);
	this->rpCPk = min(((this->cpLowThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)), ((this->cpHighThresholds[0] - this->cpMovingAverages[0]) / (3 * this->rpStdDev)));
	for (int i = 0; i < this->cpMovingAverages.size(); i++) {
		this->cpCPs[i] = (this->cpHighThresholds[i] - this->cpLowThresholds[i]) / (6 * sqrt(this->cpMovingVariances[i]));
		this->cpCPks[i] = min(((this->cpLowThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))), ((this->cpHighThresholds[i] - this->cpMovingAverages[i]) / (3 * sqrt(this->cpMovingVariances[i]))));
	}

	return;
}





TcStatisticTestMeasures TcStatisticTestMeasures::fDeserializeObjectBsonValue(bsoncxx::document::value pBsonDocument){
	try {
		bsoncxx::document::view cBsonDocumentView = pBsonDocument.view();
		TcStatisticTestMeasures cMeasuredValuesInTest;
		
		cMeasuredValuesInTest.rpTest = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Test].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceID = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ReferenceID].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceType = cBsonDocumentView[TcMeasure::TcMeasureAttributes::ReferenceType].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpMean = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean].get_double().value;
		cMeasuredValuesInTest.rpStdDev = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::StdDev].get_double().value;
		bsoncxx::array::view cMeasures = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Measures].get_array().value;
		for (const bsoncxx::array::element& cMeasure : cMeasures) {
			cMeasuredValuesInTest.cpValues.push_back(cMeasure[TcMeasure::TcMeasureAttributes::Value].get_double().value);
			cMeasuredValuesInTest.cpHighThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::HighLimit].get_double().value);
			cMeasuredValuesInTest.cpLowThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::LowLimit].get_double().value);
			cMeasuredValuesInTest.cpTimes.push_back((long long)cMeasure[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::TimestampEpoch].get_int64().value);
		}
		bsoncxx::array::view cCalibrations = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Calibration].get_array().value;
		for (const bsoncxx::array::element& cCalibration : cCalibrations) {
			cMeasuredValuesInTest.cpCalibrations.push_back(cCalibration[TcMeasure::TcMeasureAttributes::Value].get_double().value);
		}
		bsoncxx::array::view cErrors = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Errors].get_array().value;
		for (const bsoncxx::array::element& cError : cErrors) {
			cMeasuredValuesInTest.cpErrors.push_back(cError[TcMeasure::TcMeasureAttributes::Value].get_double().value);
		}
		bsoncxx::array::view cMovingStatisticValues = cBsonDocumentView[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Statistics].get_array().value;
		for (const bsoncxx::array::element& cMovingStatisticValue : cMovingStatisticValues) {
			cMeasuredValuesInTest.cpMovingAverages.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean].get_double().value);
			cMeasuredValuesInTest.cpMovingVariances.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Variance].get_double().value);
			cMeasuredValuesInTest.cpMovingMSEs.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::MSE].get_double().value);
			cMeasuredValuesInTest.cpMovingTimes.push_back((long long)cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::TimestampEpoch].get_int64().value);
		}

		cMeasuredValuesInTest();
		return(cMeasuredValuesInTest);
    } catch(exception e){
        return(TcStatisticTestMeasures());
    }
}

bsoncxx::document::value TcStatisticTestMeasures::fSerializeObjectBsonValue(TcStatisticTestMeasures pMeasuredValuesInTest){
    try {
		
		bsoncxx::builder::stream::document data_builder{};
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Test << pMeasuredValuesInTest.rpTest;
		data_builder << TcMeasure::TcMeasureAttributes::ReferenceID << pMeasuredValuesInTest.rpReferenceID;
		data_builder << TcMeasure::TcMeasureAttributes::ReferenceType << pMeasuredValuesInTest.rpReferenceType;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean << pMeasuredValuesInTest.rpMean;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::StdDev << pMeasuredValuesInTest.rpStdDev;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Size << pMeasuredValuesInTest.rpSize;

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
			movingtime_array_builder << pMeasuredValuesInTest.cpMovingTimes[i];
		}

		for (float i = 0; i < pMeasuredValuesInTest.cpErrors.size(); i++) {
			TcMeasure cMeasure;
			cMeasure.Value = pMeasuredValuesInTest.cpValues[i];
			cMeasure.HighLimit = pMeasuredValuesInTest.cpHighThresholds[i];
			cMeasure.LowLimit = pMeasuredValuesInTest.cpLowThresholds[i];
			measures_array_builder << cMeasure.to_bsonvalue();
			calibration_array_builder << pMeasuredValuesInTest.cpCalibrations[i];
			error_array_builder << pMeasuredValuesInTest.cpErrors[i];
		}

		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Errors << error_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Calibration << calibration_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Measures << measures_array_builder;



		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Statistics << bsoncxx::builder::stream::open_document
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean << movingavg_array_builder
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Variance << movingvar_array_builder
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::MSE << movingmse_array_builder
			<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::TimestampEpoch << movingtime_array_builder
			<< bsoncxx::builder::stream::close_document;

		bsoncxx::document::value cBsonDocument = data_builder << bsoncxx::builder::stream::finalize;
		return(cBsonDocument);
    } catch(exception e){
		return(bsoncxx::document::value(nullptr, 0, 0));
    }

}



TcStatisticTestMeasures TcStatisticTestMeasures::fDeserializeObjectBsonView(bsoncxx::document::view pBsonDocument){
	try {
		TcStatisticTestMeasures cMeasuredValuesInTest;
		cMeasuredValuesInTest.rpTest = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Test].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceID = pBsonDocument[TcMeasure::TcMeasureAttributes::ReferenceID].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpReferenceType = pBsonDocument[TcMeasure::TcMeasureAttributes::ReferenceType].get_utf8().value.to_string();
		cMeasuredValuesInTest.rpMean = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean].get_double().value;
		cMeasuredValuesInTest.rpStdDev = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::StdDev].get_double().value;
		bsoncxx::array::view cMeasures = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Measures].get_array().value;
		for (const bsoncxx::array::element& cMeasure : cMeasures) {
			cMeasuredValuesInTest.cpValues.push_back(cMeasure[TcMeasure::TcMeasureAttributes::Value].get_double().value);
			cMeasuredValuesInTest.cpHighThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::HighLimit].get_double().value);
			cMeasuredValuesInTest.cpLowThresholds.push_back(cMeasure[TcMeasure::TcMeasureAttributes::LowLimit].get_double().value);
			cMeasuredValuesInTest.cpTimes.push_back((long)cMeasure[TcMeasure::TcMeasureAttributes::TimestampEpoch].get_int64().value);
		}
		bsoncxx::array::view cCalibrations = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Calibration].get_array().value;
		for (const bsoncxx::array::element& cCalibration : cCalibrations) {
			cMeasuredValuesInTest.cpCalibrations.push_back(cCalibration[TcMeasure::TcMeasureAttributes::Value].get_double().value);
		}
		bsoncxx::array::view cErrors = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Errors].get_array().value;
		for (const bsoncxx::array::element& cError : cErrors) {
			cMeasuredValuesInTest.cpErrors.push_back(cError[TcMeasure::TcMeasureAttributes::Value].get_double().value);
		}
		bsoncxx::array::view cMovingStatisticValues = pBsonDocument[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Statistics].get_array().value;
		for (const bsoncxx::array::element& cMovingStatisticValue : cMovingStatisticValues) {
			cMeasuredValuesInTest.cpMovingAverages.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean].get_double().value);
			cMeasuredValuesInTest.cpMovingVariances.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Variance].get_double().value);
			cMeasuredValuesInTest.cpMovingMSEs.push_back(cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::MSE].get_double().value);
			cMeasuredValuesInTest.cpMovingTimes.push_back((long long)cMovingStatisticValue[TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::TimestampEpoch].get_int64().value);
		}
		cMeasuredValuesInTest();
		return(cMeasuredValuesInTest);
    } catch(exception e){
        return(TcStatisticTestMeasures());
    }
}
bsoncxx::document::view TcStatisticTestMeasures::fSerializeObjectBsonView(TcStatisticTestMeasures pMeasuredValuesInTest){
    try {

  		bsoncxx::builder::stream::document data_builder{};
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Test << pMeasuredValuesInTest.rpTest;
		data_builder << TcMeasure::TcMeasureAttributes::ReferenceID << pMeasuredValuesInTest.rpReferenceID;
		data_builder << TcMeasure::TcMeasureAttributes::ReferenceType << pMeasuredValuesInTest.rpReferenceType;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean << pMeasuredValuesInTest.rpMean;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::StdDev << pMeasuredValuesInTest.rpStdDev;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Size << pMeasuredValuesInTest.rpSize;
  		
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
			movingtime_array_builder << pMeasuredValuesInTest.cpMovingTimes[i];
		}

		for (float i = 0 ; i < pMeasuredValuesInTest.cpErrors.size(); i++) {
			TcMeasure cMeasure;
			cMeasure.Value = pMeasuredValuesInTest.cpValues[i];
			cMeasure.HighLimit = pMeasuredValuesInTest.cpHighThresholds[i];
			cMeasure.LowLimit = pMeasuredValuesInTest.cpLowThresholds[i];
			measures_array_builder << cMeasure.to_bsonvalue();
			calibration_array_builder << pMeasuredValuesInTest.cpCalibrations[i];
			error_array_builder << pMeasuredValuesInTest.cpErrors[i];
		}

		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Errors << error_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Calibration << calibration_array_builder;
		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Measures << measures_array_builder;
		

		data_builder << TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Statistics << bsoncxx::builder::stream::open_document
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean << movingavg_array_builder
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Variance << movingvar_array_builder
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::MSE << movingmse_array_builder
		<< TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::TimestampEpoch << movingtime_array_builder
		<< bsoncxx::builder::stream::close_document;

		bsoncxx::document::value cBsonDocument = data_builder << bsoncxx::builder::stream::finalize;
        return(cBsonDocument.view());




    } catch(exception e){
        return(bsoncxx::document::view());
    }

}




TcStatisticTestMeasures TcStatisticTestMeasures::from_bsonvalue(bsoncxx::document::value pBsonDocument) {
	return(TcStatisticTestMeasures::fDeserializeObjectBsonValue(pBsonDocument));
}
TcStatisticTestMeasures TcStatisticTestMeasures::from_bsonview(bsoncxx::document::view pBsonDocument) {
	return(TcStatisticTestMeasures::fDeserializeObjectBsonView(pBsonDocument));
}
TcStatisticTestMeasures TcStatisticTestMeasures::from_json(string pJsonDocument) {
	return(TcStatisticTestMeasures::fDeserializeObjectBsonValue(bsoncxx::from_json(pJsonDocument)));
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
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Measures = "Measures";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Variance = "Variance";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Errors = "Error";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::MSE = "MSE";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Moving = "Moving";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Statistics = "Statistics";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Mean = "Mean";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::StdDev = "StdDev";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Test = "Test";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Calibration = "Calibration";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Size = "Size";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::Timestamp = "Timestamp";
const string TcStatisticTestMeasures::TcStatisticTestMeasuresAttributes::TimestampEpoch = "TimestampEpoch";

