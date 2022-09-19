#include <stdio.h>
#include <algorithm>
#include <istream>
#include <vector>
#include <sstream>
#include <chrono>

#include <windows.h>

#include "../../../include/Agent/MeasureInjectorAgent/TcMeasureInjectorAgent.h"

using namespace std;

TcMeasureInjectorAgent::TcMeasureInjectorAgent(string pSimulationConfigurationFile, string pSimulationDataFile, string pMachineFamily, string pMachineSerialNumber, string pReferenceID, string pReferenceType, string pCollectionName, string pDatabaseName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, Priority pPriority, bool pStopped) : TcAgent(pMachineFamily, pMachineSerialNumber, pReferenceID, pReferenceType, pCollectionName, pDatabaseName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pStepRunTime, pPriority, pStopped) {
	this->rmSimulationConfigurationFile = pSimulationConfigurationFile;
	this->rmSimulationDataFile = pSimulationDataFile;

}
TcMeasureInjectorAgent::~TcMeasureInjectorAgent(){ ; }


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

bool TcMeasureInjectorAgent::fExistFile(string pFile) { 
    
	char rFilePath[FILEPATH_BUFFER_LENGTH];
	struct stat rStat_Buffer;
    snprintf(rFilePath, (size_t)FILEPATH_BUFFER_LENGTH, "%s/Simulation/%s", fGetCurrentDirectory(), pFile);

	fprintf(stdout, "[fExistFile] Look for file %s\n", rFilePath);
    fflush(stdout);
	
    if (!stat(rFilePath, &rStat_Buffer)) {
        return(true);
    } else {
        return(false);
	}
    
}

int TcMeasureInjectorAgent::fRunOneShot() {
	TcSimulation cSimulation;
	vector<TcMeasure> cMeasures;

	int rResult = fReadSimulationConfiguration(&cSimulation);
	if(rResult < 0){
		fprintf(stdout, "[fRunOneShot] " ANSI_COLOR_RED "Fail to read file %s" ANSI_COLOR_RESET "\n", this->rmSimulationConfigurationFile.c_str());
    	fflush(stdout);
		return(-1);
	}


	rResult = fReadSimulationDataTable(&cSimulation);
	if(rResult < 0){
		fprintf(stdout, "[fRunOneShot] " ANSI_COLOR_RED "Fail to read file %s" ANSI_COLOR_RESET "\n", this->rmSimulationConfigurationFile.c_str());
    	fflush(stdout);
		return(-2);
	}

	rResult = fGetSimulationMeasuresFromSimulation(&cMeasures, cSimulation);
	if(rResult < 0){
		fprintf(stdout, "[fRunOneShot] " ANSI_COLOR_RED "Fail to get measures from simulation" ANSI_COLOR_RESET "\n");
    	fflush(stdout);
		return(-3);
	}

	rResult = fInjectMeasuresIntoDatabase(&cMeasures, cSimulation);
	if(rResult < 0){
		fprintf(stdout, "[fRunOneShot] " ANSI_COLOR_RED "Fail to get measures from simulation" ANSI_COLOR_RESET "\n");
    	fflush(stdout);
		return(-3);
	}

	return(0);
}

int TcMeasureInjectorAgent::fInjectMeasuresFromSimulation(vector<TcMeasure> *pSimulatedMeasures, TcSimulation pSimulation){
	vector<TcMeasure> cSimulatedMeasures;

	for(TcMeasureInjectorAgent::TcSimulation::TcMeasureDataRow cDataRow : *(pSimulation.cpDataTable)){
		vector<double> cValues = cDataRow.cpMeasureValues;
		vector<chrono::system_clock::time_point> cTimes = cDataRow.cpTimes;
		
		for(int i = 0; i < cValues.size(); i++){
			TcMeasure cMeasure;
			TcMeasureInjectorAgent::TcSimulation::TcMeasureSpecification cMeasureSpecificiation = (*(pSimulation.cpMeasureSpecifications))[i % pSimulation.cpMeasureSpecifications->size()]; 
			cMeasure.ReferenceID = cMeasureSpecification.rpReferenceID; 
        	cMeasure.ReferenceType = cMeasureSpecification.rpReferenceType;
        	cMeasure.MeasureUnit = cMeasureSpecification.rpMeasureUnit;
        	cMeasure.MeasureType = cMeasureSpecification.rpMeasureType;

			int rRelativeTimeIndex = (pSimulation.rpSynchMode == TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode::SINGLE ? i : 0); 
			cMeasure.Timestamp = cTimes[rRelativeTimeIndex];
		    cMeasure.TimestampEpoch = (unsigned long long)chrono::duration_cast<chrono::milliseconds>(cTimes[rRelativeTimeIndex].time_since_epoch()).count();
        	cMeasure.TimestampMicroEpoch = (unsigned long long)chrono::duration_cast<chrono::microseconds>(cTimes[rRelativeTimeIndex].time_since_epoch()).count();
        	cMeasure.Value = cValues[i];

			int rResult = fInjectMeasuresIntoDatabase(pSimulation.rpDatabaseName, pSimulation.rpCollectionName, cMeasure);
			if (rResult < 0) {
				fprintf(stdout, "Inject measure fails with error %d\n", rResult);
				fflush(stdout);
				return(-1);
			}
		}
	}
	
}

int TcMeasureInjectorAgent::fInjectMeasureIntoDatabase(string pDatabaseName, string pCollectionName, TcMeasure pSimulatedMeasure){
	
	int rResult = this->cmMongoInterface->fConnectMongoDriver();
	if (rResult < 0) {
		fprintf(stdout, "Connection to Mongo Driver fails with error %d\n", rResult);
		fflush(stdout);
		return(-1);
	}
	
	rResult = this->cmMongoInterface->fGetMongoDriver()->fInsertDocument<TcMeasure>(pDatabaseName, pCollectionName, pSimulatedMeasure);
	if (rResult < 0) {
		fprintf(stdout, "Fails to inject measure with error %d\n", rResult);
		fflush(stdout);
		return(-2);
	}

	fprintf(stdout, "Number of %s measures %d read in %lld milliseconds\n", this->rmMeasureType.c_str(), cMeasureBetweenTimeValues.size(), (long long int)(rToMilliseconds-rFromMilliseconds));
	fflush(stdout);
	
	this->cmMongoInterface->fDisconnectMongoDriver();

	return(0);

}





vector<string> TcMeasureInjectorAgent::fSplitLine(vector<string> *pFields, string pLine, char pIFS) {
    vector<string> cFields;
    stringstream cInputStream(s);
    string rToken;

    while (getline (cInputStream, rToken, pIFS)) {
		if (pInputStream.bad() || pInputStream.fail()) {
			pFields = nullptr;
            return(-1);
        }
        cFields.push_back(rToken);
    }

	*pFields = cFields;
    return(0);
}

template<typename Numeric>
bool fIsNumber(string pString, Numeric *pValue){
    Numeric rNumber;
    bool rResult = (istringstream(s) >> rNumber >> ws).eof();
	*pValue = rNumber;
	return(rResult);
}



int TcMeasureInjectorAgent::fParseSimulationDataRow(TcMeasureInjectorAgent::TcSimulation *pSimulation, string pLine) {
	vector<string> cFields;
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;
	TcMeasureInjectorAgent::TcSimulation::TcMeasureDataRow cDataTableRow;

	TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode rSynchMode = cSimulation.rpSynchMode;
	uint16_t rDataDimensions = cSimulation.rpMeasureDimensions;
	uint16_t rTimeDimensions = (rSynchMode == TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode::SINGLE ? rDataDimensions : 1);
	uint32_t rTotalDimensions = rTimeDimensions + rDataDimensions;



	fSplitLine(&cFields, pLine, ",");
	if(cFields->size() != rTotalDimensions){
		return(-1);
	}

	cDataTableRow.rpTime.resize(rTimeDimensions);
	cDataTableRow.rpMeasureValues.resize(rDataDimensions);

	for(int i = 0; i < rTotalDimensions; i++){
		//Time sample are sure on the first coloumn (0) and on even coloumns, if SynchMode == SINGLE, 
		if((rTimeDimensions == 1 && i == 0) || (rTimeDimensions != 1 && i % 2 == 0)){
			uint64_t rTime = 0;
			if(!fIsNumber<uint64_t>(cFields[i], &rTime)){
				return(-2);
			}
			chrono::system_clock::time_point rTimePoint{chrono::duration_cast<time_point::duration>(chrono::nanoseconds(rTime))};
			cDataTableRow.cpTime.push_back(rTimePoint);
		}
		//Value sample are sure on the first coloumn (0) and odd coloumns, if SynchMode == SINGLE,
		else if((rTimeDimensions == 1 && i > 0) || (rTimeDimensions != 1 && i % 2 == 1)){
			double rValue = 0;
			if(!fIsNumber<double>(cFields[i], &rValue)){
				return(-3);
			}
			cDataTableRow.cpMeasureValues.push_back(rValue);
		}
	}

	cSimulation.cpMeasureSamples->push_back(cDataTableRow);
	*pSimulation = cSimulation;
	return(0);

}

/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
int TcMeasureInjectorAgent::fReadSimulationDataTable(TcSimulation* pSimulation) {
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;
	uint16_t rDataDimensions = cSimulation.rpMeasureDimensions;
	TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode rSynchMode = cSimulation.rpSynchMode;
	uint16_t rTimeDimensions = (rSynchMode == TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode::SINGLE ? rDataDimensions : 1);
	string rLine;

	char aStringSimulationDataFile[FILEPATH_BUFFER_LENGTH];
    snprintf(aStringSimulationDataFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s/Simulation/%s.csv", fGetCurrentDirectory(), this->rmSimulationDataFile.c_str());
	string rSimulationDataFile(aStringSimulationDataFile);
	if (!fExistFile(rSimulationDataFile)) {
		fprintf(stdout, ANSI_COLOR_RED "Simulation configuration file %s"  ANSI_COLOR_RESET "not found\n", rOneShotFilePath);
		fflush(stdout);
        return(-1);
    }

	ifstream cInStream;
	cInStream.open(this->rmSimulationDataFile.c_str(), ifstream::in);

	while (!cInStream.eof()) {
        getline(cInStream, ref(rLine));
        if (cInStream.bad() || cInStream.fail()) {
            return(-4);
        }
		
        int rResult = fParseSimulationDataRow(&cSimulation, rLine);
		switch(rResult){
			case 0:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_GREEN "Data Row correctly parsed %s" ANSI_COLOR_RESET "\n", rLine.c_str());
    			fflush(stdout);
				break;
			case -1:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_GREEN "Data Row fields number is different from declared Data Row dimension %d" ANSI_COLOR_RESET "\n", rTimeDimensions + rDataDimensions);
    			fflush(stdout);
				return(rResult);
			case -2:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_GREEN "Data Row Time Value is not a unsigned long long %s" ANSI_COLOR_RESET "\n", rLine.c_str());
    			return(rResult);
			case -3:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_GREEN "Data Row Measure Value is not a double %s" ANSI_COLOR_RESET "\n", rLine.c_str());
    			return(rResult);
		}
    }

	cInStream.close();
	
	*pSimulation = cSimulation;
    return(0);
}

int TcMeasureInjectorAgent::fReadSimulationConfiguration(TcSimulation* pSimulation) {
	uint16_t rDataDimensions = pSimulation->rpMeasureDimensions;
	TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode rSynchMode = pSimulation->rpSynchMode;
	uint16_t rTimeDimensions = (rSynchMode == TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode::SINGLE ? rDataDimensions : 1);
	vector<TcMeasureInjectorAgent::TcSimulation::TcMeasureDataRowRow> rDataTable;
	string rLine;

	char aStringSimulationConfigurationFile[FILEPATH_BUFFER_LENGTH];
    snprintf(aStringSimulationConfigurationFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s/Simulation/%s.ini", fGetCurrentDirectory(), this->rmSimulationConfigurationFile.c_str());
	string rSimulationConfigurationFile(aStringSimulationConfigurationFile);
	if (!fExistFile(rSimulationConfigurationFile)) {
		fprintf(stdout, ANSI_COLOR_RED "Simulation configuration file %s"  ANSI_COLOR_RESET "not found\n", rOneShotFilePath);
		fflush(stdout);
        return(-1);
    }

	size_t rSectionsStringMaxLength = 2048;
	LPWSTR pSecNames = new wchar_t[rSectionsStringMaxLength];

  	int nSectionNum = 0;
  	wstring strSecName;
	
	DWORD rSecStringLength = GetPrivateProfileSectionNames(pSecNames,rSectionsStringMaxLength,this->rmSimulationConfigurationFile.c_str());

	if (rSecStringLength){
		vector<wstring> cSecNames;
		for (wchar_t *rCurrSec = pSecNames; *rCurrSec; ++rCurrSec){
    		wstring rSectionName(rCurrSec);
    		switch(rSectionName){
				case "MEASURESPEC":
					fReadMeasureSpecificationSection(pSimulation);
					break;
				case "DB":
					fReadDBSection(pSimulation);
					break;
				case "CSV":
					fReadCSVSection(pSimulation);
					break;
				case "MACHINE":
					fReadMachineSection(pSimulation);
					break;
				default:
					fprintf(stdout, "[fReadSimulationConfiguration] " ANSI_COLOR_YELLOW "Section %s unknown" ANSI_COLOR_RESET "\n", rSectionName.c_str());
    				fflush(stdout);
					break;
			};
    		rCurrSec += rSectionName.size();
		}
  	} else if(rSecStringLength + 2 == rSectionsStringMaxLength){
		fprintf(stdout, "[fReadSimulationConfiguration] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", rSectionsStringMaxLength);
    	fflush(stdout);
		return(-1);
	}

    return(0);
}


int TcMeasureInjectorAgent::fReadMeasureSpecificationSection(TcSimulation* pSimulation) {
	

	size_t rSectionBufferMaxLength = 2048;
	LPCTSTR cSectionBuffer = new wchar_t[rSectionBufferMaxLength];
	DWORD rSectionLength = GetPrivateProfileSection("MEASURESPEC", cSectionBuffer, rSectionBufferMaxLength,this->rpSimulationConfigurationFilePath.c_str());
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;

	if (rSectionLength){
		TcMeasureInjectorAgent::TcSimulation::TcMeasureSpecification cMeasureSpec;
		for (wchar_t *rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair){
    		wstring rKey(rCurrPair);
			wstring rValue(rCurrPairrKey.size() + 1);
			switch(rKey){
				case "MEASURETYPE":
					cMeasureSpec.rpMeasureType = to_string(rValue);
					break;
				case "MEASUREUNIT":
					cMeasureSpec.rpMeasureUnit = to_string(rValue);
					break;
				case "REFERENCEID":
					cMeasureSpec.rpReferenceID = to_string(rValue);
					break;
				case "REFERENCETYPE":
					cMeasureSpec.rpReferenceType = to_string(rValue);
					break;
				case "ID":
					cMeasureSpec.rpID = (uint16_t) stoi(rValue);
					break;
				default:
					fprintf(stdout, "[fReadMeasureSpecificationSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
    				fflush(stdout);
					break;
			};
			rCurrPair += rKey.size() + rValue.size() + 1;
		}
		cSimulation.cpMeasureSpecifications->push_back(cMeasureSpec);

  	} else if(rSectionLength + 2 == rSectionBufferMaxLength){
		fprintf(stdout, "[fReadSimulationConfiguration] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", rSectionBufferMaxLength);
    	fflush(stdout);
		return(-1);
	}

	*pSimulation = cSimulation;
    return(0);
}

int TcMeasureInjectorAgent::fReadCSVSection(TcSimulation* pSimulation) {
	
	size_t rSectionBufferMaxLength = 2048;
	LPCTSTR cSectionBuffer = new wchar_t[rSectionBufferMaxLength];
	DWORD rSectionLength = GetPrivateProfileSection("CSV", cSectionBuffer, rSectionBufferMaxLength, this->rmSimulationConfigurationFile.c_str());
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;

	if (rSectionLength){
		for (wchar_t *rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair){
    		wstring rKey(rCurrPair);
			wstring rValue(rCurrPairrKey.size() + 1);
			switch(rKey){
				case "MEASUREDIMENSIONS":
					cSimulation.rpMeasureDimensions = (uint64_t) stoull(rValue);
					break;
				case "TIMEMODE":
					cSimulation.rpSynchMode = (TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode) stoi(rValue);
					break;
				default:
					fprintf(stdout, "[fReadCSVSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
    				fflush(stdout);
					break;
			};
			rCurrPair += rKey.size() + rValue.size() + 1;
		}

  	} else if(rSectionLength + 2 == rSectionBufferMaxLength){
		fprintf(stdout, "[fReadCSVSection] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", rSectionBufferMaxLength);
    	fflush(stdout);
		return(-1);
	}

	*pSimulation = cSimulation;
    return(0);
}

int TcMeasureInjectorAgent::fReadDBSection(TcSimulation* pSimulation) {
	
	size_t rSectionBufferMaxLength = 2048;
	LPCTSTR cSectionBuffer = new wchar_t[rSectionBufferMaxLength];
	DWORD rSectionLength = GetPrivateProfileSection("DB", cSectionBuffer, rSectionBufferMaxLength, this->rmSimulationConfigurationFile.c_str());
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;

	if (rSectionLength){
		for (wchar_t *rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair){
    		wstring rKey(rCurrPair);
			wstring rValue(rCurrPairrKey.size() + 1);
			switch(rKey){
				case "DATABASENAME":
					cSimulation.rpDatabaseName = to_string(rValue);
					break;
				case "COLLECTIONNAME":
					cSimulation.rpCollectionName = to_string(rValue);
					break;
				default:
					fprintf(stdout, "[fReadDBSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
    				fflush(stdout);
					break;
			};
			rCurrPair += rKey.size() + rValue.size() + 1;
		}

  	} else if(rSectionLength + 2 == rSectionBufferMaxLength){
		fprintf(stdout, "[fReadDBSection] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", rSectionBufferMaxLength);
    	fflush(stdout);
		return(-1);
	}

	*pSimulation = cSimulation;

    return(0);
}

int TcMeasureInjectorAgent::fReadMachineSection(TcSimulation* pSimulation) {
	
	size_t rSectionBufferMaxLength = 2048;
	LPCTSTR cSectionBuffer = new wchar_t[rSectionBufferMaxLength];
	DWORD rSectionLength = GetPrivateProfileSection("MACHINE", cSectionBuffer, rSectionBufferMaxLength, this->rmSimulationConfigurationFile.c_str());
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;

	if (rSectionLength){
		for (wchar_t *rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair){
    		wstring rKey(rCurrPair);
			wstring rValue(rCurrPairrKey.size() + 1);
			switch(rKey){
				case "MACHINESERIALNUMBER":
					cSimulation.rpMachineSerialNumber = to_string(rValue);
					break;
				default:
					fprintf(stdout, "[fReadMachineSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
    				fflush(stdout);
					break;
			};
			rCurrPair += rKey.size() + rValue.size() + 1;
		}

  	} else if(rSectionLength + 2 == rSectionBufferMaxLength){
		fprintf(stdout, "[fReadMachineSection] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", rSectionBufferMaxLength);
    	fflush(stdout);
		return(-1);
	}

	*pSimulation = cSimulation;

    return(0);
}



