#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Image{
public:    
    int numRows;
    int numCols;
    int maxVal;
    int minVal;
    int* histAry;
    int** imgAry;
    int** tmpAry;
    //int thrVal;
    
        Image(int numRows, int numCols, int maxVal, int minVal){
            this->numRows = numRows;
            this->numCols = numCols;
            this->maxVal = maxVal;
            this->minVal = minVal;
            //this->thrVal = thrVal;
            
            histAry = new int[maxVal + 1]{0}; //intialize all to zero
            imgAry = new int*[numRows];
            tmpAry = new int*[numRows];

            for(int i =0; i< numRows; i++){
                imgAry[i] = new int[numCols]{0};
                tmpAry[i] = new int[numCols]{0};
            }
            
            
        }

        void loadImage(int** imgAry, ifstream& inFile){ 
            while(inFile){
                for(int i=0; i<numRows; i++){
                    for(int j=0; j<numCols; j++){
                        inFile >> imgAry[i][j];
                    }
                }
            }

        }

        void computeHist(int** imgAry, int* histAry, ofstream& logFile){
            logFile << "Entering computeHist() \n";
            int i = 0;
            while(i < numRows){
                int j = 0; 
                while(j < numCols){
                    int val = imgAry[i][j];
                    if(val < minVal or val > maxVal){
                        logFile << "imgAry["  << i << "," << j << "]" << " Value is not within minVal and maxVal \n";
                        exit(1);
                    }
            
                    histAry[val] = histAry[val] + 1;
                    j++;
                }
            i++;                
        }
    
        logFile << "Leaving computeHist() \n";
    };

    void printHist(int* histAry, ofstream& histCountFile){
        histCountFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for(int i = 0; i<=maxVal; i++){
            histCountFile << i << "\t" << histAry[i] << "\n";
        }
    }

    void dispHist(int* histAry, ofstream& histGraphFile){
        histGraphFile << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for(int i = 0; i<=maxVal; i++){
            int temp = histAry[i];
            histGraphFile << i << "\t" << "(" << histAry[i] << "): ";
            while(temp > 0){
                histGraphFile << "+";
                temp--;
            }
            histGraphFile << "\n";
        }

    }

    void binThreshold(int** imgAry, int** tmpAry, int thrVal, ofstream& outFile, ofstream& logFile){
        logFile << "Entering binaryThreshold() \n";
        outFile << "\n*** The result of the binary threshold using threshold value = " << thrVal << " ***\n";
        int i = 0;
        while(i < numRows){
            int j = 0;
            while(j < numCols){
                if(imgAry[i][j] >= thrVal ){
                    logFile << "1 ";
                    tmpAry[i][j] = 1;
                }
                    else{
                        logFile << "0 ";
                        tmpAry[i][j] = 0;
                    }
                    j++;
            }
            //outFile << "\n";
            logFile << "\n";
        i++;
    }     
        logFile << "Leaving binaryThreshold() \n";
    }

    void nonBinThreshold(int** imgAry, int** tmpAry, int thrVal, ofstream& outFile, ofstream& logFile){
        logFile << "Entering nonBinaryThreshold() \n";
        outFile << "\n*** The result of the non binary thresholding using threshold value = " << thrVal << " ***\n";

        int i = 0;
        while(i < numRows){
            int j = 0;
            while(j < numCols){
                if(imgAry[i][j] >= thrVal){
                    tmpAry[i][j] = imgAry[i][j];
                }
                    else{
                        tmpAry[i][j] = 0;
                    }
                    j++;
            }

            i++;
        }

        logFile << "Leaving nonBinaryThreshold() \n";
    }

    void prettyPrint(int** Ary, ofstream& choiceFile){
        
        int min = 0;
        int max = 0;

        for(int i =0; i <numRows; i++){
            for(int j=0; j<numCols; j++){
                if(Ary[i][j] < min) min = Ary[i][j];
                if(Ary[i][j] > max) max = Ary[i][j];
            }
        }
        
        choiceFile << numRows << " " << numCols << " " << min << " " << max << " \n";
        string str = to_string(maxVal);
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



};

int main(int argc, char** argv){

    if(argc!=7){
        cout << "Your command line must include 6 parameters: Input file, Threshold value, histCountFile, histGraphFile, outFile and logFile.";
        exit(1);
    }

    string inputFile = argv[1];
    string histCountFile = argv[3];
    string histGraphFile = argv[4];
    string outFile = argv[5];
    string logFile = argv[6];

    ifstream inFile(inputFile);
    if(!inFile.is_open()){
        cout << "Error opening input file!" << endl;
        exit(1);
    }

    ofstream count(histCountFile);
    if(!count.is_open()){
        cout << "Error opening histCountFile file!" << endl;
        exit(1);
    }

    ofstream graph(histGraphFile);
    if(!graph.is_open()){
        cout << "Error opening histGraphFile file!" << endl;
        exit(1);
    }

    ofstream output(outFile);
    if(!output.is_open()){
        cout << "Error opening output file!" << endl;
        exit(1);
    }

    ofstream log(logFile);
    if(!log.is_open()){
        cout << "Error opening log file!" << endl;
        exit(1);
    }

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;

    int* histAry = new int[maxVal+1]{0};
    int** tmpAry = new int*[numRows];
    int** imgAry = new int*[numRows];

    for(int i =0; i< numRows; i++){
        imgAry[i] = new int[numCols]{0};
        tmpAry[i] = new int[numCols]{0};
    }

    Image image(numRows, numCols, maxVal, minVal);

    image.loadImage(imgAry, inFile);
    output << "*** Below is the input image ***\n";
    
    image.prettyPrint(imgAry, output);
    image.computeHist(imgAry, histAry, log);
    image.printHist(histAry, count);
    image.dispHist(histAry, graph);

    int thrVal = stoi(argv[2]);
    log << "The threshold value = " << thrVal << ". \n";
    image.binThreshold(imgAry, tmpAry, thrVal, output, log);
    image.prettyPrint(tmpAry, output);
    image.nonBinThreshold(imgAry, tmpAry, thrVal, output, log);
    image.prettyPrint(tmpAry, output);

    inFile.close();
    count.close();
    graph.close();
    output.close();
    log.close();

}
