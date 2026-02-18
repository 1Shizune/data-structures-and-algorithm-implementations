#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class Enhancement{
public:
    int numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax, newMin, newMax, thrVal;
    int** mirrorFmAry;
    int** avgAry;
    int** medianAry;
    int** GaussAry;
    int** thrAry;
    int** mask2DAry;
    int** thrAvgAry;
    int** thrMedianAry;
    int** thrGaussAry;
    int mask1DAry[9];
    int maskWeight;
    int neighbor1DAry[9];

    Enhancement(int numRows, int numCols, int minVal, int maxVal, int maskRows, int maskCols, int maskMin, int maskMax){
        this->numRows = numRows;
        this->numCols = numCols;
        this->maxVal = maxVal;
        this->minVal = minVal;
        this->maskRows = maskRows;
        this->maskCols = maskCols;
        this->maskMin = maskMin;
        this->maskMax = maskMax;
        //this->newMin = newMin;
        //this->newMax = newMax;
        //this->maskWeight = maskWeight;
        mirrorFmAry = new int*[numRows+2];
        avgAry = new int*[numRows+2];
        medianAry = new int*[numRows+2];
        GaussAry = new int*[numRows+2];
        thrAry = new int*[numRows+2];
        mask2DAry = new int*[maskRows];
        thrAvgAry = new int*[numRows+2];
        thrMedianAry = new int*[numRows+2];
        thrGaussAry = new int*[numRows+2];

        for(int i =0; i< numRows+2; ++i){
            mirrorFmAry[i] = new int[numCols+2]{0};
            avgAry[i] = new int[numCols+2]{0};
            medianAry[i] = new int[numCols+2]{0};
            GaussAry[i] = new int[numCols+2]{0};
            thrAry[i] = new int[numCols+2]{0};
            thrAvgAry[i] = new int[numCols+2]{0};
            thrMedianAry[i] = new int[numCols+2]{0};
            thrGaussAry[i] = new int[numCols+2]{0};
            //mask2DAry[i] = new int[numCols+2]{0};

        };

        for(int i=0; i<maskRows; i++){
            mask2DAry[i] = new int[maskCols]{0};
        }

    }

    void binThreshold(int** inAry, int** outAry, int thrVal, ofstream& logFile){
        logFile << "\n *** Entering binaryThreshold() ***\n";
        int i = 0;
        for(int i =0; i<=numRows; i++){
            for(int j = 0; j<=numCols; j++){
                outAry[i][j] = (inAry[i][j] >= thrVal) ? 1 : 0;
            }
        }
            logFile << "\n*** Leaving binaryThreshold() ***\n";
        }

    void prettyPrint(int** Ary, ofstream& choiceFile){
        
        int min = 9999;
        int max = 0;

        for(int i =0; i <numRows; i++){
            for(int j=0; j<numCols; j++){
                if(Ary[i][j] < min) min = Ary[i][j];
                if(Ary[i][j] > max) max = Ary[i][j];
            }
        }
        
        choiceFile << numRows << " " << numCols << " " << min << " " << max << " \n";
        string str = to_string(max);
        int width = str.length();
        int i =0;
        while(i < numRows){
            int j =0;
            while(j < numCols){
                if(Ary[i][j] > 0){
                    choiceFile << Ary[i][j] << " ";
                }
                    else{
                        choiceFile << ".";
                    }

                    string str2 = to_string(Ary[i][j]);
                    int WW = str2.length();
                    while(WW <= width){
                        choiceFile << " ";
                        WW++;
                    }
                j++;    
            }
            choiceFile << "\n";
            i++;
        }
    }

    void printMask(int** mask2DAry, ofstream& choiceFile) {
        choiceFile << maskRows << " " << maskCols << " " << maskMin << " " << maskMax << " \n";
        for (int i = 0; i < maskRows; i++) {
            for (int j = 0; j < maskCols; j++) {
                choiceFile << mask2DAry[i][j] << " ";
            }
            choiceFile << "\n";
        }
    }

    
    void mirrorFraming(int** ary, ofstream& logFile){
        logFile << "\n*** Entering mirrorFraming() ***\n";
        
        for(int i =1; i<=numCols; i++){
               ary[0][i] = ary[1][i]; 
            }
        
        for(int i=1; i<=numCols; i++){
            ary[numRows+1][i] = ary[numRows][i];
        }
        
        for(int i =0; i<= numRows+1; i++){
            ary[i][0] = ary[i][1];
        }

        for(int i= 0; i<=numRows+1; i++){
            ary[i][numCols+1] = ary[i][numCols];
        }

        logFile << "\n*** Leaving mirrorFraming() ***\n";
    }

    void loadImage(int** mirrorFmAry, ifstream& inFile){ 
            for(int i=1; i<=numRows; i++){
                for(int j=1; j<=numCols; j++){
                    if(!(inFile >> mirrorFmAry[i][j])){
                        cerr << "Error: Not enough pixels in input file\n";
                        return;
                    }
                }
            }
    }

    int loadMask(ifstream& maskFile, int** mask2DAry, ofstream& logFile){
        logFile << "\n*** Entering loadMask() ***\n";
        int maskWeight = 0;
        for(int i = 0; i<maskRows; i++){
            for(int j=0; j<maskCols; j++){
                maskFile >> mask2DAry[i][j];
                maskWeight += mask2DAry[i][j];
            }
        }

        logFile << "\n*** Exiting loadMask() ***\n";
        return maskWeight;
    }

    void loadMask2Dto1D(int** mask2DAry, int mask1DAry[9], ofstream& logFile){
        logFile << "\n*** Entering loadMask2Dto1D() ***\n";
        int index = 0;
        for(int i =0; i<maskRows; i++){
            for(int j=0; j<maskCols; j++){
                mask1DAry[index] = mask2DAry[i][j];
                index++;
            }
        }
        logFile << "\n*** Leaving loadMask2Dto1D() ***\n";
    }

    void loadNeighbor2Dto1D(int** mirrorFmAry, int i, int j, int neighbor1DAry[9], ofstream& logFile){
        logFile << "\n*** Entering loadNeighbor2Dto1D() ***\n";
        int index = 0;
        
        for(int k = -1; k<=1; k++){ //3x3 window
            for(int l= -1; l<=1; l++){
                neighbor1DAry[index] = mirrorFmAry[i+k][j+l];
                index++;               
            }
        }
        
        logFile << "\n*** Leaving loadNeighbor2dto1D() ***\n";
    }

    void sortArray(int neighbor1DAry[9]){
        sort(neighbor1DAry, neighbor1DAry + 9);
    }

    void computeMedian(int** mirrorFmAry, int** medianAry, ofstream& logFile){
        logFile << "\n*** Entering computeMedian() ***\n";

        int newMin = 9999;
        int newMax = 0;
        int i = 1;
    while(i<=numRows){
        int j = 1;
        while(j <= numCols){
            loadNeighbor2Dto1D(mirrorFmAry, i, j, neighbor1DAry, logFile);
            logFile << "\n*** Below is the conversion of mirrorFmAry[ "<< i << "][ " << j << "] to 1D array ***\n";
            print1DAry(neighbor1DAry, logFile);
            sortArray(neighbor1DAry);
            logFile << "\n*** Below is the sorted neighbor array for values i = " << i << " and j = " << j << " ***\n";
            print1DAry(neighbor1DAry, logFile);
            medianAry[i][j] = neighbor1DAry[4];
        
            if(newMin > medianAry[i][j]){
                newMin = medianAry[i][j]; 
            }
                if(newMax < medianAry[i][j]){
                    newMax = medianAry[i][j];
                }
                j++;
        }
        i++;
    }
        logFile << "\n*** Leaving computeMedian() ***\n";
    }

    void computeAvg(int** mirrorFmAry, int** avgAry, ofstream& logFile){
        logFile << "\n*** Entering computeAvg() ***\n";

        int newMin = 9999;
        int newMax = 0;
        
        for(int i =1; i <= numRows; i++){
            for(int j =1; j<= numCols; j++){
                int sum = 0;
                
                for(int r = -1; r<=1; r++){ //loop 3x3 neigborhood
                    for(int c =-1; c<=1; c++){
                        sum+= mirrorFmAry[i+r][j+c];
                    }
                }
                
                avgAry[i][j] = sum/9;
                if(avgAry[i][j] < newMin){
                    newMin = avgAry[i][j];
                }
                if(avgAry[i][j] > newMax){
                    newMax = avgAry[i][j];
                }
            }

        }
        this->newMin = newMin;
        this->newMax = newMax;
        logFile << "\n*** Leaving computeAvg() ***\n";
    }

    int convolution(int neighbor1DAry[9], int mask1DAry[9], ofstream& logFile) {
        logFile << "\n *** Entering convolution() ***\n";
    
        double result = 0.0;
        for (int i = 0; i < 9; i++) {
            result += static_cast<double>(neighbor1DAry[i]) * mask1DAry[i];
        }

        if (maskWeight == 0) {
            logFile << "Warning: maskWeight was 0, forcing to 1 to avoid divide-by-zero.\n";
            maskWeight = 1;
        }

        result /= static_cast<double>(maskWeight);

        logFile << "Convolution sum / maskWeight = " << result << "\n";
        logFile << "*** Leaving convolution() ***\n";

        return static_cast<int>(result + 0.5);
}

    
    void computeGauss(int** mirrorFmAry, int** GaussAry, ofstream& logFile){
        logFile << "\n *** Entering computeGauss() ***\n";
        int newMin = 9999;
        int newMax = 0;
        int i = 1;
        while(i <= numRows){
            int j = 1;
            while(j <= numCols){
                loadNeighbor2Dto1D(mirrorFmAry, i, j, neighbor1DAry, logFile);
                logFile << "\n*** Below is the conversion of mirrorFmAry[ "<< i << "][ " << j << "] to 1D array ***\n";
                print1DAry(neighbor1DAry, logFile);
                logFile << "\n *** Below is mask1DAry i = " << i << " j = " << j << " ***\n";
                print1DAry(mask1DAry, logFile);
                GaussAry[i][j] = convolution(neighbor1DAry, mask1DAry, logFile);
                if(newMin > GaussAry[i][j]){
                    newMin = GaussAry[i][j];
                }
                    if(newMax < GaussAry[i][j]){
                        newMax = GaussAry[i][j];
                    }
                    j++;
            }
            i++;
        }
            
        this->newMin = newMin;
        this->newMax = newMax;
        logFile << "\n *** Leaving computeGauss() ***\n";
    
    }

    void print1DAry(int* ary, ofstream& logFile){
        logFile << "\n *** Entering print1DAry() ***";
        for(int i=0; i<9; i++){
            logFile << ary[i] << " ";
        }

        logFile << "\n *** Leaving print1DAry() ***\n";
    }


};

int main(int argc, char** argv){
    cout << "Program started. argc=" << argc << endl;
    if(argc!= 8){
        cout << "Your command line must include 7 parameters: Input file, mask file, threshold value, AvgFile, MedianFile, GaussFile, and a logFile.";
        return 1;
    }
    
    string inputFile = argv[1];
    string maskFile = argv[2];
    int thrVal = stoi(argv[3]);
    string AvgFile = argv[4];
    string medianFile = argv[5];
    string GaussFile = argv[6];
    string logFile = argv[7];
    ifstream inFile(inputFile);
    if(!inFile.is_open()){
        cout << "Error opening input file!" << endl;
        return 1;
    } 

    ifstream mask(maskFile);
    if(!mask.is_open()){
        cout << "Error opening mask file!" << endl;
        return 1;
    }

    ofstream avg(AvgFile);
    if(!avg.is_open()){
        cout << "Error opening Average file!" << endl;
        return 1;
    }

    if (!avg.is_open()) cout << "avg.txt failed to open\n";

    ofstream median(medianFile);
    if(!median.is_open()){
        cout << "Error opening Median file!" << endl;
        return 1;
    }

    ofstream Gauss(GaussFile);
    if(!Gauss.is_open()){
        cout << "Error opening Gauss file!" << endl;
        return 1;
    }

    ofstream log(logFile);
    if(!log.is_open()){
        cout << "Error opening log file!" << endl;
        return 1;
    }
    
    int numRows, numCols, minVal, maxVal;
    inFile >> numRows >> numCols >> minVal >> maxVal;
    int maskRows, maskCols, maskMin, maskMax;
    mask >> maskRows >> maskCols >> maskMin >> maskMax;

        Enhancement enhancement(numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax);
        enhancement.thrVal = thrVal;

        enhancement.maskWeight = enhancement.loadMask(mask, enhancement.mask2DAry, log);
        enhancement.loadImage(enhancement.mirrorFmAry, inFile);
        enhancement.mirrorFraming(enhancement.mirrorFmAry, log);

        avg << "\n *** Below is the mirror framed input image ***\n";
        enhancement.prettyPrint(enhancement.mirrorFmAry, avg);
        enhancement.computeAvg(enhancement.mirrorFmAry, enhancement.avgAry, log);
        avg << "\n *** Below is the 3x3 averaging of the input image ***\n";
        enhancement.prettyPrint(enhancement.avgAry, avg);
        enhancement.binThreshold(enhancement.avgAry, enhancement.thrAvgAry, thrVal, log);
        avg << "\n *** Below is the result of the binary threshold of averaging image ***\n";
        enhancement.prettyPrint(enhancement.thrAvgAry, avg);

        
        enhancement.computeMedian(enhancement.mirrorFmAry, enhancement.medianAry, log);
        median << "\n *** Below is the mirror framed input image ***\n";
        enhancement.prettyPrint(enhancement.mirrorFmAry, median);
        median << "\n *** Below is the 3x3 median filter of the input image ***\n";
        enhancement.prettyPrint(enhancement.medianAry, median);
        enhancement.binThreshold(enhancement.medianAry, enhancement.thrMedianAry, thrVal, log);
        median << "\n *** Below is the result of the binary threshold of median filtered image ***\n";
        enhancement.prettyPrint(enhancement.thrMedianAry, median);
        
        Gauss << "\n *** Below is the mirror framed input image ***\n";
        enhancement.prettyPrint(enhancement.mirrorFmAry, Gauss);
        Gauss << "\n *** Below is the mask for Gaussian filter ***\n";
        enhancement.printMask(enhancement.mask2DAry, Gauss);
        enhancement.loadMask2Dto1D(enhancement.mask2DAry, enhancement.mask1DAry, log);
        enhancement.computeGauss(enhancement.mirrorFmAry, enhancement.GaussAry, log);
        Gauss << "\n *** Below is the 3x3 Gaussian filter of the input image ***\n";
        enhancement.prettyPrint(enhancement.GaussAry, Gauss);
        enhancement.binThreshold(enhancement.GaussAry, enhancement.thrGaussAry, thrVal, log);
        Gauss << "\n *** Below is the result of the binary threshold of Gaussian filtered image ***\n";
        enhancement.prettyPrint(enhancement.thrGaussAry, Gauss);


        inFile.close();
        mask.close();
        avg.close();
        median.close();
        Gauss.close();
        log.close();


}
