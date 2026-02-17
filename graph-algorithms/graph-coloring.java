import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class node{
    int id;
    node next;

        node(int id, node next){ //id must be > 0
            this.id = id;
            this.next = next;
        }
}

class coloring{
    int numNodes;
    int numUncolor;
    node[] hashTable;
    int[] colorAry;

        coloring(String inFile, int numNodes, int numUncolor){
            this.numNodes = numNodes;
            this.numUncolor = numUncolor;
            hashTable = new node[numNodes+1];
            colorAry = new int[numNodes+1]; 

            for(int i =1; i<=numNodes; i++){
                hashTable[i] = null;
            }
        
                for(int i=1; i<=numNodes; i++){
                colorAry[i] = 0;
                }
        } 

        void loadGraph(String inFile) throws IOException{
            Scanner scanner = new Scanner(new File(inFile));
            scanner.nextInt();
            int i, j;
            
            while(scanner.hasNextInt()){
                i  = scanner.nextInt();
                j = scanner.nextInt();
                hashInsert(i,j);
                hashInsert(j,i);
            }
                scanner.close();
        }

            void hashInsert(int id1, int id2){
                node node = new node(id2, hashTable[id1]); //new nodes next points to after hashTable[id1]
                hashTable[id1] = node;
            }

                void printHashTable(BufferedWriter writer) throws IOException{
                    for(int index=1; index <= numNodes; index++){
                        writer.write("hashTable[" + index + "] ->");
                        node current = hashTable[index]; //use a new (temp) variable to not affect hashTable[] directly
                        while(current!=null){
                            if(current.next == null){
                                writer.write(String.valueOf(current.id));
                            }
                                else{
                                    writer.write(current.id + "->");
                                }
                                    current = current.next;
                        }
                        writer.write("\n"); 
                    }                      
                }

                void printAry(BufferedWriter writer) throws IOException{
                    writer.write("ID" + "\t" + "Color\n");
                    
                    for(int i = 1; i<=numNodes; i++){
                        char color = (char) colorAry[i]; //convert a int to an Ascii char
                        writer.write(i + "\t" + color + "\n");
                    }
                }
                
                
                void graphColoring(BufferedWriter writer, BufferedWriter logWriter) throws IOException{
                    logWriter.write("*** Entering graphColoring()");
                    int newColor = 64;
                    while(numUncolor > 0){
                        newColor++;
                        int nodeID = 1;
                        logWriter.write("*** Before checking nodeID= " + nodeID + ", colorAry[nodeID]= " + colorAry[nodeID] + "\n");
                        while(nodeID <= numNodes){
                            if(colorAry[nodeID] ==0){
                                if(checkNeighbors(colorAry,nodeID, newColor, logWriter) == true){
                                    colorAry[nodeID] = newColor;
                                    numUncolor--;
                                }
                            }
                        
                           logWriter.write("*** after checking nodeID= " + nodeID + ", colorARY[nodeID]= " + colorAry[nodeID] + "\n");
                            nodeID++;
                        }
    
                        logWriter.write("*** below is the result using newColor= " + newColor + "\n");
                        printAry(logWriter);
                        
                    }

                        writer.write("*** Below is the final result of colorARY[]= *** \n");
                        printAry(writer);
                        logWriter.write("Leaving graphColoring() \n");
                        writer.close();
                }
                
                
                boolean checkNeighbors(int[] colorARY, int nodeID, int color, BufferedWriter logWriter) throws IOException{
                    logWriter.write("Entering checkNeighbors() \n");
                    node nextNode = hashTable[nodeID];
                    while(nextNode!=null){
                        if(colorARY[nextNode.id] == color){
                            logWriter.write("Step 3 in checkNeighbors() returning false");
                            return false;
                        }
                            nextNode = nextNode.next;
                    }
                    logWriter.write("Step 6 in checkNeighbors() returning true, also leaving.");
                    return true;
                }
}

class SanchezPersio_Project8_Main{
    public static void main(String[] args) throws IOException{
        if(args.length!=3){
            System.err.println("Usage: java SanchezPersio_Project68Main <inputFile> <outFile> <logFile>");
            System.exit(1); // Exit with an error status
        }
    
            String inputFile = args[0];
            String outFile = args[1];
            String logFile = args[2];
    
            Scanner scanner = null; //check that the input file can be opened and read
            try{ 
                scanner = new Scanner(new File(inputFile));
            }
                catch (IOException e){
                    System.err.println("Cannot open input file: " + inputFile);
                    System.exit(1);
                }
            
            BufferedWriter logWriter = null;
            try{
                logWriter = new BufferedWriter(new FileWriter(logFile));
            }
                catch(IOException e){
                    System.err.println("Cannot open logFile: " + logFile);
                    System.exit(1);
                }
            
                try{
                    new BufferedWriter(new FileWriter(outFile)).close();
                }
                    catch(IOException e){
                        System.err.println("Cannot open SSSfile: " + outFile);
                        System.exit(1);
                    }
    
                    BufferedWriter writer = new BufferedWriter(new FileWriter(outFile, true));
                    int numNodes = scanner.nextInt();
                    int numUncolor = numNodes;

                    coloring coloring = new coloring(inputFile, numNodes, numUncolor);
                    coloring.loadGraph(inputFile);
                    writer.write("*** Below is the hashTable ***\n");
                    coloring.printHashTable(writer);
                    coloring.graphColoring(writer, logWriter);

                    logWriter.close();
                    writer.close();
                    scanner.close();
    }
}