#pragma once
#ifndef TCTHRESHOLDSAGENT_H
#define TCTHRESHOLDSAGENT_H

#include "TcAgent.h"

#include <string>
#include <chrono>



using namespace std;

class TcThresholdsAgent : public TcAgent {
protected:
	
	constexpr static const int8_t kMeasureOverAlarmThreshold = 2;
	constexpr static const int8_t kMeasureOverWarningThreshold = 1;
	constexpr static const int8_t kMeasureUnderAlarmThreshold = -2;
	constexpr static const int8_t kMeasureUnderWarningThreshold = -1;
	constexpr static const int8_t kMeasureInRange = 0;

	constexpr static const int8_t kCompareThresholdOutOfRange = -1;
	constexpr static const int8_t kCompareThresholdInRange = 0;



	chrono::system_clock::time_point cmLastSampleRead;
	float rmAlarmThresholdHigh;
	float rmAlarmThresholdLow;
	float rmWarningThresholdHigh;
	float rmWarningThresholdLow;
	string rmMachineFamily;
	string rmEquipmentSN;
	string rmSensorPosition;
	string rmMeasureType;
	string rmReferenceID;
	string rmReferenceType;
	string rmMeasureUnit;
	int rmState;

public:

	TcThresholdsAgent(int pState, string pMachineFamiliy, string pEquipmentSN, string pSensorPosition, string pMeasureUnit, string pMeasuretype, string pReferenceID, string pReferenceType, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pCollectionname, string pDatabasename, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string agentid, string agentname, chrono::microseconds stepruntime, Priority priority = Priority::Medium, bool stopped = false);
	~TcThresholdsAgent();

	void fSetAlarmThresholdHigh(float pAlarmThresholdHigh);
	void fSetAlarmThresholdLow(float pAlarmThresholdLow);
	
	void fSetWarningThresholdHigh(float pWarningThresholdHigh);
	void fSetWarningThresholdLow(float pWarningThresholdLow);

	void fSetReferenceType(string pReferenceType);
	void fSetReferenceID(string pReferenceID);
	void fSetMeasureType(string pMeasureType);
	void fSetMeasureUnit(string pMeasureUnit);
	
	void fSetLastSampleRead(chrono::system_clock::time_point pLastDatabaseRead);
	

	void fSetState(int pState);
	int fGetState();

	chrono::system_clock::time_point fGetLastSampleRead();

	float fGetAlarmThresholdHigh();
	float fGetAlarmThresholdLow();

	float fGetWarningThresholdHigh();
	float fGetWarningThresholdLow();

	string fGetReferenceType();
	string fGetMeasureType();
	string fGetReferenceID();
	string fGetMeasureUnit();


	virtual int fRun();
	int fGetData(list<float>** pMeasureValues);
	int fThreasholdCompare(float pValue);
	virtual void fGetNotificationByComparing(int pComparingResult, string pSensorPosition, string pMachineFamily, string pEquipmentSN, float pValue, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pMeasureType, string pMeasureUnit, string* pNotificationBody, string* pNotificationType, string* pNotificationTitle);
	virtual void fAdjustNotificatedThresholds(float *pMeasureValue, float *pMeasureAlarmThresholdHigh, float *pMeasureAlarmThresholdLow);
};



#endif // AGENT_H