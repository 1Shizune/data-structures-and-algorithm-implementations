#include<iostream>
#include<string>
#include<fstream>
using namespace std;

class HtreeNode{
	public:
		string chStr;
		int prob;
		string code;
		HtreeNode* next;
		HtreeNode* left;
		HtreeNode* right;
		
		HtreeNode(string chStr, int prob, string code, HtreeNode* next, HtreeNode* left, HtreeNode* right){
			this->chStr = chStr;
			this->prob = prob;
			this->code = code;
			this->next = next;
			this->left = left;
			this->right = right;
		} 

		void printNode(HtreeNode* T, ofstream& fileOut){
			if(T->next != nullptr){
				fileOut << "( " << T->chStr << ", " << T->prob << ", " << T->code << ") -> ";	
			}
				else{
					fileOut << "( " << T->chStr << ", " << T->prob << ", " << T->code << ")";
				}
		}
};

class HuffmanCoding{
	public:
		HtreeNode* listHead;
		HtreeNode* Root;
			
			HuffmanCoding(HtreeNode* listHead, HtreeNode* Root){
				this->listHead = listHead;
				this->Root = Root;
			}
			
		HtreeNode* findSpot(HtreeNode* head, HtreeNode* newNode){
			HtreeNode* spot = head;
			while(spot->next!=nullptr && spot->next->prob <= newNode->prob){ //compare probabilities until the next spot is null or the nextNode's probability is greater than the newNode
				spot = spot->next;
			}
				return spot;			
		}	
	
			void listInsert(HtreeNode* spot, HtreeNode* newNode){
				newNode->next = spot->next;
				spot->next= newNode;
			}
			
				void printList(HtreeNode* listHead, ofstream& outFile){
						HtreeNode* current = listHead;
						while(current != nullptr){
						current->printNode(current, outFile);
						current = current->next;
					}
						outFile << "\n";
				}
				
				HtreeNode* constructHuffmanLList(string inFile, ofstream& logFile){
					logFile << "Entering constructHuffmanLList() method \n";
					ifstream input(inFile);
					if(!input.is_open()){
					logFile << "*** Cannot open input file! \n";
					return 0;
				}
					listHead = new HtreeNode("dummy", 0,"",nullptr, nullptr, nullptr);
					string chr;
					int prob;
					while(input >> chr >> prob){
						logFile << "In constructHuffmanLList() chr= " << chr << "probability= " << prob << "\n";
						HtreeNode* newNode = new HtreeNode(chr, prob,"", nullptr, nullptr, nullptr); //How to declare a HtreeNode with new values
						
						logFile << "In constructHuffmanLList (), printing newNode \n";
						newNode->printNode(newNode, logFile);
						
						HtreeNode* spot = findSpot(listHead, newNode);
						listInsert(spot, newNode);
						logFile << "In constructHuffmanLList(), Printing list after inserting a newNode \n";
						printList(listHead, logFile);					
					}
						return listHead;
				}

HtreeNode* constructHuffmanBinTree(HtreeNode* listHead, ofstream& logFile, ofstream& outFile) {
    logFile << "Entering constructHuffmanBinTree() method \n";

    while (listHead->next && listHead->next->next) {
        int prob = listHead->next->prob + listHead->next->next->prob;
        string chstr = listHead->next->chStr + listHead->next->next->chStr;                

        HtreeNode* newNode = new HtreeNode(chstr, prob, "", nullptr, listHead->next, listHead->next->next);

        logFile << "In constructHuffmanBinTree, printing newNode\n";
        newNode->printNode(newNode, logFile);
        logFile << "\n";

        HtreeNode* spot = findSpot(listHead, newNode);
        listInsert(spot, newNode);

        listHead->next = listHead->next->next->next;

        if (listHead->chStr == "dummy" && listHead->prob == 0) {
            listHead = listHead->next;
        }

        logFile << "In constructHuffmanBinTree, printing the list after inserting newNode \n";
        printList(listHead, logFile);
    }
 if (listHead->next) {
        int prob = listHead->prob + listHead->next->prob;
        string chstr = listHead->chStr + listHead->next->chStr;                

        HtreeNode* newNode = new HtreeNode(chstr, prob, "", nullptr, listHead, listHead->next);

        logFile << "Final merge: ";
        newNode->printNode(newNode, logFile);
        logFile << "\n";

        listHead = newNode;  // Set listHead to the new root
    }

    // Final Root Debug Print
    logFile << "Final Root Node: ";
    listHead->printNode(listHead, logFile);
    logFile << "\n";

    outFile << "Final Root Node: ";
    listHead->printNode(listHead, outFile);
    outFile << "\n";

    return listHead;  // Return the correct root
}

				bool isLeaf(HtreeNode* node) {
   					return (node->left == nullptr && node->right == nullptr);
				}
			
	void preOrderTraversal(HtreeNode* T, ofstream& outFile1) {
    	if (T == nullptr) {
        	outFile1 << "Error: Tree is empty, cannot perform traversal\n";
        	return;
    	}  
    		T->printNode(T, outFile1);
    		outFile1 << "\n";  // Ensure proper spacing   
    	if (T->left != nullptr) preOrderTraversal(T->left, outFile1);
    	if (T->right != nullptr) preOrderTraversal(T->right, outFile1);
}

			void constructCharCode(HtreeNode* T, string code, ostream& outFile1){
				if(isLeaf(T)){
					T->code = code;
					outFile1 << T->chStr << " " << T->code << "\n";
				}
					else{
						constructCharCode(T->left, (code + "0"), outFile1);
						constructCharCode(T->right, (code + "1"), outFile1);
					}
			}

	void inOrderTraversal(HtreeNode* T, ofstream& outFile) {
    	if (T == nullptr) return;
  
    	inOrderTraversal(T->left, outFile); //go left subtree
  
    	T->printNode(T, outFile);  //go current node
    	outFile << "\n";
 	
    	inOrderTraversal(T->right, outFile); //go right subtree
}

	void postOrderTraversal(HtreeNode* T, ofstream& outFile) {
    	if (T == nullptr) return;

    	postOrderTraversal(T->left, outFile); //left subtree

    	postOrderTraversal(T->right, outFile); //right subtree

    	T->printNode(T, outFile); //current node
    	outFile << "\n";
}

};

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Your command line needs to include 3 parameters: Input file, output file and log file! ";
        exit(1);
    }

    string inputFile = argv[1];
    string outFile = argv[2];
    string logFile = argv[3];

    ofstream log(logFile);
    if (!log.is_open()) {
        cout << "Error opening log file!" << endl;
        exit(1);
    }

    ofstream output(outFile);
    if (!output.is_open()) {
        cout << "Error opening output file!" << endl;
        exit(1);
    }

    // Create a HuffmanCoding object
    HuffmanCoding huffman(nullptr, nullptr);

    HtreeNode* listHead = huffman.constructHuffmanLList(inputFile, log);
    output << "In main(): printing list after list is constructed\n";
    huffman.printList(listHead, output);

    HtreeNode* root = huffman.constructHuffmanBinTree(listHead, log, output);

    output << "In main(): Printing character codes\n";
    huffman.constructCharCode(root, "", output);

    output << "In main(): Printing Pre-order traversal of the tree\n";
    huffman.preOrderTraversal(root, output);
	output << "In main(): Printing in-Order traversal of the tree\n";
    huffman.inOrderTraversal(root, output);
	output << "In main(): Printing post-Order traversal of the tree\n";
    huffman.postOrderTraversal(root, output);	
	
	output << "\n In main(): Root Node: " << root->chStr << ", " << root->prob << "\n";
    log.close();
    output.close();

    return 0;
}

