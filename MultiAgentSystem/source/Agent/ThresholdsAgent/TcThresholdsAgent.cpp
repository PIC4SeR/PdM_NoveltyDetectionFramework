#include <stdlib.h>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include "../../../include/Agent/ThresholdsAgent/TcThresholdsAgent.h"
#include "../../../include/TcMeasure.h"
#include "../../../include/TcColors.h"



TcThresholdsAgent::TcThresholdsAgent(int pState, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcAgent(pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped){
	this->rmState = pState;
	this->cmLastSampleRead = chrono::system_clock::now();
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

	return(kRunFails);
	 
	for (float rMeasureValue : *cMeasureValues) {
		rNewState = fThreasholdCompare(rMeasureValue);
		if (rNewState != TcThresholdsAgent::kMeasureInRange && rNewState != this->rmState) {
			float rMeasureAlarmThresholdHigh = this->rmAlarmThresholdHigh;
			float rMeasureAlarmThresholdLow = this->rmAlarmThresholdLow;
			fAdjustNotificatedThresholds(&rMeasureValue, &rMeasureAlarmThresholdHigh, &rMeasureAlarmThresholdLow);
			fGetNotificationByComparing(rNewState, rMeasureValue, rMeasureAlarmThresholdHigh, rMeasureAlarmThresholdLow, &rMeasureNotificationBody, &rMeasureNotificationType, &rMeasureNotificationTitle);
			rResult = fNotifyMessage(rMeasureNotificationType, rMeasureNotificationTitle, rMeasureNotificationBody);
			if (rResult < 0) {
				fprintf(stdout, ANSI_COLOR_GREEN "Send notification fails with error %d" ANSI_COLOR_RESET "\n", rResult);
				fflush(stdout);
				return(kRunFails);
			} else {
				fprintf(stdout, ANSI_COLOR_GREEN "Notification sent"  ANSI_COLOR_RESET "\n");
				fflush(stdout);
			}
		}
		this->rmState = rNewState;
	}

	
	return(kRunSuccess);
}

int TcThresholdsAgent::fGetData(list<float>** pMeasureValues) {

	int rResult = 0;

	long long rFromMilliseconds = (long long)chrono::duration_cast<chrono::milliseconds>(cmLastSampleRead.time_since_epoch()).count();
	long long rToMilliseconds = (long long) chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	
	list<float>* cMeasureValues = new list<float>();
	list<string> cMeasureBetweenTimeValues;
	if (this->cmMongoInterface->fGetDataBetweenValues(&cMeasureBetweenTimeValues, this->rmDatabaseName, this->rmCollectionName, "TimestampEpoch", to_string(rToMilliseconds), to_string(rFromMilliseconds), "rpTimestampEpoch", "Value") < 0) {
		return(kGetDataFails);
	}

	this->cmLastSampleRead = chrono::system_clock::now();

	fprintf(stdout, "Number of measures %d read in %lld milliseconds\n", (int) cMeasureBetweenTimeValues.size(), (long long int)(rToMilliseconds-rFromMilliseconds));
	fflush(stdout);
	
	

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



void TcThresholdsAgent::fGetNotificationByComparing(int pComparingResult, float pValue, float pAlarmThresholdHigh, float pAlarmThresholdLow, string *pNotificationBody, string* pNotificationType, string* pNotificationTitle) {

	if (pComparingResult != TcThresholdsAgent::kMeasureOverAlarmThreshold && pComparingResult != TcThresholdsAgent::kMeasureOverWarningThreshold && pComparingResult != TcThresholdsAgent::kMeasureUnderWarningThreshold && pComparingResult != TcThresholdsAgent::kMeasureUnderAlarmThreshold) {
		*pNotificationBody = "";
		*pNotificationType = "";
		*pNotificationTitle = "";
		return;
	}

	char aStringRoundedValue[16];
	char aStringRoundedAlarmThresholdLow[16];
	char aStringRoundedAlarmThresholdHigh[16];
	
	snprintf(aStringRoundedValue, 16, "%.1f", pValue);
	snprintf(aStringRoundedAlarmThresholdLow, 16, "%.1f", pAlarmThresholdLow);
	snprintf(aStringRoundedAlarmThresholdHigh, 16, "%.1f", pAlarmThresholdHigh);

	string rRange = "Alarm Thresholds: [" + string(aStringRoundedAlarmThresholdLow) + ", " + string(aStringRoundedAlarmThresholdHigh) + "]";
	string rTailDefaultBody = "Value: " + string(aStringRoundedValue) + "\n" + rRange;

	switch (pComparingResult) {
	case TcThresholdsAgent::kMeasureUnderAlarmThreshold:
		*pNotificationBody = "Under minimum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Alarm";
		*pNotificationTitle = "Alarm ";
		break;
	case TcThresholdsAgent::kMeasureUnderWarningThreshold:
		*pNotificationBody = "Close to minimum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Warning";
		*pNotificationTitle = "Warning ";
		break;
	case TcThresholdsAgent::kMeasureOverAlarmThreshold:
		*pNotificationBody = "Over maximum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Alarm";
		*pNotificationTitle = "Alarm ";
		break;
	case TcThresholdsAgent::kMeasureOverWarningThreshold:
		*pNotificationBody = "Close to maximum threshold\n" + rTailDefaultBody;
		*pNotificationType = "Warning";
		*pNotificationTitle = "Warning ";
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



