#pragma once

#ifndef IMONGODRIVERAGENTINTERFACE_H
#define IMONGODRIVERAGENTINTERFACE_H

	#include <inttypes.h>
	#include <string>
	#include <list>
    #include "TcMongoDriver.h"


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
	IMongoDriverAgentInterface();
	~IMongoDriverAgentInterface();

	int fConnectDriver();
	void fDisconnectDriver();
	TcMongoDriver* fGetDriver();

	int fGetInBackWindow(list<string>* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetMaxOfInBackWindow(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMaxattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetMinOfInBackWindow(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMinattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetAvgOfInBackWindow(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pAvgattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetSumOfInBackWindow(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string sumattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetBetweenValues(list<string>* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pSortattribute = "", string pProjectionattribute = "");
	int fGetGreatherThanValue(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSortattribute = "", string pProjectionattribute = "");
	int fGetLowerThanValue(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSortattribute = "", string pProjectionattribute = "");
	int fGetMinOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMinattribute);
	int fGetMinOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute);
	int fGetMinOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute);
	int fGetMaxOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMaxattribute);
	int fGetMaxOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute);
	int fGetMaxOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute);
	int fGetAvgOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pAvgattribute);
	int fGetAvgOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute);
	int fGetAvgOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute);
	int fGetSumOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string sumattribute);
	int fGetSumOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string sumattribute);
	int fGetSumOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string sumattribute);
	int fGetCountOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound);
	int fGetCountOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue);
	int fGetCountOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue);
	
	int fGetBetweenTimePoints(list<string>* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom,  string pSortattribute = "", string pProjectionattribute = "");
	int fGetAfterTimePoint(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter,  string pSortattribute = "", string pProjectionattribute = "");
	int fGetBeforeTimePoint(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore,  string pSortattribute = "", string pProjectionattribute = "");
	int fGetMinBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pMinattribute,  string pProjectionattribute = "");
	int fGetMinAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pMinattribute,  string pProjectionattribute = "");
	int fGetMinBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pMinattribute,  string pProjectionattribute = "");
	int fGetMaxBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pMaxattribute,  string pProjectionattribute = "");
	int fGetMaxAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pMaxattribute,  string pProjectionattribute = "");
	int fGetMaxBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pMaxattribute,  string pProjectionattribute = "");
	int fGetSumBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string sumattribute,  string pProjectionattribute = "");
	int fGetSumAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string sumattribute,  string pProjectionattribute = "");
	int fGetSumBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string sumattribute,  string pProjectionattribute = "");
	int fGetAvgBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pAvgattribute,  string pProjectionattribute = "");
	int fGetAvgAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pAvgattribute,  string pProjectionattribute = "");
	int fGetAvgBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pAvgattribute,  string pProjectionattribute = "");
	int fGetCountBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom,  string pProjectionattribute = "");
	int fGetCountAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter,  string pProjectionattribute = "");
	int fGetCountBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore,  string pProjectionattribute = "");
	#pragma endregion
	
	void fAddFictitiousData();
};
#endif //TCMONGODRIVER_H
