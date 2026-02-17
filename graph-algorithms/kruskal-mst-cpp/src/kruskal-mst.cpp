#include<iostream>
#include<fstream>
using namespace std;

class edge{
    public:
    int nU;
    int nW;
    int cost;
    edge* next;

    edge(int nU, int nW, int cost){
        this->nU = nU;
        this->nW = nW;
        this->cost = cost;
        next = nullptr;
    }

    void printEdge(edge* edge ,ofstream& logFile){     
            logFile << " (" << edge->nU << "," << edge->nW << "," << edge->cost << ")"; 
    }

};

class KruskalMST{
    public:
    int N; //number of nodes in the graph
    //int* whichSet = whichSet;
    int* whichSet;
    int numSets = N;
    int totalMSTCost;
    edge* edgeList;
    edge* msList;

        public:    
        KruskalMST(int N, edge* edgeList, edge* msList){
            this->N= N;
            numSets = N; 
            totalMSTCost = 0;
            this->edgeList = edgeList;
            this->msList = msList;       
            whichSet = new int[N+1];
            for(int i=0; i<N; i++){
                whichSet[i] = i;
            } 
        }

        void insertEdge(edge* listHead, edge* newEdge){
            edge* spot = findSpot(listHead, newEdge);
            newEdge->next = spot->next; //The new insertion points at the right of the spot
            spot->next = newEdge; //spot after the dummy node is the new edge
        }

        edge* findSpot(edge* listHead, edge* newEdge){
            edge* spot = listHead;
            while(spot->next!=nullptr && spot->next->cost <= newEdge->cost){ //start from head, if the cost of the current edge is less than the cost of the new edge keep moving forward (lowest to highest cost)
                spot = spot->next;
            }
                return spot;
        }

        edge* removeEdge(edge* listHead){    
            edge* tmp = listHead->next; //tmp is the first edge after the dummy node
            listHead->next = tmp->next; //set the listhead edge (after the dummy) to the second edge
            tmp->next = nullptr; //set the next edge after the dummy node to null
            return tmp; //return the first edge after the dummy node
        }

        void merge2Sets(int Ni, int Nj){
            if(whichSet[Ni] < whichSet[Nj]){
                updateWhichSet(whichSet[Nj], whichSet[Ni]);
            }
                else{
                    updateWhichSet(whichSet[Ni], whichSet[Nj]);
                }
        }

        void updateWhichSet(int a, int b){
            for(int i=1; i<=N; i++){
                if(whichSet[i] == a){
                    whichSet[i] = b;
                }
            }
        }

        void printAry(ofstream& logFile){
            for(int i=0; i<N; i++){
                logFile << " Index= " << i << "\n";
                logFile << " whichSet[i]= " << whichSet[i] << "\n";
            }
            
        }

        void printMST(edge* list, ofstream& outFile){
            edge* current = list->next;
            int cost= 0;
            outFile << "*** A Kruskal's MST of the input graph is given below ***\n";
            outFile << N << "\n";
            while(current != nullptr){
                outFile << current->nU << " " << current->nW << " " << current->cost << "\n";
                cost+= current->cost;
                current = current->next;  
            }
                outFile << "*** The total cost of a Kruskal's MST is: " << cost << " ***\n";

        }
        
        void printList(edge* list, ofstream& outfile){
            edge* current = list->next; //skip the dummy node
            while(current != nullptr ){
                outfile << "<" << current->nU << "," << current->nW << "," << current->cost << ">"; 
                if(current->next != nullptr){
                    outfile << " -> ";
                }
                current = current->next; //move to the next edge
            }
                //outfile << "<" << list->nU << "," << list->nW << "," << list->cost << "> "; //if second edge is null, remove the arrow
        }

        
};

int main(int argc, char** argv){

    if(argc!=4){
        cout << "Your command line must include 3 parameters: Input file, Output file, and a Log file!";
        exit(1);
    }

    string inputFile = argv[1];
    string outFile = argv[2];
    string logFile = argv[3];

    ofstream log(logFile); //open logFile
    if (!log.is_open()) {
        cout << "Error opening log file!" << endl;
        exit(1);
    }

    ofstream output(outFile); 
    if (!output.is_open()) {
        cout << "Error opening output file!" << endl;
        exit(1);
    }

    ifstream inFile(inputFile);
    if(!inFile.is_open()){
        cout << "Error opening input file!" << endl;
        exit(1);
    }

    int N;
    inFile >> N; //read N from input file
    int numSets = N;
    edge* edgeList = new edge(0,0,0); //edge is declared as needing (int nU,int nW, int cost)
    edge* msList = new edge(0,0,0);
    KruskalMST kruskal(N, edgeList, msList); //make constructor public to use in main
    //edge edge()
    int totalMSTCost = 0;
    log << "*** In main(), Printing the input graph *** \n";
    int nU, nW, cost;
    edge* newEdge;
    //edgeList->printEdge(newEdge,log);
    while(inFile >> nU >> nW >> cost){
        newEdge = new edge(nU, nW, cost); //dynamic allocation, allow pointers
        log << " \nIn main(), newEdge from inFile is: ";
        edgeList->printEdge(newEdge,log); 
        kruskal.insertEdge(edgeList,newEdge);           
        log << "\nIn main(), Printing edgeList after inserting the new edge: ";
        kruskal.printList(edgeList, log);
    }

    log << "\n*** In main(), at the end of printing all edges of the input graph ***\n";
    edge* nextEdge = edgeList->next;
    while(numSets > 1){
        nextEdge = kruskal.removeEdge(edgeList);
        while(kruskal.whichSet[nextEdge->nU] == kruskal.whichSet[nextEdge->nW]){ //set Kruskal class to public to use variables/arrays in main
            nextEdge = kruskal.removeEdge(edgeList);
        } 
        
        log << " In main(), the nextEdge is: ";
        edgeList->printEdge(nextEdge, log);
        kruskal.insertEdge(msList,nextEdge);
        totalMSTCost += nextEdge->cost;
        kruskal.merge2Sets(nextEdge->nU, nextEdge->nW);
        numSets--;
        log << "\nIn main(), numSets is: " << numSets << "\n";
        log << " Printing whichSet array: \n";
        kruskal.printAry(log);
        log << "In main(), printing the remaining of edgeList: \n";
        kruskal.printList(edgeList, log);
        log << " In main(), printing the growing MST list: \n";
        kruskal.printList(msList, log);
        
    }

    kruskal.printMST(msList, output);
    inFile.close();
    output.close();
    log.close();

}
