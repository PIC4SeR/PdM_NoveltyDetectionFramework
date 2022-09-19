#include <math.h>
#include <iostream>
#include <stdio.h>

#include <algorithm>

#include "../../../include/Agent/TcOneShotAgent.h"

#define FILEPATH_BUFFER_LENGTH 2048

TcOneShotAgent::TcOneShotAgent(string pMachineFamily, string pMachineSerialNumber, string pReferenceID, string pReferenceType, string pCollectionName, string pDatabaseName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, Priority pPriority, bool pStopped) : TcAgent(pCollectionname, pDatabasename, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pPriority, pStopped) { ; }
TcOneShotAgent::~TcOneShotAgent(){ ; }


string fGetCurrentDirectory() {
	try {
		//Get Current Directory String			
		TCHAR rBuffer[FILEPATH_BUFFER_LENGTH] = { 0 };
		GetModuleFileName(NULL, rBuffer, FILEPATH_BUFFER_LENGTH);
		wstring::size_type pos = wstring(rBuffer).find_last_of(L"\\/");
		wstring rCurrentPath = wstring(rBuffer).substr(0, pos);

		//setup converter
	    using convert_type = codecvt_utf8<wchar_t>;
		wstring_convert<convert_type, wchar_t> cConverter;

	    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		string rSCurrentPath = cConverter.to_bytes(rCurrentPath);
		return(rSCurrentPath);
	} catch (exception e) {
		fprintf(stdout,"[fGetCurrentDirectory] Get current working directory fails with exception %s\n", e.what());
		fflush(stdout);
        return(nullptr);
	}
}

bool TcOneShotAgent::fExistOneShotFile() { 
    
	char rOneShotFilePath[FILEPATH_BUFFER_LENGTH];
	struct stat rStat_Buffer;
    snprintf(rOneShotFilePath, (size_t)FILEPATH_BUFFER_LENGTH, "%s/%s_OneShot.ini", fGetCurrentDirectory(), this->rmAgentID);

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
    snprintf(rOneShotFilePath, (size_t)FILEPATH_BUFFER_LENGTH, "%s/%s_OneShot.ini", fGetCurrentDirectory(), this->rmAgentID);

	fprintf(stdout, "[fDeleteOneShotFile] Delete file %s\n", rOneShotFilePath);
    fflush(stdout);
	
    if (fExistOneShotFile()) {
        if (remove(rOneShotFilePath)) {
            fprintf(stdout, ANSI_COLOR_RED "One Shot File %s"  ANSI_COLOR_RESET "not found\n", rOneShotFilePath);
		    fflush(stdout);
            return(false);
        } else {
            return(true);
	    }
    } else {
        return(true);
	}
 }
void TcOneShotAgent::fRunOneShot() { ; }



int TcOneShotAgent::fRun() {
	int rResult = 0;

	if (fExistOneShotFile()) {
		fprintf(stdout, ANSI_COLOR_CYAN "One Shot %s"  ANSI_COLOR_RESET "\n", this->rmAgentID);
		fflush(stdout);
		fRunOneShot();
        fDeleteOneShotFile();
        return(kRunSuccess);
	} else {
        fprintf(stdout, ANSI_COLOR_CYAN "One Shot %s doesn't run"  ANSI_COLOR_RESET "\n", this->rmAgentID);
		fflush(stdout);
		return(kNoRun);
    }

}










