#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcMathSupporter.h"
#include "../../../include/Agent/MeasurementsDeviationDetectorAgent/TcRegressor.h"



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
            rNcol = pMatrix[i].size();
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
            rNcol = pMatrix[i].size();
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
        rNelements = pMatrix.size();
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
            rNcol = pMatrix[i].size() - 1;
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
        throw new exception(to_string(TcError::TcDeterminant::kErr_NoRows).c_str());
    }
    else {
        rNrow = pMatrix.size() - 1;
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() - 1 <= 0) {
            throw new exception(to_string(TcError::TcDeterminant::kErr_NoCols).c_str());
        }
        else if (i == 0) {
            rNcol = pMatrix[i].size() - 1;
        }
        else if (pMatrix[i].size() - 1 != rNcol) {
            throw new exception(to_string(TcError::TcDeterminant::kErr_NoConstCols).c_str());
        }
    }

    if (rNcol != rNrow) {
        throw new exception(to_string(TcError::TcDeterminant::kErr_NoSquareMatrix).c_str());
    }
    else if (rNcol != pDimension) {
        throw new exception(to_string(TcError::TcDeterminant::kErr_NoColDimensionRelationship).c_str());
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
        rNrow = pMatrix.size() - 1;
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() - 1 <= 0) {
            return(TcError::TcAdJoint::kErr_NoCols);
        }
        else if (i == 0) {
            rNcol = pMatrix[i].size() - 1;
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

    vector<vector<double>> cCofactor(rDimension - 1);
    for (int i = 0; i < rDimension - 1; i++) {
        cCofactor[i] = vector<double>(rDimension - 1);
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
        rNrow = pMatrix.size() - 1;
    }

    for (int i = 0; i < rNrow; i++) {
        if (pMatrix[i].size() - 1 <= 0) {
            return(TcError::TcInverse::kErr_NoCols);
        }
        else if (i == 0) {
            rNcol = pMatrix[i].size() - 1;
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
        return(int(e.what()));
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



#pragma region TcMathDeviationDetector


int TcMathSupporter::TcMathDeviationDetector::fDetect(double pUnitScaleFactor, vector<double> pMvals, vector<double> pVals, vector<long long> pTimes, vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> *pDeviations) {


    if (pUnitScaleFactor > pow(10, TcMathSupporter::TcMathDeviationDetector::TcConst::kOutOfRangeUnitScaleFactorExp) || pUnitScaleFactor < pow(10, -TcMathSupporter::TcMathDeviationDetector::TcConst::kOutOfRangeUnitScaleFactorExp)) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoXYvalsRelationship);
    }

    if (pMvals.size() <= 0 || pMvals.size() >= 4 * 1024) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoMvals);
    }

    if (pVals.size() <= 0 || pVals.size() >= 4 * 1024) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoVals);
    }

    if (pTimes.size() <= 0 || pTimes.size() >= 4 * 1024) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoTImes);
    }

    if (pTimes.size() != pVals.size()) {
        pDeviations = nullptr;
        return(TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kErr_NoTimeValsRelationship);
    }

    vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> cDeviations;
    double rImpulsiveTollerance = pUnitScaleFactor * TcMathSupporter::TcMathDeviationDetector::TcConst::kImpulsiveTolleranceMultiplier;
    double rLinearTollerance = pUnitScaleFactor * TcMathSupporter::TcMathDeviationDetector::TcConst::kLinearTolleranceMultiplier;
    double rConstTollerance = pUnitScaleFactor * TcMathSupporter::TcMathDeviationDetector::TcConst::kConstTolleranceMultiplier;
    double rErrorTollerance = pUnitScaleFactor * TcMathSupporter::TcMathDeviationDetector::TcConst::kErrorTolleranceMultiplier;
    TcMathSupporter::TcMathDeviationDetector cMathDeviationDetector;

    for (int i = 0; i < pMvals.size(); i++) {
        double rMlinear = 0;

        //Derivate of Xvals in i is : - rConstTollerance <= Derivate(X(i)) <= + rConstTollerance  
        if (abs(pMvals[i]) <= rConstTollerance) {
            rMlinear = 0;
        }
        else {
            //Derivate of Xvals in i is : Derivate(X(i)) >= rConstTollerance 
            if (pMvals[i] >= rConstTollerance) {
                while (abs(pMvals[i] - rMlinear) <= rErrorTollerance) {
                    rMlinear += rErrorTollerance;
                }
            } //Derivate of Xvals in i is : - rConstTollerance >= Derivate(X(i)) 
            else if (pMvals[i] <= -rConstTollerance) {
                while (abs(pMvals[i] - rMlinear) <= rErrorTollerance) {
                    rMlinear -= rErrorTollerance;
                }
            }
        }

        int j = i + 1;
        TcMathSupporter::TcMathDeviationDetector::Deviation rDeviation;
        if (abs(rMlinear) < rConstTollerance) {
            while (abs(pMvals[j]) < rConstTollerance && abs(pMvals[j] - rMlinear) <= rErrorTollerance && ++j < pMvals.size());
            rDeviation = Deviation::NoDeviation;
        }
        else if (abs(rMlinear) < rLinearTollerance) {
            while (abs(pMvals[j]) < rLinearTollerance && abs(pMvals[j] - rMlinear) <= rErrorTollerance && ++j < pMvals.size());
            rDeviation = Deviation::Linear;
        }
        else if (abs(rMlinear) < rImpulsiveTollerance) {
            while (abs(pMvals[j]) < rImpulsiveTollerance && abs(pMvals[j] - rMlinear) <= rErrorTollerance && ++j < pMvals.size());
            rDeviation = Deviation::Impulsive;
        }

        if (j > i + 1) {
            TcMathSupporter::TcMathDeviationDetector::TcMathDeviation cDeviation;

            chrono::system_clock::time_point cStartTime {chrono::milliseconds{1000}};
            chrono::system_clock::time_point cEndTime {chrono::milliseconds{1000}};


            cMathDeviationDetector.fAddDeviationInterval(rDeviation, rMlinear, i, j - 1, cStartTime, cEndTime);
            cMathDeviationDetector.fDetectDeviation(pMvals, pTimes, &cDeviation);
            i = j;
        }

    }

    *pDeviations = cDeviations;
    return(cDeviations.size() > 0 ? TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kDetectedDeviation : TcMathSupporter::TcMathDeviationDetector::TcError::TcDetect::kNoDeviationDetected);
}
bool TcMathSupporter::TcMathDeviationDetector::fDetectDeviation(vector<double> pVals, vector<long long> pTimes, TcMathDeviation *pDeviation){
        TcMathDeviation cDeviation;
        TcMathDeviationInterval cDeviationInterval;

        if(this->fmNotify == nullptr){
            return(false);
        }

        if(pVals.size() <= 0 || pVals.size() >= 4 * 1024){
            pDeviation = nullptr;
            return(TcError::TcDetect::kErr_NoVals);
        }
    
        if(pTimes.size() <= 0 || pTimes.size() >= 4 * 1024){
            pDeviation = nullptr;
            return(TcError::TcDetect::kErr_NoTImes);
        }
    
        if(pTimes.size() != pVals.size()){
            pDeviation = nullptr;
            return(TcError::TcDetect::kErr_NoTimeValsRelationship);
        }

        if(TcMathDeviationDetector::fShift(*this->cmMathDeviationIntervals, &cDeviation)){
            fmNotify(cDeviation);
            return(true);
        }
        
        if(fDrift(*this->cmMathDeviationIntervals, &cDeviation)){
            long long cOutOfThresholdPredictedTime;
            vector<long long> cTimes;
            vector<double> cVals;
           
            TcRegressor<long long, double> *cRegressor = new TcRegressor<long long, double>();
            int rStartDeviationIndex = cDeviation.fGetStartIndex();
            int rEndDeviationIndex = cDeviation.fGetEndIndex();
            int rOrder = (int) sqrt(rEndDeviationIndex - rStartDeviationIndex); 

            copy(cTimes.begin(), pTimes.begin() + rEndDeviationIndex - (2*rOrder), pTimes.begin() + rEndDeviationIndex);
            copy(cVals.begin(), pVals.begin() + rEndDeviationIndex - (2*rOrder), pVals.begin() + rEndDeviationIndex);
          

            int rResult = cRegressor->fModel(cTimes, cVals, rOrder);
            rResult = cRegressor->fTrain();
            cRegressor->fValidate();
            rResult = cRegressor->fPredict(cVals, &cOutOfThresholdPredictedTime);

            fmNotify(cDeviation);

            return(true);
        }
        
        if(fDrop(*this->cmMathDeviationIntervals, &cDeviation)){
            fmNotify(cDeviation);
            return(true);
        }
        
        if(fGlitch(*this->cmMathDeviationIntervals, &cDeviation)){
            fmNotify(cDeviation);
            return(true);
        }

        return(false);
}


TcMathSupporter::TcMathDeviationDetector::TcMathDeviationDetector(TcMathSupporter::TcMathDeviationDetector::fNotifyDeviation pNotify){
    this->fmNotify = pNotify;
}

TcMathSupporter::TcMathDeviationDetector::TcMathDeviationDetector() {
}

TcMathSupporter::TcMathDeviationDetector::~TcMathDeviationDetector(){
    if(this->cmMathDeviationIntervals != nullptr){
        delete this->cmMathDeviationIntervals;
        this->cmMathDeviationIntervals = nullptr;
    }
}


vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> TcMathSupporter::TcMathDeviationDetector::fGetDeviationIntervals(){
    return(*(this->cmMathDeviationIntervals));
}


vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> TcMathSupporter::TcMathDeviationDetector::fGetDeviations() {
    return(*(this->cmDeviations));
}



void TcMathSupporter::TcMathDeviationDetector::fAddDeviationInterval(TcMathSupporter::TcMathDeviationDetector::Deviation pDeviation, double pMLinear, int pStartIndex, int pEndIndex, chrono::system_clock::time_point pStartTime, chrono::system_clock::time_point pEndTime){
    this->cmMathDeviationIntervals->push_back(TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval(pDeviation, pMLinear, pStartIndex, pEndIndex, pStartTime, pEndTime));
}


void TcMathSupporter::TcMathDeviationDetector::fAddDeviation(TcMathSupporter::TcMathDeviationDetector::DeviationDistribution pDeviation, double pMLinear, double pDevVal, double pNormVal, int pStartLinearIndex, int pEndLinearIndex, chrono::system_clock::time_point pStartTime, chrono::system_clock::time_point pEndTime){
    this->cmDeviations->push_back(TcMathSupporter::TcMathDeviationDetector::TcMathDeviation(pDeviation, pMLinear, pDevVal, pNormVal, pStartLinearIndex, pEndLinearIndex, pStartTime, pEndTime));
}

void TcMathSupporter::TcMathDeviationDetector::fSetNotifyFunction(TcMathSupporter::TcMathDeviationDetector::fNotifyDeviation pNotify){
    this->fmNotify = pNotify;
}

void TcMathSupporter::TcMathDeviationDetector::fSetDeviations(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviation> pDeviations) {
    *this->cmDeviations = pDeviations;
}
void TcMathSupporter::TcMathDeviationDetector::fSetDeviationIntervals(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDeviationIntervals) {
    *this->cmMathDeviationIntervals = pDeviationIntervals;
}



bool TcMathSupporter::TcMathDeviationDetector::fShift(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation* pDeviation) { return(true); }
bool TcMathSupporter::TcMathDeviationDetector::fDrop(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation* pDeviation) { return(true); }
bool TcMathSupporter::TcMathDeviationDetector::fGlitch(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation* pDeviation) { return(true); }
bool TcMathSupporter::TcMathDeviationDetector::fDrift(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation* pDeviation) { return(true); }
bool TcMathSupporter::TcMathDeviationDetector::fUnknown(vector<TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval> pDetectedIntervals, TcMathSupporter::TcMathDeviationDetector::TcMathDeviation* pDeviation) { return(true); }



#pragma endregion

#pragma region TcMathDeviationInterval
   
    TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::TcMathDeviationInterval() {
        this->rmDeviation = TcMathSupporter::TcMathDeviationDetector::Deviation::NoDeviation;
        this->rmMLinear = 0.00;
        this->rmStartIndex = 0;
        this->rmEndIndex = 0;
        this->cmStartTime = chrono::system_clock::time_point();
        this->cmEndTime = chrono::system_clock::time_point();
    }

    TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::TcMathDeviationInterval(TcMathSupporter::TcMathDeviationDetector::Deviation pDeviation, double pMLinear, int pStartLinearIndex, int pEndLinearIndex, chrono::system_clock::time_point pStartTime, chrono::system_clock::time_point pEndTime){
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

  

    chrono::system_clock::time_point  TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetStartTime() {
        return(this->cmStartTime);
    }

    chrono::system_clock::time_point  TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fGetEndTime() {
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


    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetStartTime(chrono::system_clock::time_point pStartTime) {
        this->cmStartTime = pStartTime;
    }

    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetEndTime(chrono::system_clock::time_point pEndTime) {
        this->cmEndTime = pEndTime;
    }

    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviationInterval::fSetObject(TcMathSupporter::TcMathDeviationDetector::Deviation pDeviation, double pMLinear, int pStartIndex, int pEndIndex, chrono::system_clock::time_point pStartTime, chrono::system_clock::time_point pEndTime){
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
        this->cmStartTime = chrono::system_clock::time_point();
        this->cmEndTime = chrono::system_clock::time_point();
    }
   

    TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::TcMathDeviation(DeviationDistribution pDeviation, double pDevVal, double pNormVal, double pMLinear, int pStartIndex, int pEndIndex, chrono::system_clock::time_point pStartTime, chrono::system_clock::time_point pEndTime){
        this->rmDeviation = pDeviation;
        this->rmDevVal = pDevVal;
        this->rmNormVal = pNormVal;
        this->rmMLinear = pMLinear;
        this->rmStartIndex = pStartIndex;
        this->rmEndIndex = pEndIndex;
        this->cmStartTime = pStartTime;
        this->cmEndTime = pEndTime;
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
    
    chrono::system_clock::time_point  TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetStartTime(){
        return(this->cmStartTime);
    }

    chrono::system_clock::time_point  TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fGetEndTime(){
        return(this->cmEndTime);
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


    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetStartTime(chrono::system_clock::time_point pStartTime){
        this->cmStartTime = pStartTime;
    }

    void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetEndTime(chrono::system_clock::time_point pEndTime){
        this->cmEndTime = pEndTime;
    }

     void TcMathSupporter::TcMathDeviationDetector::TcMathDeviation::fSetObject(DeviationDistribution pDeviation, double pDevVal, double pNormVal, double pMLinear, int pStartIndex, int pEndIndex, chrono::system_clock::time_point pStartTime, chrono::system_clock::time_point pEndTime){
        this->rmDeviation = pDeviation;
        this->rmDevVal = pDevVal;
        this->rmNormVal = pNormVal;
        this->rmMLinear = pMLinear;
        this->rmStartIndex = pStartIndex;
        this->rmEndIndex = pEndIndex;
        this->cmStartTime = pStartTime;
        this->cmEndTime = pEndTime;
    }

#pragma endregion

  
