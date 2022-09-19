
#include <stdio.h>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

#include "../../include/Agent/TcAgent.h"
#include "../../include/TcNotification.h"
#include "../../include/TcMeasure.h"


TcAgent::TcAgent(string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : IAgent(pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped) {
	this->rmMachineSerialNumber = pMachineSerialNumber;
	this->rmMachineFamily = pMachineFamily;
	this->rmDatabaseName = pDatabaseName;
	this->rmCollectionName = pCollectionName;
	this->cmMongoInterface = new IMongoDriverAgentInterface("Agent DB Application Interface", pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort);
	this->cmNotifierInterface = new INotifyDriverAgentInterface(pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort);
}

TcAgent::~TcAgent() {
	if (this->cmMongoInterface == nullptr) {
		delete this->cmMongoInterface;
		this->cmMongoInterface = nullptr;
	}
}

void TcAgent::fSetDatabasename(string pDatabaseName) {
	this->rmDatabaseName = pDatabaseName;
}

string TcAgent::fGetDatabasename() {
	return(this->rmDatabaseName);
}

void TcAgent::fSetCollectionName(string pCollectionName) {
	this->rmCollectionName = pCollectionName;
}

string TcAgent::fGetCollectionName() {
	return(this->rmCollectionName);
}




string TcAgent::fGetMachineSerialNumber() {
	return(this->rmMachineSerialNumber);
}

void TcAgent::fSetMachineSerialNumber(string pMachineSerialNumber) {
	this->rmMachineSerialNumber = pMachineSerialNumber;
}



string TcAgent::fGetMachineFamily() {
	return(this->rmMachineFamily);
}


void TcAgent::fSetMachineFamily(string pEquipmentFamily) {
	this->rmMachineFamily = pEquipmentFamily;
}






int TcAgent::fRun() {
	

	list<string> cLastTemperatures;
	string rAvgTemperature;
	string rMaxTemperature;
	string rMinTemperature;
	string rSumTemperatures;
	list<string> cTemperaturesBetween;
	list<string> cTemperaturesGreatherThan;
	list<string> cTemperaturesLowerThan;
	string rAvgTemperatureBetweenValues;
	string rAvgTemperatureGreatherThenValue;
	string rAvgTemperatureLowerThanValue;
	string rMaxTemperatureBetweenValues;
	string rMaxTemperatureGreatherThenValue;
	string rMaxTemperatureLowerThanValue;
	string rMinTemperatureBetweenValues;
	string rMinTemperatureGreatherThenValue;
	string rMinTemperatureLowerThanValue;
	string rSumTemperatureBetweenValues;
	string rSumTemperatureGreatherThenValue;
	string rSumTemperatureLowerThanValue;
	string rCountTemperatureBetweenValues;
	string rCountTemperatureGreatherThenValue;
	string rCountTemperatureLowerThanValue;

	int rResult = fGetData(&cLastTemperatures, &rAvgTemperature, &rMaxTemperature, &rMinTemperature, &rSumTemperatures, 
		&cTemperaturesBetween, &cTemperaturesGreatherThan, &cTemperaturesLowerThan, 
		&rAvgTemperatureBetweenValues, &rAvgTemperatureGreatherThenValue, &rAvgTemperatureLowerThanValue,
		&rMaxTemperatureBetweenValues, &rMaxTemperatureGreatherThenValue, &rMaxTemperatureLowerThanValue,
		&rMinTemperatureBetweenValues, &rMinTemperatureGreatherThenValue, &rMinTemperatureLowerThanValue,
		&rSumTemperatureBetweenValues, &rSumTemperatureGreatherThenValue, &rSumTemperatureLowerThanValue,
		&rCountTemperatureBetweenValues, &rCountTemperatureGreatherThenValue, &rCountTemperatureLowerThanValue

	);

	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Get data from database fails with error %d" ANSI_COLOR_RESET "\n", rResult);
		fflush(stdout);
		return(kRunFails);
	}

	/*rResult = fNotifyMessage("Alert", "Temperature Alert", "The temperature is increasing");
	if (rResult < 0) {
		fprintf(stdout, "Send notification fails with error %d\n", rResult);
		fflush(stdout);
		return(kRunFails);
	}
	*/

	return(kRunSuccess);
}


int TcAgent::fNotifyMessage(string pType, string pTitle, string pBody, string pReferenceID, string pReferenceFamily, string pTestName) {

	if (pType.empty()) {
		return(kNotifyFails);
	}
	if (pTitle.empty()) {
		return(kNotifyFails);
	}
	if (pBody.empty()) {
		return(kNotifyFails);
	}

	TcNotification cNotification(pType, pTitle, pBody, pReferenceID, pReferenceFamily, pTestName);
	int rResult = this->cmNotifierInterface->fSendNotification(cNotification.fGetNotificationString());
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Send notification fails with error %d" ANSI_COLOR_RESET "\n", rResult);
		fflush(stdout);
		return(kNotifyFails);
	}

	return(kNotifySuccess);
}



int TcAgent::fGetData(list<string>* pLastTemperatures, string* pAvgTemperature, string* pMaxTemperature, string* pMinTemperature, string* pSumTemperatures, list<string>* pTemperaturesBetween, list<string>* pTemperaturesGreatherThan, list<string>* pTemperaturesLowerThan, string* pAvgTemperatureBetweenValues, string* pAvgTemperatureGreatherThanValue, string* pAvgTemperatureLowerThanValue, string* pMaxTemperatureBetweenValues, string* pMaxTemperatureGreatherThanValue, string* pMaxTemperatureLowerThanValue, string* pMinTemperatureBetweenValues, string* pMinTemperatureGreatherThanValue, string* pMinTemperatureLowerThanValue, string* pSumTemperatureBetweenValues, string* pSumTemperatureGreatherThanValue, string* pSumTemperatureLowerThanValue, string* pCountTemperatureBetweenValues, string* pCountTemperatureGreatherThanValue, string* pCountTemperatureLowerThanValue) {
	
	int rResult = this->cmMongoInterface->fConnectMongoDriver();
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Connection to Mongo Driver fails with error %d" ANSI_COLOR_RESET "\n", rResult);
		fflush(stdout);
		return(kGetDataFails);
	}

	/*
	string s = cData.back();
	bsoncxx::document::value v = bsoncxx::from_json(s);
	auto t = v.view()["Timestamp"].type();
	auto d = v.view()["Timestamp"].get_date();
	*/


	this->cmMongoInterface->fAddFictitiousData();
	
	
	auto to = chrono::high_resolution_clock::now();
	auto from = this->cmLastRunTime;

	/*
	list<TcMeasure> cTemperaturesBetweenTimePoints;
	if (this->cmMongoInterface->fGetMeasuresInBackWindow<TcMeasure>(&cTemperaturesBetweenTimePoints, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", 100, "", "Sensor", "Temp", "Timestamp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}
	*/


	/*list<string> cTemperaturesBetweenTimePoints;
	if (this->cmMongoInterface->fGetMeasuresBetweenTimePoints(&cTemperaturesBetweenTimePoints, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, from, "Sensor", "Temp", "Timestamp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	list<string> cTemperaturesAfterTimePoint;
	if (this->cmMongoInterface->fGetMeasuresAfterTimePoint(&cTemperaturesAfterTimePoint, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", from, "Sensor", "Temp", "Timestamp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	list<string> cTemperaturesBeforeTimePoint;
	if (this->cmMongoInterface->fGetMeasuresBeforeTimePoint(&cTemperaturesBeforeTimePoint, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, "Sensor", "Temp", "Timestamp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMinTemperatureBetweenTimePoints;
	if (this->cmMongoInterface->fGetMinMeasureBetweenTimePoints(&cMinTemperatureBetweenTimePoints, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, from, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMinTemperatureAfterTimePoints;
	if (this->cmMongoInterface->fGetMinMeasureAfterTimePoint(&cMinTemperatureBetweenTimePoints, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", from, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMinTemperatureBeforeTimePoint;
	if (this->cmMongoInterface->fGetMinMeasureBeforeTimePoint(&cMinTemperatureBetweenTimePoints, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMaxTemperatureBetweenTimePoints;
	if (this->cmMongoInterface->fGetMaxMeasureBetweenTimePoints(&cMaxTemperatureBetweenTimePoints, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, from, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMaxTemperatureAfterTimePoint;
	if (this->cmMongoInterface->fGetMaxMeasureAfterTimePoint(&cMaxTemperatureAfterTimePoint, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", from, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}


	string cMaxTemperatureBeforeTimePoint;
	if (this->cmMongoInterface->fGetMaxMeasureBeforeTimePoint(&cMaxTemperatureBeforeTimePoint, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cAvgTemperatureBetweenTimePoints;
	if (this->cmMongoInterface->fGetAvgMeasureBetweenTimePoints(&cAvgTemperatureBetweenTimePoints, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, from, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cAvgTemperatureAfterTimePoint;
	if (this->cmMongoInterface->fGetAvgMeasureAfterTimePoint(&cAvgTemperatureAfterTimePoint, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", from, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cAvgTemperatureBeforeTimePoint;
	if (this->cmMongoInterface->fGetAvgMeasureBeforeTimePoint(&cAvgTemperatureBeforeTimePoint, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", to, "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}*/


	list<string> cTemperaturesInBackWindow;
	if (this->cmMongoInterface->fGetMeasuresInBackWindow(&cTemperaturesInBackWindow, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", 100, "Sensor", "Temp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cAvgTemperatureInBackWindow;
	if (this->cmMongoInterface->fGetAvgOfMeasuresInBackWindow(&cAvgTemperatureInBackWindow, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", 100, "Value", "Sensor", "Temp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMaxTemperatureInBackWindow;
	if (this->cmMongoInterface->fGetMaxOfMeasuresInBackWindow(&cMaxTemperatureInBackWindow, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", 100, "Value", "Sensor", "Temp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMinTemperatureInBackWindow;
	if (this->cmMongoInterface->fGetMinOfMeasuresInBackWindow(&cMinTemperatureInBackWindow, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", 100, "Value", "Sensor", "Temp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}
	
	string cSumTemperatureInBackWindow;
	if (this->cmMongoInterface->fGetSumOfMeasuresInBackWindow(&cSumTemperatureInBackWindow, "SpeaTestDB", TcMeasure::kCollection, "Timestamp", 100, "Value", "Sensor", "Temp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	list<string> cTemperaturesBetweenValues;
	if (this->cmMongoInterface->fGetMeasuresBetweenValues(&cTemperaturesBetweenValues, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "28", "Sensor", "Temp", "Timestamp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	list<string> cTemperaturesGreatherThanValue;
	if (this->cmMongoInterface->fGetMeasuresGreatherThanValue(&cTemperaturesGreatherThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "28", "Sensor", "Temp", "Timestamp", "Value") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	list<string> cTemperaturesLowerThanValue;
	if (this->cmMongoInterface->fGetMeasuresLowerThanValue(&cTemperaturesLowerThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Sensor", "Temp", "Timestamp", "Value") < 0) {
		return(kGetDataFails);
	}

	string cAvgTemperatureBetweenValues;
	if (this->cmMongoInterface->fGetAvgOfMeasuresBetweenValues(&cAvgTemperatureBetweenValues, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "28", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cAvgTemperatureGreatherThanValue;
	if (this->cmMongoInterface->fGetAvgOfMeasuresGreatherThanValue(&cAvgTemperatureGreatherThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cAvgTemperatureLowerThanValue;
	if (this->cmMongoInterface->fGetAvgOfMeasuresLowerThanValue(&cAvgTemperatureLowerThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMaxTemperatureBetweenValues;
	if (this->cmMongoInterface->fGetMaxOfMeasuresBetweenValues(&cMaxTemperatureBetweenValues, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "28", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMaxTemperatureGreatherThanValue;
	if (this->cmMongoInterface->fGetMaxOfMeasuresGreatherThanValue(&cMaxTemperatureGreatherThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		return(kGetDataFails);
	}

	string cMaxTemperatureLowerThanValue;
	if (this->cmMongoInterface->fGetMaxOfMeasuresLowerThanValue(&cMaxTemperatureLowerThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMinTemperatureBetweenValues;
	if (this->cmMongoInterface->fGetMinOfMeasuresBetweenValues(&cMinTemperatureBetweenValues, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "28", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMinTemperatureGreatherThanValue;
	if (this->cmMongoInterface->fGetMinOfMeasuresGreatherThanValue(&cMinTemperatureGreatherThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cMinTemperatureLowerThanValue;
	if (this->cmMongoInterface->fGetMinOfMeasuresLowerThanValue(&cMinTemperatureLowerThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cSumTemperatureBetweenValues;
	if (this->cmMongoInterface->fGetSumOfMeasuresBetweenValues(&cSumTemperatureBetweenValues, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "28", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cSumTemperatureGreatherThanValue;
	if (this->cmMongoInterface->fGetSumOfMeasuresGreatherThanValue(&cSumTemperatureGreatherThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cSumTemperatureLowerThanValue;
	if (this->cmMongoInterface->fGetSumOfMeasuresLowerThanValue(&cSumTemperatureLowerThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Value", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cCountTemperatureBetweenValues;
	if (this->cmMongoInterface->fGetCountOfMeasuresBetweenValues(&cCountTemperatureBetweenValues, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "28", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cCountTemperatureGreatherThanValue;
	if (this->cmMongoInterface->fGetCountOfMeasuresGreatherThanValue(&cCountTemperatureGreatherThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}

	string cCountTemperatureLowerThanValue;
	if (this->cmMongoInterface->fGetCountOfMeasuresLowerThanValue(&cCountTemperatureLowerThanValue, "SpeaTestDB", TcMeasure::kCollection, "Value", "33", "Sensor", "Temp") < 0) {
		this->cmMongoInterface->fDisconnectMongoDriver();
		return(kGetDataFails);
	}


	this->cmMongoInterface->fDisconnectMongoDriver();

	*pLastTemperatures = cTemperaturesInBackWindow;
	*pAvgTemperature = cAvgTemperatureInBackWindow;
	*pMaxTemperature = cMaxTemperatureInBackWindow;
	*pMinTemperature = cMinTemperatureInBackWindow;
	*pSumTemperatures = cSumTemperatureInBackWindow;

	*pTemperaturesBetween = cTemperaturesBetweenValues;
	*pTemperaturesGreatherThan = cTemperaturesGreatherThanValue;
	*pTemperaturesLowerThan = cTemperaturesLowerThanValue;

	*pAvgTemperatureBetweenValues = cAvgTemperatureBetweenValues;
	*pAvgTemperatureGreatherThanValue = cAvgTemperatureGreatherThanValue;
	*pAvgTemperatureLowerThanValue = cAvgTemperatureLowerThanValue;

	*pMaxTemperatureBetweenValues = cMaxTemperatureBetweenValues;
	*pMaxTemperatureGreatherThanValue = cMaxTemperatureGreatherThanValue;
	*pMaxTemperatureLowerThanValue = cMaxTemperatureLowerThanValue;

	*pMinTemperatureBetweenValues = cMinTemperatureBetweenValues;
	*pMinTemperatureGreatherThanValue = cMinTemperatureGreatherThanValue;
	*pMinTemperatureLowerThanValue = cMinTemperatureLowerThanValue;

	*pSumTemperatureBetweenValues = cSumTemperatureBetweenValues;
	*pSumTemperatureGreatherThanValue = cSumTemperatureGreatherThanValue;
	*pSumTemperatureLowerThanValue = cSumTemperatureLowerThanValue;
	
	*pCountTemperatureBetweenValues = cCountTemperatureBetweenValues;
	*pCountTemperatureGreatherThanValue = cCountTemperatureGreatherThanValue;
	*pCountTemperatureLowerThanValue = cCountTemperatureLowerThanValue;

	return(kGetDataSuccess);

}



