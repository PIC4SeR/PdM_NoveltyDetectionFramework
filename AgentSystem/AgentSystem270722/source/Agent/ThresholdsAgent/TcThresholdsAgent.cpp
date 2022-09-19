#include <stdlib.h>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include "../../../include/Agent/ThresholdsAgent/TcThresholdsAgent.h"
#include "../../../include/TcMeasure.h"

using namespace Spea::Archimede::MultiAgentSystem::Agent;

TcThresholdsAgent::TcThresholdsAgent(int pState, string pSensorPosition, string pMeasureUnit, string pMeasuretype, string pReferenceid, string pReferenceType, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pMachineSerialNumber, pMachineFamily, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped){
	this->rmState = pState;
	this->cmLastSampleRead = chrono::system_clock::now();
	this->rmSensorPosition = pSensorPosition;
	this->rmMeasureUnit = pMeasureUnit;
	this->rmReferenceType = pReferenceType;
	this->rmReferenceID = pReferenceid;
	this->rmMeasureType = pMeasuretype;
	this->rmWarningThresholdHigh = pWarningThresholdHigh;
	this->rmWarningThresholdLow = pWarningThresholdLow;
	this->rmAlarmThresholdHigh = pAlarmThresholdHigh;
	this->rmAlarmThresholdLow = pAlarmThresholdLow;
}

TcThresholdsAgent::~TcThresholdsAgent() { ; }

void TcThresholdsAgent::fSetAlarmThresholdHigh(float pAlarmThresholdHigh) {
	this->rmAlarmThresholdHigh = pAlarmThresholdHigh;
}

void TcThresholdsAgent::fSetAlarmThresholdLow(float pAlarmThresholdLow) {
	this->rmAlarmThresholdLow = pAlarmThresholdLow;
}

void TcThresholdsAgent::fSetWarningThresholdHigh(float pWarningThresholdHigh) {
	this->rmWarningThresholdHigh = pWarningThresholdHigh;
}

void TcThresholdsAgent::fSetWarningThresholdLow(float pWarningThresholdLow) {
	this->rmWarningThresholdLow = pWarningThresholdLow;
}

void TcThresholdsAgent::fSetMeasureType(string pMeasureType) {
	this->rmMeasureType = pMeasureType;
}

void TcThresholdsAgent::fSetReferenceID(string pReferenceID) {
	this->rmReferenceID = pReferenceID;
}

void TcThresholdsAgent::fSetReferenceType(string pReferenceType) {
	this->rmReferenceType = pReferenceType;
}

void TcThresholdsAgent::fSetMeasureUnit(string pMeasureUnit) {
	this->rmMeasureUnit = pMeasureUnit;
}

void TcThresholdsAgent::fSetLastSampleRead(chrono::system_clock::time_point pLastSampleRead) {
	this->cmLastSampleRead = pLastSampleRead;
}

void TcThresholdsAgent::fSetState(int pState) {
	this->rmState = pState;
}


int TcThresholdsAgent::fGetState() {
	return(this->rmState);
}



chrono::system_clock::time_point TcThresholdsAgent::fGetLastSampleRead() {
	return(this->cmLastSampleRead);
}



string TcThresholdsAgent::fGetMeasureUnit() {
	return(this->rmMeasureUnit);
}


string TcThresholdsAgent::fGetReferenceType() {
	return(this->rmReferenceType);
}


float TcThresholdsAgent::fGetAlarmThresholdHigh() {
	return(this->rmAlarmThresholdHigh);
}
float TcThresholdsAgent::fGetAlarmThresholdLow() {
	return(this->rmAlarmThresholdLow);
}

float TcThresholdsAgent::fGetWarningThresholdHigh() {
	return(this->rmWarningThresholdHigh);
}
float TcThresholdsAgent::fGetWarningThresholdLow() {
	return(this->rmWarningThresholdLow);
}

string TcThresholdsAgent::fGetReferenceID() {
	return(this->rmReferenceID);
}

string TcThresholdsAgent::fGetMeasureType() {
	return(this->rmMeasureType);
}



int TcThresholdsAgent::fRun() {
	list<float>* cMeasureValues = new list<float>();
	string rMeasureNotificationType;
	string rMeasureNotificationTitle;
	string rMeasureNotificationBody;
	int rNewState = 0;
	int rResult = 0;

	

	rResult = fGetData(&cMeasureValues);
	if (rResult < 0) {
		fprintf(stdout, ANSI_COLOR_RED "Get data from database fails with error %d"  ANSI_COLOR_RESET "\n", rResult);
		fflush(stdout);
		return(kRunFails);
	}
	 
	for (float rMeasureValue : *cMeasureValues) {
		rNewState = fThreasholdCompare(rMeasureValue);
		if (rNewState != TcThresholdsAgent::kMeasureInRange && rNewState != this->rmState) {
			float rMeasureAlarmThresholdHigh = this->rmAlarmThresholdHigh;
			float rMeasureAlarmThresholdLow = this->rmAlarmThresholdLow;
			fAdjustNotificatedThresholds(&rMeasureValue, &rMeasureAlarmThresholdHigh, &rMeasureAlarmThresholdLow);
			fGetNotificationByComparing(rNewState, this->rmSensorPosition, this->rmMachineFamily, this->rmMachineSerialNumber, rMeasureValue, rMeasureAlarmThresholdHigh, rMeasureAlarmThresholdLow, this->rmMeasureType, this->rmMeasureUnit, &rMeasureNotificationBody, &rMeasureNotificationType, &rMeasureNotificationTitle);
			rResult = fNotifyMessage(rMeasureNotificationType, rMeasureNotificationTitle, rMeasureNotificationBody, this->rmMachineSerialNumber, this->rmMachineFamily);
			if (rResult < 0) {
				fprintf(stdout, ANSI_COLOR_GREEN "Send notification fails with error %d" ANSI_COLOR_RESET "\n", rResult);
				fflush(stdout);
				return(kRunFails);
			} else {
				fprintf(stdout, ANSI_COLOR_GREEN "Send %s notification"  ANSI_COLOR_RESET "\n", this->rmMeasureType.c_str());
				fflush(stdout);
			}
		}
		this->rmState = rNewState;
	}

	
	return(kRunSuccess);
}

int TcThresholdsAgent::fGetData(list<float>** pMeasureValues) {

	int rResult = cmMongoInterface->fConnectDriver();
	if (rResult < 0) {
		fprintf(stdout, "Connection to Mongo Driver fails with error %d\n", rResult);
		fflush(stdout);
		return(kGetDataFails);
	}

	long long rFromMilliseconds = (long long)chrono::duration_cast<chrono::milliseconds>(cmLastSampleRead.time_since_epoch()).count();
	long long rToMilliseconds = (long long) chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	
	list<float>* cMeasureValues = new list<float>();
	list<string> cMeasureBetweenTimeValues;
	if (this->cmMongoInterface->fGetMeasuresBetweenValues(&cMeasureBetweenTimeValues, this->rmDatabaseName, this->rmCollectionName, "rpTimestampEpoch", to_string(rToMilliseconds), to_string(rFromMilliseconds), this->rmReferenceID, this->rmReferenceType, this->rmMeasureType, "rpTimestampEpoch", "Value") < 0) {
		cmMongoInterface->fDisconnectDriver();
		return(kGetDataFails);
	}

	this->cmLastSampleRead = chrono::system_clock::now();

	fprintf(stdout, "Number of %s measures %d read in %lld milliseconds\n", this->rmMeasureType.c_str(), cMeasureBetweenTimeValues.size(), (long long int)(rToMilliseconds-rFromMilliseconds));
	fflush(stdout);
	
	cmMongoInterface->fDisconnectDriver();

	for(string rStringMeasure : cMeasureBetweenTimeValues) {
		bsoncxx::document::value rMeasure = bsoncxx::from_json(rStringMeasure);
		float rMeasureValue = rMeasure.view()["Value"].get_double().value;
		cMeasureValues->push_back(rMeasureValue);
	}

	*pMeasureValues = cMeasureValues;

	return(kGetDataSuccess);
}





int TcThresholdsAgent::fThreasholdCompare(float pValue) {

	if (pValue > this->rmAlarmThresholdHigh) {
		return(TcThresholdsAgent::kMeasureOverAlarmThreshold);
	} else if (pValue > this->rmWarningThresholdHigh) {
		return(TcThresholdsAgent::kMeasureOverWarningThreshold);
	}

	if (pValue < this->rmAlarmThresholdLow) {
		return(TcThresholdsAgent::kMeasureUnderAlarmThreshold);
	} else if (pValue < this->rmWarningThresholdLow) {
		return(TcThresholdsAgent::kMeasureUnderWarningThreshold);
	}

	return(TcThresholdsAgent::kMeasureInRange);
}



void TcThresholdsAgent::fGetNotificationByComparing(int pComparingResult, string pSensorPosition, string pMachineFamily, string pMachineSerialNumber, float pValue, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pMeasureType, string pMeasureUnit, string *pNotificationBody, string* pNotificationType, string* pNotificationTitle) {

	if (pComparingResult != TcThresholdsAgent::kMeasureOverAlarmThreshold && pComparingResult != TcThresholdsAgent::kMeasureOverWarningThreshold && pComparingResult != TcThresholdsAgent::kMeasureUnderWarningThreshold && pComparingResult != TcThresholdsAgent::kMeasureUnderAlarmThreshold) {
		*pNotificationBody = "";
		*pNotificationType = "";
		*pNotificationTitle = "";
		return;
	}

	char aStringRoundedValue[16];
	char aStringRoundedAlarmThresholdLow[16];
	char aStringRoundedAlarmThresholdHigh[16];
	
	sprintf_s(aStringRoundedValue, "%.1f", pValue);
	sprintf_s(aStringRoundedAlarmThresholdLow, "%.1f", pAlarmThresholdLow);
	sprintf_s(aStringRoundedAlarmThresholdHigh, "%.1f", pAlarmThresholdHigh);

	string rRange = /*"Warning Thresholds: [" + string(aStringRoundedWarningThresholdLow) + " " + pMeasureUnit + ", " + string(aStringRoundedWarningThresholdHigh) + " " + pMeasureUnit + "]\n*/"Alarm Thresholds: [" + string(aStringRoundedAlarmThresholdLow) + " " + pMeasureUnit + ", " + string(aStringRoundedAlarmThresholdHigh) + " " + pMeasureUnit + "]";
	string rDefaultTitle = "on " + pMachineFamily + " SN: " + pMachineSerialNumber;
	string rHeadDefaultBody = pMeasureType + " in the " + pSensorPosition;
	string rTailDefaultBody = "Value: " + string(aStringRoundedValue) + " " + pMeasureUnit + "\n" + rRange;

	switch (pComparingResult) {
	case TcThresholdsAgent::kMeasureUnderAlarmThreshold:
		*pNotificationBody = rHeadDefaultBody + " under minimum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Alarm";
		*pNotificationTitle = "Alarm " + rDefaultTitle;
		break;
	case TcThresholdsAgent::kMeasureUnderWarningThreshold:
		*pNotificationBody = rHeadDefaultBody + " close to minimum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Warning";
		*pNotificationTitle = "Warning " + rDefaultTitle;
		break;
	case TcThresholdsAgent::kMeasureOverAlarmThreshold:
		*pNotificationBody = rHeadDefaultBody + " over maximum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Alarm";
		*pNotificationTitle = "Alarm " + rDefaultTitle;
		break;
	case TcThresholdsAgent::kMeasureOverWarningThreshold:
		*pNotificationBody = rHeadDefaultBody +  " close to maximum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Warning";
		*pNotificationTitle = "Warning " + rDefaultTitle;
		break;
	default:
		*pNotificationBody = "";
		*pNotificationType = "";
		*pNotificationTitle = "";
	}

	return;
}

void TcThresholdsAgent::fAdjustNotificatedThresholds(float* pMeasureValue, float* pMeasureAlarmThresholdHigh, float* pMeasureAlarmThresholdLow) {
	return;
}



