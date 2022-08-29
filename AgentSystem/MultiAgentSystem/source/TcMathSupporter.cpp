#include <math.h>
#include <stdexcept>
	
#include "../include/TcMathSupporter.h"
#include "../include/TcRegressor.h"


using namespace std;

#pragma region TcMathSupporter
int TcMathSupporter::fTranspose(vector<vector<double>> pMatrix, vector<vector<double>>* pTransposed) {

    int rNrow = 0;
    int rNcol = 0;

    if (pMatrix.size() <= 0) {
        pTransposed = nullptr;
        return(TcError::TcTranspose::kErr_NoRows);
    }
    else {
        rNrow = pMatrix.size();
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() <= 0) {
            pTransposed = nullptr;
            return(TcError::TcTranspose::kErr_NoCols);
        }
        else if (i == 0) {
            rNcol = (int) pMatrix[i].size();
        }
        else if (pMatrix[i].size() != rNcol) {
            pTransposed = nullptr;
            return(TcError::TcTranspose::kErr_NoConstCols);
        }
    }

    vector<vector<double>> cTransposed(rNcol);
    for (int row = 0; row < rNrow; row++) {
        cTransposed[row] = vector<double>(rNrow);
    }

    for (int row = 0; row < rNrow; row++) {
        for (int col = 0; col < rNcol; col++) {
            cTransposed[col][row] = pMatrix[row][col];
        }
    }

    *pTransposed = cTransposed;
    return(TcError::TcTranspose::kValidTranspose);
}
int TcMathSupporter::fMultiplyMatrix(vector<vector<double>> pMatrixA, vector<vector<double>> pMatrixB, vector<vector<double>>* pMultiply, int pDimension) {

    int rNrowA = 0;
    int rNcolA = 0;

    if (pMatrixA.size() <= 0) {
        return(TcError::TcMultiply::kErr_NoRows);
    }
    else {
        rNrowA = pMatrixA.size();
    }

    for (int i = 0; i < rNrowA; i++) {
        if (pMatrixA[i].size() <= 0) {
            return(TcError::TcMultiply::kErr_NoCols);
        }
        else if (i == 0) {
            rNcolA = pMatrixA[i].size();
        }
        else if (pMatrixA[i].size() != rNcolA) {
            return(TcError::TcMultiply::kErr_NoConstCols);
        }
    }

    int rNrowB = 0;
    int rNcolB = 0;

    if (pMatrixB.size() <= 0) {
        return(TcError::TcMultiply::kErr_NoRows);
    }
    else {
        rNrowB = pMatrixB.size();
    }

    for (int i = 0; i < rNrowB; i++) {
        if (pMatrixB[i].size() <= 0) {
            return(TcError::TcMultiply::kErr_NoCols);
        }
        else if (i == 0) {
            rNcolB = pMatrixB[i].size();
        }
        else if (pMatrixB[i].size() != rNcolB) {
            return(TcError::TcMultiply::kErr_NoConstCols);
        }
    }


    if (rNcolA != rNrowB) {
        return(TcError::TcMultiply::kErr_NoRowColRelationship);
    }

    vector<vector<double>> cMultiply(rNrowA);
    for (int row = 0; row < rNrowA; row++) {
        cMultiply[row] = vector<double>(rNcolB);
    }

    for (int i = 0; i < rNrowA; i++) {
        for (int k = 0; k < rNcolB; k++) {
            for (int j = 0; j < rNcolA; j++) {
                cMultiply[i][k] += pMatrixA[i][j] * pMatrixB[j][k];
            }
        }
    }

    *pMultiply = cMultiply;
    return(TcError::TcMultiply::kValidMultiply);
}
int TcMathSupporter::fMultiplyMatrixVector(vector<vector<double>> pMatrix, vector<double> pVector, vector<double>* pMultiply) {
    int rNrow = 0;
    int rNcol = 0;

    if (pMatrix.size() <= 0) {
        return(TcError::TcMultiplyMatrixVector::kErr_NoRows);
    }
    else {
        rNrow = pMatrix.size();
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() <= 0) {
            return(TcError::TcMultiplyMatrixVector::kErr_NoCols);
        }
        else if (i == 0) {
            rNcol = (int) pMatrix[i].size();
        }
        else if (pMatrix[i].size() != rNcol) {
            return(TcError::TcMultiplyMatrixVector::kErr_NoConstCols);
        }
    }

    int rNelements = 0;

    if (pVector.size() <= 0) {
        return(TcError::TcMultiplyMatrixVector::kErr_VectNoelements);
    }
    else {
        rNelements = (int) pMatrix.size();
    }

    if (rNelements != rNcol) {
        return(TcError::TcMultiplyMatrixVector::kErr_NoRowColRelationship);
    }


    vector<double> cMultiply(rNrow);

    for (int row = 0; row < rNrow; row++) {
        for (int col = 0; col < rNelements; col++) {
            cMultiply[row] += (pMatrix[row][col] * pVector[col]);
        }
    }

    *pMultiply = cMultiply;
    return(TcError::TcMultiplyMatrixVector::kValidMultiply);
}
int TcMathSupporter::fGetCofactor(vector<vector<double>> pMatrix, vector<vector<double>>* pCofactor, int pExcludedRow, int pExcludedCol) {

    int rNrow = 0;
    int rNcol = 0;

    if (pMatrix.size() - 1 <= 0) {
        return(TcError::TcCofactor::kErr_NoRows);
    }
    else {
        rNrow = pMatrix.size() - 1;
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() - 1 <= 0) {
            return(TcError::TcCofactor::kErr_NoCols);
        }
        else if (i == 0) {
            rNcol = (int) pMatrix[i].size() - 1;
        }
        else if (pMatrix[i].size() - 1 != rNcol) {
            return(TcError::TcCofactor::kErr_NoConstCols);
        }
    }

    if (pExcludedRow < 0 || pExcludedRow > rNrow) {
        return(TcError::TcCofactor::kErr_InvalidExcludeRow);
    }
    else if (pExcludedCol < 0 || pExcludedCol > rNcol) {
        return(TcError::TcCofactor::kErr_InvalidExcludeCol);
    }

    vector<vector<double>> cCofactor(rNrow);
    for (int row = 0; row < rNrow; row++) {
        cCofactor[row] = vector<double>(rNcol);
    }

    for (int row = 0; row < rNrow; row++) {
        for (int col = 0; col < rNcol; col++) {
            if (row != pExcludedRow && col != pExcludedCol) {
                cCofactor[row][col] = pMatrix[row][col];
                col = (col + 1) % (rNcol);
                row = (col == 0 ? row + 1 : row);
            }
        }
    }

    *pCofactor = cCofactor;
    return(TcError::TcCofactor::kValidCofactor);
}
/* Recursive function for finding determinant of matrix.
n is current dimension of pMatrix[][]. */
double TcMathSupporter::fDeterminant(vector<vector<double>> pMatrix, int pDimension)
{
    int rNrow = 0;
    int rNcol = 0;

    if (pMatrix.size() - 1 <= 0) {
        throw runtime_error(to_string(TcError::TcDeterminant::kErr_NoRows).c_str());
    }
    else {
        rNrow = (int) pMatrix.size() - 1;
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() - 1 <= 0) {
            throw runtime_error(to_string(TcError::TcDeterminant::kErr_NoCols).c_str());
        }
        else if (i == 0) {
            rNcol = (int) pMatrix[i].size() - 1;
        }
        else if (pMatrix[i].size() - 1 != rNcol) {
            throw runtime_error(to_string(TcError::TcDeterminant::kErr_NoConstCols).c_str());
        }
    }

    if (rNcol != rNrow) {
        throw runtime_error(to_string(TcError::TcDeterminant::kErr_NoSquareMatrix).c_str());
    }
    else if (rNcol != pDimension) {
        throw runtime_error(to_string(TcError::TcDeterminant::kErr_NoColDimensionRelationship).c_str());
    }



    double rDeterminant = 0; // Initialize result


    vector<vector<double>> cCofactor; // To store cofactors
    int rSign = 1;  // To store sign multiplier 

    // Iterate for each element of first row
    for (int f = 0; f < pDimension; f++) {
        // Getting Cofactor of pMatrix[0][f]                                    
        fGetCofactor(pMatrix, &cCofactor, 0, f);
        rDeterminant += rSign * pMatrix[0][f] * fDeterminant(cCofactor, pDimension - 1);
        // terms are to be added with alternate sign
        rSign = -rSign;
    }

    return(rDeterminant);
}
int TcMathSupporter::fAdJoint(vector<vector<double>> pMatrix, vector<vector<double>>* pADJoint)
{
    int rSign = 1;
    int rNrow = 0;
    int rNcol = 0;
    int rDimension = 0;

    if (pMatrix.size() - 1 <= 0) {
        return(TcError::TcAdJoint::kErr_NoRows);
    }
    else {
        rNrow = (int) pMatrix.size() - 1;
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() - 1 <= 0) {
            return(TcError::TcAdJoint::kErr_NoCols);
        }
        else if (i == 0) {
            rNcol = (int) pMatrix[i].size() - 1;
        }
        else if (pMatrix[i].size() - 1 != rNcol) {
            return(TcError::TcAdJoint::kErr_NoConstCols);
        }
    }


    if (rNcol != rNrow) {
        return(TcError::TcAdJoint::kErr_NoSquareMatrix);
    }
    else {
        rDimension = rNrow;
    }

    vector<vector<double>> cADJoint(rDimension);
    for (int i = 0; i < rDimension; i++) {
        cADJoint[i] = vector<double>(rDimension);
    }

    vector<vector<double>> cCofactor(rDimension - (int)1);
    for (int i = 0; i < rDimension - 1; i++) {
        cCofactor[i] = vector<double>(rDimension - (int)1);
    }

    if (rDimension == 1) {
        cADJoint[0][0] = 1;
        *pADJoint = cADJoint;
        return(TcError::TcAdJoint::kValidADJoint);
    }



    for (int i = 0; i < rDimension; i++) {
        for (int j = 0; j < rDimension; j++) {
            // Get cofactor of pMatrix[i][j]
            fGetCofactor(pMatrix, &cCofactor, i, j);

            // sign of pADJoint[j][i] positive if sum of row
            // and column indexes is even.
            rSign = (((i + j) % 2 == 0) ? 1 : -1);

            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            cADJoint[j][i] = rSign * fDeterminant(cCofactor, rDimension - 1);
        }
    }

    *pADJoint = cADJoint;
    return(TcError::TcAdJoint::kValidADJoint);
}
// Function to calculate and store inverse, returns false if
// matrix is singular
int TcMathSupporter::fInverse(vector<vector<double>> pMatrix, vector<vector<double>>* pInverse) {

    int rNrow = 0;
    int rNcol = 0;
    int rDimension = 0;
    double rDeterminant = 0;

    if (pMatrix.size() - 1 <= 0) {
        return(TcError::TcInverse::kErr_NoRows);
    }
    else {
        rNrow = (int) pMatrix.size() - 1;
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() - 1 <= 0) {
            return(TcError::TcInverse::kErr_NoCols);
        }
        else if (i == 0) {
            rNcol = (int) pMatrix[i].size() - 1;
        }
        else if (pMatrix[i].size() - 1 != rNcol) {
            return(TcError::TcInverse::kErr_NoConstCols);
        }
    }


    if (rNcol != rNrow) {
        return(TcError::TcInverse::kErr_NoSquareMatrix);
    }
    else {
        rDimension = rNrow;
    }


    // Find determinant of pMatrix[][]
    try {
        rDeterminant = fDeterminant(pMatrix, pMatrix.size());
        if (rDeterminant == 0) {
            pInverse = nullptr;
            return(TcError::TcInverse::kErr_DeterminantNull);
        }
    }
    catch (exception e) {
        return(TcError::TcInverse::kErr_DeterminantNull);
    }




    vector<vector<double>> cADJoint(rDimension);
    vector<vector<double>> cInverse(rDimension);

    for (int row = 0; row < rDimension; row++) {
        cADJoint[row] = vector<double>(rDimension);
        cInverse[row] = vector<double>(rDimension);
    }

    if (fAdJoint(pMatrix, &cADJoint) < 0) {
        pInverse = nullptr;
        return(TcError::TcInverse::kErr_ADJointOperationFail);
    }
    else if (&cADJoint == nullptr) {
        pInverse = nullptr;
        return(TcError::TcInverse::kErr_ADJointNull);
    }
    else if (cADJoint.size() <= 0) {
        pInverse = nullptr;
        return(TcError::TcInverse::kErr_ADJointEmpty);
    }


    // Find Inverse using formula "fInverse(pMatrix) = pADJoint(pMatrix)/det(pMatrix)"
    for (int i = 0; i < cADJoint.size(); i++) {
        for (int j = 0; j < cADJoint.size(); j++) {
            cInverse[i][j] = cADJoint[i][j] / rDeterminant;
        }
    }


    *pInverse = cInverse;
    return(TcError::TcInverse::kValidInverse);
}
#pragma endregion


#pragma region TcMathDeviationType
const string TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kShift = "Shift";
const string TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrift = "Drift";
const string TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kGlitch = "Glitch";
const string TcMathSupporter::TcMathDeviationDetector::TcMathDeviationType::kDrop = "Drop";
#pragma endregion


#pragma region TcMathDeviationDetector
int TcMathSupporter::TcMathDeviationDetector::fDetect(vector<double> pMvals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> *pDeviations) {
    if (pMvals.size() <= 0) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoMvals);
    }

    if (pVals.size() <= 0) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoVals);
    }

    if (pTimes.size() <= 0) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoTimes);
    }

    if (pTimes.size() != pVals.size()) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoTimeValsRelationship);
    }

     if (pMTimes.size() != pMvals.size()) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoTimeValsRelationship);
     }

    vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cDeviations;
    double rLinearTollerance = this->rmLinearTollerance;
    double rConstTollerance = this->rmConstTollerance;
    double rErrorTollerance = this->rmErrorTollerance;
    TcMathSupporter::TcMathDeviationDetector cMathDeviationDetector;

    for (int i = 0; i < pMvals.size(); i++) {
        double rMlinear = pMvals[i];
        int j = i + 1;

        TcMathSupporter::TcMathDeviationDetector::Deviation rDeviation;
        
        if (abs(rMlinear)/abs(pVals[i]) < rConstTollerance) {
            while (j < pMvals.size() && abs(pMvals[j]) <= abs(pVals[j]) * rConstTollerance && abs(pMvals[j] - rMlinear) <= abs(rMlinear) * rErrorTollerance) { j++; }
            rDeviation = Deviation::NoDeviation;
        } else if (abs(rMlinear)/abs(pVals[i]) < rLinearTollerance) {
            while (j < pMvals.size() && abs(pMvals[j]) > abs(pVals[j]) * rConstTollerance && abs(pMvals[j]) <= abs(pVals[j]) * rLinearTollerance && abs(pMvals[j] - rMlinear) <= abs(rMlinear) * rErrorTollerance) { j++; }
            rDeviation = Deviation::Linear;
        } else {
            while (j < pMvals.size() && abs(pMvals[j]) > abs(pVals[j]) * rLinearTollerance && abs(pMvals[j] - rMlinear)  <= abs(rMlinear) * rErrorTollerance) { j++; }
            rDeviation = Deviation::Impulsive;
        }

        unsigned long long rStartTimeMilliseconds = pTimes[i];
        unsigned long long rEndTimeMilliseconds = pTimes[j];
        chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime{ chrono::milliseconds(rStartTimeMilliseconds) };
        chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime{ chrono::milliseconds(rEndTimeMilliseconds) };
        this->cmMathDeviationIntervals->push_back(TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval(rDeviation, rMlinear, i, j, cStartTime, cEndTime));
        i = j - 1;
    }

    this->fDetectDeviation(pMvals, pMTimes, pVals, pTimes, &cDeviations);

    *pDeviations = cDeviations;
    return(cDeviations.size() > 0 ? TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kDetectedDeviation : TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kNoDeviationDetected);
}
int TcMathSupporter::TcMathDeviationDetector::fDetectDeviation(vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, vector<TcMathDeviation> *pDeviations){
        vector<TcMathDeviation> cDeviations = *pDeviations;
        TcMathDeviation cDeviation;

       
        if(pMVals.size() <= 0 || pMVals.size() >= 4 * 1024){
            pDeviations = nullptr;
            return(TcError::TcDetect::kErr_NoVals);
        }
    
        if(pTimes.size() <= 0 || pTimes.size() >= 4 * 1024){
            pDeviations = nullptr;
            return(TcError::TcDetect::kErr_NoTimes);
        }

        if (pMTimes.size() <= 0 || pMTimes.size() >= 4 * 1024) {
            pDeviations = nullptr;
            return(TcError::TcDetect::kErr_NoTimes);
        }
    
        if(pTimes.size() != pVals.size()){
            pDeviations = nullptr;
            return(TcError::TcDetect::kErr_NoTimeValsRelationship);
        }

        if (pMTimes.size() != pMVals.size()) {
            pDeviations = nullptr;
            return(TcError::TcDetect::kErr_NoTimeValsRelationship);
        }

        fShift(this->rmSenseFactor, *this->cmMathDeviationIntervals, pMVals, pMTimes, pVals, pTimes, &cDeviations);
        fDrift(this->rmSenseFactor, *this->cmMathDeviationIntervals, pMVals, pMTimes, pVals, pTimes, &cDeviations);
        
        fDrop(this->rmLinearVariationPercentage, this->rmLevelTollerance, this->rmSenseFactor, *this->cmMathDeviationIntervals, pMVals, pMTimes, pVals, pTimes, &cDeviations);
        fGlitch(this->rmLinearVariationPercentage, this->rmLevelTollerance, this->rmSenseFactor, *this->cmMathDeviationIntervals, pMVals, pMTimes, pVals, pTimes, &cDeviations);

        *pDeviations = cDeviations;        
        return(TcError::TcDetect::kValidDetection);
}
chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> TcMathSupporter::TcMathDeviationDetector::fGetLastSampleRead(){
    if(this->cmMathDeviationIntervals == nullptr){
        return(chrono::high_resolution_clock::now());
    }

    if(this->cmMathDeviationIntervals->back().fIsConstant()){
        return(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::nanoseconds(this->cmMathDeviationIntervals->back().fGetEndTime().time_since_epoch().count())));
	} else {
		int rLastSampleMem = this->cmMathDeviationIntervals->size() - 1;
		while(!this->cmMathDeviationIntervals->at(rLastSampleMem--).fIsConstant() && rLastSampleMem >= 0);
		return(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::nanoseconds(this->cmMathDeviationIntervals->at(rLastSampleMem+1).fGetEndTime().time_since_epoch().count())));
	}
}
TcMathSupporter::TcMathDeviationDetector::TcMathDeviationDetector() {
    this->rmSenseFactor = TcDefault::rpSenseFactor;
    this->rmDeltaFactor = TcDefault::rpDeltaFactor;
    this->rmErrorTollerance = TcDefault::rpErrorTollerance;
    this->rmLinearTollerance = TcDefault::rpLinearTollerance;
    this->rmConstTollerance = TcDefault::rpConstTollerance;
    this->rmLevelTollerance = TcDefault::rpLevelTollerance;
    this->rmLinearVariationPercentage = TcDefault::rpLinearVariationPercentage;
    this->rmConstVariationPercentage = TcDefault::rpConstVariationPercentage;
    this->rmImpulsiveVariationPercentage = TcDefault::rpImpulsiveVariationPercentage;
    this->cmMathDeviationIntervals = new vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval>();
    this->cmDeviations = new vector<TcMathDeviation>();
}
TcMathSupporter::TcMathDeviationDetector::TcMathDeviationDetector(double pSenseFactor, double pDeltaFactor, double pErrorTollerance, double pLinearTollerance, double pConstTollerance, double pLevelTollerance, double pConstVariationPercentage, double pLinearVariationPercentage, double pImpulsiveVariationPercentage) {
    this->rmSenseFactor = pSenseFactor;
    this->rmDeltaFactor = pDeltaFactor;
    this->rmErrorTollerance = pErrorTollerance;
    this->rmLinearTollerance = pLinearTollerance;
    this->rmConstTollerance = pConstTollerance;
    this->rmLevelTollerance = pLevelTollerance;
    this->rmLinearVariationPercentage = pLinearVariationPercentage;
    this->rmConstVariationPercentage = pConstVariationPercentage;
    this->rmImpulsiveVariationPercentage = pImpulsiveVariationPercentage;
    this->cmMathDeviationIntervals = new vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval>();
    this->cmDeviations = new vector<TcMathDeviation>();
}
TcMathSupporter::TcMathDeviationDetector::~TcMathDeviationDetector(){
    if(this->cmMathDeviationIntervals != nullptr){
        delete this->cmMathDeviationIntervals;
        this->cmMathDeviationIntervals = nullptr;
    }

    if (this->cmDeviations != nullptr) {
        delete this->cmDeviations;
        this->cmDeviations = nullptr;
    }
}
vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> TcMathSupporter::TcMathDeviationDetector::fGetDeviationIntervals(){
    return(*(this->cmMathDeviationIntervals));
}
vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> TcMathSupporter::TcMathDeviationDetector::fGetDeviations() {
    return(*(this->cmDeviations));
}
void TcMathSupporter::TcMathDeviationDetector::fAddDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution pDeviation, double pMLinear, double pDevVal, double pNormVal, int pStartLinearIndex, int pEndLinearIndex, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pStartTime, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pEndTime, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pDetectionTime){
    this->cmDeviations->push_back(TcMathSupporter::TcMathDeviationDetector::TcMathDeviation(pDeviation, pMLinear, pDevVal, pNormVal, pStartLinearIndex, pEndLinearIndex, pStartTime, pEndTime, pDetectionTime));
}
void TcMathSupporter::TcMathDeviationDetector::fSetDeviations(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> pDeviations) {
    *this->cmDeviations = pDeviations;
}
void TcMathSupporter::TcMathDeviationDetector::fSetDeviationIntervals(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDeviationIntervals) {
    *this->cmMathDeviationIntervals = pDeviationIntervals;
}
void TcMathSupporter::TcMathDeviationDetector::fShift(double pSenseFactor, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation>* pDeviations) {
    if (pDetectedIntervals.size() < kMinShiftSample) {
        return;
    }

    double rValueBeforeShift = 0.00;
    double rValueAfterShift = 0.00;

    double rMImpulsiveBeforeShift = 0.00;
    double rMImpulsiveAfterShift = 0.00;
    double rMImpulsiveOnDescent = 0.00;
    double rMImpulsiveOnAscent = 0.00;

    int rDecreaseFound = 0;
    int rIncreaseFound = 0;


    bool rShiftFound = false;

    vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cDeviations = *pDeviations;
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation cDeviation;

    double rNormVal;
    double rDevVal;
 
    for (int i = 0; i < pDetectedIntervals.size(); i++) {
        double rMlinear = 0.00;
        if (pDetectedIntervals[i].fIsImpulsive(&rMlinear) || (pDetectedIntervals[i].fIsLinear(&rMlinear) && abs(rMlinear) > abs(pVals[pDetectedIntervals[i].fGetEndIndex()])  * pSenseFactor)) {
            if (rMlinear > 0 && !rDecreaseFound) {
                if (!rIncreaseFound) {
                    rNormVal = pVals[pDetectedIntervals[i].fGetStartIndex()];
                    cDeviation.fSetNorm(rNormVal);
                    chrono::milliseconds cStartDuration(pTimes[pDetectedIntervals[i].fGetStartIndex()]);
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime(cStartDuration);
                    cDeviation.fSetStartTime(cStartTime);
                    cDeviation.fSetStartIndex(pDetectedIntervals[i].fGetStartIndex());
                    cDeviation.fSetMLinear(rMlinear);
                    rIncreaseFound = 1;
                }
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                rMImpulsiveOnAscent = rMlinear;
            } else if (rMlinear < 0 && !rIncreaseFound) {
                if (!rDecreaseFound) {
                    rNormVal = pVals[pDetectedIntervals[i].fGetStartIndex()];
                    cDeviation.fSetNorm(rNormVal);
                    chrono::milliseconds cStartDuration(pTimes[pDetectedIntervals[i].fGetStartIndex()]);
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime(cStartDuration);
                    cDeviation.fSetStartTime(cStartTime);
                    cDeviation.fSetStartIndex(pDetectedIntervals[i].fGetStartIndex());
                    cDeviation.fSetMLinear(rMlinear);
                    rDecreaseFound = 1;
                }
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                rMImpulsiveOnDescent = rMlinear;
            }
            else {
                rDecreaseFound = 0;
                rIncreaseFound = 0;
                cDeviation.fClearObject();
            }
        } else if (pDetectedIntervals[i].fIsConstant() || (pDetectedIntervals[i].fIsLinear(&rMlinear) && abs(rMlinear) <= abs(pVals[pDetectedIntervals[i].fGetEndIndex()]) * pSenseFactor)) {
            if (rDecreaseFound || rIncreaseFound) {
                rDevVal = pVals[pDetectedIntervals[i].fGetStartIndex()];
                cDeviation.fSetDev(rDevVal);
                chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetEndIndex() - (int)1]));
                cDeviation.fSetEndTime(cEndTime);
                cDeviation.fSetEndIndex(pDetectedIntervals[i].fGetStartIndex());
                cDeviation.fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Shift);
                cDeviation.fSetDetectionTime(chrono::high_resolution_clock::now());
                rShiftFound = true;
            }
        }

        if (rShiftFound) {
            rIncreaseFound = 0;
            rDecreaseFound = 0;
            rShiftFound = false;
            cDeviations.push_back(cDeviation);
            cDeviation.fClearObject();
        }
    }

    *pDeviations = cDeviations;
    return;
}
void TcMathSupporter::TcMathDeviationDetector::fDrop(double pLinearVariationPercentage, double pLevelTollerance, double pSenseFactor, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation>* pDeviations) {
    
    if (pDetectedIntervals.size() < kMinDropSample) {
        return;
    }

    double rValueBeforeDrop = 0.00;
    double rValueAfterDrop = 0.00;
    double rValueOnDrop = 0.00;

    double rMImpulsiveBeforeDrop = 0.00;
    double rMImpulsiveAfterDrop = 0.00;
    double rMImpulsiveOnAscent = 0.00;
    double rMImpulsiveOnDescent = 0.00;

    double rLinearVariationPercentage = pLinearVariationPercentage;
    double rLevelTollerance = pLevelTollerance;

    int rImpulsiveDecreaseFound = 0;
    int rImpulsiveIncreaseFound = 0;
    int rLinearDecreaseFound = 0;
    int rLinearIncreaseFound = 0;
    bool rDropFound = false;

    vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cDeviations = *pDeviations;
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation cDeviation;

    double rNormVal;
    double rDevVal;
    for (int i = 0; i < pDetectedIntervals.size(); i++) {
        double rMlinear = 0.00;
        if (pDetectedIntervals[i].fIsImpulsive(&rMlinear)) {
            if (rMlinear < 0 && !rImpulsiveIncreaseFound) {
                if (!rImpulsiveDecreaseFound) {
                    rNormVal = pVals[pDetectedIntervals[i].fGetStartIndex()];
                    cDeviation.fSetNorm(rNormVal);
                    chrono::milliseconds cStartDuration(pTimes[pDetectedIntervals[i].fGetStartIndex()]);
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime(cStartDuration);
                    cDeviation.fSetStartTime(cStartTime);
                    cDeviation.fSetStartIndex(pDetectedIntervals[i].fGetStartIndex());
                    cDeviation.fSetMLinear(rMlinear);
                    rImpulsiveDecreaseFound = 1;
                }
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                rMImpulsiveOnDescent = rMlinear;
            }
            else if (rMlinear < 0 && rImpulsiveDecreaseFound) {
                rNormVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                rMImpulsiveOnAscent = rMlinear;
                rImpulsiveIncreaseFound = 1;
                if (fAreMeasuresLeveled(rNormVal, cDeviation.fGetNorm(), rLevelTollerance)) {
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetEndIndex() - (int)1]));
                    cDeviation.fSetEndTime(cEndTime);
                    cDeviation.fSetEndIndex(pDetectedIntervals[i].fGetEndIndex());
                    cDeviation.fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch);
                    cDeviation.fSetDetectionTime(chrono::high_resolution_clock::now());
                    rDropFound = true;
                }
            }
        }
        else if (pDetectedIntervals[i].fIsLinear(&rMlinear) && rImpulsiveDecreaseFound) {
            if (rMlinear < 0 && !rImpulsiveIncreaseFound && abs(rMlinear - rMImpulsiveOnDescent) / rMImpulsiveOnDescent < rLinearVariationPercentage) {
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                rLinearIncreaseFound = 1;
            }
            else if (rMlinear > 0 && (!rImpulsiveIncreaseFound && abs(abs(rMlinear) - rMImpulsiveOnDescent) / rMImpulsiveOnDescent < rLinearVariationPercentage || rImpulsiveIncreaseFound && abs(rMlinear - rMImpulsiveOnAscent) / abs(rMImpulsiveOnAscent) < rLinearVariationPercentage)) {
                rNormVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                rLinearDecreaseFound = 1;
                if (rNormVal < cDeviation.fGetNorm() || fAreMeasuresLeveled(rNormVal, cDeviation.fGetNorm(), rLevelTollerance)) {
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetEndIndex() - 1]));
                    cDeviation.fSetEndTime(cEndTime);
                    cDeviation.fSetEndIndex(pDetectedIntervals[i].fGetEndIndex());
                    cDeviation.fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch);
                    rDropFound = false;
                }
            }
            else {
                rImpulsiveIncreaseFound = 0;
                rImpulsiveDecreaseFound = 0;
                rLinearIncreaseFound = 0;
                rLinearDecreaseFound = 0;
                cDeviation.fClearObject();
            }
        }
        else {
            rImpulsiveIncreaseFound = 0;
            rImpulsiveDecreaseFound = 0;
            rLinearIncreaseFound = 0;
            rLinearDecreaseFound = 0;
            cDeviation.fClearObject();
        }

        if (rDropFound) {
            rImpulsiveIncreaseFound = 0;
            rImpulsiveDecreaseFound = 0;
            rLinearIncreaseFound = 0;
            rLinearDecreaseFound = 0;
            rDropFound = false;
            cDeviations.push_back(cDeviation);
            cDeviation.fClearObject();
        }
    }

    *pDeviations = cDeviations;
    return;

}
bool TcMathSupporter::TcMathDeviationDetector::fAreMeasuresLeveled(double pNormValue, double pDevValue, double pLevelSenseFactor) {
    pLevelSenseFactor = pLevelSenseFactor > 1 ? 1 : (pLevelSenseFactor < 0 ? 0.0001 : pLevelSenseFactor);
    return(abs(pNormValue - pDevValue) /abs(pDevValue) <= pLevelSenseFactor);
}
bool TcMathSupporter::TcMathDeviationDetector::fAreDerivatesSimilar(double pDerivateValue1, double pDerivateValue2, double pDerivatesSimilarity) {
    pDerivatesSimilarity = pDerivatesSimilarity > 1 ? 1 : (pDerivatesSimilarity < 0 ? 0.0001 : pDerivatesSimilarity);
    if (pDerivateValue1*pDerivateValue2 > 0) {
        return(abs(pDerivateValue1 - pDerivateValue2) / abs(pDerivateValue1) <= pDerivatesSimilarity);
    } else {
        return(abs(pDerivateValue1 + pDerivateValue2) / abs(pDerivateValue1) <= pDerivatesSimilarity);
    }
}
void TcMathSupporter::TcMathDeviationDetector::fGlitch(double pLinearVariationPercentage, double pLevelTollerance, double pSenseFactor, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation>* pDeviations) {
    if (pDetectedIntervals.size() < kMinGlitchSample) {
        return;
    }

    double rValueBeforeGlitch = 0.00;
    double rValueAfterGlitch = 0.00;
    double rValueOnGlitch = 0.00;
    double rMImpulsiveBeforeGlitch = 0.00;
    double rMImpulsiveAfterGlitch = 0.00;
    double rMImpulsiveOnDescent = 0.00;
    double rMImpulsiveOnAscent = 0.00;
    double rLinearVariationPercentage = pLinearVariationPercentage;
    double rLevelTollerance = pLevelTollerance;
    bool rImpulsiveIncreaseFound = false;
    bool rImpulsiveDecreaseFound = false;
    bool rLinearDecreaseFound = false;
    bool rLinearIncreaseFound = false;
    int rGlitchFound = 0;

    vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cDeviations = *pDeviations;
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation cDeviation;

    double rNormVal;
    double rDevVal;
    for (int i = 0; i < pDetectedIntervals.size(); i++) {
        double rMlinear = 0.00;
        if (pDetectedIntervals[i].fIsImpulsive(&rMlinear)){ 
            if(rMlinear > 0 && !rImpulsiveDecreaseFound) {
                if(!rImpulsiveIncreaseFound){
                    rNormVal = pVals[pDetectedIntervals[i].fGetStartIndex()];
                    cDeviation.fSetNorm(rNormVal);
                    chrono::milliseconds cStartDuration(pTimes[pDetectedIntervals[i].fGetStartIndex()]);
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime(cStartDuration);
                    cDeviation.fSetStartTime(cStartTime);
                    cDeviation.fSetStartIndex(pDetectedIntervals[i].fGetStartIndex());
                    cDeviation.fSetMLinear(rMlinear);
                    rImpulsiveIncreaseFound = true;
                }
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                rMImpulsiveOnAscent = rMlinear;
            } else if (rMlinear < 0 && rImpulsiveIncreaseFound) {
                rNormVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                rMImpulsiveOnDescent = rMlinear;
                rImpulsiveDecreaseFound = true;
                if(fAreMeasuresLeveled(rNormVal, cDeviation.fGetNorm(), rLevelTollerance)){
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetEndIndex() - 1]));
                    cDeviation.fSetEndTime(cEndTime);
                    cDeviation.fSetEndIndex(pDetectedIntervals[i].fGetEndIndex());
                    cDeviation.fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch);
                    cDeviation.fSetDetectionTime(chrono::high_resolution_clock::now());
                    rGlitchFound = true;
                }
            }
        } else if (pDetectedIntervals[i].fIsLinear(&rMlinear) && rImpulsiveIncreaseFound){
            if(rMlinear > 0 && !rImpulsiveDecreaseFound && abs(rMlinear - rMImpulsiveOnAscent) /rMImpulsiveOnAscent < rLinearVariationPercentage) {
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                rLinearIncreaseFound = true;
            }
            else if(rMlinear < 0 && (!rImpulsiveDecreaseFound && abs(abs(rMlinear) - rMImpulsiveOnAscent) / rMImpulsiveOnAscent < rLinearVariationPercentage || rImpulsiveDecreaseFound && abs(rMlinear - rMImpulsiveOnDescent) /abs(rMImpulsiveOnDescent) < rLinearVariationPercentage)){
                rNormVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                rLinearDecreaseFound = true;
                if(rNormVal < cDeviation.fGetNorm() || fAreMeasuresLeveled(rNormVal, cDeviation.fGetNorm(), rLevelTollerance)){
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetEndIndex() - 1]));
                    cDeviation.fSetEndTime(cEndTime);
                    cDeviation.fSetEndIndex(pDetectedIntervals[i].fGetEndIndex());
                    cDeviation.fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Glitch);
                    cDeviation.fSetDetectionTime(chrono::high_resolution_clock::now());
                    rGlitchFound = true;
                }
            } else {
                rImpulsiveDecreaseFound = false;
                rImpulsiveIncreaseFound = false;
                rLinearIncreaseFound = false;
                rLinearDecreaseFound = false;
                cDeviation.fClearObject();
            }
        } else {
            rImpulsiveDecreaseFound = false;
            rImpulsiveIncreaseFound = false;
            rLinearIncreaseFound = false;
            rLinearDecreaseFound = false;
            cDeviation.fClearObject();
        }

        if(rGlitchFound){
            rImpulsiveDecreaseFound = false;
            rImpulsiveIncreaseFound = false;
            rLinearIncreaseFound = false;
            rLinearDecreaseFound = false;
            rGlitchFound = false;
            cDeviations.push_back(cDeviation);
            cDeviation.fClearObject();
        }
    }

    *pDeviations = cDeviations;
    return;



 }
void TcMathSupporter::TcMathDeviationDetector::fDrift(double pSenseFactor, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, vector<double> pMVals, vector<unsigned long long> pMTimes, vector<double> pVals, vector<unsigned long long> pTimes, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation>* pDeviations) {
    if (pDetectedIntervals.size() < kMinShiftSample) {
        return;
    }

    double rValueBeforeDrift = 0.00;
    double rValueAfterDrift = 0.00;

    double rMImpulsiveBeforeDrift = 0.00;
    double rMImpulsiveAfterDrift = 0.00;
    double rMImpulsiveOnDescent = 0.00;
    double rMImpulsiveOnAscent = 0.00;

    bool rDecreaseFound = 0;
    int rDecreaseCount = 0;
    bool rIncreaseFound = 0;
    int rIncreaseCount = 0;
    bool rDriftFound = 0;

    vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cDeviations = *pDeviations;
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation cDeviation;

    double rNormVal;
    double rDevVal;
    double rLinearShiftRef = 0;

    for (int i = 0; i < pDetectedIntervals.size(); i++) {
        double rMlinear = 0.00;
        if (pDetectedIntervals[i].fIsImpulsive(&rMlinear) || (pDetectedIntervals[i].fIsLinear(&rMlinear) && abs(rMlinear) > abs(pVals[pDetectedIntervals[i].fGetEndIndex()]) * pSenseFactor)) {
            if (rMlinear > 0 && !rDecreaseFound) {
                if (!rIncreaseFound) {
                    rNormVal = pVals[pDetectedIntervals[i].fGetStartIndex()];
                    cDeviation.fSetNorm(rNormVal);
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetStartIndex()]));
                    cDeviation.fSetStartTime(cStartTime);
                    cDeviation.fSetStartIndex(pDetectedIntervals[i].fGetStartIndex());
                    cDeviation.fSetMLinear(rMlinear);
                    rIncreaseFound = true;
                }
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetEndIndex() - 1]));
                cDeviation.fSetEndTime(cEndTime);
                cDeviation.fSetEndIndex(pDetectedIntervals[i].fGetEndIndex());
                cDeviation.fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift);
                cDeviation.fSetDetectionTime(chrono::high_resolution_clock::now());
                rMImpulsiveOnAscent = rMlinear;
                rIncreaseCount++;
                rDriftFound = true;
            }
            else if (rMlinear < 0 && !rIncreaseFound) {
                if (!rDecreaseFound) {
                    rNormVal = pVals[pDetectedIntervals[i].fGetStartIndex()];
                    cDeviation.fSetNorm(rNormVal);
                    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cStartTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetStartIndex()]));
                    cDeviation.fSetStartTime(cStartTime);
                    cDeviation.fSetStartIndex(pDetectedIntervals[i].fGetStartIndex());
                    cDeviation.fSetMLinear(rMlinear);
                    rDecreaseFound = true;
                }
                rDevVal = pVals[pDetectedIntervals[i].fGetEndIndex()];
                cDeviation.fSetDev(rDevVal);
                chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> cEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(chrono::milliseconds(pTimes[pDetectedIntervals[i].fGetEndIndex() - 1]));
                cDeviation.fSetEndTime(cEndTime);
                cDeviation.fSetEndIndex(pDetectedIntervals[i].fGetEndIndex());
                cDeviation.fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::Drift);
                cDeviation.fSetDetectionTime(chrono::high_resolution_clock::now());
                rMImpulsiveOnDescent = rMlinear;
                rDecreaseCount++;
                rDriftFound = true;
            }
            else {
                rDecreaseFound = false;
                rIncreaseFound = false;
                rIncreaseCount = 0;
                rDecreaseCount = 0;
                rDriftFound = false;
                cDeviation.fClearObject();
            }
        }
        else if (pDetectedIntervals[i].fIsConstant() || (pDetectedIntervals[i].fIsLinear(&rMlinear) && abs(rMlinear) <= abs(pVals[pDetectedIntervals[i].fGetEndIndex()]) * pSenseFactor)) {
            rIncreaseFound = false;
            rDecreaseFound = false;
            rIncreaseCount = 0;
            rDecreaseCount = 0;
            rDriftFound = false;
            cDeviation.fClearObject();
        }
    }


    if (rDriftFound) {
        rIncreaseFound = false;
        rDecreaseFound = false;
        rIncreaseCount = 0;
        rDecreaseCount = 0;
        rDriftFound = false;
        cDeviations.push_back(cDeviation);
        cDeviation.fClearObject();
    }

    *pDeviations = cDeviations;
    return;
}
#pragma endregion


#pragma region TcMathDeviationInterval
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::TcMathDeviationInterval() {
        this->rmDeviation = TcMathSupporter::TcMathDeviationDetector::Deviation::NoDeviation;
        this->rmMLinear = 0.00;
        this->rmStartIndex = 0;
        this->rmEndIndex = 0;
        this->cmStartTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>();
        this->cmEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>();
    }
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::TcMathDeviationInterval(TcMathSupporter::TcMathDeviationDetector::Deviation pDeviation, double pMLinear, int pStartLinearIndex, int pEndLinearIndex, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pStartTime, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pEndTime){
        this->rmDeviation = pDeviation;
        this->rmMLinear = pMLinear;
        this->rmStartIndex = pStartLinearIndex;
        this->rmEndIndex = pEndLinearIndex;
        this->cmStartTime = pStartTime;
        this->cmEndTime = pEndTime;
    }
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::~TcMathDeviationInterval() {
        ;
    }
    double TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetMLinear(){
        return(this->rmMLinear);
    }
    TcMathSupporter::TcMathDeviationDetector::Deviation TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetDeviation(){
        return(this->rmDeviation);
    }
    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>  TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetStartTime() {
        return(this->cmStartTime);
    }
    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>  TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetEndTime() {
        return(this->cmEndTime);
    }
    int TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetStartIndex(){
        return(this->rmStartIndex);
    }
    int TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetEndIndex(){
        return(this->rmEndIndex);
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetMLinear(double pMLinear){
        this->rmMLinear = pMLinear;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetDeviation(TcMathSupporter::TcMathDeviationDetector::Deviation pDeviation){
        this->rmDeviation = pDeviation;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetStartIndex(int pStartIndex){
        this->rmStartIndex = pStartIndex;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetEndIndex(int pEndIndex){
        this->rmEndIndex = pEndIndex;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetStartTime(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pStartTime) {
        this->cmStartTime = pStartTime;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetEndTime(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pEndTime) {
        this->cmEndTime = pEndTime;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetObject(TcMathSupporter::TcMathDeviationDetector::Deviation pDeviation, double pMLinear, int pStartIndex, int pEndIndex, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pStartTime, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pEndTime){
        this->rmDeviation = pDeviation;
        this->rmMLinear = pMLinear;
        this->rmStartIndex = pStartIndex;
        this->rmEndIndex = pEndIndex;
        this->cmStartTime = pStartTime;
        this->cmEndTime = pEndTime;
    }
    bool TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fIsConstant(){
        return(this->rmDeviation == Deviation::NoDeviation);
    }
    bool TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fIsLinear(double *pMLinear){
        if(this->rmDeviation == Deviation::Linear){
            *pMLinear = this->rmMLinear;
            return(true);
        } else{
            pMLinear = nullptr;
            return(false);
        }
    }
    bool TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fIsImpulsive(double *pMLinear){
        if(this->rmDeviation == Deviation::Impulsive){
            *pMLinear = this->rmMLinear;
            return(true);
        } else{
            pMLinear = nullptr;
            return(false);
        }
    }
#pragma endregion


#pragma region TcMathDeviation
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::TcMathDeviation() {
        this->rmDeviation = TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::NoDeviation;
        this->rmDevVal = 0;
        this->rmNormVal = 0;
        this->rmMLinear = 0;
        this->rmStartIndex = 0;
        this->rmEndIndex = 0;
        this->cmStartTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>();
        this->cmEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>();
    }
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::TcMathDeviation(DeviationDistribution pDeviation, double pDevVal, double pNormVal, double pMLinear, int pStartIndex, int pEndIndex, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pStartTime, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pEndTime, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pDetectionTime){
        this->rmDeviation = pDeviation;
        this->rmDevVal = pDevVal;
        this->rmNormVal = pNormVal;
        this->rmMLinear = pMLinear;
        this->rmStartIndex = pStartIndex;
        this->rmEndIndex = pEndIndex;
        this->cmStartTime = pStartTime;
        this->cmEndTime = pEndTime;
        this->cmDetectionTime = pDetectionTime;
    }
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::~TcMathDeviation() {
        ;
    }
    double TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetDev(){
        return(this->rmDevVal);
    }
    double TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetNorm(){
        return(this->rmNormVal);
    }
    double TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetMLinear(){
        return(this->rmMLinear);
    }
    TcMathSupporter::TcMathDeviationDetector::DeviationDistribution TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetDeviation(){
        return(this->rmDeviation);
    }
    int TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetStartIndex() {
        return(this->rmStartIndex);
    }
    int TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetEndIndex() {
        return(this->rmEndIndex);
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetStartIndex(int pStartIndex) {
        this->rmStartIndex = pStartIndex;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetEndIndex(int pEndIndex) {
        this->rmEndIndex = pEndIndex;
    }
    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetStartTime(){
        return(this->cmStartTime);
    }
    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>  TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetEndTime(){
        return(this->cmEndTime);
    }
    chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>  TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetDetectionTime() {
        return(this->cmDetectionTime);
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetDetectionTime(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pDetectionTime) {
        this->cmDetectionTime = pDetectionTime;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetMLinear(double pMLinear){
        this->rmMLinear = pMLinear;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetDev(double pDevVal){
        this->rmDevVal = pDevVal;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetNorm(double pNormVal){
        this->rmNormVal = pNormVal;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution pDeviation){
        this->rmDeviation = pDeviation;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetStartTime(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pStartTime){
        this->cmStartTime = pStartTime;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetEndTime(chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pEndTime){
        this->cmEndTime = pEndTime;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetObject(DeviationDistribution pDeviation, double pDevVal, double pNormVal, double pMLinear, int pStartIndex, int pEndIndex, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pStartTime, chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds> pEndTime){
        this->rmDeviation = pDeviation;
        this->rmDevVal = pDevVal;
        this->rmNormVal = pNormVal;
        this->rmMLinear = pMLinear;
        this->rmStartIndex = pStartIndex;
        this->rmEndIndex = pEndIndex;
        this->cmStartTime = pStartTime;
        this->cmEndTime = pEndTime;
    }
    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fClearObject() {
        this->rmDeviation = TcMathSupporter::TcMathDeviationDetector::DeviationDistribution::NoDeviation;
        this->rmDevVal = 0;
        this->rmNormVal = 0;
        this->rmMLinear = 0;
        this->rmStartIndex = 0;
        this->rmEndIndex = 0;
        this->cmStartTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>();
        this->cmEndTime = chrono::time_point<chrono::high_resolution_clock, chrono::nanoseconds>(); 
    }
#pragma endregion