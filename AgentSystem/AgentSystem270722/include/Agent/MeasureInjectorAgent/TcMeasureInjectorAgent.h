#pragma once

#ifndef TCMEASUREINJECTORAGENT_H
#define TCMEASUREINJECTORAGENT_H

#include <string>
#include <chrono>

#include "../../TcMeasure.h"
#include "../TcOneShotAgent.h"



using namespace std;

namespace Spea {
    namespace Archimede {
        namespace MultiAgentSystem {
            namespace Agent {

                class TcMeasureInjectorAgent : public TcOneShotAgent {
                private:

                    constexpr static const int8_t kBadConfigurationStreamError = -1;
                    constexpr static const int8_t kParsingCSVDataRowError = -2;
                    constexpr static const int8_t kSectionBufferOverflowError = -3;

                    constexpr static const int8_t kReadCSVDataFail = -3;
                    constexpr static const int8_t kInjectionFail = -4;
                    constexpr static const int8_t kDatabaseConnectionFail = -5;
                    constexpr static const int8_t kDatabaseQueryFail = -6;
                    constexpr static const int8_t kReadConfigurationFail = -7;
                    constexpr static const int8_t kReadCSVFail = -8;


                    constexpr static const int8_t kReadCSVSectionOk = 2;
                    constexpr static const int8_t kReadMeasureSpecSectionOk = 3;
                    constexpr static const int8_t kReadMachineSectionOk = 4;
                    constexpr static const int8_t kReadDBSectionOk = 5;


                    constexpr static const int8_t kReadConfiguration = 1;
                    constexpr static const int8_t kReadCSVSection = 2;
                    constexpr static const int8_t kReadMeasureSpecSection = 3;
                    constexpr static const int8_t kReadMachineSection = 4;
                    constexpr static const int8_t kReadDBSection = 5;
                    constexpr static const int8_t kReadCSVData = 1;
                    constexpr static const int8_t kReadCSVDataRow = 2;
                    constexpr static const int8_t kParsingDataRow = 3;
                    constexpr static const int8_t kInjectData = 1;
                    constexpr static const int8_t kInjectMeasure = 2;


                    constexpr static const int8_t kParsingDataRowCorrect = 0;
                    constexpr static const int8_t kParsingDataRowFormatIncorrect = 0;
                    constexpr static const int8_t kParsingDataRowNumberIncorrect = 0;
                    constexpr static const int8_t kParsingDataRowFormatUnknown = 0;

                    constexpr static const int8_t kInjectionCorrect = 0;
                    constexpr static const int8_t kInjectionIncorrect = 0;
                    constexpr static const int8_t kInjectionUnknown = 0;



                    static const char kInterRowCSVSeparator;
                    static const char kIntraRowCSVSeparator;

                    string rmSimulationConfigurationFile;
                    string rmSimulationDataFile;


                    class TcSimulation {
                    public:
                        class TcMeasureSpecification {
                        public:
                            string rpMeasureType;
                            string rpMeasureUnit;
                            string rpReferenceID;
                            string rpReferenceType;
                            uint16_t rpID;
                            TcMeasureSpecification() { ; }
                            ~TcMeasureSpecification() { ; }
                            TcMeasureSpecification(string pMeasureType, string pMeasureUnit, string pReferenceID, string pReferenceType, uint16_t pID) {
                                this->rpMeasureType = pMeasureType;
                                this->rpMeasureUnit = pMeasureUnit;
                                this->rpReferenceID = pReferenceID;
                                this->rpReferenceType = pReferenceType;
                                this->rpID = pID;
                            }

                        };
                        enum class TcSynchronizationMode {
                            SINGLE = 0x00,
                            MULTIPLE = 0x01
                        };
                        class TcMeasureDataRow {
                        public:
                            vector<double> cpMeasureValues;
                            vector<chrono::time_point<chrono::system_clock, chrono::milliseconds>> cpTimes;

                            TcMeasureDataRow() { ; }
                            ~TcMeasureDataRow() { ; }

                            void fSetMeasureValues(vector<double> pMeasureValues) {}
                            void fSetTimeValues(vector<chrono::time_point<chrono::system_clock, chrono::milliseconds>> pMeasureTimes) {}

                            vector<double>  fGetMeasureValues() { return(this->cpMeasureValues); }
                            vector<chrono::time_point<chrono::system_clock, chrono::milliseconds>> fGetTimeValues() { return(this->cpTimes); }


                        };

                        string rpMachineSerialNumber;
                        string rpDatabaseName;
                        string rpCollectionName;
                        TcSynchronizationMode rpSynchMode;
                        uint64_t rpMeasureDimensions;
                        vector<TcMeasureSpecification>* cpMeasureSpecifications;
                        vector<TcMeasureDataRow>* cpMeasureSamples;


                        TcSimulation() {
                            this->cpMeasureSpecifications = new vector<TcMeasureSpecification>();
                            this->cpMeasureSamples = new vector<TcMeasureDataRow>();
                        }

                        TcSimulation(string pMachineSerialNumber, string pDatabasaseName, string pCollectionName, TcSynchronizationMode pSynchMode, uint8_t pMeasureDimensions) {
                            this->rpMachineSerialNumber = pMachineSerialNumber;
                            this->rpDatabaseName = pDatabasaseName;
                            this->rpCollectionName = pCollectionName;
                            this->rpSynchMode = pSynchMode;
                            this->rpMeasureDimensions = pMeasureDimensions;
                            this->cpMeasureSpecifications = new vector<TcMeasureSpecification>();
                            this->cpMeasureSamples = new vector<TcMeasureDataRow>();
                        }

                        ~TcSimulation() {}
                    };

                    static bool fExistFile(string pFile);
                    static string fGetCurrentDirectory();
                    static int fSplitLine(vector<string>* pFields, string pLine, char pIFS = ' ');


                public:


                    TcMeasureInjectorAgent();
                    TcMeasureInjectorAgent(string pSimulationConfigurationFile, string pSimulationDataFile, string pMachineFamily, string pMachineSerialNumber, string pReferenceID, string pReferenceType, string pDatabaseName, string pCollectionName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort, string pNotifyDriverRemoteConnectionHost, uint16_t pNotifyDriverRemoteConnectionPort, string pAgentID, string pAgentName, Priority pPriority = Priority::Medium);
                    ~TcMeasureInjectorAgent();



                    int fInjectMeasuresFromSimulation(TcMeasureInjectorAgent::TcSimulation pSimulation);
                    int fInjectMeasureIntoDatabase(string pDatabaseName, string pCollectionName, TcMeasure pSimulatedMeasure);
                    int fRunOneShot();
                    int fParseSimulationDataRow(TcMeasureInjectorAgent::TcSimulation* pSimulation, string pLine);


                    int fReadSimulationDataTable(TcMeasureInjectorAgent::TcSimulation* pSimulation);
                    int fReadSimulationConfiguration(TcMeasureInjectorAgent::TcSimulation* pSimulation);
                    int fReadMeasureSpecificationSection(string pMeasureSpecificationSectionName, TcMeasureInjectorAgent::TcSimulation* pSimulation);
                    int fReadCSVSection(TcMeasureInjectorAgent::TcSimulation* pSimulation);
                    int fReadDBSection(TcMeasureInjectorAgent::TcSimulation* pSimulation);
                    int fReadMachineSection(TcMeasureInjectorAgent::TcSimulation* pSimulation);

                    static string fToString(wstring pWString);
                    static string fToString(wchar_t* pWString);
                    static wstring fToWString(string pString);





                };
            }
        }
    }
}

#endif