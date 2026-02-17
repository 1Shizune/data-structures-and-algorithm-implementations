#include<iostream>
#include<string>
#include<fstream>
using namespace std;

class listNode{
public: //private by default and values are inaccessible outside of the listNode class, make it public	
	int data;
	listNode* next; //pointer to the next value (next node)

	listNode(int data, listNode* next){ //class listNode constructor
		this->data = data;
		this->next = next;
	}

		void printNode(listNode* node)
		{
			if(node and node->next){
				cout << "(" << node->data << " ," << node->next->data << ") ->";
			}
			 else{
			 	cout << "(" << node->data << " , NULL)" << endl;
			 }
		}

};

	class HTable{		
		public:
		char op;
		int data;
		int bucketSize;
		listNode** hashTable;
	
			HTable(int data, int bucketSize){
				this->data=data;
				this->bucketSize=bucketSize;
				this->hashTable= new listNode*[bucketSize];
				for(int i=0; i<bucketSize; i++)
				{
					this->	hashTable[i] = new listNode(-9999, nullptr); //each hashTable[i] points to a dummy node, hashTable[i] points to a bucket which contains a linked list, the first element in the linked list of every bucket is a dummy node
				}
			
			}		
	
		int hashFunction(int data) 
		{
			int i=1; //multiply each digit of the data number by an increasing value, from rightmost to leftmost
			int sum=0;
			while(data!=0) 
			{
				sum += ((data%10) * i);
				i++;
				data/= 10;
			}
			
			return (sum % bucketSize);
		}
		
			listNode* findSpot(int index, int data, ofstream& logFile){
				logFile << "*** Entering findSpot method. Index= " << index << " data= " << data << endl; 
				listNode* spot = hashTable[index];
				while(spot->next != nullptr && spot->next->data < data)
				{
					spot = spot->next;
				}
					return spot;
			}
			
			void printList(int index, ofstream& oFile){
				oFile << "hashTable[" << index << "]:";
				listNode* current = hashTable[index]; //start from the dummy node of hashTable[index], printNode only prints one node so iterate throuh entire linked list
				while(current != nullptr)
				{
					oFile << "(" <<current->data;
					if(current->next != nullptr ) //check that the next node isnt the last node, doesnt point to null
					{
						oFile << ", " << current->next->data << ") ->"; 
					 } 
					else{
						oFile << ", " << "NULL)" << endl;
					}
					current = current->next; //move the current pointer to the next node after printing
				}
			
			}
			
			
			void hashInsert(int index, int data, ofstream& logFile){
				logFile << "*** Entering hashInsert method; Index= " << index << " data= " << data << endl;
				listNode* spot = findSpot(index,data,logFile);
				if(spot->next != nullptr && spot->next->data == data){
					logFile << "*** Warning: Data is already in the database! No insertion! \n";
				}
					else{
						logFile << "***Printing list before hashInsert operation*** \n";
						printList(index, logFile);
						listNode* newNode = new listNode(data,spot->next);
						spot->next = newNode;
						logFile << "*** Printing list after hashInsert operation *** \n" << endl;
						printList(index, logFile);
					}
				logFile << "Leaving hashInsert() \n";
			}
			
			void hashDelete(int index, int data, ofstream& logFile){
				logFile << "*** Entering hashDelete method. Index= " << index << " data= " << data << endl;
				listNode* spot = findSpot(index, data, logFile);
				if(spot->next == nullptr or spot->next->data !=data){
					logFile << "*** Warning: Data is not in the hashTable! \n";
				}
				else{
					logFile << "*** Printing list before hashDelete operation *** \n";
					printList(index, logFile);
					listNode* temp = spot->next;
					spot->next = temp->next;
					temp->next = nullptr;
					logFile << "*** Printing list after hashDelete operation *** \n;";
					printList(index, logFile);
				}
				logFile << "Leaving hashDelete() \n";
			}
			
			void hashRetrieval(int index, int data, ofstream& logFile){
				logFile << "*** Entering hashRetrieval method. Index= " << index << " data= " << data << endl; 	
				listNode* spot = findSpot(index, data, logFile);
				if(spot->next == nullptr or spot->next->data != data){
					logFile << "*** Warning: The record is not in the database! \n";
				}
				else{
					logFile << "Yes, the record is in the database! \n";
				}
			logFile << "Leaving hashRetrieval operation... \n";
			}
			
			void printHashTable(ofstream& outFile){
				outFile << "*** Printing hash table *** \n";
				for(int i=0; i<bucketSize; i++)
				{
					printList(i, outFile);
				}
			}
			
		void informationProcessing(string inFile, ofstream& outFile, ofstream& logFile)
			{
				logFile << "*** Entering information processing method. \n";
				ifstream input(inFile);
				if(!input.is_open()){
					logFile << "*** Cannot open input file! \n";
					return;
				}
				bool flag = true;
				char op;
				int data; 
				while(input >> op >> data){			
				int index = hashFunction(data); 
				
				logFile << "In informationProcessing: Op= " << op << " data= " << data << " index= " << index << "\n";
				outFile << "In informationProcessing: Op= " << op << " data= " << data << " index= " << index << "\n";
				
				logFile << "*** Print list before operation *** \n";
				printList(index,logFile);
				
				if(op == '+'){
					logFile << "*** Call hashInsert() *** \n";
					hashInsert(index, data, logFile);
				}
					else if(op == '-'){
						logFile << "*** Call hashDelete() *** \n";
						hashDelete(index, data, logFile);
					}
						else if(op == '?'){
							logFile << "*** Call hashRetrieval() *** \n";
							hashRetrieval(index,data, logFile);
						}
							else{
								logFile << "op is an unrecognized operation! \n";
								outFile << "op is an unrecognized operation: " << op << "\n";
								flag = false;
							}
			
					if(flag){
						logFile	<< "*** Print list after operation ***";
						printList(index, logFile);
						outFile << "*** Printing hash table after a valid operation \n";
						printHashTable(outFile);
					}
					flag=true;
				}
		logFile << "*** Leaving informationProcessing method. \n";
			
			}

			


};



int main(int argc, char** argv){
	if(argc!=5){
		cout << "Your command line needs to include 4 parameters: Input file, bucket size, output file and log file! ";
		exit(1);
	}
string inputFile = argv[1];
int bucketSize = stoi(argv[2]);
string outputFile = argv[3];
string logFile = argv[4];

ofstream outFile(outputFile);
ofstream logFileStream(logFile);
if(!outFile.is_open()){
	cout << "Unable to open output file";
	return 1;
}
if(!logFileStream.is_open()){
	cout << "Unable to open log file";
	return 1;
}

HTable hashTable(-1, bucketSize);
hashTable.informationProcessing(inputFile, outFile, logFileStream);

outFile.close();
logFileStream.close();

}




