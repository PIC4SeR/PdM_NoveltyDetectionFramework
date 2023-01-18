#include <string>
#include <chrono>
#include <thread>

#include "../include/TcAgentSystem.h"
#include "../include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h"

using namespace std;

TcAgentSystem::TcAgentSystem(string pSystemid, string pSystemname) {
	this->rmSystemid = pSystemid;
	this->rmSystemname = pSystemname;
	this->cmManager = new TcAgentManager();
}
TcAgentSystem::~TcAgentSystem() {
	if (this->cmManager != nullptr){
		delete this->cmManager;
	}
}
void TcAgentSystem::fExecuteManager() {
	this->cmManager->fExecute();
}
void TcAgentSystem::fStopManager() {
	this->cmManager->fSetStopped(true);
}
void TcAgentSystem::fLoadManager(string pManagerid, string pManagername, chrono::microseconds pScheduleminwaittime, chrono::microseconds pExecutionwaittime) {
	this->cmManager->fSetId(pManagerid);
	this->cmManager->fSetName(pManagername);
	this->cmManager->fSetScheduleMinWaitTime(pScheduleminwaittime);
	this->cmManager->fSetExecutionWaitTime(pExecutionwaittime);
}
void TcAgentSystem::fLoadAgent(IAgent* pAgent) {
	this->cmManager->fAddAgent(pAgent);
}
void TcAgentSystem::fSetName(string pSystemname)
{
	this->rmSystemname = pSystemname;
}

void TcAgentSystem::fSetId(string pSystemid)
{
	this->rmSystemid = pSystemid;
}

void TcAgentSystem::fSetManager(TcAgentManager* pManager)
{
	this->cmManager = pManager;
}

TcAgentManager* TcAgentSystem::fGetManager()
{
	return(this->cmManager);
}


string TcAgentSystem::fGetId()
{
	return(this->rmSystemid);
}

string TcAgentSystem::fGetName()
{
	return(this->rmSystemname);
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
	string rMongoDBConnectionHost = "localhost";
	uint16_t rMongoDBConnectionPort = 27017;

	TcAgentSystem* system = new TcAgentSystem("S - 0", "System - 0");
	system->fLoadManager("AM0", "Agent Manager", chrono::microseconds(10000), chrono::microseconds(50000000000000));
	
	for (int i = 1; i < 7; i++)
	{
		system->fLoadAgent(new TcErrorDegradationTimeEstimator(false, false, "../../conf.json", "InfoDB", "Configuration", rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort, string("AG") + to_string(i), 15, i, "MAE", 150, 15, 3000, 15, 
			chrono::duration_cast<chrono::milliseconds>(chrono::hours(1)),  string("TestResult"), string("Prediction"), string("MAE-Degradation-Time-Estimator") + to_string(i), chrono::microseconds(1000000), chrono::system_clock::now(), TcAgent::Priority::High, false));
	}

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
