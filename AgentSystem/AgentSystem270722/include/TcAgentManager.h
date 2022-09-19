#pragma once
#ifndef TCAGENTMANAGER_H
#define TCAGENTMANAGER_H

#include <map>
#include <string>
#include <list>
#include <queue>
#include <boost/circular_buffer.hpp>
#include <boost/circular_buffer_fwd.hpp>

#include "Agent/IAgent.h"
#include "Agent/TcAgentStatus.h"


namespace Spea {
	namespace Archimede {
		namespace MultiAgentSystem {

			using namespace Spea::Archimede::MultiAgentSystem::Agent;

			class TcAgentManager {

			private:

				string rmManagername;
				string rmManagerid;
				list<IAgent*>* cmAgents;
				map<IAgent*, boost::circular_buffer<TcAgentStatus*>* >* cmAgentshistory;
				chrono::microseconds cmScheduleminwaittime;
				chrono::microseconds cmExecutionwaittime;
				atomic<bool> cmStopped;

			public:

				TcAgentManager();
				TcAgentManager(string pManagerid, string pManagername, chrono::microseconds pSchedulewaittime, chrono::microseconds pExecutionwaittime);
				~TcAgentManager();


				void fAddAgent(IAgent* pAgent);

				chrono::microseconds fSchedule(priority_queue<IAgent*>* pReadyagents);
				void fRun(priority_queue<IAgent*>* pReadyagents);

				void fExecute();

				void fSetExecutionWaitTime(chrono::microseconds pExecutionwaittime);
				void fSetScheduleMinWaitTime(chrono::microseconds pScheduleminwaittime);
				void fSetName(string pManagername);
				void fSetStopped(bool pStopped);
				void fSetId(string agentid);


				string fGetId();
				bool fGetStopped();
				string fGetName();
				chrono::microseconds fGetScheduleMinWaitTime();
				chrono::microseconds fGetExecutionWaitTime();
			};
		}
	}
}
#endif // AGENTMANAGER_H
