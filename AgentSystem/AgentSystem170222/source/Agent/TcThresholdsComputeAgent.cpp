#include <stdlib.h>
#include <cmath>
#include "../../include/Agent/TcThresholdsAgent.h"
#include "../../include/Agent/TcThresholdsComputeAgent.h"


TcThresholdsComputeAgent::TcThresholdsComputeAgent(int pState, string pSensorPosition, string pMeasureUnit, string pMeasuretype, string pReferenceid, string pReferenceType, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime, Priority pPriority, bool pStopped) : TcThresholdsAgent(pState, pSensorPosition, pMeasureUnit, pMeasuretype, pReferenceid, pReferenceType, pWarningThresholdHigh, pWarningThresholdLow, pAlarmThresholdHigh, pAlarmThresholdLow, pMachineSerialNumber, pMachineFamily, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pNextRunTime, pPriority, pStopped) {}
TcThresholdsComputeAgent::~TcThresholdsComputeAgent() { ; }
void TcThresholdsComputeAgent::fAdjustNotificatedThresholds(float* pMeasureValue, float* pMeasureAlarmThresholdHigh, float* pMeasureAlarmThresholdLow) {
	*pMeasureValue = ((180 / 3.1415) * acos(*pMeasureValue));
	*pMeasureAlarmThresholdHigh = ((180 / 3.1415) * acos(*pMeasureAlarmThresholdHigh));
	*pMeasureAlarmThresholdLow = ((180 / 3.1415) * acos(*pMeasureAlarmThresholdLow));
	return;
}
