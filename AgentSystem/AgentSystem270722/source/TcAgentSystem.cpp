#include <string>
#include <chrono>
#include "../include/TcAgentSystem.h"
#include "../include/Agent/TcAgent.h"
#include "../include/Agent/ThresholdsAgent/TcThresholdsAgent.h"
#include "../include/Agent/ThresholdsAgent/TcThresholdsComputeAgent.h"
#include "../include/Agent/MeasurementsDeviationDetectorAgent/TcMeasurementsDeviationDetectorAgent.h"
#include "../include/Agent/MeasureInjectorAgent/TcMeasureInjectorAgent.h"

using namespace Spea::Archimede::MultiAgentSystem;

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
	pManagerThread = new thread(&TcAgentSystem::fExecuteManager, this);
	return;
}
void TcAgentSystem::fWaitManager(thread* pManagerThread) {
	pManagerThread->join();
	return;
}

/*
* 
* 
* Generic Configuration File (GCF) which contains informations valid for each Agent like Database, Notifier, Equipment Serial Number etc
* 
* [Notifier]
* IP=
* PORT=
* 
* [Database]
* CONNECTIONTYPE=
* IP=
* PORT=
* USERNAME=
* PASSWORD=
* 
*
*/

int main()
{
	string rMongoDBConnectionType = "mongodb";
	string rMongoDBConnectionHost = "its1mongodb";
	uint16_t rMongoDBConnectionPort = 27017;
	string rMongoDBDatabasename = "SpeaTestDB2";
	string rMongoDBCollectionname = "Measures";

	string rNotifierHost = "172.18.255.10";
	uint16_t rNotifierPort = 56124;

	string rEquipmentSerialNumber = "FDS014AA";
	string rReferenceType = "Sensor";

	string rPositions[] = { "Internal Electrical Top Box", "External Environment", "Chassis", "Axis 4", "Axis 3", "Axis 2", "Axis 1", "Front Fixed Clamp", "Axis 5", "Axis 8", "Rear Mobile Clamp", "Main Supply" };
	string rReferenceIDs[] = { "60f058626c58e0f392fad300", "60f058626c58e0f392fad301", "60f058626c58e0f392fad302", "60f058626c58e0f392fad303", "60f058626c58e0f392fad304" , "60f058626c58e0f392fad305" , "60f058626c58e0f392fad306" , "60f058626c58e0f392fad307" , "60f058626c58e0f392fad308" , "60f058626c58e0f392fad309", "60f058626c58e0f392fad310", "60ec18a50003e70037000003"};

	string rAgentIDs[] = { "TTA", "THA", "TPA", "TAZA", "TCL1A", "TCL2A", "TCL3A", "TLVL1L2A", "TLVL1L3A", "TLVL2L3A", "TAPL1A", "TAPL2A", "TAPL3A", "TAPTOTA" };
	string rAgentNames[] = { "Threshold Temperature Agent", "Threshold Humidity Agent", "Threshold Pressure Agent", "Threshold Acceleration Z Agent","Threshold Current L1 Agent", "Threshold Current L2 Agent", "Threshold Current L3 Agent", "Threshold Line voltage L1-L2 Agent", "Threshold Line voltage L1-L3 Agent", "Threshold Line voltage L2-L3 Agent", "Threshold Active power L1 Agent", "Threshold Active power L2 Agent", "Threshold Active power L3 Agent", "Threshold Active power Total Agent"};
	string rMeasureTypes[] = { "Temperature", "Humidity", "Pressure", "Acceleration_z", "Current L1", "Current L2", "Current L3", "Line voltage L1-L2", "Line voltage L1-L3", "Line voltage L2-L3", "Active power L1", "Active power L2", "Active power L3", "Active power Total" };
	string rMeasureUnits[] = { "C", "%", "mBar", "degree", "A", "A", "A", "V", "V", "V", "W", "W", "W", "W" };

	map<string, vector<float>> cThreasholdMap;
	cThreasholdMap.insert(make_pair<string, vector<float>>("Temperature", {30.0, 20.0, 32.0, 18.0}));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Humidity", { 60.0, 20.0, 80.0, 0.0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Pressure", { 1400.0, -300.0, 1600.0, -400.0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Acceleration_z", { 0.99, -0.7, 1, -1 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Current L1", { 10.5, 0.5, 11, 0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Current L2", { 8.0, 0.5, 11, 0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Current L3", { 10.5, 0.5, 11, 0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Line voltage L1-L2", { 490, 200, 500, 180 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Line voltage L1-L3", { 490, 200, 500, 180 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Line voltage L2-L3", { 490, 200, 500, 180 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Active power L1", { 690, 10, 700, 0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Active power L2", { 900, 10, 1000, 0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Active power L3", { 690, 10, 700, 0 }));
	cThreasholdMap.insert(make_pair<string, vector<float>>("Active power Total", { 900, 10, 1000, 0 }));


	TcAgentSystem* system = new TcAgentSystem("S - 0", "System - 0");
	system->fLoadManager("AM0", "Agent Manager", chrono::microseconds(1000000), chrono::microseconds(10000000000));

	/*for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 3;  j++) {
			system->fLoadAgent(new TcThresholdsAgent(0, rPositions[i], rMeasureUnits[j], rMeasureTypes[j], rReferenceIDs[i], rReferenceType, cThreasholdMap.at(rMeasureTypes[j])[0], cThreasholdMap.at(rMeasureTypes[j])[1], cThreasholdMap.at(rMeasureTypes[j])[2], cThreasholdMap.at(rMeasureTypes[j])[3], "4080", rEquipmentSerialNumber,
				rMongoDBDatabasename, rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
				rNotifierHost, rNotifierPort,
				rAgentIDs[j], rAgentNames[j], chrono::microseconds(5000000))
			);
		}
	}*/

	/*system->fLoadAgent(new TcThresholdsComputeAgent(0, rPositions[7], rMeasureUnits[3], rMeasureTypes[3], rReferenceIDs[7], rReferenceType, cThreasholdMap.at(rMeasureTypes[3])[0], cThreasholdMap.at(rMeasureTypes[3])[1], cThreasholdMap.at(rMeasureTypes[3])[2], cThreasholdMap.at(rMeasureTypes[3])[3], "4080", rEquipmentSerialNumber,
		rMongoDBDatabasename, rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
		rNotifierHost, rNotifierPort,
		rAgentIDs[3], rAgentNames[3], chrono::microseconds(5000000))
	);

	system->fLoadAgent(new TcThresholdsComputeAgent(0, rPositions[10], rMeasureUnits[3], rMeasureTypes[3], rReferenceIDs[10], rReferenceType, cThreasholdMap.at(rMeasureTypes[3])[0], cThreasholdMap.at(rMeasureTypes[3])[1], cThreasholdMap.at(rMeasureTypes[3])[2], cThreasholdMap.at(rMeasureTypes[3])[3], "4080", rEquipmentSerialNumber, rMongoDBDatabasename,
		rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
		rNotifierHost, rNotifierPort,
		rAgentIDs[3], rAgentNames[3], chrono::microseconds(5000000))
	);*/


	/*for (int j = 4; j < 13; j++) {
		system->fLoadAgent(new TcThresholdsAgent(0, rPositions[11], rMeasureUnits[j], rMeasureTypes[j], rReferenceIDs[11], rReferenceType, cThreasholdMap.at(rMeasureTypes[j])[0], cThreasholdMap.at(rMeasureTypes[j])[1], cThreasholdMap.at(rMeasureTypes[j])[2], cThreasholdMap.at(rMeasureTypes[j])[3], "4080", rEquipmentSerialNumber,
			rMongoDBDatabasename, rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
			rNotifierHost, rNotifierPort,
			rAgentIDs[j], rAgentNames[j], chrono::microseconds(5000000))
		);
	}*/
	


	/*system->fLoadAgent(new TcAgent(rEquipmentSerialNumber, "4080", rMongoDBDatabasename,
		rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
		rNotifierHost, rNotifierPort,
		"AP0", "Agente prova", chrono::microseconds(5000000))
	);*/

	/*system->fLoadAgent(new TcMeasurementsDeviationDetectorAgent("agrdt801", "", rEquipmentSerialNumber, "4080", rMongoDBDatabasename, "Measures_Debug",
		rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
		rNotifierHost, rNotifierPort,
		"AP0", "Agente prova", chrono::microseconds(30000000))
	);*/


	system->fLoadAgent(new TcMeasureInjectorAgent("Simulation\\MIA0.ini", "Simulation\\MIA0.csv", "4080", "agrdt801", "", "", rMongoDBDatabasename, "Measures",
		rMongoDBConnectionType, rMongoDBConnectionHost, rMongoDBConnectionPort,
		rNotifierHost, rNotifierPort,
		"MIA0", "MeasureInjector0")
	);
	
	thread cManagerThread;
	system->fStartManager(&cManagerThread);
	system->fWaitManager(&cManagerThread);
	system->fStopManager();

	
	
	return(0);

}
