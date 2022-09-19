// AgentSystem.h : Include file for standard system include files,
// or project specific include files.
#pragma once

#ifndef TCAGENTSYSTEM_H
#define TCAGENTSYSTEM_H



#include <string>
#include <AgentSystem/include/TcAgentManager.h>
#include <AgentSystem/include/Agent/IAgent.h>




class TcAgentSystem {


private:
	string rmSystemid;
	string rmSystemname;
	TcAgentManager* cmManager;

public:

	TcAgentSystem(string pSystemid, string pSystemname);
	~TcAgentSystem();
	
	void fLoadManager(string oManagerid, string pManagername, chrono::microseconds pScheduleminwaittime, chrono::microseconds pExecutionwaittime);
	void fExecuteManager();
	void fStopManager();
	void fAddAgentToManager(IAgent* pAgent);

	void fSetName(string pSystemname);
	void fSetId(string pSystemid);
	void fSetManager(TcAgentManager* pManager);

	TcAgentManager* fGetManager();
	string fGetId();
	string fGetName();


};


#endif // AGENTSYSTEM_H


// TODO: Reference additional headers your program requires here.
