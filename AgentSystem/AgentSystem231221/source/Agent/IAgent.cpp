

#include <chrono>
#include <string>

#include "../../include/Agent/IAgent.h"



IAgent::IAgent() {
	rmAgentid = "";
	rmAgentname = "";
	cmRunpriority = Priority::Medium;
	cmStopped = true;
	cmStepruntime = chrono::microseconds(60000000);
	cmLastruntime = chrono::high_resolution_clock::now();
	cmNextruntime = cmLastruntime + cmStepruntime;
}
IAgent::IAgent(string agentid, string agentname, chrono::microseconds stepruntime, Priority priority, bool stopped){
	rmAgentid = agentid;
	rmAgentname = agentname;
	cmRunpriority = priority;
	cmStopped = stopped;
	cmStepruntime = stepruntime;
	cmLastruntime = chrono::high_resolution_clock::now();
	cmNextruntime = cmLastruntime + cmStepruntime;
}
IAgent::IAgent(const IAgent& other) {
	rmAgentid = other.rmAgentid;
	rmAgentname = other.rmAgentname;
	cmRunpriority = other.cmRunpriority;
	cmStopped.store(other.cmStopped.load());
	cmStepruntime = other.cmStepruntime;
	cmLastruntime = other.cmLastruntime;
	cmNextruntime = other.cmNextruntime;
}
IAgent::IAgent(IAgent&& other) {
	rmAgentid = other.rmAgentid;
    rmAgentname = other.rmAgentname;
	cmRunpriority = other.cmRunpriority;
	cmStopped.store(other.cmStopped.load());
	cmStepruntime = other.cmStepruntime;
	cmLastruntime = other.cmLastruntime;
	cmNextruntime = other.cmNextruntime;
	other.rmAgentid = "";
	other.rmAgentname = "";
	other.cmRunpriority = Priority::Medium;
	other.cmStopped = false;
	other.cmStepruntime = chrono::microseconds(0);
	other.cmLastruntime = chrono::time_point<chrono::high_resolution_clock>();
	other.cmNextruntime = chrono::time_point<chrono::high_resolution_clock>();
}
IAgent::~IAgent(){}
IAgent& IAgent::operator=(const IAgent& other){
	
	if (this != &other) {
		rmAgentid = other.rmAgentid;
		rmAgentname = other.rmAgentname;
		cmRunpriority = other.cmRunpriority;
		cmStopped.store(other.cmStopped.load());
		cmStepruntime = other.cmStepruntime;
		cmLastruntime = other.cmLastruntime;
		cmNextruntime = other.cmNextruntime;
	}

	return(*this);

}
IAgent& IAgent::operator=(IAgent&& other) {

	if (this != &other) {
		rmAgentid = other.rmAgentid;
		rmAgentname = other.rmAgentname;
		cmRunpriority = other.cmRunpriority;
		cmStopped.store(other.cmStopped.load());
		cmStepruntime = other.cmStepruntime;
		cmLastruntime = other.cmLastruntime;
		cmNextruntime = other.cmNextruntime;
		other.rmAgentid = "";
		other.rmAgentname = "";
		other.cmRunpriority = Priority::Medium;
		other.cmStopped.store(false);
		other.cmStepruntime = chrono::microseconds(0);
		other.cmLastruntime = chrono::time_point<chrono::high_resolution_clock>();
		other.cmNextruntime = chrono::time_point<chrono::high_resolution_clock>();
	}

	return(*this);

}

void IAgent::fSetLastRunTime(chrono::time_point<chrono::high_resolution_clock> last_runtime)
{
	last_runtime = last_runtime;
}

void IAgent::fSetNextRunTime(chrono::time_point<chrono::high_resolution_clock> nextruntime)
{
	cmNextruntime = nextruntime;
}

void IAgent::fSetStepRunTime(chrono::microseconds stepruntime)
{
	cmStepruntime = stepruntime;
}

void IAgent::fSetName(string agentname)
{
	rmAgentname = agentname;
}

void IAgent::fSetStopped(bool stopped)
{
	cmStopped.store(stopped);
}

void IAgent::fSetRunPriority(Priority runpriority)
{
	cmRunpriority = runpriority;
}

void IAgent::fSetId(string agentid)
{
	rmAgentid = agentid;
}

string IAgent::fGetId()
{
	return(rmAgentid);
}

Priority IAgent::fGetRunPriority()
{
	return(cmRunpriority);
}

bool IAgent::fGetStopped()
{
	return(cmStopped.load());
}

string IAgent::fGetName()
{
	return(rmAgentname);
}

chrono::time_point<chrono::high_resolution_clock> IAgent::fGetLastRunTime()
{
	return(cmLastruntime);
}

chrono::microseconds IAgent::fGetStepRunTime()
{
	return(cmStepruntime);
}

chrono::time_point<chrono::high_resolution_clock> IAgent::fGetNextRunTime()
{
	return(cmNextruntime);
}

chrono::microseconds IAgent::fWaitFor(){
	chrono::microseconds waitfor = chrono::duration_cast<chrono::microseconds>(cmNextruntime - chrono::high_resolution_clock::now());
	return(waitfor.count() > 0 ? waitfor : chrono::microseconds(0));
}

bool IAgent::fRunnable() {
	return(!cmStopped);
}

bool IAgent::fReady() {
	return(cmNextruntime < chrono::high_resolution_clock::now());
}

int IAgent::fRun(){
	return(0);
}

void IAgent::operator()(promise<optional<int>>& promise) {
	try {

		chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();

		int result = fRun();
		promise.set_value_at_thread_exit(result);

		chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
		this->cmLastruntime = end;
		this->cmNextruntime = end + this->cmStepruntime;
	
	}
	catch (exception e) {
		promise.set_exception_at_thread_exit(exception_ptr());
	}
}


bool operator<(const IAgent& Lagent, const IAgent& Ragent) {
	return((Lagent.cmRunpriority > Ragent.cmRunpriority ? true : Lagent.cmNextruntime < Ragent.cmNextruntime));
}

bool operator==(const IAgent& Lagent, const IAgent& Ragent) {
	return(Lagent.cmRunpriority == Ragent.cmRunpriority && Lagent.cmNextruntime == Ragent.cmNextruntime);
}

bool operator>(const IAgent& Lagent, const IAgent& Ragent) { 
	return(!(Lagent == Ragent) && !(Lagent < Ragent)); 
}

bool operator<=(const IAgent& Lagent, const IAgent& Ragent) { 
	return(!(Lagent > Ragent)); 
}

bool operator>=(const IAgent& Lagent, const IAgent& Ragent) { 
	return(!(Lagent < Ragent)); 
}

bool operator!=(const IAgent& Lagent, const IAgent& Ragent) { 
	return(Lagent < Ragent || Lagent > Ragent); 
}