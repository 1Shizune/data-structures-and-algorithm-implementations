#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class imgCompress{
public: 
    int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal, distanceChoice;
    int** ZFAry;
    int** skeletonAry;

        imgCompress(int numRows, int numCols, int minVal, int maxVal, int newMinVal, int newMaxVal, int distanceChoice){
            this->numRows = numRows;
            this->numCols = numCols;
            this->minVal = minVal;
            this->maxVal = maxVal;
            this->newMinVal = newMinVal;
            this->newMaxVal =newMaxVal;
            this->distanceChoice = distanceChoice;
            ZFAry = new int*[numRows + 2];
            skeletonAry = new int*[numRows +2];
        
            for(int i=0; i<numRows+2; ++i){
                ZFAry[i] = new int[numCols+2]{0};
                skeletonAry[i] = new int[numCols+2]{0};
            }
        
        }

        void reset(){
            setZero(ZFAry);
            setZero(skeletonAry);
        }
    
        void setZero(int** Ary){
            for(int i = 0; i<numRows+2; i++){
                for(int j =0; j<numCols+2; j++){
                    Ary[i][j] = 0;
                }
            }
        }

        void loadImage(ifstream& inFile, int** Ary){ //the header of the input file has been noted in main, only send the picture without the header
            for(int i=1; i<=numRows; i++){
                for(int j=1; j<=numCols; j++){
                    inFile >> Ary[i][j]; 
                }
            }
        }

        void loadSkeleton(ifstream& skeletonFile, int** Ary, ofstream& logFile){
            logFile << "*** Entering loadSkeleton() ***";
            int i, j, val;
            while(skeletonFile >> i >> j >> val){
                Ary[i][j] = val;
            }
            logFile << "*** Leaving loadSkeleton() ***";
        }

        void distancePass1(int** ZFAry, int distanceChoice, ofstream& logFile){
            logFile << "*** Entering distancePass1() ***\n";
            for(int i=1; i<=numRows; i++){
                for(int j =1; j<=numCols; j++){
                    if(ZFAry[i][j] > 0){
                        int a = ZFAry[i-1][j-1];
                        int b = ZFAry[i-1][j];
                        int c = ZFAry[i-1][j+1];
                        int d = ZFAry[i][j-1];

                        if(distanceChoice == 4){
                            ZFAry[i][j] = min(b,d) + 1;
                        }
                            else if(distanceChoice == 8){
                                ZFAry[i][j] = min({a,b,c,d}) + 1;
                            }

                    }
                }
            }
            logFile << "*** Leaving distancePass1() ***\n";
        }

        void distancePass2(int** ZFAry, int distanceChoice, ofstream& logFile){
            for(int i = numRows; i>=1; --i){ //scan bottom to top
                for(int j = numCols; j>=1; --j){ //scan right to left
                    int e = ZFAry[i][j+1]; //right
                    int f = ZFAry[i][j-1]; //low left
                    int g = ZFAry[i+1][j]; //down
                    int h = ZFAry[i+1][j+1]; //low right

                    if(distanceChoice == 4){
                        ZFAry[i][j] = min({ZFAry[i][j], e+1, g+1});
                    }
                        else if(distanceChoice == 8){
                            ZFAry[i][j] = min({ZFAry[i][j], e+1, f+1, g+1, h+1});
                        }

                }
            }
            logFile << "*** Leaving distancePass2() ***\n";
        }

        void expansionPass1(int** ZFAry, int distanceChoice, ofstream& logFile) {
            for (int i = 1; i <= numRows; ++i) {
                for (int j = 1; j <= numCols; ++j) {
                    int val = ZFAry[i][j];
                    if (val > 0) {

                        for (int di = -1; di <= 1; ++di) {
                            for (int dj = -1; dj <= 1; ++dj) {
                                if (di == 0 && dj == 0) continue;
                                if (distanceChoice == 4 && abs(di) + abs(dj) == 2) continue;
                                int ni = i + di;
                                int nj = j + dj;
                                if (ni >= 1 && ni <= numRows && nj >= 1 && nj <= numCols) {
                                    ZFAry[ni][nj] = max(ZFAry[ni][nj], val - 1);
                                }
                            }
                        }
                    }
                }
            }
        }


        void expansionPass2(int** ZFAry, int distanceChoice, ofstream& logFile) {
            for (int i = numRows; i >= 1; --i) {
                for (int j = numCols; j >= 1; --j) {
                    int val = ZFAry[i][j];
                    if (val > 0) {
                
                        for (int di = -1; di <= 1; ++di) {
                            for (int dj = -1; dj <= 1; ++dj) {
                                if (di == 0 && dj == 0) continue;
                                if (distanceChoice == 4 && abs(di) + abs(dj) == 2) continue;
                                int ni = i + di;
                                int nj = j + dj;
                                if (ni >= 1 && ni <= numRows && nj >= 1 && nj <= numCols) {
                                    ZFAry[ni][nj] = max(ZFAry[ni][nj], val - 1);
                                }
                            }
                        }
                    }
                }
            }
        }


        bool IsLocalMaxima(int** ZFAry, int i, int j, int distanceChoice){
            
            int p = ZFAry[i][j];
            if(p <= 0){
                return false;
            }
            
            if(distanceChoice == 4){
                int neighbors[4] = { ZFAry[i-1][j], ZFAry[i+1][j], ZFAry[i][j-1], ZFAry[i][j+1] };
                for(int n = 0; n <4; ++n){
                if(p < neighbors[n]){
                   return false; 
                }
            }
        }
            else if(distanceChoice == 8){
                int neighbors[8] = { ZFAry[i-1][j-1], ZFAry[i-1][j], ZFAry[i-1][j+1], ZFAry[i][j-1], ZFAry[i][j+1], ZFAry[i+1][j-1], ZFAry[i+1][j], ZFAry[i+1][j+1]};
                for(int n = 0; n<8; ++n){
                    if(p < neighbors[n]){
                    return false;
                }
            }
        }
         return true;
    }
        
        void computeLocalMaxima(int** ZFAry, int** skeletonAry, int distanceChoice, ofstream& logFile){
            logFile << "*** Entering computeLocalMaxima() ***\n";

            for(int i = 1; i <= numRows; ++i){
                for(int j = 1; j<=numCols; ++j){
                    if(IsLocalMaxima(ZFAry, i, j, distanceChoice)){
                        skeletonAry[i][j] = ZFAry[i][j];
                    }
                        else{
                            skeletonAry[i][j] = 0;
                        }
                }
            }

            int skeletonCount = 0;
            for(int i=1; i<=numRows; ++i){
                for(int j=1; j<=numCols; ++j){
                    if(skeletonAry[i][j] > 0) skeletonCount++;
                }
            }
            logFile << "Skeleton points found: " << skeletonCount << "\n";
            cout << "Skeleton points found: " << skeletonCount << endl;

            logFile << "*** Leaving computeLocalMaxima() ***\n";
        }

        void extractSkeleton(int** skeletonAry, ofstream& skeletonFile, ofstream& logFile){
            for(int i = 1; i<=numRows; ++i){
                for(int j =1; j<=numCols; ++j){
                    if(skeletonAry[i][j] > 0){
                        skeletonFile << i << " " << j << " " << skeletonAry[i][j] << "\n";
                    }
                }
                    skeletonFile << "\n";
            }
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

        void binThreshold(int** ZFAry, ofstream& decompressedFile){
            for(int i = 1; i<=numRows; ++i){
                for(int j = 1; j<=numCols; j++){
                    if(ZFAry[i][j] >= 1){
                        decompressedFile << "1 ";
                    
                    }
                    else{
                        decompressedFile << "0 ";
                    }
                }
                decompressedFile << "\n";
            }
        }


        void distanceTransform(int** ZFAry, int distanceChoice, ofstream& prettyPrintFile, ofstream& logFile){
            logFile << "*** Entering distanceTransform() ***\n";
            distancePass1(ZFAry, distanceChoice, logFile);
            prettyPrintFile << "*** Result of 1st pass distance transforms with choice = " << distanceChoice << " ***\n";
            prettyPrint(ZFAry, prettyPrintFile);
            distancePass2(ZFAry, distanceChoice, logFile);
            prettyPrintFile << "*** Result of 2nd pass distance transforms with choice = " << distanceChoice << " ***\n";
            prettyPrint(ZFAry, prettyPrintFile);
            logFile << "*** Leaving distanceTransform() ***\n";
        }

        void compression(int** ZFAry, int distanceChoice, int** skeletonAry, ofstream& skeletonFile, ofstream& prettyPrintFile, ofstream& logFile){
            logFile << "*** Entering compression() ***\n";
            computeLocalMaxima(ZFAry, skeletonAry, distanceChoice, logFile);
            prettyPrintFile << "*** Result of computeLocalMaxima skeletonAry with choice = " << distanceChoice << " ***\n";
            prettyPrint(skeletonAry, prettyPrintFile);
            extractSkeleton(skeletonAry, skeletonFile, logFile);
            logFile << "*** Leaving compression() ***\n";
            int writtenCount = 0;
            for(int i=1;i<=numRows;++i){
                for(int j=1;j<=numCols;++j){
                    if(skeletonAry[i][j] > 0){
                    skeletonFile << skeletonAry[i][j] << " ";
                    writtenCount++;
                }
            }
                skeletonFile << "\n";
        }
            logFile << "Wrote " << writtenCount << " skeleton points to file.\n";
        }

        void deCompression(int** ZFAry, int distanceChoice, ofstream& prettyPrintFile, ofstream& logFile){
            logFile << "*** Entering deCompression() ***\n";
            expansionPass1(ZFAry, distanceChoice, logFile);
            prettyPrintFile << "*** Result of 1st expansion pass with choice = " << distanceChoice << " ***\n";
            prettyPrint(ZFAry, prettyPrintFile);
            expansionPass2(ZFAry, distanceChoice, logFile);
            prettyPrintFile << "*** Result of 2nd pass expansion with choice = " << distanceChoice << " ***\n";
            prettyPrint(ZFAry, prettyPrintFile);
            logFile << "*** Leaving deCompression() ***\n";
        }

};

int main(int argc, char** argv){
if(argc != 7){
    cout << "Your command line must include 6 parameters: Input file, distance choice, prettyPrintFile, skeletonFile, decompressedFile, and a logFile.";
    return 1;
}

    string inputFile = argv[1];
    int distanceChoice = stoi(argv[2]);
    string prettyPrintFile = argv[3];
    string skeletonFile = argv[4];
    string decompressedFile = argv[5];
    string logFile = argv[6];

    ifstream inFile(inputFile);
    if(!inFile.is_open()){
        cout << "Error opening input file.";
        return 1;
    }

    ofstream prettyPrint(prettyPrintFile);
    if(!prettyPrint.is_open()){
        cout << "Error opening prettyPrintFile.";
        return 1;
    }

    ofstream skeleton(skeletonFile);
    if(!skeleton.is_open()){
        cout << "Error opening skeletonFile.";
        return 1;
    }   

    ofstream decompressed(decompressedFile);
    if(!decompressed.is_open()){
        cout << "Error opening decompressedFile.";
        return 1;
    }

    ofstream log(logFile);
    if(!log.is_open()){
        cout << "Error opening logFile.";
        return 1;
    }

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows >> numCols >> minVal >> maxVal;

    imgCompress compress(numRows, numCols, minVal, maxVal, 9999, -9999, distanceChoice);
    compress.reset();
    compress.loadImage(inFile, compress.ZFAry);
    prettyPrint << "*** Below is the input image ***\n";
    compress.prettyPrint(compress.ZFAry, prettyPrint);
    compress.distanceTransform(compress.ZFAry, distanceChoice, prettyPrint, log);
    compress.compression(compress.ZFAry, distanceChoice, compress.skeletonAry, skeleton, prettyPrint, log);
    
    skeleton.close();
    ifstream checkSkeleton(skeletonFile);
    if (checkSkeleton.peek() == ifstream::traits_type::eof()) {
        cout << "Skeleton file is empty after compression step!" << endl;
        log << "Skeleton file empty after compression." << endl;
        return 0;
    }
        checkSkeleton.close();


    ifstream skeletonIn(skeletonFile);
    if(!skeletonIn.is_open()){
        cout << "Error reopening skeleton file.";
        return 1;
    }

    compress.setZero(compress.ZFAry);
    compress.loadSkeleton(skeletonIn, compress.ZFAry, log);
    prettyPrint << "*** Below is the loaded skeleton with choice = " << distanceChoice << " ***\n";
    compress.prettyPrint(compress.ZFAry, prettyPrint);
    compress.deCompression(compress.ZFAry, distanceChoice, prettyPrint, log);
    decompressed << numRows << " " << numCols << " " << minVal << " " << maxVal << " \n";
    compress.binThreshold(compress.ZFAry, decompressed);

    inFile.close();
    prettyPrint.close();
    skeletonIn.close();
    decompressed.close();
    log.close();

return 0;
}
