#pragma once

#ifndef TCLINEARREGRESSOR_H
#define TCLINEARREGRESSOR_H

#include <vector>
#include <chrono>
#include <stdio.h>
#include <exception>

//y = a0 + a1*x

using namespace std;

template <class x, class y>
class TcLinearRegressor {
    private:
        vector<x> rmX;
        vector<y> rmY;
        double rmA0;
        double rmA1;
    
    public:
    class TcError {
    public:
        class TcPrediction {
        public:
            static const int kValidPrediction = 0;
            static const int kErr_NoWeights = -1;
            static const int kErr_NoWeightsFeaturesRelationship = -2;
            static const int kErr_NoFeatures = -3;
        };
        class TcTraining {
        public:
            static const int kValidTraining = 0;
            static const int kErr_NoLabels = -4;
            static const int kErr_NoLabelsFeaturesRelationship = -5;
            static const int kErr_NoFeatures = -3;
            static const int kErr_MathMultiply = -6;
            static const int kErr_MathMultiplyMatVect = -7;
            static const int kErr_MathInverse = -8;
            static const int kErr_MathTranspose = -9;
        };
    };

    TcLinearRegressor() {}
    ~TcLinearRegressor() {}

    void fTrain(vector<x> pX, vector<y> pY) {
        double sumX;
        double sumX2;
        double sumY;
        double sumXY;
        int n = pX.size();
        
        if(n != pY.size()){
            __throw_runtime_error("Y and X sizes are different");
        }

        this->rmX = pX;
        this->rmY = pY;

        for(int i=0;i<n;i++){
            sumX = sumX + pX[i];
            sumX2 = sumX2 + pX[i]*pX[i];
            sumY = sumY + pY[i];
            sumXY = sumXY + pX[i] * ((x) pY[i]);
        }

        /* Calculating a and b */
        this->rmA1 = (double) (n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX);
        this->rmA0 = (double) (sumY - this->rmA1*sumX)/n;
        
        fprintf(stdout, "(%s) Calculated value of A0 is %f and A1 is %f\n", __func__, this->rmA0, this->rmA1);
		fflush(stdout);

        fprintf(stdout, "(%s) Equation of best fit is: y = %f + %fx\n", __func__, this->rmA0, this->rmA1);
		fflush(stdout);
    }
    
    void fPredict(x pFeature, y* pPredictedValue){
        *pPredictedValue = (y) (this->rmA0 + this->rmA1*pFeature);
    }
};



#endif

