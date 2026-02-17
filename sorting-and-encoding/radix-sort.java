import java.io.*;
import java.util.*;

import javax.swing.text.DefaultEditorKit.CutAction;

class listNode{
    int data;
    listNode next;

    listNode(int data, listNode next){
        this.data=data;
        this.next=next;
    }

        void printNode(listNode node, BufferedWriter writer) throws IOException{
            if(node.next != null){
                writer.write("(" + node.data + ", " + node.next.data + ") ->");
            }
                else{
                    writer.write("(" + node.data + ", NULL)" );
                }
        }

}

    class LLQueue{
        listNode head;
        listNode tail;

        LLQueue(listNode head, listNode tail){
            //listNode dummyNode = new listNode(-999, null);
            this.head = new listNode(-999, null);
            this.tail = this.head;
        }
        
            
    
        listNode deleteQ(LLQueue Q, BufferedWriter logWriter) throws IOException {
            logWriter.write("Entering deleteQ()\n");
        
           
            if (Q.head == Q.tail) { // This means the queue is empty
                logWriter.write("Q is empty\n");
                return null;
            }
        
          
            listNode temp = Q.head.next;
        
           
            if (Q.tail == temp) { // Only one real node in the queue
                Q.tail = Q.head;
            }
        
           
            Q.head.next = temp.next;
        
            
            temp.next = null;
        
          
            logWriter.write("*** Leaving deleteQ(); temp.data = " + temp.data + " ***\n");
        
        
            return temp;
        }
        
    
               
    }

   class RadixSort{
        int tableSize =10;
        LLQueue[][] hashTable;
        int currentTable;
        int previousTable;
        int maxLength;
        int offSet;
        int digitPosition;
        int currentDigit;

            RadixSort(){
                hashTable = new LLQueue[2][tableSize];
                for(int i=0; i<2; i++){
                    for(int j=0; j<tableSize; j++){
                        hashTable[i][j] = new LLQueue(null,null);
                    }
                }
                currentTable =0; //all of these values start at baseline 0, except the previousTable which is the opposite of currentTable;
                previousTable=1;
                maxLength= 0;
                offSet=0;
                digitPosition=0;
                currentDigit=0;
            
            }
    
            void insertQ(LLQueue queue, listNode node) {
                node.next = null;
                if (queue.head.next == null) {
                    queue.head.next = node;
                    queue.tail = node;
                } else {
                    queue.tail.next = node;
                    queue.tail = node;
                }
            }
            
            void printQueue(LLQueue[] whichTable, int index , BufferedWriter writer) throws IOException{
                //BufferedWriter writer = new BufferedWriter(new FileWriter(outFile));
                if(index < 0 || index >= whichTable.length || whichTable[index] == null){
                    writer.write("At index: " + index + ": NULL \n");
                }
                writer.write("At index: " + index + ": \n");
                listNode current = whichTable[index].head;
                while(current!= null){
                    if(current.next != null){
                        writer.write("(" + current.data + ", " + current.next.data + ") ->") ;
                    }
                    
                    else{
                        writer.write("(" + current.data + ", NULL) ->");
                    }
                        current=current.next;
                }
                   writer.write("NULL");
                    //writer.close();
            }
            
                void printTable(LLQueue[] table, String outFile) throws IOException{
                    BufferedWriter writer = new BufferedWriter(new FileWriter(outFile));
                    writer.write("Printing Hash Table: \n");
                    for(int i=0; i<table.length; i++){                               
                            printQueue(table, i, writer);  
                    }
                    
                  // writer.close();
                }
            
                void printSortedData(LLQueue[] whichTable, String outFile) throws IOException {
                    try (BufferedWriter writer = new BufferedWriter(new FileWriter(outFile, true))) {
                        writer.write("\nSorted Data Output:\n");
                        int count = 0;
                
                        for (int i = 0; i < whichTable.length; i++) {
                            listNode current = whichTable[i].head.next; // Skip dummy node
                            
                            while (current != null) {
                                writer.write((current.data - offSet) + " "); // Adjust with offSet
                
                                count++;
                                if (count % 10 == 0) { // 10 numbers per line
                                    writer.write("\n");
                                }
                
                                current = current.next;
                            }
                        }
                
                        if (count % 10 != 0) { // Ensure newline at end if needed
                            writer.write("\n");
                        }
                    }
                }
                
            
                int getLength(int data){
                String dataLength = Integer.toString(data);
                return dataLength.length();
            }
            
            int getIndex(int data, int position) {
                String dataLength = String.valueOf(data);
                int length = dataLength.length();
            
                if (position >= length) return 0; // If position exceeds number length, return 0
            
                int digit = (data / (int) Math.pow(10, position)) % 10; // Extract the correct digit
                return Math.abs(digit) % 10; // Ensure it is within range 0-9
            }
            
            void RSort(String inFile, String outFile, BufferedWriter logWriter) throws IOException {
                logWriter.write("*** Entering RSort() ***\n");
                BufferedWriter writer = new BufferedWriter(new FileWriter(outFile, true));
             
                digitPosition = 0;
                currentTable = 0;
            
               
                File file = new File(inFile);
                Scanner scanner = new Scanner(file);
            
                while (scanner.hasNextInt()) {
                    int data = scanner.nextInt();
                    data += offSet; // Apply offset to handle negatives
                    listNode newNode = new listNode(data, null);
            
                    int hashIndex = getIndex(data, digitPosition);
                    insertQ(hashTable[currentTable][hashIndex], newNode);
                }
                scanner.close();
            
                logWriter.write("*** in RSort: after inserting all data from inFile into hashTable[0] ***\n");
                printTable(hashTable[currentTable], outFile);
            
                
                while (digitPosition < maxLength) {
                    digitPosition++;
                    previousTable = currentTable;
                    currentTable = (currentTable + 1) % 2; // Swap between 0 and 1
            
                 
                    
                    writer.write("In RSort(), digitPosition= " + digitPosition + ", currentTable= " + currentTable + ", previousTable= " + previousTable + "\n");
            
                    
                    writer.write("*** In RSort(), printing previous hashTable ***\n");
                    printTable(hashTable[previousTable], outFile);
            
                   
                    int tableIndex = 0;
            
                   
                    while (tableIndex < tableSize) {
                        logWriter.write("Processing tableIndex=" + tableIndex + " from previousTable=" + previousTable + "\n");
            
                        while (hashTable[previousTable][tableIndex].head.next != null) {
                            logWriter.write("Before deleteQ: tableIndex=" + tableIndex + ", head.next=" + hashTable[previousTable][tableIndex].head.next.data + "\n");
            
                           
                            listNode newNode = hashTable[previousTable][tableIndex].deleteQ(hashTable[previousTable][tableIndex], logWriter);
                            
                            if (newNode != null) {
                                int hashIndex = getIndex(newNode.data, digitPosition);
            
                               
                                insertQ(hashTable[currentTable][hashIndex], newNode);
                                logWriter.write("Moved node " + newNode.data + " to index " + hashIndex + " in currentTable\n");
                            }
                        }
            
                        
                        logWriter.write("In RSort(), finished moving one queue from previousTable to currentTable; tableIndex= " + tableIndex + "\n");
                        tableIndex++;
                    }
                }
            
               
                writer.write("Printing the sorted data\n");
                printSortedData(hashTable[currentTable], outFile);
            
                
                logWriter.write("*** Leaving RSort() ***\n");
            
                //writer.close();
            }
            
            
            void preProcessing(String inFile, BufferedWriter writer) throws IOException {
                writer.write("Entering preProcessing()\n");
                
                int negativeNum = 0;
                int maxNum = Integer.MIN_VALUE; // Track the largest number for maxLength
                
                try {
                    File file = new File(inFile);
                    Scanner scanner = new Scanner(file);
                    
                    while (scanner.hasNextInt()) { // Read all numbers from the input file
                        int num = scanner.nextInt();
                        
                        if (num < negativeNum) {
                            negativeNum = num;
                        }
                        
                        if (num > maxNum) { // Track the largest number
                            maxNum = num;
                        }
                    }
                    scanner.close();
                    
                    if (negativeNum < 0) {
                        offSet = Math.abs(negativeNum);
                    } else {
                        offSet = 0;
                    }
                    
                    maxNum += offSet; // Adjust maxNum based on offset
                    maxLength = getLength(maxNum); // Get length of the largest number
                    
                    if (maxLength == 0) {
                        maxLength = 1; // Ensure at least one digit is processed
                    }
                    
                    writer.write("In preProcessing(): negativeNum=" + negativeNum + ", offSet=" + offSet + ", maxNum=" + maxNum + ", maxLength= " + maxLength + "\n");
                    writer.write("Leaving preProcessing()\n");
                    
                } catch (FileNotFoundException e) {
                    System.out.println("File not found!");
                }
            }



}

class SanchezPersio_Project3_Main{
    public static void main(String[] args) {
        if (args.length != 3) {
            System.err.println("Usage: java SanchezPersio_Project3_Main <inputFile> <outputFile1> <logFile>");
            System.exit(1); // Exit with an error status
        }
        
        String inputFile = args[0];
        String outputFile1 = args[1];
        String logFile = args[2];
    
        try (BufferedWriter logWriter = new BufferedWriter(new FileWriter(args[2]))) { //every build method shares a BufferedWriter so that they can all write to logFile w/o overriding eachother
            RadixSort radixSort = new RadixSort();
            radixSort.preProcessing(inputFile,logWriter);
            radixSort.RSort(inputFile, outputFile1, logWriter);
        }
        catch (Exception e) {
            e.printStackTrace();
          }  
    
    }
}
