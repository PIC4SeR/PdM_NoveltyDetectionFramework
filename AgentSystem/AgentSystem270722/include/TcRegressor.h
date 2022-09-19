#pragma once

#ifndef TCREGRESSOR_H
#define TCREGRESSOR_H

#include <vector>
#include <chrono>
#include "TcMathSupporter.h"


namespace Spea {
    namespace Archimede {
        namespace MultiAgentSystem {
            namespace Tools {

                //predicted = L(W, X) = W0 + W1*X1 + W2*X2 + ... + Wn*Xn


                using namespace std;


                template <typename L, typename F> class TcRegressor {
                private:

                    template <typename L, typename F>
                    class TcValidation {
                    public:
                        chrono::system_clock::time_point cmValidationStart;
                        chrono::system_clock::time_point cmValidationEnd;
                        vector<L> cmPredictedLabels;
                        L rmSST;
                        L rmSSR;
                        L rmSSE;
                        L rmRSquared;

                        TcValidation(vector<L> pPredictedLabels, L pSST, L pSSR, L pSSE, L pRSquared) {
                            this->cmPredictedLabels = pPredictedLabels;
                            this->rmSST = pSST;
                            this->rmSSR = pSSR;
                            this->rmSSE = pSSE;
                            this->rmRSquared = pRSquared;
                        }

                        L fGetSST() {
                            return(this->rmSST);
                        }
                        L fGetSSR() {
                            return(this->rmSSR);
                        }

                        double fGetSSE() {
                            return(this->rmSSE);
                        }

                        vector<vector<L>> fGetPredictedLabels() {
                            return(this->cmPredictedLabels);
                        }

                    };

                    template <typename L, typename F> class TcTraining {
                    public:
                        int rmExitStatus;
                        chrono::system_clock::time_point cmTrainingStart;
                        chrono::system_clock::time_point cmTrainingEnd;
                        vector<vector<F>> cmTrainingFeatures;
                        vector<L> cmTrainingLabels;
                        vector<F> cmWeights;

                        TcTraining(vector<vector<F>> pTrainingFeatures, vector<L> pTrainingLabels) {
                            this->cmTrainingLabels = pTrainingLabels;
                            this->cmTrainingFeatures = pTrainingFeatures;
                            this->rmExitStatus = TcError::TcTraining::kValidTraining;
                            this->cmTrainingStart = chrono::system_clock::now();
                        }

                        inline void fSetLearnedWeights(vector<F> pWeights) {
                            this->cmWeights = pWeights;
                            this->cmTrainingEnd = chrono::system_clock::now();
                        }
                        inline vector<L> fGetTrainingLabels() {
                            return(this->cmTrainingLabels);
                        }
                        inline vector<F> fGetWeights() {
                            return(this->cmWeights);
                        }
                        inline vector<vector<F>> fGetTrainingFeatures() {
                            return(this->cmTrainingFeatures);
                        }
                        inline int fGetExitStatus() {
                            return(this->rmExitStatus);
                        }

                    };

                    template <typename L, typename F> class TcPrediction {
                    public:
                        chrono::system_clock::time_point cmPredictionStart;
                        chrono::system_clock::time_point cmPredictionEnd;
                        vector<F> cmPredictionFeatures;
                        L rmPredictedLabel;
                        vector<F> cmWeigths;
                        int rmExitStatus;

                        TcPrediction() {
                            ;
                        }



                        TcPrediction(vector<F> pPredictionFeatures, vector<F> pWeights) {
                            this->cmPredictionFeatures = pPredictionFeatures;
                            this->cmWeights = pWeights;
                            this->cmPredictionStart = chrono::system_clock::now();
                            this->rmExitStatus = TcError::TcPrediction::kValidPrediction;
                            this->rmPredictedLabel = 0;
                        }



                        inline void fSetPredictedLabel(L pPredictedLabel) {
                            this->rmPredictedLabel = pPredictedLabel;
                            this->cmPredictionEnd = chrono::system_clock::now();
                        }


                        inline L fGetPredictedLabel() {
                            return(this->rmPredictedLabel);
                        }

                        inline vector<F> fGetWeights() {
                            return(this->cmWeights);
                        }

                        inline vector<F> fGetPredictionFeatures() {
                            return(this->cmPredictionFeatures);
                        }

                        inline int fGetExitStatus() {
                            return(this->rmExitStatus);
                        }
                    };


                    vector<F>* cmWeights;
                    vector<vector<F>>* cmTrainingFeatures;
                    vector<L>* cmTrainingLabels;
                    vector<L>* cmPredictedLabels;
                    L rmSST;
                    L rmSSR;
                    L rmSSE;
                    L rmRSquared;

                    vector<TcValidation<L, F>*>* cmValidations;
                    vector<TcTraining<L, F>*>* cmTrainings;
                    vector<TcPrediction<L, F>*>* cmPredictions;


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
                        this->cmTrainings = new vector<TcTraining<L, F>*>();
                        this->cmPredictions = new vector<TcPrediction<L, F>*>();
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

                    inline vector<F> fGetWeight() {
                        return(this->cmWeights);
                    }
                    inline L fGetRSquared() {
                        return(this->rmRSquared);
                    }
                    inline vector<L> fGetTrainingLabels() {
                        return(this->cmTrainingLabels);
                    }
                    inline vector<vector<F>> fGetTrainingFeatures() {
                        return(this->cmTrainingFeatures);
                    }
                    inline int fModel(vector<L> pTrainingLabelsSet, vector<F> pTrainingFeaturesSet, int pOrder) {

                        if (pOrder < 1) {
                            return(TcError::TcModel::kErr_NoOrder);
                        }


                        if (pTrainingFeaturesSet.size() < 0) {
                            return(TcError::TcModel::kErr_NoTrainingSet);
                        }

                        if (pTrainingFeaturesSet.size() - pOrder < pOrder) {
                            return(TcError::TcModel::kErr_NoSquareMatrixableVector);
                        }

                        vector<vector<F>> cTrainingFeatures(pTrainingFeaturesSet.size() - pOrder);

                        for (int i = 0; i < pTrainingFeaturesSet.size() - pOrder; i++) {
                            cTrainingFeatures[i] = vector<F>(pOrder);
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
                    inline int fModel(vector<F> pTrainingSet, int pOrder) {

                        if (pOrder < 1) {
                            return(TcError::TcModel::kErr_NoOrder);
                        }


                        if (pTrainingSet.size() < 0) {
                            return(TcError::TcModel::kErr_NoTrainingSet);
                        }

                        if (pTrainingSet.size() - pOrder < pOrder) {
                            return(TcError::TcModel::kErr_NoSquareMatrixableVector);
                        }

                        vector<vector<F>> cTrainingFeatures(pTrainingSet.size() - pOrder);
                        vector<L> cTrainingLabels(pTrainingSet.size() - pOrder);

                        for (int i = 0; i < pTrainingSet.size() - pOrder; i++) {
                            cTrainingFeatures = vector<F>(pOrder);
                        }

                        for (int i = 0; i < pTrainingSet.size() - pOrder; i++) {
                            cTrainingFeatures[i][0] = 1;
                            int j = 0;
                            for (j = i; j < i + pOrder; j++) {
                                cTrainingFeatures[i][j - i + 1] = (F)pTrainingSet[j];
                            }
                            cTrainingLabels[i] = (L)pTrainingSet[j];
                        }

                        *this->cmTrainingFeatures = vector<F>(cTrainingFeatures);
                        *this->cmTrainingLabels = vector<L>(cTrainingLabels);
                        return(TcError::TcModel::kValidModel);
                    }
                    inline int fTrain() {


                        TcRegressor::TcTraining<L, F>* cTraining = new TcRegressor::TcTraining<L, F>(*(this->cmTrainingFeatures), *(this->cmTrainingLabels));
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

                        vector<F> cWeights;

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
                            cTraining->fSetLearnedWeights(vector<F>());
                            this->cmWeights = nullptr;
                            return(TcError::TcTraining::kErr_MathInverse);
                        }

                        TcMathSupporter::fMultiplyMatrix(cXXT_Inv, cXT, &cXXTInvXT, cXXT_Inv.size());
                        if (&cXXTInvXT == nullptr) {
                            cTraining->rmExitStatus = TcError::TcTraining::kErr_MathMultiply;
                            cTraining->fSetLearnedWeights(vector<F>());
                            this->cmWeights = nullptr;
                            return(TcError::TcTraining::kErr_MathMultiply);
                        }


                        TcMathSupporter::fMultiplyMatrixVector(cXXTInvXT, cY, &cWeights);
                        if (&cWeights == nullptr) {
                            cTraining->rmExitStatus = TcError::TcTraining::kErr_MathMultiplyMatVect;
                            cTraining->fSetLearnedWeights(vector<F>());
                            this->cmWeights = nullptr;
                            return(TcError::TcTraining::kErr_MathMultiplyMatVect);
                        }

                        cTraining->rmExitStatus = TcError::TcTraining::kValidTraining;
                        cTraining->fSetLearnedWeights(cWeights);
                        *this->cmWeights = vector<F>(cWeights);
                        return(TcError::TcTraining::kValidTraining);
                    }
                    inline void fValidate() {
                        L rSum = 0;
                        for (int i = 0; i < this->cmTrainingLabels->size(); i++) {
                            rSum += (*this->cmTrainingLabels)[i];
                        }

                        L rYMean = rSum / ((L)this->cmTrainingLabels->size());
                        for (int i = 0; i < this->cmTrainingLabels->size(); i++) {
                            L rYpredict = 0;
                            if (fPredict((*(this->cmTrainingFeatures))[i], &rYpredict) > 0) {
                                (*this->cmPredictedLabels)[i] = rYpredict;
                                this->rmSST += pow((*this->cmTrainingLabels)[i] - rYMean, 2);
                                this->rmSSR += pow((*this->cmPredictedLabels)[i] - rYMean, 2);
                                this->rmSSE += pow((*this->cmTrainingLabels)[i] - (*this->cmPredictedLabels)[i], 2);
                            }
                        }

                        this->rmRSquared = this->rmSSR / this->rmSST;
                        this->cmValidations->push_back(new TcValidation<L, F>(*(this->cmPredictedLabels), this->rmSST, this->rmSSR, this->rmSSE, this->rmRSquared));
                    }
                    inline int fPredict(vector<F> pFeatures, L* pPredictedvalue) {
                        int i, j, n = 0;
                        L cYpredict;


                        TcPrediction<L, F> cPrediction;/* = new TcRegressor<L, F>::TcPrediction<L, F>();*/
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

                        cYpredict = (L)(*(this->cmWeights))[0];
                        for (i = 0; i < pFeatures.size(); i++) {
                            cYpredict += (((L)(*(this->cmWeights))[i + 1]) * ((L)pFeatures[i]));
                        }

                        cPrediction.fSetPredictedLabel(cYpredict);
                        cPrediction.rmExitStatus = TcError::TcPrediction::kValidPrediction;
                        pPredictedvalue = new L(cYpredict);
                        return(TcError::TcPrediction::kValidPrediction);

                    }
                };
            }
        }
    }
}



#endif

