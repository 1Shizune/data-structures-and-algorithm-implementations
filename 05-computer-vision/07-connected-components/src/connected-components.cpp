#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class Property{
    public:
        int label, numPixels, minR, minC, maxR, maxC;
};

class ccLablel{
public:    
    int numRows, numCols, minVal, maxVal, newLabel,trueNumCC, newMin, newMax;
    int** zeroFramedAry;  
    int nonZeroNeighbors[5];
    int* EQTable;
    Property* CCproperty;

    ccLablel(int numRows, int numCols, int minVal, int maxVal, int trueNumCC){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        newLabel = 0;
        this->trueNumCC = trueNumCC;
        newMin = 0;
        newMax = trueNumCC;
        
        EQTable = new int[(numRows * numCols) + 1];
        for(int i =0; i<= (numRows * numCols); i++){
            EQTable[i] = i;
        }

        CCproperty = new Property[trueNumCC +1];

        zeroFramedAry = new int*[numRows + 2];
        for(int i=0; i<numRows +2; ++i){
            zeroFramedAry[i] = new int[numCols +2];
            for(int j=0; j<numCols+2; j++){
                zeroFramedAry[i][j] = 0;
            }
        }
    }

    void zero2D(int** Ary){
        for(int i =0; i< numRows +2; i++){
            for(int j=0; j< numCols +2; j++){
                Ary[i][j] =0;
            }
        }
    }

    void negative1D(int* Ary){
        for(int i =0; i< (numRows * numCols)/4; i++){
            Ary[i] = -1;
        }
    }

    void loadImage(ifstream& inFile, int** Ary){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                inFile >> Ary[i][j]; 
            }
        }
    }

    void printImg(int** Ary, ofstream& labelFile){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                int val = Ary[i][j];
                labelFile << val << " ";
            }
            labelFile << "\n";
        }
        labelFile.flush();
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


    void connect8Pass1(){
        for(int i=1; i<=numRows; i++){
            for(int j = 1; j<=numCols; j++){
                if(zeroFramedAry[i][j] > 0){
                    int a = zeroFramedAry[i-1][j-1];
                    int b = zeroFramedAry[i-1][j];
                    int c = zeroFramedAry[i-1][j+1];
                    int d = zeroFramedAry[i][j-1];

                    int neighbors[4] = {a,b,c,d};
                    int minLabel = 9999;
                    int nonZeroCount =0;

                    for(int k=0; k<4; k++){
                        if(neighbors[k] > 0){
                            nonZeroCount++;
                            minLabel = min(minLabel, neighbors[k]);
                        }
                    }
                
                    if(nonZeroCount == 0){
                        newLabel++;
                        zeroFramedAry[i][j] = newLabel;
                    }
                    else if(nonZeroCount == 1){
                        zeroFramedAry[i][j] = minLabel;
                    }
                    else{
                        zeroFramedAry[i][j] = minLabel;
                        for(int k=0; k<4; k++){
                            if(neighbors[k] > 0){
                                int n = neighbors[k];
                                if(n!=minLabel){
                                    EQTable[n] = minLabel;
                                }
                            }
                    }
                }
            }
        }
        newMax = newLabel;
    }

}

    void flatten1EQTable() {
        for (int i = 1; i <= newLabel; i++) {
            int root = i;
            while (EQTable[root] != root) {
                root = EQTable[root];
            }
            EQTable[i] = root;
        }
    }

    void connect8pass2(){
        flatten1EQTable();
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                int val = zeroFramedAry[i][j];
                if(val >0){
                    zeroFramedAry[i][j] = EQTable[val];
                }
            }
        }
    }

    void connect4Pass1(){
        for(int i=1; i<=numRows; i++){
            for(int j =1; j<=numCols; j++){
                if(zeroFramedAry[i][j] > 0){
                    int b = zeroFramedAry[i-1][j];
                    int d = zeroFramedAry[i][j-1];

                    if(b == 0 && d == 0){
                        newLabel++;
                        zeroFramedAry[i][j] = newLabel;
                    }

                    else if(b>0 && d ==0){
                        zeroFramedAry[i][j] = b;
                    }

                    else if(b == 0 && d> 0){
                        zeroFramedAry[i][j] = d;
                    }

                    else{
                        int minLabel = min(b, d);
                        zeroFramedAry[i][j] = minLabel;

                        if(b!=minLabel){
                            EQTable[b] = minLabel;
                        }
                        if(d!=minLabel){
                            EQTable[d] = minLabel;
                        }
                    }
                }
            }
        }
        newMax = newLabel;
    }


    void flatten2EQTable() {
    for (int i = 1; i <= newLabel; i++) {
        int root = i;
        while (EQTable[root] != root) {
            root = EQTable[root];
        }
        EQTable[i] = root;
    }
}

    void connect4pass2(){
        flatten2EQTable();
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                int val = zeroFramedAry[i][j];
                if(val >0){
                    zeroFramedAry[i][j] = EQTable[val];
                }
            }
        }
    }

    void connectPass3(int** zeroFramedAry, int* EQTable, Property* CCproperty, int trueNumCC, ofstream& logFile){
    logFile << "*** Entering connectPass3() ***\n";
    logFile.flush();
    
    for(int i=1; i<=trueNumCC; i++){
        CCproperty[i].label = i;
        CCproperty[i].numPixels = 0;
        CCproperty[i].minR = numRows;
        CCproperty[i].minC = numCols;
        CCproperty[i].maxR = 0;
        CCproperty[i].maxC = 0;
    }

    for(int i =1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            int pixel = zeroFramedAry[i][j];
            if(pixel > 0){
                zeroFramedAry[i][j] = EQTable[pixel];
                int k = zeroFramedAry[i][j];
                
                if(k >= 1 && k <= trueNumCC) {
                    CCproperty[k].numPixels++;
                    if(i < CCproperty[k].minR) CCproperty[k].minR = i;
                    if(i > CCproperty[k].maxR) CCproperty[k].maxR = i;
                    if(j < CCproperty[k].minC) CCproperty[k].minC = j;
                    if(j > CCproperty[k].maxC) CCproperty[k].maxC = j;
                } else {
                    logFile << "WARNING: Invalid label k=" << k << " at position (" << i << "," << j << ")\n";
                    logFile.flush();
                }
            }
        }
    }
    
    logFile << "*** Leaving connectPass3() ***\n";
    logFile.flush();
}

    void updateEQTable(int** zeroFramedAry, int* EQTable){

        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {

                if (zeroFramedAry[i][j] > 0) {

                    int neighbors[8] = {
                        zeroFramedAry[i-1][j-1],
                        zeroFramedAry[i-1][j],
                        zeroFramedAry[i-1][j+1],
                        zeroFramedAry[i][j-1],
                        zeroFramedAry[i][j+1],
                        zeroFramedAry[i+1][j-1],
                        zeroFramedAry[i+1][j],
                        zeroFramedAry[i+1][j+1]
                    };

                    int nonZero[8];
                    int nzCount = 0;

                    for (int k = 0; k < 8; k++) {
                        if (neighbors[k] > 0) {
                            nonZero[nzCount] = neighbors[k];
                            nzCount++;
                        }
                    }

                if(nzCount > 1) {
                    continue;
                }

                for (int a = 0; a < nzCount - 1; a++) {
                    int minIndex = a;
                    for (int b = a + 1; b < nzCount; b++) {
                        if (nonZero[b] < nonZero[minIndex]) {
                            minIndex = b;
                        }
                    }
                    int temp = nonZero[a];
                    nonZero[a] = nonZero[minIndex];
                    nonZero[minIndex] = temp;
                }

                int minLabel = nonZero[0];

                for (int k = 1; k < nzCount; k++) {
                    int lbl = nonZero[k];
                    if (lbl != minLabel) {
                        EQTable[lbl] = minLabel;
                    }
                }
            }
        }
    }
}

    int manageEQTableFinal() {
    for (int i = 1; i <= newLabel; i++) {
        int root = i;
        while (EQTable[root] != root) {
            root = EQTable[root];
        }
        EQTable[i] = root;
    }

    int newID = 0;
    int relabel[10000];
    for (int i = 1; i <= newLabel; i++) {
        int r = EQTable[i];
        if (relabel[r] == 0) {
            relabel[r] = ++newID;
        }
        EQTable[i] = relabel[r];
    }

    return newID;
}


    void printCCProperty(ofstream& propertyFile) {
        propertyFile << numRows << " " << numCols << " " << newMin << " " << newMax << "\n";

        propertyFile << this->trueNumCC << "\n";

        for (int i = 1; i <= this->trueNumCC; i++) {
            propertyFile << CCproperty[i].label << "\n";     
            propertyFile << CCproperty[i].numPixels << "\n"; 
            propertyFile << (CCproperty[i].minR - 1)<< " " << (CCproperty[i].minC - 1)<< "\n";
            propertyFile << (CCproperty[i].maxR - 1)<< " " << (CCproperty[i].maxC - 1)<< "\n"; 
        }
        propertyFile.flush();
    }

    void printEQTable(ofstream& choiceFile){
    choiceFile << "Index\tEQTable[index]\n";
    choiceFile << "---------------------\n";
    for (int i = 1; i <= newLabel; i++) {
        choiceFile << i << "\t" << EQTable[i] << "\n";
    }   
}


    void drawBoxes(int** zeroFramedAry, Property* CCproperty, int trueNumCC, ofstream& logFile){
    logFile << "*** Entering drawBoxes() ***\n";
    int boxVal = 1;
 


    for(int k=1; k<=trueNumCC; k++){
        int minR = CCproperty[k].minR;
        int minC = CCproperty[k].minC;
        int maxR = CCproperty[k].maxR;
        int maxC = CCproperty[k].maxC;

        for(int j=minC; j<=maxC; j++){
            zeroFramedAry[minR][j] = boxVal;
            zeroFramedAry[maxR][j] = boxVal;
        }

        for(int i=minR; i<=maxR; i++){
            zeroFramedAry[i][minC] = boxVal;
            zeroFramedAry[i][maxC] = boxVal;
        }
    }

    logFile << "*** Leaving drawBoxes() ***\n";
    logFile.flush();
}


    void connected8(ofstream& prettyPrintFile, ofstream& logFile) {
    logFile << "*** Entering connected8() ***\n";

    connect8Pass1();
    prettyPrintFile << "*** After connected8 pass1, newLabel = " << newLabel << "\n";
    prettyPrint(zeroFramedAry, prettyPrintFile);
    prettyPrintFile << "*** EQTable after pass1\n";
    printEQTable(prettyPrintFile);

    connect8pass2();
    prettyPrintFile << "*** After connected8 pass2\n";
    prettyPrint(zeroFramedAry, prettyPrintFile);
    prettyPrintFile << "*** EQTable after pass2\n";
    printEQTable(prettyPrintFile);

    trueNumCC = manageEQTableFinal();
    prettyPrintFile << "*** After manageEQTable, trueNumCC = " << trueNumCC << "\n";
    printEQTable(prettyPrintFile);

    delete[] CCproperty;
    CCproperty = new Property[trueNumCC + 1];


    newMin = 0;
    newMax = trueNumCC;

    connectPass3(zeroFramedAry, EQTable, CCproperty, trueNumCC, logFile);

    prettyPrintFile << "*** After connected8 pass3\n";
    prettyPrint(zeroFramedAry, prettyPrintFile);
    prettyPrintFile << "*** EQTable after pass3\n";
    printEQTable(prettyPrintFile);

    logFile << "*** Leaving connected8() ***\n";
}


void connected4(ofstream& prettyPrintFile, ofstream& logFile) {
    logFile << "*** Entering connected4() ***\n";

    connect4Pass1();
    prettyPrintFile << "*** After connected4 pass1, newLabel = " << newLabel << "\n";
    prettyPrint(zeroFramedAry, prettyPrintFile);
    prettyPrintFile << "*** EQTable after pass1\n";
    printEQTable(prettyPrintFile);

    connect4pass2();
    prettyPrintFile << "*** After connected4 pass2\n";
    prettyPrint(zeroFramedAry, prettyPrintFile);
    prettyPrintFile << "*** EQTable after pass2\n";
    printEQTable(prettyPrintFile);

    trueNumCC = manageEQTableFinal();
    prettyPrintFile << "*** After manageEQTable, trueNumCC = " << trueNumCC << "\n";
    printEQTable(prettyPrintFile);

    delete[] CCproperty;
    CCproperty = new Property[trueNumCC + 1];


    newMin = 0;
    newMax = trueNumCC;

    connectPass3(zeroFramedAry, EQTable, CCproperty, trueNumCC, logFile);

    prettyPrintFile << "*** After connected4 pass3\n";
    prettyPrint(zeroFramedAry, prettyPrintFile);
    prettyPrintFile << "*** EQTable after pass3\n";
    printEQTable(prettyPrintFile);

    logFile << "*** Leaving connected4() ***\n";
}


};

int main(int argc, char** argv) {
    if(argc != 7){
        cout << "Your command line must include 6 parameters: Input file, connectedness (4 or 8), prettyPrintFile, labelFile, propertyFile, and a logFile.";
        return 1;
    }

    string inputFile = argv[1];
    int connectedness = stoi(argv[2]);
    string prettyPrintFile = argv[3];
    string labelFile = argv[4];
    string propertyFile = argv[5];
    string logFileName = argv[6];

    ifstream inFile(inputFile);
    ofstream prettyPrint(prettyPrintFile);
    ofstream labelOut(labelFile);
    ofstream propertyOut(propertyFile);
    ofstream logFile(logFileName);

    if(!inFile.is_open()) {
        cout << "Error opening input file: " << inputFile << endl;
        return 1;
    }
    if(!prettyPrint.is_open()) {
        cout << "Error opening prettyPrint file: " << prettyPrintFile << endl;
        return 1;
    }
    if(!labelOut.is_open()) {
        cout << "Error opening labelFile: " << labelFile << endl;
        return 1;
    }
    if(!propertyOut.is_open()) {
        cout << "Error opening propertyFile: " << propertyFile << endl;
        return 1;
    }
    if(!logFile.is_open()) {
        cout << "Error opening logFile: " << logFileName << endl;
        return 1;
    }

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows >> numCols >> minVal >> maxVal;

    ccLablel cc(numRows, numCols, minVal, maxVal, (numRows*numCols)/4);
    cc.zero2D(cc.zeroFramedAry);
    cc.loadImage(inFile, cc.zeroFramedAry);

    prettyPrint << "*** Below is the input image ***\n";
    cc.prettyPrint(cc.zeroFramedAry, prettyPrint);

    if(connectedness == 4)
        cc.connected4(prettyPrint, logFile);
    else if(connectedness == 8)
        cc.connected8(prettyPrint, logFile);
    else {
        cout << "Error: connectedness must be 4 or 8.\n";
        return 1;
    }

    prettyPrint << "*** The number of true connected components in the image is: " 
                << cc.trueNumCC << "\n";

    labelOut << numRows << " " << numCols << " " << cc.newMin << " " << cc.newMax << "\n";
    cc.printImg(cc.zeroFramedAry, labelOut);

    propertyOut << "*** Printing the property File\n";
    cc.printCCProperty(propertyOut);

    cc.drawBoxes(cc.zeroFramedAry, cc.CCproperty, cc.trueNumCC, logFile);

    prettyPrint << "*** Printing the connected components with bounding boxes ***\n";
    cc.prettyPrint(cc.zeroFramedAry, prettyPrint);

    inFile.close();
    prettyPrint.close();
    labelOut.close();
    propertyOut.close();
    logFile.close();
    //:3
    return 0;
}



