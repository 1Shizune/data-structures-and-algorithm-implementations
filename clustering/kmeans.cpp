#include<iostream>
#include <fstream>
#include<cmath>
using namespace std;

class Kmean{
    public:
    class Point{
		public:
		    double Xcoord= 0.0;
        double Ycoord= 0.0;
        int label= 0;
        double distance= 9999.00;

            Point() {};
			Point(double Xcoord, double Ycoord, int label, double distance){ //inner function constructor
                this->Xcoord = Xcoord;
                this->Ycoord = Ycoord;
                this->label = label;
                this->distance = distance;
            };
    };
	  int K;
    int numPts;
    Point *object = new Point[numPts]; //dynamic allocation using new, pair with delete after using
    int numRows;
    int numCols;
    int** displayAry;
    Point* KcentroidAry = new Point[K+1];
    int change;

	Kmean(int K, int numPts, int numRows, int numCols, int** displayAry)
    	: K(K), numPts(numPts), numRows(numRows), numCols(numCols) {
    	this->displayAry = new int*[numRows];
    	for(int i = 0; i < numRows; i++) {
        this->displayAry[i] = new int[numCols]();
    }  
    	this->object = new Point[numPts];  // Ensure the array is allocated correctly
} 

			void printPointSet(Point* pointSet, ofstream& logFile) {
    			for (int i = 0; i < numPts; i++) {
        		logFile << "*** pointSet index = " << i 
                << " Xcoord= " << pointSet[i].Xcoord
                << " Ycoord= " << pointSet[i].Ycoord
                << " label= " << pointSet[i].label
                << " distance= " << pointSet[i].distance << "\n";
    		}
		}  

        void loadPointSet(ifstream& inFile, Point* pointSet, ofstream& logFile) {
    		logFile << "*** Entering loadPointSet() method ***\n";
    		int index = 0;

    		double x, y;
    		while (inFile >> x >> y) {  
        	if (index >= numPts) {
            	logFile << "*** More points in file than expected! ***\n";
            	break;
        	}
        		pointSet[index].Xcoord = x;
        		pointSet[index].Ycoord = y;
        		pointSet[index].label = 0;
        		pointSet[index].distance = 9999.00;
        		index++;
    }
    	logFile << "*** After loading the pointSet, below are the pointSet info ***\n";
    	printPointSet(pointSet, logFile);
}
			
			void plotDisplayAry(Point* pointSet, int** displayAry){
    		for (int i = 0; i < numRows; i++) {
        		for (int j = 0; j < numCols; j++) {
            		displayAry[i][j] = 0;
        	}
    	}

    	
    	for (int i = 0; i < numPts; i++) { //Plot each point in pointSet
        int x = static_cast<int>(pointSet[i].Xcoord);  // Cast to int
        int y = static_cast<int>(pointSet[i].Ycoord);  // Cast to int
        
        if (y >= 0 && y < numRows && x >= 0 && x < numCols) {  // Ensure valid indices
            displayAry[y][x] = pointSet[i].label;
        }
    }
    		
    		for (int i = 0; i < K; i++) { //Plot each centroid in KcentroidAry
        		int x = static_cast<int>(KcentroidAry[i].Xcoord);  // Cast to int
        		int y = static_cast<int>(KcentroidAry[i].Ycoord);  // Cast to int
        
        		if (y >= 0 && y < numRows && x >= 0 && x < numCols) {  // Ensure valid indices
            	displayAry[y][x] = KcentroidAry[i].label;
        	}
		}
	}
			void computeCentroids(Point* pointSet, Point* KcentroidAry, ofstream& logFile){
				logFile << "*** Entering computeCentroids() method ***\n";
				double sumX[K+1]={0};
				double sumY[K+1]={0};
				int totalPt[K+1]={0};
				int index =0;
				while(index<numPts){
					int label = pointSet[index].label;
					sumX[label] += pointSet[index].Xcoord;
					sumY[label] += pointSet[index].Ycoord;
					totalPt[label]++;
					index++;
				}
					int label=1;
					while(label <= K){
if (totalPt[label] > 0) {
    KcentroidAry[label].Xcoord = (sumX[label] / totalPt[label]);
    KcentroidAry[label].Ycoord = (sumY[label] / totalPt[label]);
} else {
    logFile << "*** Warning the cluster # is empty ! Label= " << label << " ***\n";
}
					label++;
				}
					logFile << "*** Leaving computeCentroids() method ***\n";
			}
			
			double computeDist(Point pt, Point centroid){
				 int x1 = static_cast<int>(pt.Xcoord);
    			int y1 = static_cast<int>(pt.Ycoord);
    			int x2 = static_cast<int>(centroid.Xcoord);
    			int y2 = static_cast<int>(centroid.Ycoord);
    			return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
			}
			
			int DistanceMinLabel(Kmean::Point* pt, Kmean::Point* KcentroidAry, ofstream& logFile){
				logFile << "*** Entering DistanceMinLabel() method ***\n";
				logFile << "*** Before computation: pt.label= " << pt->label << ", pt.distance= " << pt->distance << " ***\n";
				double minDist = 9999;
				int minLabel =0;
				int label = 1;
				while(label <= K){
					double dist = computeDist(*pt, KcentroidAry[label]);
					if (dist < minDist - 0.0001){
						minDist = dist;
						minLabel = label;
					}
					label++;
				}
					pt->distance = minDist;
					pt->label = minLabel;	
						
					logFile << "*** After computation: pt.label= " << pt->label << ", pt.distance= " << pt->distance << " ***\n";
    				logFile << "*** Leaving DistanceMinLabel() method ***\n";

    				return minLabel; // Return the closest centroid label		
			}
			
			void zero2d(int** Ary){
				for(int i=0; i<numRows; i++){
					for(int j=0; j <numCols; j++){
						Ary[i][j] = 0;
					}
				}
			}
			
	void PrettyPrint(int** displayAry, ofstream& outFile1) {
    	for(int i = 0; i < numRows; i++) {
        	for(int j = 0; j < numCols; j++) {
            	if(displayAry[i][j] <= 0) {
                	outFile1 << " ";
            	} else if(displayAry[i][j] < 65) {
                	outFile1 << displayAry[i][j];
            	} else {
                	outFile1 << (char)displayAry[i][j];
            	}
        	}
        	outFile1 << endl;
    	}
    	outFile1.flush();
	}
			
			void assignLabel(Point* pointSet, int K) {
    			for (int i = 0; i < numPts; i++) {
        		pointSet[i].label = (i % K) + 1; // Cycle through K labels
    		}
		}
			
			void kMeansClustering(Point* pointSet, int K, Point* KcentroidAry, ofstream& outFile1, ofstream& logFile){
				logFile << "*** Entering kMeansClustering() method *** \n";
				int iteration =0;
				assignLabel(pointSet,K);
				int change = 1; // Start with a non-zero value to enter the loop
				while (change > 0) {
    			plotDisplayAry(pointSet, displayAry);
    			outFile1 << "*** Result of the iteration #" << iteration << " ***\n";
    			PrettyPrint(displayAry, outFile1);

    			computeCentroids(pointSet, KcentroidAry, logFile);
    			change = 0; // Reset change before checking all points
    			int index = 0;
    
				while (index < numPts) {
        		int oldLabel = pointSet[index].label;
        		DistanceMinLabel(&pointSet[index], KcentroidAry, logFile);

        		if (pointSet[index].label != oldLabel) {
            		change++; // Count changes correctly
        		}
        			index++;
    		}
    
    iteration++;
    logFile << "*** End of Iteration #" << iteration << ", Changes: " << change << " ***\n";
}
				logFile << "*** Leaving kMeansClusterin() method ***\n";
					
		}

};

int main(int argc, char* argv[]){
if(argc!=5){
		cout << "Your command line needs to include 4 parameters: Input file, K value, output file and log file! ";
		exit(1);
	}
	
int K = atoi(argv[1]);
string inFileName = argv[2];
string outFileName = argv[3];
string logFileName = argv[4];

ifstream inFile(inFileName);
ofstream outFile1(outFileName);
ofstream logFile(logFileName);
	
	if (!inFile) {
        cerr << "Error opening input file: " << argv[2] << endl;
        return 1;
    }
    if (!outFile1) {
        cerr << "Error opening output file: " << argv[3] << endl;
        return 1;
    }
    if (!logFile) {
        cerr << "Error opening log file: " << argv[4] << endl;
        return 1;
    }
    
    int numRows, numCols, numPts;
	if (!(inFile >> numRows >> numCols >> numPts)) {
    	logFile << "*** Error reading input file header! ***\n";
    	return 1;
	}

	Kmean::Point* pointSet = new Kmean::Point[numPts]; // Initialize pointSet with numPts
	 
    int** displayAry = new int*[numRows];
    for(int i=0; i < numRows; i++){
    	displayAry[i] = new int[numCols]; 
	}
	
	Kmean kmean(K, numPts, numRows, numCols, displayAry);
	kmean.zero2d(displayAry);
	
	//Kmean::Point* pointSet = new Kmean::Point[numPts];
	
	Kmean::Point* KcentroidAry = new Kmean::Point[K+1];
	
	
	kmean.loadPointSet(inFile, pointSet, logFile);
	
	kmean.kMeansClustering(pointSet, K, KcentroidAry, outFile1 ,logFile);
	
	logFile << "*** In main() After kMeansClustering, below are the pointSet info: ***\n";
	kmean.printPointSet(pointSet, logFile);
	
	inFile.close();
    outFile1.close();
    logFile.close();

    
    for (int i = 0; i < numRows; i++) { // Free dynamic allocated memory
        delete[] displayAry[i];
    }
    delete[] displayAry;
    delete[] pointSet;
    delete[] KcentroidAry;

    return 0;
    
}
