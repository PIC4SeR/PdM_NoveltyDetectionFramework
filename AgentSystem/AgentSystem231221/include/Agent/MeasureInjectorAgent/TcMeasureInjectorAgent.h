#pragma once

#ifndef TCMEASUREINJECTORAGENT_H
#define TCMEASUREINJECTORAGENT_H

#include <string>
#include <chrono>
#include "../../TcMeasure.h"
#include "../TcOneShotAgent.h"

using namespace std;

class TcMeasureInjectorAgent : public TcOneShotAgent {
private:

    
    class TcSimulation{
        public:
            class TcMeasureSpecification {
                public:
                    string rpMeasureType;
                    string rpMeasureUnit;
                    string rpReferenceID;
                    string rpReferenceType;
                    uint16_t rpID;
                    
                    TcMeasureSpecification(string rpMeasureType, string pMeasureUnit, string pReferenceID, string pReferenceType, uint16_t pID){
                        this->rpMeasureType = pMeasureType;
                        this->rpMeasureUnit = pMeasureUnit;
                        this->rpReferenceID = pReferenceID;
                        this->rpReferenceType = pReferenceType;
                        this->rpID = pID;
                    }

                    ~TcMeasureSpecification();

            };
            enum class TcSynchronizationMode {
                SINGLE,
                MULTIPLE
            };

            class TcMeasureDataRow{
                public:
                    vector<double> cpMeasureValues;
                    vector<chrono::system_clock::time_point> cpTimes;
                    
                    TcMeasureDataRow();
                    ~TcMeasureDataRow();
            };

            string rpMachineSerialNumber;
            string rpDatabasaseName;
            string rpCollectionName;
            TcSynchronizationMode rpSynchMode;
            uint64_t rpMeasureDimensions;
            vector<TcMeasureSpecification> *cpMeasureSpecifications;
            vector<TcMeasureDataRow> *cpMeasureSamples;

            TcSimulation(string pMachineSerialNumber, string pDatabasaseName, string pCollectionName, TcSynchronizationMode pSynchMode, uint8_t pMeasureDimensions){
                this->rpMachineSerialNumber = pMachineSerialNumber;
                this->rpDatabasaseName = pDatabasaseName;
                this->rpCollectionName = pCollectionName;
                this->rpSynchMode = pSynchMode;
                this->rpMeasureDimensions = pMeasureDimensions;
                this->cpMeasureSpecifications = new vector<TcMeasureSpecification>();
                this->cpMeasureSamples = new vector<TcMeasureDataRow>();
            }
            ~TcSimulation(){
                if(this->cpMeasureSpecifications != nullptr){
                    delete this->cpMeasureSpecifications;
                    this->cpMeasureSpecifications = nullptr;
                }

                if(this->cpMeasureSamples != nullptr){
                    delete this->cpMeasureSamples;
                    this->cpMeasureSamples = nullptr;
                }


            }
    };

    bool fExistFile(string pFile);
    string fGetCurrentDirectory():


public:

	TcMeasureInjectorAgent(string pSimulationConfigurationFile, string pSimulationDataFile, string pMachineFamily, string pMachineSerialNumber, string pReferenceID, string pReferenceType, string pCollectionName, string pDatabaseName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, chrono::microseconds pStepRunTime, Priority pPriority = Priority::Medium, bool pStopped = false);
	~TcMeasureInjectorAgent();

    
    int fInjectMeasuresFromSimulation(vector<TcMeasure> *pSimulatedMeasures, TcSimulation pSimulation);
    int fInjectMeasureIntoDatabase(string pDatabaseName, string pCollectionName, TcMeasure pSimulatedMeasure);
    int fRunOneShot();
    int fParseSimulationDataRow(TcMeasureInjectorAgent::TcSimulation *pSimulation, TcMeasureDataRow *pDataTableRow, string pLine);
    int fReadSimulationDataTable(istream &pInStream, TcSimulation* pSimulation);
    int fReadSimulationConfiguration(TcSimulation* pSimulation);
    int fReadMeasureSpecificationSection(TcSimulation* pSimulation);
    int fReadCSVSection(TcSimulation* pSimulation);
    int fReadDBSection(TcSimulation* pSimulation);
    int fReadMachineSection(TcSimulation* pSimulation);

    
	

	

};


#endif