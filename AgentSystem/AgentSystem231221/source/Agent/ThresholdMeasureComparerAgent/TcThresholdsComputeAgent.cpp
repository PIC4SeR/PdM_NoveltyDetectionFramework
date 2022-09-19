#include <stdlib.h>
#include <cmath>
#include "../../include/Agent/TcThresholdsAgent.h"
#include "../../include/Agent/TcThresholdsComputeAgent.h"


TcThresholdsComputeAgent::TcThresholdsComputeAgent(int pState, string pMachineFamily, string pEquipmentSN, string pSensorPosition, string pMeasureUnit, string pMeasuretype, string pReferenceid, string pReferenceType, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pCollectionname, string pDatabasename, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string agentid, string agentname, chrono::microseconds stepruntime, Priority priority, bool stopped) : TcThresholdsAgent(pState, pMachineFamily, pEquipmentSN, pSensorPosition, pMeasureUnit, pMeasuretype, pReferenceid, pReferenceType, pWarningThresholdHigh, pWarningThresholdLow, pAlarmThresholdHigh, pAlarmThresholdLow, pCollectionname, pDatabasename, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, agentid, agentname, stepruntime, priority, stopped) {}
TcThresholdsComputeAgent::~TcThresholdsComputeAgent() { ; }
void TcThresholdsComputeAgent::fAdjustNotificatedThresholds(float* pMeasureValue, float* pMeasureAlarmThresholdHigh, float* pMeasureAlarmThresholdLow) {
	*pMeasureValue = ((180 / 3.1415) * acos(*pMeasureValue));
	*pMeasureAlarmThresholdHigh = ((180 / 3.1415) * acos(*pMeasureAlarmThresholdHigh));
	*pMeasureAlarmThresholdLow = ((180 / 3.1415) * acos(*pMeasureAlarmThresholdLow));
	return;
}
