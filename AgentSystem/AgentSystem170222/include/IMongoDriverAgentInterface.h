/**
	@file TcMongoDriver.h
	@author Giuseppe Pedone
	@version 2.0 04/05/2021
*/
#pragma once
#ifndef IMONGODRIVERAGENTINTERFACE_H
#define IMONGODRIVERAGENTINTERFACE_H

	#include <inttypes.h>
	#include <string>
	#include <list>
    #include "TcMongoDriver.h"
	#include "Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h"

	

	using namespace std;
	

	class IMongoDriverAgentInterface
	{
	private:
		#pragma region Internal Attributes
			TcMongoDriver* cmMongoDriver;
		#pragma endregion

				

	public:
		#pragma region External Error Codes
			constexpr const static int8_t kConnectionFails = -1;
			constexpr const static int8_t kQueryFails = -2;
			constexpr const static int8_t kGetSuccess = 0;
			constexpr const static int8_t kConnectionSuccess = 0;
		#pragma endregion



		#pragma region External Functions
				IMongoDriverAgentInterface(string pMongoDriverName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort);
				~IMongoDriverAgentInterface();

				int fConnectMongoDriver();
				void fDisconnectMongoDriver();

				string fComposeFilterByMeasureAttributes(string pReferenceid, string pReferencetype, string pMeasuretype);

                int fGetMeasuresInBackWindow(list<string> *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pGroupattribute = "", string pProjectionattribute = "");
				int fGetMaxOfMeasuresInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMaxattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pGroupattribute = "", string pProjectionattribute = "");
				int fGetMinOfMeasuresInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMinattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pGroupattribute = "", string pProjectionattribute = "");
				int fGetAvgOfMeasuresInBackWindow(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pAvgattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pGroupattribute = "", string pProjectionattribute = "");
				int fGetSumOfMeasuresInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string sumattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pGroupattribute = "", string pProjectionattribute = "");
				

				int fGetStatisticsOfMeasuresBetweenValues(list<TcStatisticTestMeasures>* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, int pWindow, string pMovavgattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pGroupattribute = "", string pProjectionattribute = "");
				int fGetStatisticsOfMeasuresLowerThanValue(list<TcStatisticTestMeasures>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, int pWindow, string pMovavgattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pGroupattribute = "", string pProjectionattribute = "");
				int fGetStatisticsOfMeasuresGreatherThanValue(list<TcStatisticTestMeasures>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, int pWindow, string pMovavgattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pGroupattribute = "", string pProjectionattribute = "");


				int fGetMeasuresBetweenValues(list<string> *pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pProjectionattribute = "");
                int fGetMeasuresGreatherThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pProjectionattribute = "");
                int fGetMeasuresLowerThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pProjectionattribute = "");
				
				int fGetMinOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMinattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetMinOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetMinOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				
				int fGetMaxOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMaxattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetMaxOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetMaxOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				
				int fGetAvgOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pAvgattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetAvgOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetAvgOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				
				int fGetSumOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string sumattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetSumOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string sumattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetSumOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string sumattribute, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				
				int fGetCountOfMeasuresBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetCountOfMeasuresGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				int fGetCountOfMeasuresLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pReferenceid = "", string pReferencetype = "", string pMeasuretype = "");
				/*
				int fGetMeasuresBetweenTimePoints(list<string>* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pProjectionattribute = "");
				int fGetMeasurespAfterTimePoint(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pProjectionattribute = "");
				int fGetMeasurespBeforeTimePoint(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pReferencetype = "", string pMeasuretype = "", string pSortattribute = "", string pProjectionattribute = "");
				int fGetMinMeasureBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pMinattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetMinMeasurepAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pMinattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetMinMeasurepBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pMinattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				
				int fGetMaxMeasureBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pMaxattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetMaxMeasurepAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pMaxattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetMaxMeasurepBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pMaxattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				
				int fGetSumMeasureBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string sumattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetSumMeasurepAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string sumattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetSumMeasurepBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string sumattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				
				int fGetAvgMeasureBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pAvgattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetAvgMeasurepAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pAvgattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetAvgMeasurepBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pAvgattribute, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				
				int fGetCountMeasureBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetCountMeasurepAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				int fGetCountMeasurepBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pReferencetype = "", string pMeasuretype = "", string pProjectionattribute = "");
				*/
		#pragma endregion
				void fAddFictitiousData();
	};



	

#endif //TCMONGODRIVER_H
