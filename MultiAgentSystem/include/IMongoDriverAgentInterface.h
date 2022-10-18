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

	TcMongoDriver* fGetDriver();

	int fGetData(list<string>* pOutput, string pDatabase, string pCollection, string pFilterattribute = "", string pFiltervalue = "", string pSortattribute = "", int pLimit = 0, int pSkip = 0, string pGroupattribute = "", list<string> pProjectionattributes = std::list<string>({""}));
	int fGetDataMaxOf(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMaxattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetDataMinOf(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMinattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetDataAvgOf(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pAvgattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetDataSumOf(string* pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pSumattribute, string pGroupattribute = "", string pProjectionattribute = "");
	int fGetDataBetweenValues(list<string>* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pSortattribute = "", string pProjectionattribute = "");
	int fGetDataGreatherThanValue(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSortattribute = "", string pProjectionattribute = "");
	int fGetDataLowerThanValue(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSortattribute = "", string pProjectionattribute = "");
	int fGetDataMinOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMinattribute);
	int fGetDataMinOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute);
	int fGetDataMinOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute);
	int fGetDataMaxOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMaxattribute);
	int fGetDataMaxOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute);
	int fGetDataMaxOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute);
	int fGetDataAvgOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pAvgattribute);
	int fGetDataAvgOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute);
	int fGetDataAvgOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute);
	int fGetDataSumOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pSumattribute);
	int fGetDataSumOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute);
	int fGetDataSumOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute);
	int fGetDataCountOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound);
	int fGetDataCountOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue);
	int fGetDataCountOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue);
	
	int fGetDataBetweenTimePoints(list<string>* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom,  string pSortattribute = "", string pProjectionattribute = "");
	int fGetDataAfterTimePoint(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter,  string pSortattribute = "", string pProjectionattribute = "");
	int fGetDataBeforeTimePoint(list<string>* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore,  string pSortattribute = "", string pProjectionattribute = "");
	int fGetDataMinBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pMinattribute,  string pProjectionattribute = "");
	int fGetDataMinAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pMinattribute,  string pProjectionattribute = "");
	int fGetDataMinBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pMinattribute,  string pProjectionattribute = "");
	int fGetDataMaxBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pMaxattribute,  string pProjectionattribute = "");
	int fGetDataMaxAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pMaxattribute,  string pProjectionattribute = "");
	int fGetDataMaxBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pMaxattribute,  string pProjectionattribute = "");
	int fGetDataSumBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pSumattribute,  string pProjectionattribute = "");
	int fGetDataSumAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pSumattribute,  string pProjectionattribute = "");
	int fGetDataSumBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pSumattribute,  string pProjectionattribute = "");
	int fGetDataAvgBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom, string pAvgattribute,  string pProjectionattribute = "");
	int fGetDataAvgAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter, string pAvgattribute,  string pProjectionattribute = "");
	int fGetDataAvgBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore, string pAvgattribute,  string pProjectionattribute = "");
	int fGetDataCountBetweenTimePoints(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, chrono::time_point<chrono::high_resolution_clock> pTo, chrono::time_point<chrono::high_resolution_clock> pFrom,  string pProjectionattribute = "");
	int fGetDataCountAfterTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pAfter,  string pProjectionattribute = "");
	int fGetDataCountBeforeTimePoint(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, chrono::time_point<chrono::high_resolution_clock> pBefore,  string pProjectionattribute = "");
	#pragma endregion
	
	void fAddFictitiousData();
};
#endif //TCMONGODRIVER_H