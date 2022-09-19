#pragma once
#ifndef TCTHRESHOLDSPLANARITYAGENT_H
#define TCTHRESHOLDSPLANARITYAGENT_H



#include <string>
#include <chrono>
#include "TcThresholdsAgent.h"

namespace Spea {
	namespace Archimede {
		namespace MultiAgentSystem {
			namespace Agent {
				class TcThresholdsComputeAgent : public TcThresholdsAgent {
				public:
					TcThresholdsComputeAgent(int pState, string pSensorPosition, string pMeasureUnit, string pMeasuretype, string pReferenceid, string pReferenceType, float pWarningThresholdHigh, float pWarningThresholdLow, float pAlarmThresholdHigh, float pAlarmThresholdLow, string pMachineSerialNumber, string pMachineFamily, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentname, chrono::microseconds pStepRunTime, chrono::time_point<chrono::high_resolution_clock> pNextRunTime = chrono::high_resolution_clock::now(), Priority pPriority = Priority::Medium, bool pStopped = false);
					~TcThresholdsComputeAgent();
					void fAdjustNotificatedThresholds(float* pMeasureValue, float* pMeasureAlarmThresholdHigh, float* pMeasureAlarmThresholdLow);
				};
			}
		}
	}
}
#endif // AGENT_H