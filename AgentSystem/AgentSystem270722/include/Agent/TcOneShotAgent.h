#pragma once
#ifndef TCONESHOTAGENT_H
#define TCONESHOTAGENT_H





//  global compilation flag configuring windows sdk headers
//  preventing inclusion of min and max macros clashing with <limits>
#define NOMINMAX 1
//  override byte to prevent clashes with <cstddef>
#define byte win_byte_override

#include <Windows.h> // gdi plus requires Windows.h
// ...includes for other windows header that may use byte...

//  Define min max macros required by GDI+ headers.
#ifndef max
	#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
	#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


#include <codecvt>

//  Undefine min max macros so they won't collide with <limits> header content.
#undef min
#undef max
//  Undefine byte macros so it won't collide with <cstddef> header content.
#undef byte

#include "TcAgent.h"



#define FILEPATH_BUFFER_LENGTH 2048



namespace Spea {
	namespace Archimede {
		namespace MultiAgentSystem {
			namespace Agent {

				using namespace std;
				using namespace Spea::Archimede::MultiAgentSystem::Agent;

				class TcOneShotAgent : public TcAgent {
				private:

					bool fExistOneShotFile();
					bool fDeleteOneShotFile();

				public:

					TcOneShotAgent();
					TcOneShotAgent(string pMachineFamily, string pMachineSerialNumber, string pReferenceID, string pReferenceType, string pCollectionName, string pDatabaseName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, Priority pPriority = Priority::Medium);
					~TcOneShotAgent();

					static string fGetCurrentDirectory();


					virtual int fRunOneShot();
					virtual int fRun();


				};
			}
		}
	}
}
#endif