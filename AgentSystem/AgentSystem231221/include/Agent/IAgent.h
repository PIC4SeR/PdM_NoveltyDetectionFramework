#pragma once

#ifndef IAGENT_H
#define IAGENT_H

#include <optional>
#include <chrono>
#include <future>
#include <list>



enum class Priority { VeryHigh = 5, High = 4, Medium = 3, Low = 2, VeryLow = 1 };


using namespace std;


class IAgent{

protected:

	string rmAgentid;
	string rmAgentname;
	atomic<bool> cmStopped;
	Priority cmRunpriority;

	chrono::time_point<chrono::high_resolution_clock> cmLastruntime;
	chrono::time_point<chrono::high_resolution_clock> cmNextruntime;
	chrono::microseconds cmStepruntime;

public:

	

	IAgent();
	IAgent(string agentid, string agentname, chrono::microseconds stepruntime, Priority priority = Priority::Medium, bool stopped = false);
	IAgent(const IAgent& other);
	IAgent(IAgent&& other);
	~IAgent();


	IAgent& operator=(const IAgent& other);
	IAgent& operator=(IAgent&& other);


	void fSetLastRunTime(chrono::time_point<chrono::high_resolution_clock> lastruntime);
	void fSetNextRunTime(chrono::time_point<chrono::high_resolution_clock> nextruntime);
	void fSetStepRunTime(chrono::microseconds stepruntime);
	void fSetName(string agentname);
	void fSetStopped(bool stopped);
	void fSetRunPriority(Priority runpriority);
	void fSetId(string agentid);

	string fGetId();
	Priority fGetRunPriority();
	bool fGetStopped();
	string fGetName();
	chrono::time_point<chrono::high_resolution_clock> fGetLastRunTime();
	chrono::time_point<chrono::high_resolution_clock> fGetNextRunTime();
	chrono::microseconds fGetStepRunTime();
	
	bool fRunnable();
	bool fReady();
	virtual int fRun();
	chrono::microseconds fWaitFor();

	void operator()(promise<optional<int>>& promise);

	friend bool operator<(const IAgent& Lagent, const IAgent& Ragent);
	friend bool operator==(const IAgent& Lagent, const IAgent& Ragent);
	friend bool operator>(const IAgent& Lagent, const IAgent& Ragent);
	friend bool operator<=(const IAgent& Lagent, const IAgent& Ragent);
	friend bool operator>=(const IAgent& Lagent, const IAgent& Ragent);

};



#endif // IAGENT_H

