#pragma comment (lib, "Ws2_32.lib")
#include <stdio.h>
#include <algorithm>
#include <istream>
#include <vector>
#include <sstream>
#include <chrono>

#include <windows.h>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "../../../include/Agent/MeasureInjectorAgent/TcMeasureInjectorAgent.h"

#define SECTION_BUFFER_LENGTH 4096
 
using namespace Spea::Archimede::MultiAgentSystem::Agent;

const char TcMeasureInjectorAgent::kInterRowCSVSeparator = '\n';
const char TcMeasureInjectorAgent::kIntraRowCSVSeparator = ',';

TcMeasureInjectorAgent::TcMeasureInjectorAgent() { ; }
TcMeasureInjectorAgent::TcMeasureInjectorAgent(string pSimulationConfigurationFile, string pSimulationDataFile, string pMachineFamily, string pMachineSerialNumber, string pReferenceID, string pReferenceType, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, Priority pPriority) : TcOneShotAgent(pMachineFamily, pMachineSerialNumber, pReferenceID, pReferenceType, pDatabaseName, pCollectionName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort, pNotifyDriverRemoteConnectionHost, pNotifyDriverRemoteConnectionPort, pAgentID, pAgentName, pPriority){
	this->rmSimulationConfigurationFile = pSimulationConfigurationFile;
	this->rmSimulationDataFile = pSimulationDataFile;
}
TcMeasureInjectorAgent::~TcMeasureInjectorAgent(){ ; }
string TcMeasureInjectorAgent::fToString(wstring pWString) {
	//setup converter
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cConverter;
	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return(string(cConverter.to_bytes(pWString)));
}
string TcMeasureInjectorAgent::fToString(wchar_t* pWString) {
	//setup converter
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cConverter;
	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return(string(cConverter.to_bytes(pWString)));
}
wstring TcMeasureInjectorAgent::fToWString(string pString) {
	//setup converter
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cConverter;
	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return(wstring(cConverter.from_bytes(pString)));
}
string TcMeasureInjectorAgent::fGetCurrentDirectory() {
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
	} catch (exception e) {
		fprintf(stdout,"[fGetCurrentDirectory] Get current working directory fails with exception %s\n", e.what());
		fflush(stdout);
        return("");
	}
}
bool TcMeasureInjectorAgent::fExistFile(string pFilepath) { 
    
	char rFilePath[FILEPATH_BUFFER_LENGTH];
	struct stat rStat_Buffer;
    snprintf(rFilePath, (size_t)FILEPATH_BUFFER_LENGTH, "%s", pFilepath.c_str());

	fprintf(stdout, "[fExistFile] Look for file %s\n", rFilePath);
    fflush(stdout);
	
    if (!stat(rFilePath, &rStat_Buffer)) {
        return(true);
    } else {
        return(false);
	}
    
}

int TcMeasureInjectorAgent::fRunOneShot() {
	TcMeasureInjectorAgent::TcSimulation cSimulation;
	vector<TcMeasure> cMeasures;

	int rResult = fReadSimulationConfiguration(&cSimulation);
	if(rResult < 0){
		fprintf(stdout, "[fRunOneShot] " ANSI_COLOR_RED "Fail to read file %s" ANSI_COLOR_RESET "\n", this->rmSimulationConfigurationFile.c_str());
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kReadConfigurationFail);
	}


	rResult = fReadSimulationDataTable(&cSimulation);
	if(rResult < 0){
		fprintf(stdout, "[fRunOneShot] " ANSI_COLOR_RED "Fail to read file %s" ANSI_COLOR_RESET "\n", this->rmSimulationConfigurationFile.c_str());
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kReadCSVFail);
	}

	rResult = fInjectMeasuresFromSimulation(cSimulation);
	if(rResult < 0){
		fprintf(stdout, "[fRunOneShot] " ANSI_COLOR_RED "Fail to get measures from simulation" ANSI_COLOR_RESET "\n");
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kInjectionFail);
	}

	return(0);
}
int TcMeasureInjectorAgent::fInjectMeasuresFromSimulation(TcMeasureInjectorAgent::TcSimulation pSimulation){
	vector<TcMeasure> cSimulatedMeasures;

	for(TcMeasureInjectorAgent::TcSimulation::TcMeasureDataRow cDataRow : *(pSimulation.cpMeasureSamples)){
		vector<double> cValues = cDataRow.cpMeasureValues;
		vector<chrono::time_point<chrono::system_clock, chrono::milliseconds>> cTimes = cDataRow.cpTimes;
		
		for(int i = 0; i < cValues.size(); i++){
			TcMeasure cMeasure;
			TcMeasureInjectorAgent::TcSimulation::TcMeasureSpecification cMeasureSpecificiation = (*(pSimulation.cpMeasureSpecifications))[i % pSimulation.cpMeasureSpecifications->size()]; 
			cMeasure.rpReferenceID = cMeasureSpecificiation.rpReferenceID;
        	cMeasure.rpReferenceType = cMeasureSpecificiation.rpReferenceType;
        	cMeasure.rpMeasureUnit = cMeasureSpecificiation.rpMeasureUnit;
        	cMeasure.rpMeasureType = cMeasureSpecificiation.rpMeasureType;

			int rRelativeTimeIndex = (pSimulation.rpSynchMode == TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode::SINGLE ? i : 0); 
			cMeasure.rpTimestamp = cTimes[rRelativeTimeIndex];
		    cMeasure.rpTimestampEpoch = (unsigned long long)chrono::duration_cast<chrono::milliseconds>(cTimes[rRelativeTimeIndex].time_since_epoch()).count();
        	cMeasure.rpTimestampMicroEpoch = (unsigned long long)chrono::duration_cast<chrono::microseconds>(cTimes[rRelativeTimeIndex].time_since_epoch()).count();
        	cMeasure.rpValue = cValues[i];

			int rResult = this->fInjectMeasureIntoDatabase(pSimulation.rpDatabaseName, pSimulation.rpCollectionName, cMeasure);
			if (rResult < 0) {
				fprintf(stdout, "Inject measure fails with error %d\n", rResult);
				fflush(stdout);
				return(TcMeasureInjectorAgent::kInjectionFail);
			}
		}
	}

	return(TcMeasureInjectorAgent::kInjectionCorrect);
	
}
int TcMeasureInjectorAgent::fInjectMeasureIntoDatabase(string pDatabaseName, string pCollectionName, TcMeasure pSimulatedMeasure){
	
	int rResult = TcMeasureInjectorAgent::TcAgent::cmMongoInterface->fConnectDriver();
	if (rResult < 0) {
		fprintf(stdout, "Connection to Mongo Driver fails with error %d\n", rResult);
		fflush(stdout);
		return(TcMeasureInjectorAgent::kDatabaseConnectionFail);
	}
	
	rResult = TcMeasureInjectorAgent::TcAgent::cmMongoInterface->fGetDriver()->fInsertDocument<TcMeasure>(pDatabaseName, pCollectionName, pSimulatedMeasure);
	if (rResult < 0) {
		fprintf(stdout, "Fails to inject measure with error %d\n", rResult);
		fflush(stdout);
		return(TcMeasureInjectorAgent::kDatabaseQueryFail);
	}
	
	TcMeasureInjectorAgent::TcAgent::cmMongoInterface->fDisconnectDriver();

	return(TcMeasureInjectorAgent::kInjectionCorrect);

}
int TcMeasureInjectorAgent::fSplitLine(vector<string> *pFields, string pLine, char pIFS) {
    vector<string> cFields;
	stringstream cInputStream(pLine);
    string rToken;

    while (getline(cInputStream, rToken, pIFS)) {
		if (cInputStream.bad() || cInputStream.fail()) {
			pFields = nullptr;
            return(-1);
        }
        cFields.push_back(rToken);
    }

	*pFields = cFields;
    return(cFields.size());
}

template<typename Numeric>
bool fIsNumber(string pString, Numeric *pValue){
    Numeric rNumber;
    bool rResult = (istringstream(pString) >> rNumber >> ws).eof();
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



	TcMeasureInjectorAgent::fSplitLine(&cFields, pLine, ',');
	if(cFields.size() != rTotalDimensions){
		return(TcMeasureInjectorAgent::kParsingDataRowNumberIncorrect);
	}



	for(int i = 0; i < rTotalDimensions; i++){
		//Time sample are sure on the first coloumn (0) and on even coloumns, if SynchMode == SINGLE, 
		if((rTimeDimensions == 1 && i == 0) || (rTimeDimensions != 1 && i % 2 == 0)){
			uint64_t rTime = 0;
			if(!fIsNumber<uint64_t>(cFields[i], &rTime)){
				return(TcMeasureInjectorAgent::kParsingDataRowFormatIncorrect);
			}
			chrono::time_point<chrono::system_clock, chrono::milliseconds> rTimePoint{chrono::duration_cast<chrono::milliseconds>(chrono::nanoseconds(rTime))};
			cDataTableRow.cpTimes.push_back(rTimePoint);
		}
		//Value sample are sure on the first coloumn (0) and odd coloumns, if SynchMode == SINGLE,
		else if((rTimeDimensions == 1 && i > 0) || (rTimeDimensions != 1 && i % 2 == 1)){
			double rValue = 0;
			if(!fIsNumber<double>(cFields[i], &rValue)){
				return(TcMeasureInjectorAgent::kParsingDataRowFormatIncorrect);
			}
			cDataTableRow.cpMeasureValues.push_back(rValue);
		}
	}

	cSimulation.cpMeasureSamples->push_back(cDataTableRow);
	*pSimulation = cSimulation;
	return(TcMeasureInjectorAgent::kParsingDataRowCorrect);

}
/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
int TcMeasureInjectorAgent::fReadSimulationDataTable(TcMeasureInjectorAgent::TcSimulation* pSimulation) {
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;
	uint16_t rDataDimensions = cSimulation.rpMeasureDimensions;
	TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode rSynchMode = cSimulation.rpSynchMode;
	uint16_t rTimeDimensions = (rSynchMode == TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode::SINGLE ? rDataDimensions : 1);
	string rLine;
	int rResult = 0;

	char aStringSimulationDataFile[FILEPATH_BUFFER_LENGTH];
    snprintf(aStringSimulationDataFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s\\%s", TcMeasureInjectorAgent::fGetCurrentDirectory().c_str(), this->rmSimulationDataFile.c_str());
	string rSimulationDataFile(aStringSimulationDataFile);
	if (!TcMeasureInjectorAgent::fExistFile(rSimulationDataFile)) {
		fprintf(stdout, ANSI_COLOR_RED "Simulation configuration file %s"  ANSI_COLOR_RESET " not found\n", rSimulationDataFile.c_str());
		fflush(stdout);
		return(-1);
	}
	

	ifstream cInStream;
	cInStream.open(this->rmSimulationDataFile.c_str(), ifstream::in);
	if (cInStream.bad() || cInStream.fail()) {
		fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_RED "Bad Configuration Stream Error %s" ANSI_COLOR_RESET "\n", rLine.c_str());
		fflush(stdout);
		return(TcMeasureInjectorAgent::kBadConfigurationStreamError);
	}


	while (getline(cInStream, rLine, TcMeasureInjectorAgent::kInterRowCSVSeparator)){
        if (cInStream.bad() || cInStream.fail()) {
			fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_RED "Bad Configuration Stream Error %s" ANSI_COLOR_RESET "\n", rLine.c_str());
			fflush(stdout);
			return(TcMeasureInjectorAgent::kBadConfigurationStreamError);
        }
		
        rResult = fParseSimulationDataRow(&cSimulation, rLine);
		switch(rResult){
			case 0:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_GREEN "Data Row correctly parsed line(%s)" ANSI_COLOR_RESET "\n", rLine.c_str());
    			fflush(stdout);
				rResult = TcMeasureInjectorAgent::kParsingDataRowCorrect;
				break;
			case -1:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_RED "Data Row fields number is different from declared Data Row dimension %d" ANSI_COLOR_RESET "\n", rTimeDimensions + rDataDimensions);
    			fflush(stdout);
				rResult = TcMeasureInjectorAgent::kParsingDataRowNumberIncorrect;
				break;
			case -2:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_RED "Data Row Time Value is not a unsigned long long, line(%s)" ANSI_COLOR_RESET "\n", rLine.c_str());
				fflush(stdout);
				rResult = TcMeasureInjectorAgent::kParsingDataRowFormatIncorrect;
				break;
			case -3:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_RED "Data Row Measure Value is not a double, line(%s)" ANSI_COLOR_RESET "\n", rLine.c_str());
				fflush(stdout);
				rResult = TcMeasureInjectorAgent::kParsingDataRowFormatIncorrect;
				break;
			default:
				fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_YELLOW "Unknown simulation data raw parsing result, line(%s)" ANSI_COLOR_RESET "\n", rLine.c_str());
				fflush(stdout);
				rResult = TcMeasureInjectorAgent::kParsingDataRowFormatUnknown;
				break;
		}

		if (rResult < 0) {
			fprintf(stdout, "[fReadSimulationDataTable] " ANSI_COLOR_YELLOW "Parsing error occurred (%d), line(%s)" ANSI_COLOR_RESET "\n", rResult, rLine.c_str());
			fflush(stdout);
			cInStream.close();
			return(rResult);
		}
    }

	cInStream.close();
	*pSimulation = cSimulation;

	
    return(TcMeasureInjectorAgent::kGetDataSuccess);
}
int TcMeasureInjectorAgent::fReadSimulationConfiguration(TcMeasureInjectorAgent::TcSimulation* pSimulation) {
	uint16_t rDataDimensions = pSimulation->rpMeasureDimensions;
	TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode rSynchMode = pSimulation->rpSynchMode;
	uint16_t rTimeDimensions = (rSynchMode == TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode::SINGLE ? rDataDimensions : 1);
	vector<TcMeasureInjectorAgent::TcSimulation::TcMeasureDataRow> rDataTable;
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;
	string rLine;

	char aStringSimulationConfigurationFile[FILEPATH_BUFFER_LENGTH];
    snprintf(aStringSimulationConfigurationFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s\\%s", TcMeasureInjectorAgent::fGetCurrentDirectory().c_str(), this->rmSimulationConfigurationFile.c_str());
	string rSimulationConfigurationFile(aStringSimulationConfigurationFile);
	if (!TcMeasureInjectorAgent::fExistFile(rSimulationConfigurationFile)) {
		fprintf(stdout, ANSI_COLOR_RED "Simulation configuration file %s"  ANSI_COLOR_RESET "not found\n", aStringSimulationConfigurationFile);
		fflush(stdout);
        return(-1);
    }


	char rSecNames[SECTION_BUFFER_LENGTH];
	DWORD rSecStringLength = GetPrivateProfileSectionNamesA(rSecNames, 2048, rSimulationConfigurationFile.c_str());



	if (rSecStringLength){
		vector<wstring> cSecNames;
		for (char* rCurrSecName = rSecNames; *rCurrSecName; ++rCurrSecName){
    		string rSectionName(rCurrSecName);

			if (rSectionName.starts_with("MEASURESPEC")) {
				fReadMeasureSpecificationSection(rSectionName, &cSimulation);
			} else if (rSectionName == "DATABASE") {
				fReadDBSection(&cSimulation);
			} else if (rSectionName == "CSV") {
				fReadCSVSection(&cSimulation);
			} else if (rSectionName == "MACHINE") {
				fReadMachineSection(&cSimulation);
			} else{
				fprintf(stdout, "[fReadSimulationConfiguration] " ANSI_COLOR_YELLOW "Section %s unknown" ANSI_COLOR_RESET "\n", rSectionName.c_str());
				fflush(stdout);
			}

    		rCurrSecName += rSectionName.size();
		}
  	} else if(rSecStringLength + 2 == SECTION_BUFFER_LENGTH){
		fprintf(stdout, "[fReadSimulationConfiguration] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", SECTION_BUFFER_LENGTH);
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kSectionBufferOverflowError);
	}

	*pSimulation = cSimulation;

    return(0);
}
int TcMeasureInjectorAgent::fReadMeasureSpecificationSection(string pMeasureSpecificationSectionName, TcMeasureInjectorAgent::TcSimulation* pSimulation) {
	

	int rResult = 0;
	char cSectionBuffer[SECTION_BUFFER_LENGTH];
	char aStringSimulationConfigurationFile[FILEPATH_BUFFER_LENGTH];
	snprintf(aStringSimulationConfigurationFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s\\%s", TcMeasureInjectorAgent::fGetCurrentDirectory().c_str(), this->rmSimulationConfigurationFile.c_str());
	DWORD rSectionLength = GetPrivateProfileSectionA(pMeasureSpecificationSectionName.c_str(), cSectionBuffer, (DWORD)4096, aStringSimulationConfigurationFile);
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;
	

	vector<string> rKVPairID;
	rResult = fSplitLine(&rKVPairID, string(pMeasureSpecificationSectionName), '/');
	int rMeasureSepcificationID = stoi(rKVPairID[1]);


	if (rSectionLength){
		TcMeasureInjectorAgent::TcSimulation::TcMeasureSpecification cMeasureSpec;
		cMeasureSpec.rpID = rMeasureSepcificationID;
		for (char *rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair){
			vector<string> rKVPair;
			rResult = fSplitLine(&rKVPair, string(rCurrPair), '=');
			string rKey(rKVPair[0]);
			string rValue(rKVPair[1]);

			if (rKey == "MEASURETYPE") {
				cMeasureSpec.rpMeasureType = rValue;
			} else if (rKey == "MEASUREUNIT") {
				cMeasureSpec.rpMeasureUnit = rValue;
			} else if (rKey == "REFERENCEID") {
				cMeasureSpec.rpReferenceID = rValue;
			} else if (rKey == "REFERENCETYPE") {
				cMeasureSpec.rpReferenceType = rValue;
			} else {
				fprintf(stdout, "[fReadMeasureSpecificationSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
				fflush(stdout);
			}

			rCurrPair += rKey.size() + rValue.size() + 1;
		}

		
		cSimulation.cpMeasureSpecifications->push_back(cMeasureSpec);

  	} else if(rSectionLength + 2 == SECTION_BUFFER_LENGTH){
		fprintf(stdout, "[fReadSimulationConfiguration] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", SECTION_BUFFER_LENGTH);
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kSectionBufferOverflowError);
	}

	*pSimulation = cSimulation;
    return(TcMeasureInjectorAgent::kReadMeasureSpecSectionOk);
}
int TcMeasureInjectorAgent::fReadCSVSection(TcMeasureInjectorAgent::TcSimulation* pSimulation) {
	
	int rResult = 0;

	char cSectionBuffer[SECTION_BUFFER_LENGTH];
	char aStringSimulationConfigurationFile[FILEPATH_BUFFER_LENGTH];
	snprintf(aStringSimulationConfigurationFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s\\%s", TcMeasureInjectorAgent::fGetCurrentDirectory().c_str(), this->rmSimulationConfigurationFile.c_str());
	DWORD rSectionLength = GetPrivateProfileSectionA("CSV", cSectionBuffer, (DWORD)4096, aStringSimulationConfigurationFile);
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;

	if (rSectionLength){
		for (char* rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair) {
			vector<string> rKVPair;
			rResult = fSplitLine(&rKVPair, string(rCurrPair), '=');
			string rKey(rKVPair[0]);
			string rValue(rKVPair[1]);

			if (rKey == "MEASUREDIMENSIONS") {
				cSimulation.rpMeasureDimensions = (uint64_t)stoull(rValue);
			} else if (rKey == "TIMEMODE") {
				cSimulation.rpSynchMode = (TcMeasureInjectorAgent::TcSimulation::TcSynchronizationMode)stoi(rValue);
			} else {
				fprintf(stdout, "[fReadCSVSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
				fflush(stdout);
			}

			rCurrPair += rKey.size() + rValue.size() + 1;
		}

  	} else if(rSectionLength + 2 == SECTION_BUFFER_LENGTH){
		
		fprintf(stdout, "[fReadCSVSection] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", SECTION_BUFFER_LENGTH);
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kSectionBufferOverflowError);
	}

	*pSimulation = cSimulation;
    return(TcMeasureInjectorAgent::kReadCSVSectionOk);
}
int TcMeasureInjectorAgent::fReadDBSection(TcMeasureInjectorAgent::TcSimulation* pSimulation) {
	
	int rResult = 0;
	char cSectionBuffer[SECTION_BUFFER_LENGTH];
	char aStringSimulationConfigurationFile[FILEPATH_BUFFER_LENGTH];
	snprintf(aStringSimulationConfigurationFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s\\%s", TcMeasureInjectorAgent::fGetCurrentDirectory().c_str(), this->rmSimulationConfigurationFile.c_str());
	DWORD rSectionLength = GetPrivateProfileSectionA("DATABASE", cSectionBuffer, (DWORD)4096, aStringSimulationConfigurationFile);
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;
	

	if (rSectionLength){
		for (char* rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair) {
			
			vector<string> rKVPair;
			rResult = fSplitLine(&rKVPair, string(rCurrPair), '=');
			string rKey(rKVPair[0]);
			string rValue(rKVPair[1]);

			if (rKey == "DATABASENAME") {
				cSimulation.rpDatabaseName = rValue;
			}
			else if (rKey == "COLLECTIONNAME") {
				cSimulation.rpCollectionName = rValue;
			}
			else {
				fprintf(stdout, "[fReadDBSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
				fflush(stdout);
			}

			rCurrPair += strlen(rCurrPair);
		}
		

  	} else if(rSectionLength + 2 == SECTION_BUFFER_LENGTH){
		fprintf(stdout, "[fReadDBSection] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", SECTION_BUFFER_LENGTH);
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kSectionBufferOverflowError);
	}

	*pSimulation = cSimulation;

    return(TcMeasureInjectorAgent::kReadDBSectionOk);
}
int TcMeasureInjectorAgent::fReadMachineSection(TcMeasureInjectorAgent::TcSimulation* pSimulation) {
	
	int rResult = 0;
	char cSectionBuffer[SECTION_BUFFER_LENGTH];
	char aStringSimulationConfigurationFile[FILEPATH_BUFFER_LENGTH];
	snprintf(aStringSimulationConfigurationFile, (size_t)FILEPATH_BUFFER_LENGTH, "%s\\%s", TcMeasureInjectorAgent::fGetCurrentDirectory().c_str(), this->rmSimulationConfigurationFile.c_str());
	DWORD rSectionLength = GetPrivateProfileSectionA("MACHINE", cSectionBuffer, (DWORD)4096, aStringSimulationConfigurationFile);
	TcMeasureInjectorAgent::TcSimulation cSimulation = *pSimulation;



	if (rSectionLength){
		for (char* rCurrPair = cSectionBuffer; *rCurrPair; ++rCurrPair) {
			vector<string> rKVPair;
			rResult = fSplitLine(&rKVPair, string(rCurrPair), '=');
			string rKey(rKVPair[0]);
			string rValue(rKVPair[1]);

			if (rKey == "MACHINESERIALNUMBER") {
				cSimulation.rpMachineSerialNumber = rValue;
			} else {
				fprintf(stdout, "[fReadMachineSection] " ANSI_COLOR_YELLOW "Key %s unknown" ANSI_COLOR_RESET "\n", rKey.c_str());
				fflush(stdout);
			}

			rCurrPair += strlen(rCurrPair);
		}

  	} else if(rSectionLength + 2 == SECTION_BUFFER_LENGTH){
		fprintf(stdout, "[fReadMachineSection] " ANSI_COLOR_RED "Buffer to small(%d) to contain to Section Names string" ANSI_COLOR_RESET "\n", SECTION_BUFFER_LENGTH);
    	fflush(stdout);
		return(TcMeasureInjectorAgent::kSectionBufferOverflowError);
	}

	*pSimulation = cSimulation;

    return(TcMeasureInjectorAgent::kReadMachineSectionOk);
}



