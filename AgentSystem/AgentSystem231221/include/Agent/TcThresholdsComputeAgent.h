#pragma once
#ifndef TCTHRESHOLDSPLANARITYAGENT_H
#define TCTHRESHOLDSPLANARITYAGENT_H

#include "TcThresholdsAgent.h"

#include <string>
#include <chrono>



using namespace std;

class TcThresholdsComputeAgent : public TcThresholdsAgent {
private:

public:

	TcThresholdsComputeAgent(int pState, string pMachineFamily, string pEquipmentSN, string pSensorPosition, string pMeasureUnit, string pMeasuretype, string pReferenceID, string pReferenceType, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pCollectionname, string pDatabasename, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string agentid, string agentname, chrono::microseconds stepruntime, Priority priority = Priority::Medium, bool stopped = false);
	~TcThresholdsComputeAgent();
	void fAdjustNotificatedThresholds(float* pMeasureValue, float* pMeasureAlarmThresholdHigh, float* pMeasureAlarmThresholdLow);
};



#endif // AGENT_H