#include "../../include/Agent/TcOneShotAgent.h"

using namespace std;

#define FILEPATH_BUFFER_LENGTH 2048


using namespace Spea::Archimede::MultiAgentSystem::Agent;

TcOneShotAgent::TcOneShotAgent() { ; }
TcOneShotAgent::TcOneShotAgent(string pMachineFamily, string pMachineSerialNumber, string pReferenceID, string pReferenceType, string pCollectionName, string pDatabaseName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, Priority pPriority) : TcAgent(pMachineSerialNumber, pMachineFamily, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, chrono::microseconds(0), chrono::high_resolution_clock::now(), pPriority, false) { ; }
TcOneShotAgent::~TcOneShotAgent(){ ; }


string TcOneShotAgent::fGetCurrentDirectory() {
	try {
		//Get Current Directory String			
		TCHAR rBuffer[FILEPATH_BUFFER_LENGTH] = { 0 };
		GetModuleFileName(NULL, rBuffer, FILEPATH_BUFFER_LENGTH);

		int rLength = mbstowcs(nullptr, rBuffer, 0);

		// Allocate a temporary string
		wchar_t* aTempString = new wchar_t[rLength + 1];

		// Do the actual conversion
		mbstowcs(aTempString, rBuffer, rLength + 1);

		wstring::size_type pos = wstring(aTempString).find_last_of(L"\\/");
		wstring rCurrentPath = wstring(aTempString).substr(0, pos);

		// Free the temporary string
		delete[] aTempString;

		//setup converter
		wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cConverter;

		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		string rSCurrentPath = cConverter.to_bytes(rCurrentPath);
		return(rSCurrentPath);
	}
	catch (exception e) {
		fprintf(stdout, "[fGetCurrentDirectory] Get current working directory fails with exception %s\n", e.what());
		fflush(stdout);
		return(nullptr);
	}
}

bool TcOneShotAgent::fExistOneShotFile() { 
    
	char rOneShotFilePath[FILEPATH_BUFFER_LENGTH];
	struct stat rStat_Buffer;
    snprintf(rOneShotFilePath, (size_t)FILEPATH_BUFFER_LENGTH, "%s\\%s_OneShot.ini", fGetCurrentDirectory().c_str(), this->rmAgentID.c_str());

	fprintf(stdout, "[fExistOneShotFile] Look for file %s\n", rOneShotFilePath);
    fflush(stdout);
	
    if (!stat(rOneShotFilePath, &rStat_Buffer)) {
        return(true);
    } else {
        return(false);
	}
    
}
bool TcOneShotAgent::fDeleteOneShotFile() { 

    char rOneShotFilePath[FILEPATH_BUFFER_LENGTH];
    snprintf(rOneShotFilePath, (size_t)FILEPATH_BUFFER_LENGTH, "%s/%s_OneShot.ini\n", TcOneShotAgent::fGetCurrentDirectory().c_str(), this->rmAgentID);

	fprintf(stdout, "[fDeleteOneShotFile] Delete file %s\n", rOneShotFilePath);
    fflush(stdout);
	
    if (fExistOneShotFile()) {
        if (remove(rOneShotFilePath)) {
            fprintf(stdout, ANSI_COLOR_RED "One Shot File %s "  ANSI_COLOR_RESET " not found\n", rOneShotFilePath);
		    fflush(stdout);
            return(false);
        } else {
            return(true);
	    }
    } else {
        return(true);
	}
 }
int TcOneShotAgent::fRunOneShot() { 
	return 0;
}



int TcOneShotAgent::fRun() {
	int rResult = 0;

	if (fExistOneShotFile()) {
		fprintf(stdout, ANSI_COLOR_CYAN "One Shot %s"  ANSI_COLOR_RESET "\n", this->rmAgentID.c_str());
		fflush(stdout);
		fRunOneShot();
        fDeleteOneShotFile();
        return(kRunSuccess);
	} else {
        fprintf(stdout, ANSI_COLOR_CYAN "One Shot %s doesn't run"  ANSI_COLOR_RESET "\n", this->rmAgentID.c_str());
		fflush(stdout);
		return(kRunFails);
    }

}










