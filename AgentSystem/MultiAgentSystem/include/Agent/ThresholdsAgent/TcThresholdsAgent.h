#pragma once
#ifndef TCTHRESHOLDSAGENT_H
#define TCTHRESHOLDSAGENT_H

#include <string>
#include <chrono>
#include "../TcAgent.h"

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
	int rmState;

public:

	TcThresholdsAgent(int pState, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pAgentID, string pAgentname, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
	~TcThresholdsAgent();

	void fSetAlarmThresholdHigh(float pAlarmThresholdHigh);
	void fSetAlarmThresholdLow(float pAlarmThresholdLow);

	void fSetWarningThresholdHigh(float pWarningThresholdHigh);
	void fSetWarningThresholdLow(float pWarningThresholdLow);

	void fSetLastSampleRead(chrono::system_clock::time_point pLastDatabaseRead);
	void fSetState(int pState);
	int fGetState();

	chrono::system_clock::time_point fGetLastSampleRead();

	float fGetAlarmThresholdHigh();
	float fGetAlarmThresholdLow();

	float fGetWarningThresholdHigh();
	float fGetWarningThresholdLow();

	virtual int fRun();
	int fGetData(list<float>** pMeasureValues);
	int fThreasholdCompare(float pValue);
	virtual void fGetNotificationByComparing(int pComparingResult, float pValue, float pAlarmThresholdHigh, float pAlarmThresholdLow, string* pNotificationBody, string* pNotificationType, string* pNotificationTitle);
	virtual void fAdjustNotificatedThresholds(float* pMeasureValue, float* pMeasureAlarmThresholdHigh, float* pMeasureAlarmThresholdLow);
};
			

#endif // AGENT_H