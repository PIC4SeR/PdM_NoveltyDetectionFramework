#pragma once

#ifndef TCREGRESSOR_H
#define TCREGRESSOR_H

#include <vector>
#include <chrono>
#include "TcMathSupporter.h"

//predicted = L(W, X) = W0 + W1*X1 + W2*X2 + ... + Wn*Xn


using namespace std;


                
class TcRegressor {

private:

    
    class TcValidation {
    public:
        chrono::system_clock::time_point cmValidationStart;
        chrono::system_clock::time_point cmValidationEnd;
        vector<double> cmPredictedLabels;
        double rmSST;
        double rmSSR;
        double rmSSE;
        double rmRSquared;

        TcValidation(vector<double> pPredictedLabels, double pSST, double pSSR, double pSSE, double pRSquared) {
            this->cmPredictedLabels = pPredictedLabels;
            this->rmSST = pSST;
            this->rmSSR = pSSR;
            this->rmSSE = pSSE;
            this->rmRSquared = pRSquared;
        }

        double fGetSST() {
            return(this->rmSST);
        }
        double fGetSSR() {
            return(this->rmSSR);
        }

        double fGetSSE() {
            return(this->rmSSE);
        }

        vector<double> fGetPredictedLabels() {
            return(this->cmPredictedLabels);
        }

    };

    
    class TcTraining {
    public:
        int rmExitStatus;
        chrono::system_clock::time_point cmTrainingStart;
        chrono::system_clock::time_point cmTrainingEnd;
        vector<vector<double>> cmTrainingFeatures;
        vector<double> cmTrainingLabels;
        vector<double> cmWeights;

        TcTraining(vector<vector<double>> pTrainingFeatures, vector<double> pTrainingLabels) {
            this->cmTrainingLabels = pTrainingLabels;
            this->cmTrainingFeatures = pTrainingFeatures;
            this->rmExitStatus = TcError::TcTraining::kValidTraining;
            this->cmTrainingStart = chrono::system_clock::now();
        }

        inline void fSetLearnedWeights(vector<double> pWeights) {
            this->cmWeights = pWeights;
            this->cmTrainingEnd = chrono::system_clock::now();
        }
        inline vector<double> fGetTrainingLabels() {
            return(this->cmTrainingLabels);
        }
        inline vector<double> fGetWeights() {
            return(this->cmWeights);
        }
        inline vector<vector<double>> fGetTrainingFeatures() {
            return(this->cmTrainingFeatures);
        }
        inline int fGetExitStatus() {
            return(this->rmExitStatus);
        }

    };

    
    class TcPrediction {
    public:
        chrono::system_clock::time_point cmPredictionStart;
        chrono::system_clock::time_point cmPredictionEnd;
        vector<double> cmPredictionFeatures;
        double rmPredictedLabel;
        
        vector<double> cmWeights;
        int rmExitStatus;

        TcPrediction() {
            ;
        }



        TcPrediction(vector<double> pPredictionFeatures, vector<double> pWeights) {
            this->cmPredictionFeatures = pPredictionFeatures;
            this->cmWeights = pWeights;
            this->cmPredictionStart = chrono::system_clock::now();
            this->rmExitStatus = TcError::TcPrediction::kValidPrediction;
            this->rmPredictedLabel = 0;
        }



        inline void fSetPredictedLabel(double pPredictedLabel) {
            this->rmPredictedLabel = pPredictedLabel;
            this->cmPredictionEnd = chrono::system_clock::now();
        }


        inline double fGetPredictedLabel() {
            return(this->rmPredictedLabel);
        }

        inline vector<double> fGetWeights() {
            return(this->cmWeights);
        }

        inline vector<double> fGetPredictionFeatures() {
            return(this->cmPredictionFeatures);
        }

        inline int fGetExitStatus() {
            return(this->rmExitStatus);
        }
    };


    vector<double>* cmWeights;
    vector<vector<double>>* cmTrainingFeatures;
    vector<double>* cmTrainingLabels;
    vector<double>* cmPredictedLabels;
    double rmSST;
    double rmSSR;
    double rmSSE;
    double rmRSquared;

    vector<TcValidation*>* cmValidations;
    vector<TcTraining*>* cmTrainings;
    vector<TcPrediction*>* cmPredictions;


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
        class TcModel {
        public:
            static const int kValidModel = 0;
            static const int kErr_NoOrder = -1;
            static const int kErr_NoTrainingSet = -2;
            static const int kErr_NoSquareMatrixableVector = -10;
        };
        class TcValidation {
        public:
            static const int kErr_NoOrder = -1;
            static const int kErr_NoTrainingSet = -2;
        };

    };

    TcRegressor() {
        this->cmTrainings = new vector<TcTraining*>();
        this->cmPredictions = new vector<TcPrediction*>();
    }
    ~TcRegressor() {
        if (this->cmTrainings != nullptr) {
            delete this->cmTrainings;
            this->cmTrainings = nullptr;
        }
        if (this->cmPredictions != nullptr) {
            delete this->cmPredictions;
            this->cmPredictions = nullptr;
        }
        if (this->cmWeights != nullptr) {
            delete this->cmWeights;
            this->cmWeights = nullptr;
        }
        if (this->cmTrainingFeatures != nullptr) {
            delete this->cmTrainingFeatures;
            this->cmTrainingFeatures = nullptr;
        }
        if (this->cmTrainingLabels != nullptr) {
            delete this->cmTrainingLabels;
            this->cmTrainingLabels = nullptr;
        }
        if (this->cmPredictedLabels != nullptr) {
            delete this->cmPredictedLabels;
            this->cmPredictedLabels = nullptr;
        }
    }

    inline vector<double> fGetWeight() {
        return(*(this->cmWeights));
    }
    inline double fGetRSquared() {
        return(this->rmRSquared);
    }
    inline vector<double> fGetTrainingLabels() {
        return(*(this->cmTrainingLabels));
    }
    inline vector<vector<double>> fGetTrainingFeatures() {
        return(*(this->cmTrainingFeatures));
    }
    inline int fModel(vector<double> pTrainingLabelsSet, vector<double> pTrainingFeaturesSet, int pOrder) {

        if (pOrder < 1) {
            return(TcError::TcModel::kErr_NoOrder);
        }


        if (pTrainingFeaturesSet.size() < 0) {
            return(TcError::TcModel::kErr_NoTrainingSet);
        }

        if (pTrainingFeaturesSet.size() - pOrder < pOrder) {
            return(TcError::TcModel::kErr_NoSquareMatrixableVector);
        }

        vector<vector<double>> cTrainingFeatures(pTrainingFeaturesSet.size() - pOrder);

        for (int i = 0; i < pTrainingFeaturesSet.size() - pOrder; i++) {
            cTrainingFeatures[i] = vector<double>(pOrder);
        }

        for (int i = 0; i < pTrainingFeaturesSet.size() - pOrder; i++) {
            cTrainingFeatures[i][0] = 1;
            int j = 0;
            for (j = i; j < i + pOrder; j++) {
                cTrainingFeatures[i][j - i + 1] = pTrainingFeaturesSet[j];
            }
        }

        *(this->cmTrainingFeatures) = cTrainingFeatures;
        *(this->cmTrainingLabels) = pTrainingLabelsSet;
        return(TcError::TcModel::kValidModel);
    }
    inline int fModel(vector<double> pTrainingSet, int pOrder) {

        if (pOrder < 1) {
            return(TcError::TcModel::kErr_NoOrder);
        }


        if (pTrainingSet.size() < 0) {
            return(TcError::TcModel::kErr_NoTrainingSet);
        }

        if (pTrainingSet.size() - pOrder < pOrder) {
            return(TcError::TcModel::kErr_NoSquareMatrixableVector);
        }

        vector<vector<double>> cTrainingFeatures(pTrainingSet.size() - pOrder);
        vector<double> cTrainingLabels(pTrainingSet.size() - pOrder);

        for (int i = 0; i < pTrainingSet.size() - pOrder; i++) {
            cTrainingFeatures[i] = vector<double>(pOrder);
        }

        for (int i = 0; i < pTrainingSet.size() - pOrder; i++) {
            cTrainingFeatures[i][0] = 1;
            int j = 0;
            for (j = i; j < i + pOrder; j++) {
                cTrainingFeatures[i][j - i + 1] = (double)pTrainingSet[j];
            }
            cTrainingLabels[i] = (double)pTrainingSet[j];
        }

        *this->cmTrainingFeatures = vector<vector<double>>(cTrainingFeatures);
        *this->cmTrainingLabels = vector<double>(cTrainingLabels);
        return(TcError::TcModel::kValidModel);
    }
    inline int fTrain() {


        TcTraining* cTraining = new TcTraining(*(this->cmTrainingFeatures), *(this->cmTrainingLabels));
        this->cmTrainings->push_back(cTraining);

        if (this->cmTrainingFeatures == nullptr || this->cmTrainingFeatures->size() < 1) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_NoFeatures;
            return(TcError::TcTraining::kErr_NoFeatures);
        }
        else if (this->cmTrainingLabels == nullptr || this->cmTrainingLabels->size() < 1) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_NoLabels;
            return(TcError::TcTraining::kErr_NoLabels);
        }
        else if (this->cmTrainingLabels->size() != this->cmTrainingFeatures->size()) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_NoLabelsFeaturesRelationship;
            return(TcError::TcTraining::kErr_NoLabelsFeaturesRelationship);
        }

        vector<double> cWeights;

        vector<vector<double>> cX;
        copy(this->cmTrainingFeatures->begin(), this->cmTrainingFeatures->end(), cX.begin());


        vector<double> cY;
        copy(this->cmTrainingLabels->begin(), this->cmTrainingLabels->end(), cY.begin());

        vector<vector<double>> cXT;
        vector<vector<double>> cXXT;
        vector<vector<double>> cXXT_Inv;
        vector<vector<double>> cXXTInvXT;

        TcMathSupporter::fTranspose(cX, &cXT);
        if (&cXT == nullptr) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_MathTranspose;
            this->cmWeights = nullptr;
            return(TcError::TcTraining::kErr_MathTranspose);
        }

        TcMathSupporter::fMultiplyMatrix(cX, cXT, &cXXT, cX.size());
        if (&cXXT == nullptr) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_MathMultiply;
            this->cmWeights = nullptr;
            return(TcError::TcTraining::kErr_MathMultiply);
        }

        TcMathSupporter::fInverse(cXXT, &cXXT_Inv);
        if (&cXXT_Inv == nullptr) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_MathInverse;
            cTraining->fSetLearnedWeights(vector<double>());
            this->cmWeights = nullptr;
            return(TcError::TcTraining::kErr_MathInverse);
        }

        TcMathSupporter::fMultiplyMatrix(cXXT_Inv, cXT, &cXXTInvXT, cXXT_Inv.size());
        if (&cXXTInvXT == nullptr) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_MathMultiply;
            cTraining->fSetLearnedWeights(vector<double>());
            this->cmWeights = nullptr;
            return(TcError::TcTraining::kErr_MathMultiply);
        }


        TcMathSupporter::fMultiplyMatrixVector(cXXTInvXT, cY, &cWeights);
        if (&cWeights == nullptr) {
            cTraining->rmExitStatus = TcError::TcTraining::kErr_MathMultiplyMatVect;
            cTraining->fSetLearnedWeights(vector<double>());
            this->cmWeights = nullptr;
            return(TcError::TcTraining::kErr_MathMultiplyMatVect);
        }

        cTraining->rmExitStatus = TcError::TcTraining::kValidTraining;
        cTraining->fSetLearnedWeights(cWeights);
        *this->cmWeights = vector<double>(cWeights);
        return(TcError::TcTraining::kValidTraining);
    }
    inline void fValidate() {
        double rSum = 0;
        for (int i = 0; i < this->cmTrainingLabels->size(); i++) {
            rSum += (*this->cmTrainingLabels)[i];
        }

        double rYMean = rSum / ((double)this->cmTrainingLabels->size());
        for (int i = 0; i < this->cmTrainingLabels->size(); i++) {
            double rYpredict = 0;
            if (fPredict((*(this->cmTrainingFeatures))[i], &rYpredict) > 0) {
                (*this->cmPredictedLabels)[i] = rYpredict;
                this->rmSST += pow((*this->cmTrainingLabels)[i] - rYMean, 2);
                this->rmSSR += pow((*this->cmPredictedLabels)[i] - rYMean, 2);
                this->rmSSE += pow((*this->cmTrainingLabels)[i] - (*this->cmPredictedLabels)[i], 2);
            }
        }

        this->rmRSquared = this->rmSSR / this->rmSST;
        this->cmValidations->push_back(new TcValidation(*(this->cmPredictedLabels), this->rmSST, this->rmSSR, this->rmSSE, this->rmRSquared));
    }
    inline int fPredict(vector<double> pFeatures, double* pPredictedvalue) {
        int i, j, n = 0;
        double cYpredict;


        TcPrediction cPrediction;/* = new TcRegressor<L, F>::TcPrediction<L, F>();*/
        this->cmPredictions->push_back(&cPrediction);

        if (pFeatures.size() < 1) {
            pPredictedvalue = nullptr;
            cPrediction.rmExitStatus = TcError::TcPrediction::kErr_NoFeatures;
            cPrediction.fSetPredictedLabel(cYpredict);
            return(TcError::TcPrediction::kErr_NoFeatures);
        }
        else if (this->cmWeights == nullptr || this->cmWeights->size() < 1) {
            pPredictedvalue = nullptr;
            cPrediction.rmExitStatus = TcError::TcPrediction::kErr_NoWeights;
            cPrediction.fSetPredictedLabel(cYpredict);
            return(TcError::TcPrediction::kErr_NoWeights);
        }
        else if (this->cmWeights->size() + 1 != pFeatures.size()) {
            pPredictedvalue = nullptr;
            cPrediction.rmExitStatus = TcError::TcPrediction::kErr_NoWeightsFeaturesRelationship;
            cPrediction.fSetPredictedLabel(cYpredict);
            return(TcError::TcPrediction::kErr_NoWeightsFeaturesRelationship);
        }

        cYpredict = (double)(*(this->cmWeights))[0];
        for (i = 0; i < pFeatures.size(); i++) {
            cYpredict += (((double)(*(this->cmWeights))[i + 1]) * ((double)pFeatures[i]));
        }

        cPrediction.fSetPredictedLabel(cYpredict);
        cPrediction.rmExitStatus = TcError::TcPrediction::kValidPrediction;
        pPredictedvalue = &cYpredict;
        return(TcError::TcPrediction::kValidPrediction);

    }
};



#endif

