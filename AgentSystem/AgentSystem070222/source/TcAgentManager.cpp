#include "../include/TcAgentManager.h"

#include <thread>
#include <future>
#include <string>
#include <algorithm>
#include <optional>


#define MINWAIT(a,b) (a.count() < b.count() ? a : b)

TcAgentManager::TcAgentManager(){
	
	cmAgents = new list<IAgent*>();
	cmAgentshistory = new map<IAgent*, boost::circular_buffer<TcAgentStatus*>*>();
	cmScheduleminwaittime = chrono::microseconds(1000000);
	cmExecutionwaittime = chrono::microseconds(1000000);
	cmStopped.store(false);
}
TcAgentManager::TcAgentManager(string managerid, string managername, chrono::microseconds schedulewaittime, chrono::microseconds executionwaittime)
{
	rmManagerid = managerid;
	rmManagername = managername;
	cmAgents = new list<IAgent*>();
	cmAgentshistory = new map<IAgent*, boost::circular_buffer<TcAgentStatus*>*>();
	cmScheduleminwaittime = schedulewaittime;
	cmStopped.store(false);
}
TcAgentManager::~TcAgentManager(){


	if (cmAgents != nullptr) {
		cmAgents->clear();
		delete cmAgents;
	}
	if (cmAgentshistory != nullptr) {
		cmAgentshistory->clear();
		delete cmAgentshistory;
	}
}

void TcAgentManager::fSetExecutionWaitTime(chrono::microseconds executionwaittime)
{
	cmExecutionwaittime = executionwaittime;
}
void TcAgentManager::fSetScheduleMinWaitTime(chrono::microseconds scheduleminwaittime)
{
	cmScheduleminwaittime = scheduleminwaittime;
}
void TcAgentManager::fSetName(string managername)
{
	rmManagername = managername;
}
void TcAgentManager::fSetStopped(bool stopped)
{
	cmStopped.store(stopped);
}
void TcAgentManager::fSetId(string managerid)
{
	rmManagerid = managerid;
}

string TcAgentManager::fGetId()
{
	return(rmManagerid);
}
bool TcAgentManager::fGetStopped()
{
	return(cmStopped.load());
}
string TcAgentManager::fGetName()
{
	return(rmManagername);
}
chrono::microseconds TcAgentManager::fGetScheduleMinWaitTime()
{
	return(cmScheduleminwaittime);
}
chrono::microseconds TcAgentManager::fGetExecutionWaitTime()
{
	return(cmExecutionwaittime);
}

chrono::microseconds TcAgentManager::fSchedule(priority_queue<IAgent*>* pReadyagents) {
	chrono::microseconds schedulewait = chrono::microseconds(cmScheduleminwaittime);

	fprintf(stdout, "Start scheduling\n");
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

	/*fprintf(stdout, "End scheduling - next programmed scheduling in next %lld sec\n", chrono::duration_cast<chrono::seconds>(schedulewait).count());
	fflush(stdout);*/
	
	return(schedulewait);
}
void TcAgentManager::fRun(priority_queue<IAgent*>* pReadyagents) {

	if (!pReadyagents->empty()) {
		fprintf(stdout, "Start running all ready Agents\n");
		fflush(stdout);
	}

	while (!pReadyagents->empty()) {

		chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
		bool agentstatus = false;
		optional<int> result = 0;
		promise<optional<int>> p;
		chrono::microseconds agentWait = chrono::microseconds(cmExecutionwaittime);

		IAgent* agent = pReadyagents->top();


		fprintf(stdout, "============================================================================================\n");
		fprintf(stdout, "Agent with AgentId %s and AgentName %s is starting\n", agent->fGetId().c_str(), agent->fGetName().c_str());
		fflush(stdout);

		thread* agentThread = new thread(ref(*agent), ref(p));
		future<optional<int>> f = p.get_future();

		future_status status = f.wait_for(agentWait);
		chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();

		if (status == future_status::ready) {
			result = f.get();
			agentstatus = true;
			fprintf(stdout, "Ready - Agent ends its task normally in %lld ms\n", chrono::duration_cast<chrono::milliseconds>(end - start).count());
			fflush(stdout);
		} else if (status == future_status::timeout) {
			fprintf(stdout, "Timeout - Agent doesn't end its task normally in %lld ms\n", chrono::duration_cast<chrono::milliseconds>(end - start).count());
			fflush(stdout);
			agentstatus = false;
		}

		

		if(cmAgentshistory->find(agent) == cmAgentshistory->end()) {
			boost::circular_buffer<TcAgentStatus*>* agentStatusList = new boost::circular_buffer<TcAgentStatus*>();
			agentStatusList->push_back(new TcAgentStatus(agentstatus, chrono::duration_cast<chrono::microseconds>(end - start), start));
			cmAgentshistory->insert(make_pair(agent, agentStatusList));
		} else {
			cmAgentshistory->at(agent)->push_back(new TcAgentStatus(agentstatus, chrono::duration_cast<chrono::microseconds>(end - start), start));
		}

		pReadyagents->pop();
		

		fprintf(stdout, "Agent with AgentId %s and AgentName %s is ending\n", agent->fGetId().c_str(), agent->fGetName().c_str());
		fprintf(stdout, "============================================================================================\n");
		fflush(stdout);

	}

	if (!pReadyagents->empty()) {
		fprintf(stdout, "Ends running of all Agents\n");
		fflush(stdout);
	}

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
	cmAgents->push_back(agent);
    cmAgentshistory->insert(make_pair(agent, new boost::circular_buffer<TcAgentStatus*>(1024)));
}
