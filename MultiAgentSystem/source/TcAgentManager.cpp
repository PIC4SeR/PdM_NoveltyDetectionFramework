#include "../include/TcAgentManager.h"

#include <thread>
#include <future>
#include <string>
#include <algorithm>
#include <optional>

#define MINWAIT(a,b) (a.count() < b.count() ? a : b)

using namespace std;

TcAgentManager::TcAgentManager(){
	
	this->cmAgents = new list<IAgent*>();
	this->cmAgentshistory = new map<IAgent*, boost::circular_buffer<TcAgentStatus*>*>();
	this->cmScheduleminwaittime = chrono::microseconds(1000000);
	this->cmExecutionwaittime = chrono::microseconds(1000000);
	this->cmStopped.store(false);
}
TcAgentManager::TcAgentManager(string managerid, string managername, chrono::microseconds schedulewaittime, chrono::microseconds executionwaittime)
{
	this->rmManagerid = managerid;
	this->rmManagername = managername;
	this->cmAgents = new list<IAgent*>();
	this->cmAgentshistory = new map<IAgent*, boost::circular_buffer<TcAgentStatus*>*>();
	this->cmScheduleminwaittime = schedulewaittime;
	this->cmStopped.store(false);
}
TcAgentManager::~TcAgentManager(){
	
	if (this->cmAgents != nullptr) {
		this->cmAgents->clear();
		delete cmAgents;
	}

	if (this->cmAgentshistory != nullptr) {
		this->cmAgentshistory->clear();
		delete cmAgentshistory;
	}
}

void TcAgentManager::fSetExecutionWaitTime(chrono::microseconds executionwaittime)
{
	this->cmExecutionwaittime = executionwaittime;
}
void TcAgentManager::fSetScheduleMinWaitTime(chrono::microseconds scheduleminwaittime)
{
	this->cmScheduleminwaittime = scheduleminwaittime;
}
void TcAgentManager::fSetName(string managername)
{
	this->rmManagername = managername;
}
void TcAgentManager::fSetStopped(bool stopped)
{
	this->cmStopped.store(stopped);
}
void TcAgentManager::fSetId(string managerid)
{
	this->rmManagerid = managerid;
}

string TcAgentManager::fGetId()
{
	return(this->rmManagerid);
}
bool TcAgentManager::fGetStopped()
{
	return(this->cmStopped.load());
}
string TcAgentManager::fGetName()
{
	return(this->rmManagername);
}
chrono::microseconds TcAgentManager::fGetScheduleMinWaitTime()
{
	return(this->cmScheduleminwaittime);
}
chrono::microseconds TcAgentManager::fGetExecutionWaitTime()
{
	return(this->cmExecutionwaittime);
}

chrono::microseconds TcAgentManager::fSchedule(priority_queue<IAgent*>* pReadyagents) {
	chrono::microseconds schedulewait = chrono::microseconds(cmScheduleminwaittime);

	fprintf(stdout, "(%s) Start scheduling\n", __func__);
	fflush(stdout);

	for (IAgent* agent : *cmAgents) {
		if (agent->fRunnable()) {
			if (agent->fReady()) {
				schedulewait = chrono::microseconds(0);
				pReadyagents->push(agent);
			} else {
				schedulewait = MINWAIT(schedulewait, agent->fWaitFor());
			}
		}
	}

	fprintf(stdout, "(%s) End scheduling - next programmed scheduling in next %d sec\n", __func__, (int) chrono::duration_cast<chrono::seconds>(schedulewait).count());
	fflush(stdout);
	
	return(schedulewait);
}
void TcAgentManager::fRun(priority_queue<IAgent*>* pReadyagents) {

	fprintf(stdout, "(%s) Starts Run\n", __func__);
	fflush(stdout);

	while (!pReadyagents->empty()) {

		//chrono::time_point<chrono::high_resolution_clock> 
		auto start = chrono::high_resolution_clock::now();
		bool agentstatus = false;
		optional<int> result = 0;
		promise<optional<int>> p;
		chrono::microseconds agentWait = chrono::microseconds(cmExecutionwaittime);

		IAgent* agent = pReadyagents->top();


		fprintf(stdout, "============================================================================================\n");
		fprintf(stdout, "(%s) Agent with AgentId %s and AgentName %s is starting\n", __func__, agent->fGetId().c_str(), agent->fGetName().c_str());
		fflush(stdout);

		thread agentThread = thread(ref(*agent), ref(p));
		agentThread.detach();
		
		future<optional<int>> f = p.get_future();

		future_status status = f.wait_for(agentWait);
		//chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
		auto end = chrono::high_resolution_clock::now();
		double elapsed_time = chrono::duration<double, std::milli>(end - start).count();

		if (status == future_status::ready) {
			result = f.get();
			agentstatus = true;
			//fprintf(stdout, "(%s) Ready - Agent ends its task normally in %d ms\n", __func__, (int) chrono::duration_cast<chrono::milliseconds>(end - start).count());
			fprintf(stdout, "(%s) Ready - Agent ends its task normally in %f ms\n", __func__, (double) elapsed_time);
			fflush(stdout);
		} else if (status == future_status::timeout) {
			fprintf(stdout, "(%s) Timeout - Agent doesn't end its task normally in %d ms\n", __func__, (int) chrono::duration_cast<chrono::milliseconds>(end - start).count());
			fflush(stdout);
			agentstatus = false;
		}

		if(this->cmAgentshistory->find(agent) == this->cmAgentshistory->end()) {
			boost::circular_buffer<TcAgentStatus*>* agentStatusList = new boost::circular_buffer<TcAgentStatus*>();
			agentStatusList->push_back(new TcAgentStatus(agentstatus, chrono::duration_cast<chrono::microseconds>(end - start), start));
			this->cmAgentshistory->insert(make_pair(agent, agentStatusList));
		} else {
			this->cmAgentshistory->at(agent)->push_back(new TcAgentStatus(agentstatus, chrono::duration_cast<chrono::microseconds>(end - start), start));
		}

		pReadyagents->pop();
		

		fprintf(stdout, "(%s) Agent with AgentId %s and AgentName %s is ending\n", __func__, agent->fGetId().c_str(), agent->fGetName().c_str());
		fprintf(stdout, "============================================================================================\n");
		fflush(stdout);
						
	}

	fprintf(stdout, "(%s) Ends Running\n", __func__);
	fflush(stdout);

}
void TcAgentManager::fExecute() {
	while (!cmStopped.load()) {
		priority_queue<IAgent*> cReadyagents;
		chrono::microseconds schedulewait = fSchedule(&cReadyagents);
		fRun(&cReadyagents);
		this_thread::sleep_for(schedulewait);
	}
}
void TcAgentManager::fAddAgent(IAgent* agent) {
	this->cmAgents->push_back(agent);
    this->cmAgentshistory->insert(make_pair(agent, new boost::circular_buffer<TcAgentStatus*>(1024)));
}
