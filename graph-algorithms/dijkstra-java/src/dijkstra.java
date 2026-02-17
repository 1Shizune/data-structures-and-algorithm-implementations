import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class DijkstraSSS{
    int numNodes;
    int sourceNode;
    int minNode;
    int currentNode;
    int newCost;
    int[][] costMatrix;
    int[] Parent;
    int[] ToDo;
    int[] Best;

        DijkstraSSS(int numNodes, int sourceNode){
            this.numNodes = numNodes;   
            this.sourceNode = sourceNode;
            costMatrix = new int[numNodes+1][numNodes+1];
            Parent = new int[numNodes + 1];
            ToDo = new int[numNodes + 1];
            Best = new int[numNodes + 1];
            for(int l=1; l<=numNodes; l++){
                for(int m=1; m<=numNodes; m++){
                    if(l==m){
                        costMatrix[l][m] =0;
                    }
                        else{
                            costMatrix[l][m] = 999;
                        }                  
                }
            }
            for(int i=1; i<=numNodes; i++){
                ToDo[i] = 1;
            }
                for(int j=1; j<=numNodes; j++){
                    Best[j] = 999;
                }
                    for(int k=1; k<=numNodes; k++){
                        Parent[k] = sourceNode;
                    }
        
        }

            void loadCostMatrix(String inputFile, int[][] costMatrix) throws IOException{
                Scanner scanner = new Scanner(new File(inputFile)); 
                int n1, n2, cost;
                if(scanner.hasNextInt()){
                    scanner.nextInt();
                }
                
                while(scanner.hasNextInt()){
                    n1 = scanner.nextInt();
                    n2 = scanner.nextInt();
                    cost = scanner.nextInt();
                    costMatrix[n1][n2] = cost;
                }
                    scanner.close();
            }
                
                void initBest(int sourceNode){
                    for(int i=0; i<Best.length; i++){
                        Best[i] = costMatrix[sourceNode][i];
                    }
                
                }
        
                    void initParent(int sourceNode){
                        for(int i=0; i<Parent.length; i++){
                            Parent[i] = sourceNode;
                        }
                    }

                        void initToDo(int sourceNode){
                            for(int i=0; i<ToDo.length; i++){
                                if(i == sourceNode){
                                    ToDo[i] = 0;
                                }
                                    else{
                                        ToDo[i] = 1;
                                    }
                            }
                        }
    
                        boolean checkToDo(){
                            for(int i=0; i<ToDo.length; i++){
                                if(ToDo[i] == 1){
                                    return false;
                                }
                            }
                                return true;
                        }
                        
                        int findMinNode(BufferedWriter logwriter) throws IOException{
                            logwriter.write("*** Entering findMinNode() method \n***");
                            int minCost = 999;
                            int minNode = 0;
                            int index = 1;

                            while(index <= numNodes){
                                if(ToDo[index] == 1 && Best[index]< minCost){
                                    minCost = Best[index];
                                    minNode = index;
                                }
                                    index++;
                            }
                        
                            logwriter.write("*** Leaving findMinNode(): minNode is: " + minNode + " ***\n");
                            return minNode;
                        }
    
                            void printParent(BufferedWriter logwriter) throws IOException{
                                logwriter.write("*** Entering printParent() method ***\n");
                                logwriter.write("Below is the Parent array \n");
                                for(int i=0; i<Parent.length; i++){
                                    logwriter.write("Index= " + i + " Parent= " + Parent[i] + "\n");
                                }
                                logwriter.write("*** Exiting printParent() method ***\n");
                            }

                                void printToDo(BufferedWriter logWriter) throws IOException{
                                    logWriter.write("*** Entering printToDo() method \n");
                                    logWriter.write("Below is the ToDo array \n");
                                    for(int i=0; i<ToDo.length; i++){
                                        logWriter.write("Index= " + i + " ToDo= " + ToDo[i] + "\n");
                                    }
                                        logWriter.write("*** Exiting printToDo() method ***\n");
                                }
                                    void printBest(BufferedWriter logWriter) throws IOException{
                                        logWriter.write("*** Entering printBest() method \n");
                                        logWriter.write("Below is the Best array \n");
                                        for(int i=0; i<Best.length; i++){
                                            logWriter.write("Index= " + i + " Best= " + Best[i] + "\n");
                                        }
                                            logWriter.write("*** Exiting printBest() method ***\n");
                                    }
      
                                    void printShortestPath(int currentNode, int sourceNode, BufferedWriter writer) throws IOException {
                                        writer.write("The path from " + sourceNode + " to " + currentNode + " : ");
                                        
                                        if (sourceNode == currentNode) {
                                            writer.write(sourceNode + " 🡢 " + sourceNode + " : cost = 0\n");
                                            return;
                                        }
                                    
                                        StringBuilder path = new StringBuilder();
                                        int node = currentNode;
                                        while (node != sourceNode && node != 0) {
                                            path.insert(0, " 🡢 " + node);
                                            node = Parent[node];
                                        }
                                    
                                        if (node == 0) {
                                            writer.write("No path exists from " + sourceNode + " to " + currentNode + "\n");
                                        } else {
                                            path.insert(0, sourceNode); //insert the source node at the beginning of the shortest path
                                            writer.write(path.toString() + " : cost = " + Best[currentNode] + "\n");
                                        }
                                    }
                                    
                                    
                                        /*void printShortestPath(int currentNode, int sourceNode, BufferedWriter writer) throws IOException{
                                        int father = Parent[currentNode];
                                        writer.write("*** Entering printShortestPath() method \n");
                                        writer.write("The path from " + sourceNode + " to " + currentNode + ": \n");
                                        if(currentNode == sourceNode){ //default case
                                            writer.write(sourceNode + " 🡢 " + sourceNode + " : cost = 0\n");
                                            return;
                                        }
                                        
                                        //StringBuilder path = new StringBuilder();
                                        //writer.write(sourceNode);
                                        while(father != sourceNode){
                                            writer.write(father + " 🡢 ");
                                            father = Parent[father];
                                        }
                                            writer.write(sourceNode + " 🡢 " + currentNode);
                                            writer.write("\n" + father + ": cost=" + Best[currentNode] + "\n");
                                            //writer.close();
                                    }
                                    */
      
      
     }   







class SanchezPersio_Project6_Main {
    public static void main(String[] args) throws IOException{
        if(args.length != 3){
            System.err.println("Usage: java SanchezPersio_Project6_Main <inputFile> <SSSfile> <logFile>");
            System.exit(1); // Exit with an error status
        }
    
            String inputFile = args[0];
            String SSSfile = args[1];
            String logFile = args[2];

            //File file = new File(inputFile);
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
                    new BufferedWriter(new FileWriter(SSSfile)).close();
                }
                    catch(IOException e){
                        System.err.println("Cannot open SSSfile: " + SSSfile);
                        System.exit(1);
                    }
            
                    BufferedWriter SSS = new BufferedWriter(new FileWriter(SSSfile));
            
            int numNodes = scanner.nextInt(); //Allocate and initialize the data structures
            SSS.write("================================================================================================= \n");
            SSS.write("There are " + numNodes + " nodes in the input graph. Below are all pairs of shortest paths: \n");
            
            int sourceNode = 1;
            //int minNode =0;
            //int currentNode =0;
            //int newCost = 0;

            DijkstraSSS dijkstra = new DijkstraSSS(numNodes, sourceNode); //create a DijkstrasSSS instance

            dijkstra.loadCostMatrix(inputFile, dijkstra.costMatrix);
            while(sourceNode <= numNodes){
                logWriter.write("*** The source node= " + sourceNode + " ***\n"); 
                dijkstra.initBest(sourceNode);
                dijkstra.initParent(sourceNode);       
                dijkstra.initToDo(sourceNode);        
            
                while(!dijkstra.checkToDo()){
                    int minNode = dijkstra.findMinNode(logWriter);
                    logWriter.write("*** In main(), minNode is " + minNode + " ***\n");        
                    dijkstra.ToDo[minNode] = 0;        
                    dijkstra.printToDo(logWriter);        
                    dijkstra.printParent(logWriter);        
                    dijkstra.printBest(logWriter);        
                    
                    int childNode =1;        
                    logWriter.write("*** In main(), childNode is: " + childNode + " ***\n");
                    while(childNode <= numNodes){
                        if(dijkstra.ToDo[childNode] == 1){
                            int newCost = dijkstra. Best[minNode] + dijkstra.costMatrix[minNode][childNode];
                            if(newCost < dijkstra.Best[childNode]){
                                logWriter.write(" *** In main(); newCost: " + newCost + " < Best[childNode]: " + dijkstra.Best[childNode] + " ***\n");
                                dijkstra.Best[childNode] = newCost;
                                dijkstra.Parent[childNode] = minNode;
                                dijkstra.printParent(logWriter);
                                dijkstra.printBest(logWriter);
                            }
                        }
                            childNode++;
                    } 
                }
                    
                    SSS.write("================================================================================================= \n");
                    int currentNode = 1;
                    while(currentNode <= numNodes){
                        dijkstra.printShortestPath(currentNode, sourceNode, SSS);
                        currentNode++;
                    }
                        sourceNode++;
            }
                logWriter.close();
                SSS.close();
                scanner.close();
            }

        
}
