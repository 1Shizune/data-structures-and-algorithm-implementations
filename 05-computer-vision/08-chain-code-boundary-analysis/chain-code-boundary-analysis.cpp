#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <limits>
using namespace std;

class ChainCode{
    public:
    struct point{
        int row, col;
        bool operator!=(const point& other) const {
        return row != other.row || col != other.col;
    }
};

    
    struct CCproperty{
        int label, numPixels, minRow, minCol, maxRow, maxCol;
    };
    
    int numCC, numRows, numCols, minVal, maxVal, currentLabel;
    CCproperty CC;
    int** imgAry;
    int** boundaryAry;
    int** CCAry;
    point coordOffset[8] = {
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1}
    };

    int zeroTable[8] = {6, 0, 0, 2, 2, 4, 4, 6};
    point startP, currentP, nextP;
    int lastQ, nextDir, PchainDir;

    ChainCode(int numCC, int numRows, int numCols, int minVal, int maxVal){
        this->numCC = numCC;
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;

        imgAry = new int*[numRows + 2];
        boundaryAry = new int*[numRows + 2];
        CCAry = new int*[numRows + 2];

        for(int i=0; i<numRows +2; ++i){
            imgAry[i] = new int[numCols +2]{0};
            boundaryAry[i] = new int[numCols +2]{0};
            CCAry[i] = new int[numCols +2]{0};
        }
    }

    void zeroFramed(int** Ary){
        for(int i = 0; i < numRows + 2; i++){
            for(int j = 0; j< numCols +2; j++){
                Ary[i][j] = 0;
            }
        }
    }
    
    void loadImage(int** imgAry, ifstream& inFile){
        for(int i =1; i<=numRows; i++){
            for(int j =1; j<=numCols; j++){
               inFile >> imgAry[i][j];
            }
        }
    }

    void clearCCAry(int** CCAry){
        for(int i =0; i< numRows +2; i++){
            for(int j =0; j< numCols +2; j++){
                CCAry[i][j] =0;
            }
        }
    }

    void loadCCAry(int ccLabel, int** ccAry){
        for(int i =1; i<=numRows; i++){
            for(int j =1; j<=numCols; j++){
                if(imgAry[i][j] == ccLabel){
                    ccAry[i][j] = imgAry[i][j];
                }
            }
        }
    }
    
    int findNextP(point currentP, int startDir, ofstream& logFile){
        logFile << "*** Entering findNextP() ***\n";
        int index = startDir;
        bool found = false;
        PchainDir = -1;

        while(!found){
            int row = currentP.row + coordOffset[index].row;
            int col = currentP.col + coordOffset[index].col;
            if(imgAry[row][col] == currentLabel){
               PchainDir = index;
               found = true;
            }
                else{
                    index = (index + 1) % 8;
                }
            logFile << "*** in findNextP(): index = " << index << "; Row, col = (" << row << ", " << col << "); PchainDir = " << PchainDir << "; found = " << found << "; imgAry value = " << imgAry[row][col] << "\n";
        }
        logFile << "*** Leaving findNextP() ***\n";
        return PchainDir;
    }

    void getChainCode(CCproperty CC, int** CCAry, ofstream& chainCodeFile, ofstream& logFile){
        logFile << "*** Entering getChainCode() ***\n";
        currentLabel = CC.label;
        bool found = false;
        
        for(int i =1; i<=numRows && !found; i++){
            for(int j =1; j<=numCols && !found; j++){
                if(CCAry[i][j] == currentLabel){
                    startP = {i, j};
                    currentP = startP;
                    found = true;
                }
            } 
        }      
            
        chainCodeFile << CC.label << " " << startP.row << " " << startP.col << "\n";
        int lastQ = 4;
        int stepCount = 0;
        do{
            int nextQ = (lastQ+1) % 8;
            int PchainDir = findNextP(currentP, nextQ, logFile);
            chainCodeFile << PchainDir << " ";
            nextP.row = currentP.row + coordOffset[PchainDir].row;
            nextP.col = currentP.col + coordOffset[PchainDir].col;
            currentP = nextP;

                    if(PchainDir == 0){
                        lastQ = zeroTable[7];
                    }
                        else{
                            lastQ = zeroTable[PchainDir - 1];
                        }
                        stepCount++;
                        logFile << "LastQ = " << lastQ << "; nextQ = " << nextQ << "; currentP.row = " << currentP.row << "; currentP.col = " << currentP.col << "; nextP.row = " << nextP.row << "; nextP.col = " << nextP.col << "; \n";
                    } 
                    while(!(currentP.row == startP.row && currentP.col == startP.col && stepCount > 0));         
                    chainCodeFile << "\n";
                    logFile << "\n*** Leaving getChainCode() ***\n";
    
        }

        void constructBoundary(int** boundaryAry, ifstream& chainCodeFile){
            int rows, cols, minV, maxV, numComponents;
            if(!(chainCodeFile >> rows >> cols >> minV >> maxV)) {
                cerr << "Error reading chain file header\n";
                return;
            }
            if(!(chainCodeFile >> numComponents)) {
                cerr << "Error reading number of components\n";
                return;
            }
            
            chainCodeFile.ignore(numeric_limits<streamsize>::max(), '\n');

            for(int i = 0; i < numRows + 2; ++i)
                for(int j = 0; j < numCols + 2; ++j)
                    boundaryAry[i][j] = 0;

            string line;
            for(int c = 0; c < numComponents; ++c){
                int label, startRow, startCol;
                if(!(chainCodeFile >> label >> startRow >> startCol)) {
                    cerr << "Error reading component header for component " << c << "\n";
                    return;
                }

                chainCodeFile.ignore(numeric_limits<streamsize>::max(), '\n');

                if(!getline(chainCodeFile, line)) {
                    line = "";
                }

                boundaryAry[startRow][startCol] = label;
                point currentP = { startRow, startCol };

                istringstream iss(line);
                int dir;
                while(iss >> dir){
                    if(dir < 0 || dir > 7) {
                    continue;
                }
                nextP.row = currentP.row + coordOffset[dir].row;
                nextP.col = currentP.col + coordOffset[dir].col;

                if(nextP.row >= 0 && nextP.row < numRows + 2 && nextP.col >= 0 && nextP.col < numCols + 2){
                    boundaryAry[nextP.row][nextP.col] = label;
                }
                currentP = nextP;
        }
    }
}



    void prettyPrint(int** Ary, ofstream& prettyPrintFile) {
    int minVal = 9999;
    int maxVal = 0;

    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            if(Ary[i][j] < minVal) minVal = Ary[i][j];
            if(Ary[i][j] > maxVal) maxVal = Ary[i][j];
        }
    }

    prettyPrintFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";

    int width = to_string(maxVal).length();

    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            int val = Ary[i][j];
            if(val > 0)
                prettyPrintFile << val;
            else
                prettyPrintFile << ".";

            int len = (val > 0) ? to_string(val).length() : 1;
            for(int k=len; k<=width; k++)
                prettyPrintFile << " ";

            prettyPrintFile << " ";
        }
        prettyPrintFile << "\n";
    }
    prettyPrintFile.flush();
}

    void AryToFile(int** Ary, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                outFile << Ary[i][j] << " ";
            }
            outFile << "\n";
        }
    }

};

int main(int argc, char** argv){
    if(argc != 7){
        cout << "Your command line must include 6 parameters: labelFile, propertyFile, outFile, chainCodeFile, boundaryFile and a logFile.";
        return 1;
    }

    string labelFile = argv[1];
    string propertyFile = argv[2];
    string outFile = argv[3];
    string chainCodeFile = argv[4];
    string boundaryFile = argv[5];
    string logFile = argv[6];

    ifstream labelIn(labelFile);
    if(!labelIn.is_open()){
        cout << "Error opening label file.";
        return 1;
    }

    ifstream propertyIn(propertyFile);
    if(!propertyIn.is_open()){
        cout << "Error opening property file.";
        return 1;
    }

    ofstream outOut(outFile);
    if(!outOut.is_open()){
        cout << "Error opening out file.";
        return 1;
    }

    ofstream chainOut(chainCodeFile);
    if(!chainOut.is_open()){
        cout << "Error opening chain code file.";
        return 1;
    }

    ofstream boundaryOut(boundaryFile);
    if(!boundaryOut.is_open()){
        cout << "Error opening boundary file.";
        return 1;
    }

    ofstream logOut(logFile);
    if(!logOut.is_open()){
        cout << "Error opening log file.";
        return 1;
    }

    int numRows, numCols, minVal, maxVal, PropnumRows, PropnumCols, PropminVal, PropmaxVal, numCC;
    labelIn >> numRows >> numCols >> minVal >> maxVal;
    propertyIn >> PropnumRows >> PropnumCols >> PropminVal >> PropmaxVal;
    propertyIn >> numCC;
    ChainCode cc(numCC, numRows, numCols, minVal, maxVal);
    cc.zeroFramed(cc.imgAry);
    cc.loadImage(cc.imgAry, labelIn);
    outOut << "*** Below is the loaded imgAry of input labelFile ***\n";
    cc.prettyPrint(cc.imgAry, outOut);

    chainOut << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
    chainOut << numCC << "\n";
    int originalNumCC = numCC;
    numCC = originalNumCC;

    while(numCC > 0){
        propertyIn >> cc.CC.label;
        propertyIn >> cc.CC.numPixels;
        propertyIn >> cc.CC.minRow >> cc.CC.minCol;
        propertyIn >> cc.CC.maxRow >> cc.CC.maxCol; 

        cc.clearCCAry(cc.CCAry);
        cc.loadCCAry(cc.CC.label, cc.CCAry);
        logOut << "*** Below is the loaded CCAry of connected component label " << cc.CC.label << " ***\n";
        cc.prettyPrint(cc.CCAry, logOut);
        cc.getChainCode(cc.CC, cc.CCAry, chainOut, logOut);
    
        numCC--;
    }
    chainOut.close();
    propertyIn.clear();
    propertyIn.seekg(0);
    propertyIn >> PropnumRows >> PropnumCols >> PropminVal >> PropmaxVal;
    propertyIn >> numCC;
    
    ifstream chainIn(chainCodeFile);
    if(!chainIn.is_open()){
        cout << "Error reopening chain code file.";
        return 1;
    }   

    
    chainIn.clear();
    chainIn.seekg(0);

    cc.constructBoundary(cc.boundaryAry, chainIn);
    outOut << "*** Below is the objects boundaries of the input label image ***\n";
    cc.prettyPrint(cc.boundaryAry, outOut);
    boundaryOut << "*** Below is the objects boundaries of the input label image ***\n";
    cc.AryToFile(cc.boundaryAry, boundaryOut);

    labelIn.close();
    propertyIn.close();
    outOut.close();
    chainOut.close();
    boundaryOut.close();
    logOut.close();
}
