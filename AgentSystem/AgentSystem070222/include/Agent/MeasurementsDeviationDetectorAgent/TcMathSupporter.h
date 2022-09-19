#pragma once


#ifndef TCMATHSUPPORTER_H
#define TCMATHSUPPORTER_H

#include <vector>
#include <cmath>
#include <chrono>
#include <string>


using namespace std;

class TcMathSupporter {
private:
public:
    class TcError{
    public:
        class TcCofactor {
        private:
        public:
            static const int kValidCofactor = 0;
            static const int kErr_NoCols = -3;
            static const int kErr_NoRows = -4;
            static const int kErr_NoConstCols = -7;
            static const int kErr_InvalidExcludeRow = -10;
            static const int kErr_InvalidExcludeCol = -11;
        };
        class TcDeterminant {
        private:
        public:
            static const int kValidDeterminant = 0;
            static const int kErr_NoCols = -3;
            static const int kErr_NoRows = -4;
            static const int kErr_NoConstCols = -7;
            static const int kErr_NoSquareMatrix = -12;
            static const int kErr_NoColDimensionRelationship = -15;
        };

        class TcTranspose {
        private:
        public:
            static const int kValidTranspose = 0;
            static const int kErr_NoCols = -3;
            static const int kErr_NoRows = -4;
            static const int kErr_NoConstCols = -7;
        };
        class TcMultiply {
        private:
        public:
            static const int kValidMultiply = 0;
            static const int kErr_NoCols = -3;
            static const int kErr_NoRows = -4;
            static const int kErr_NoConstCols = -7;
            static const int kErr_NoRowColRelationship = -6;
        };
        class TcMultiplyMatrixVector {
        private:
        public:
            static const int kValidMultiply = 0;
            static const int kErr_NoCols = -3;
            static const int kErr_NoRows = -4;
            static const int kErr_NoConstCols = -7;
            static const int kErr_NoRowColRelationship = -6;
            static const int kErr_VectNoelements = -14;
        };
        class TcLinearRegression {
        private:
        public:
            static const int kValidDerivate = 0;
            static const int kErr_NoYvals = -1;
            static const int kErr_NoXvals = -2;
            static const int kErr_NoXYvalsRelationship = -5;
        };
        class TcAdJoint {
        private:
        public:
            static const int kValidADJoint = 0;
            static const int kErr_NoCols = -3;
            static const int kErr_NoRows = -4;
            static const int kErr_NoConstCols = -7;
            static const int kErr_NoSquareMatrix = -12;
            static const int kErr_NoColDimensionRelationship = -15;
        };
        class TcInverse {
        private:
        public:
            static const int kValidInverse = 0;
            static const int kErr_NoCols = -3;
            static const int kErr_NoRows = -4;
            static const int kErr_NoConstCols = -7;
            static const int kErr_NoSquareMatrix = -12;
            static const int kErr_DeterminantNull = -13;
            static const int kErr_ADJointOperationFail = -16;
            static const int kErr_ADJointNull = -17;
            static const int kErr_ADJointEmpty = -18;

        };
        class TcDerivate {
        private:
        public:
            static const int kValidDerivate = 0;
            static const int kErr_NoYvals = -1;
            static const int kErr_NoXvals = -2;
            static const int kErr_NoXYvalsRelationship = -5;


        };
    };






   class TcMathDeviationDetector{
        public:
            
            enum class Deviation { NoDeviation, Linear, Impulsive };
            enum class DeviationDistribution { NoDeviation, Glitch, Drift, Drop, Shift };
            
            class TcDeviationType{
                private:
                public:
                static const string kShift;
                static const string kDrift;
                static const string kGlitch;
                static const string kDrop;
            };

            static const uint8_t kMinShiftSample = 3;
            static const uint8_t kMaxShiftSample = 3;
            static const uint8_t kMaxDriftSample = 5;
            static const uint8_t kMinDriftSample = 3;
            static const uint8_t kMinDropSample = 4;
            static const uint8_t kMinGlitchSample = 3;


            class TcMathDeviationInterval{
                
                private:
                    Deviation rmDeviation;
                    double rmMLinear;
                    chrono::high_resolution_clock::time_point cmStartTime;
                    chrono::high_resolution_clock::time_point cmEndTime;
                    int rmStartIndex;
                    int rmEndIndex;

                public:
                    
                   
                    TcMathDeviationInterval();
                    TcMathDeviationInterval(Deviation pDeviation, double pMLinear, int pStartIndex, int pEndIndex, chrono::high_resolution_clock::time_point pStartTime, chrono::high_resolution_clock::time_point pEndTime);
                    ~TcMathDeviationInterval();
                    double fGetMLinear();
                    Deviation fGetDeviation();
                    chrono::high_resolution_clock::time_point fGetEndTime();
                    chrono::high_resolution_clock::time_point fGetStartTime();
                    int fGetStartIndex();
                    int fGetEndIndex();
                    void fSetMLinear(double pMLinear);
                    void fSetDeviation(Deviation pDeviation);
                    void fSetStartIndex(int pStartLinearIndex);
                    void fSetEndIndex(int pEndLinearIndex);
                    void fSetStartTime(chrono::high_resolution_clock::time_point pStartTime);
                    void fSetEndTime(chrono::high_resolution_clock::time_point pEndTIme);
                    bool fIsConstant();
                    bool fIsLinear(double *pMLinear);
                    bool fIsImpulsive(double *pMLinear);
                    void fSetObject(TcMathSupporter::TcMathDeviationDetector::Deviation pDeviation, double pMLinear, int pStartIndex, int pEndIndex, chrono::high_resolution_clock::time_point cmStartTime, chrono::high_resolution_clock::time_point cmEndTime);
            };

            


            class TcMathDeviation {

            private:

                DeviationDistribution rmDeviation;
                double rmDevVal;
                double rmNormVal;
                double rmMLinear;
                chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cmStartTime;
                chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds cmEndTime;
                int rmStartIndex;
                int rmEndIndex;

            public:

                TcMathDeviation();
                TcMathDeviation(DeviationDistribution pDeviation, double pDevVal, double pNormVal, double pMLinear, int pStartIndex, int pEndIndex, chrono::high_resolution_clock::time_point pStartTime, chrono::high_resolution_clock::time_point pEndTime);
                ~TcMathDeviation();
                double fGetMLinear();
                double fGetDev();
                DeviationDistribution fGetDeviation();
                double fGetNorm();
                int fGetStartIndex();
                int fGetEndIndex();
                chrono::high_resolution_clock::time_point fGetEndTime();
                chrono::high_resolution_clock::time_point fGetStartTime();
                void fSetStartIndex(int pStartLinearIndex);
                void fSetEndIndex(int pEndLinearIndex);
                void fSetStartTime(chrono::high_resolution_clock::time_point pStartTime);
                void fSetEndTime(chrono::high_resolution_clock::time_point pEndTIme);
                void fSetNorm(double pNormVal);
                void fSetDev(double pDevVal);
                void fSetDeviation(DeviationDistribution pDeviation);
                void fSetMLinear(double pMLinear);
                void fSetObject(DeviationDistribution pDeviation, double pDevVal, double pNormVal, double pMLinear, int pStartIndex, int pEndIndex, chrono::high_resolution_clock::time_point pStartTime, chrono::high_resolution_clock::time_point pEndTime);

            };


            class TcConst{
            public:
                static const int kAlignmentTolleranceMultiplier = 1/10;
                static const int kErrorTolleranceMultiplier = 10;
			    static const int kLinearTolleranceMultiplier = 50;
                static const int kImpulsiveTolleranceMultiplier = 500;
                static const int kConstTolleranceMultiplier = 5;
                static const int kMLinearMultiplier = 2;
                static const int kOutOfRangeUnitScaleFactorExp = 24;
                static const int kMConstMultiplier = 4;



            };
            class TcError {
            public:
                static class TcDetect {
                public:
                    static const int kValidDetection = 0;
                    static const int kErr_NoYvals = -1;
                    static const int kErr_NoXvals = -2;
                    static const int kErr_NoMvals = -19;
                    static const int kErr_NoXYvalsRelationship = -5;
                    static const int kErr_UnitScaleFactorOutOfRange = -20;
                    static const int kErr_NoVals = -21;
                    static const int kErr_NoTImes = -22;
                    static const int kErr_NoTimeValsRelationship = -23;
                    static const int kDetectedDeviation = 1;
                    static const int kNoDeviationDetected = -24;
                };
            };

            typedef int (*FcNotifyDeviation)(TcStatisticTestMeasures pStatisticTestMeasures, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation pDeviation);

          

            vector<TcMathDeviationInterval> *cmMathDeviationIntervals;
            vector<TcMathDeviation> *cmDeviations;
            FcNotifyDeviation fNotify;

            TcMathDeviationDetector(FcNotifyDeviation pNotify);
            TcMathDeviationDetector();
            ~TcMathDeviationDetector();

            FcNotifyDeviation fGetNotifyFunction();
            vector<TcMathDeviationInterval> fGetDeviationIntervals();
            vector<TcMathDeviation> fGetDeviations();

            void fSetDeviations(vector<TcMathDeviation> pDeviations);
            void fSetDeviationIntervals(vector<TcMathDeviationInterval> pDeviationIntervals);
            void fSetNotifyFunction(FcNotifyDeviation pNotify);


            void fAddDeviationInterval(Deviation pDeviation, double pMLinear, int pStartIndex, int pEndIndex, chrono::high_resolution_clock::time_point pStartTime, chrono::high_resolution_clock::time_point pEndTime);
            void fAddDeviation(DeviationDistribution pDeviation, double pMLinear, double pDevVal, double pNormVal, int pStartLinearIndex, int pEndLinearIndex, chrono::high_resolution_clock::time_point pStartTime, chrono::high_resolution_clock::time_point pEndTime);
            
            int fDetect(TcStatisticTestMeasures pStatisticTestMeasures, double pUnitScaleFactor, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long>  pTimes, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation>* pDeviations);
            bool fDetectDeviation(int pUnitScaleFactor, TcStatisticTestMeasures pStatisticTestMeasures, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, TcMathDeviation* pDeviation);

            static bool fShift(int pUnitScaleFactor, vector<TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, TcMathDeviation* pDeviation);
            static bool fDrop(int pUnitScaleFactor, vector<TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, TcMathDeviation* pDeviation);
            static bool fGlitch(int pUnitScaleFactor, vector<TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, TcMathDeviation* pDeviation);
            static bool fDrift(int pUnitScaleFactor, vector<TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, TcMathDeviation* pDeviation);
            static bool fUnknown(vector<TcMathDeviationInterval> pDetectedIntervals, TcMathDeviation* pDeviation);
    };
    
    static int fTranspose(vector<vector<double>> pMatrix, vector<vector<double>>* pTransposed);
    static int fMultiplyMatrix(vector<vector<double>> pMatrixA, vector<vector<double>> pMatrixB, vector<vector<double>>* pMultiply, int pDimension);
    static int fMultiplyMatrixVector(vector<vector<double>> pMatrix, vector<double> pVector, vector<double>* pMultiply);
    static int fGetCofactor(vector<vector<double>> pMatrix, vector<vector<double>>* pCofactor, int pExcludedRow, int pExcludedCol);
    static double fDeterminant(vector<vector<double>> pMatrix, int pDimension);
    static int fAdJoint(vector<vector<double>> pMatrix, vector<vector<double>>* pADJoint);
    static int fInverse(vector<vector<double>> pMatrix, vector<vector<double>>* pInverse);
    template<class X, class Y>
    static int fDerivate(vector<Y> pYvalues, vector<X> pXvalues, vector<X>* pMcoefficients);

};



template<class X, class Y>
int TcMathSupporter::fDerivate(vector<Y> pYvalues, vector<X> pXvalues, vector<X>* pMcoefficients) {

    if (&pYvalues == nullptr || pYvalues.size() < 1) {
        return(TcError::TcDerivate::kErr_NoYvals);
    }
    if (&pXvalues == nullptr || pXvalues.size() < 1) {
        return(TcError::TcDerivate::kErr_NoXvals);
    }
    else if (pYvalues.size() != pXvalues.size()) {
        return(TcError::TcDerivate::kErr_NoXYvalsRelationship);
    }

    int rNvalues = pXvalues.size();
    vector<double> cMcoefficents(rNvalues - 1);

    for (int i = 0; i < rNvalues - 1; i++) {
        cMcoefficents[i] = (pXvalues[i + 1] - pXvalues[i]) / (pYvalues[i + 1] - pYvalues[i]);
    }

    *pMcoefficients = cMcoefficents;
    return (TcError::TcDerivate::kValidDerivate);
}

#endif