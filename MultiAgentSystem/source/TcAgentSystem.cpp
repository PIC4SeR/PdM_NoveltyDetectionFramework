#include <string>
#include <chrono>
#include <thread>

#include "../include/TcAgentSystem.h"
#include "../include/TcLinearRegressor.h"
#include "../include/Agent/TcAgent.h"
#include "../include/Agent/ThresholdsAgent/TcThresholdsAgent.h"
#include "../include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h"

using namespace std;

TcAgentSystem::TcAgentSystem(string pSystemid, string pSystemname) {
	rmSystemid = pSystemid;
	rmSystemname = pSystemname;
	cmManager = new TcAgentManager();
}
TcAgentSystem::~TcAgentSystem() {
	if (cmManager != nullptr){
		delete cmManager;
	}
}
void TcAgentSystem::fExecuteManager() {
	cmManager->fExecute();
}
void TcAgentSystem::fStopManager() {
	cmManager->fSetStopped(true);
}
void TcAgentSystem::fLoadManager(string pManagerid, string pManagername, chrono::microseconds pScheduleminwaittime, chrono::microseconds pExecutionwaittime) {
	cmManager->fSetId(pManagerid);
	cmManager->fSetName(pManagername);
	cmManager->fSetScheduleMinWaitTime(pScheduleminwaittime);
	cmManager->fSetExecutionWaitTime(pExecutionwaittime);
}
void TcAgentSystem::fLoadAgent(IAgent* pAgent) {
	cmManager->fAddAgent(pAgent);
}
void TcAgentSystem::fSetName(string pSystemname)
{
	rmSystemname = pSystemname;
}
void TcAgentSystem::fSetId(string pSystemid)
{
	rmSystemid = pSystemid;
}
void TcAgentSystem::fSetManager(TcAgentManager* pManager)
{
	cmManager = pManager;
}
TcAgentManager* TcAgentSystem::fGetManager()
{
	return(cmManager);
}
string TcAgentSystem::fGetId()
{
	return(rmSystemid);
}
string TcAgentSystem::fGetName()
{
	return(rmSystemname);
}
void TcAgentSystem::fStartManager(thread* pManagerThread) {
	*pManagerThread = thread(&TcAgentSystem::fExecuteManager, this);
	return;
}
void TcAgentSystem::fWaitManager(thread* pManagerThread) {
	try{

		if(pManagerThread == nullptr){
			fprintf(stdout, "(%s) Manager-Thread pointer is null\n", __func__);
			fflush(stdout);
		}

		if(!pManagerThread->joinable()){
			fprintf(stdout, "(%s) Manager-Thread is not joinable\n", __func__);
			fflush(stdout);
		}

		pManagerThread->join();
	}
	catch(system_error se){
		fprintf(stdout, "(%s) %s, error_code %d\n", __func__, se.what(), se.code().value());
		fflush(stdout);
	}

	return;
}

int main()
{
	string rMongoDBConnectionType = "mongodb";
	string rMongoDBConnectionHost = "127.0.0.1";
	uint16_t rMongoDBConnectionPort = 27017;
	
	string rMongoDBDatabasename = "InfoDB";
	string rMongoDBCollectionname = "TRLOW_rf";

	TcAgentSystem* system = new TcAgentSystem("S - 0", "System - 0");
	system->fLoadManager("AM0", "Agent Manager", chrono::microseconds(10000), chrono::microseconds(50000000000000));
	
	for (int i = 1; i < 120; i++)
	{
		system->fLoadAgent(new TcErrorDegradationTimeEstimator(4, i, "MAE", 40, 5, 3000, 4, 
			chrono::duration_cast<chrono::milliseconds>(chrono::hours(1)), rMongoDBDatabasename, 
			rMongoDBCollectionname, rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
			string("MSE-DTE"), string("MSE-Degradation-Time-Estimator"), chrono::microseconds(10000)));
	}
	/*
	
	system->fLoadAgent(new TcErrorDegradationTimeEstimator(10, 1, "MAE", 5000, 30, 3000, 5, chrono::duration_cast<chrono::milliseconds>(chrono::hours(1)), rMongoDBDatabasename, rMongoDBCollectionname, rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
		string("MAE-DTE"), string("MAE-Degradation-Time-Estimator"), chrono::microseconds(1000000)));

	system->fLoadAgent(new TcErrorDegradationTimeEstimator(10, 1, "RMSE", 5000, 30, 3000, 5, chrono::duration_cast<chrono::milliseconds>(chrono::hours(1)), rMongoDBDatabasename, rMongoDBCollectionname, rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
		string("RMSE-DTE"), string("RMSE-Degradation-Time-Estimator"), chrono::microseconds(1000000)));
	*/

	try{
		thread cManagerThread;
		system->fStartManager(&cManagerThread);
		system->fWaitManager(&cManagerThread);
		system->fStopManager();
	} catch(std::exception e){
		printf("%s", e.what());
	}
	
	
	return(0);

}
