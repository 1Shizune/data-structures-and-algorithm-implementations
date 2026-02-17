import java.io.BufferedReader;
import java.io.FileReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;


class listNode { //define what a listNode is in a linked list (a value and a pointer to the next node)
    int data;
    listNode next;   
    
    listNode(int data, listNode next){ //listNode constructor
        this.data = data;
        this.next = next;
    }
    
}    
 
    class LLStack{
        public Boolean isEmpty(){
            return top.next == null;
        }
        listNode top = new listNode(-999,null);
        private BufferedWriter writer; // BufferedWriter as an instance variable
    
        public void setWriter(BufferedWriter writer) { 
        this.writer = writer; 
    }


        public void push(int num) {
            listNode newNode = new listNode(num, top.next); // Attach the new node to the top ***edited
            top.next = newNode; // Point the space next to the top node to the address of the new node being added, LIFO principle
        }
        
            
            public int pop() throws IOException{ 
                //writer.write("Stack is empty!");
                if(top.next == null){              
                //isEmpty() = true;
                writer.write("The Stack is Empty!");       
                writer.flush();
                return -999;    
                }
                int data = top.next.data; 
                top.next = top.next.next;
                //writer.close();
                return data;
        }    
        
        void printNode(BufferedWriter writer) throws IOException{
            listNode current = top.next; //top is a the head dummy node so make the next node be the current node         
            while(current!= null)
            {
                if(current.next == null){
                    writer.write(" (" + current.data + "," + "null" + ") \n");
                }
                    else{
                        writer.write(" (" + current.data + "," + current.next.data + ") \n");
                    }               
                current = current.next;
            }
            writer.newLine();
        }    
          
                    public static void buildStack(LLStack S, String inFile, String outFile1, BufferedWriter logWriter){ //every build method shares a BufferedWriter so that they can all write to logFile w/o overriding eachother
                        try (BufferedReader reader = new BufferedReader(new FileReader(inFile));
		                    BufferedWriter writer = new BufferedWriter(new FileWriter(outFile1));
                            /*BufferedWriter writer1 = new BufferedWriter(new FileWriter(logFile))*/){
                            
                            S.setWriter(writer);
                            logWriter.write("Entering buildStack()!\n");
                            String line;
                            while((line = reader.readLine()) != null){

                            String[] numbers = line.split("\\s+"); //split by spaces 
                            for(String num: numbers){
                                int value = Integer.parseInt(num);
                                S.push(value);
                                logWriter.write("Pushed " + value + " onto the stack\n");
                            }                         
                        }                                                                                                                 
                            S.printNode(writer); //write stack contents to outFile                                                         
                           logWriter.write("Leaving buildStack !\n");
                        
                            writer.flush();
                            logWriter.flush();                        
                        }   

                        catch(IOException e){
                            e.printStackTrace();
                        }
                        
                    } 

    }   
    



class LLQueue{
    listNode head = new listNode(-999,null);
    listNode tail = head; //tail is initially also the head since there is only one node
    boolean isEmpty = false;
   public void insertQ(int data, BufferedWriter writer) throws IOException
   {    
     listNode newNode = new listNode(data, null);
     tail.next = newNode;
     tail = newNode;
    
    writer.write("After pop stack newNodes data is " + data + "\n");   
    writer.flush();
    //System.out.println("Inserted into queue: " + data); // Debugging
    }
    
        public listNode deleteQ()
        {
             //delete the first node and continue down the queue (return head.next of the queue)
            if(head.next == null){ 
                //System.out.println("Queue is empty, returning null.");
                isEmpty = true; 
                return null;
            }
                listNode removeNode = head.next;
                head.next = head.next.next;
            
            if(head.next == null) //if after deletion the queue is empty, reset the tail
            {
                tail=head;
            }
            return removeNode;
        }
                        
            public static void buildQueue(LLStack S, LLQueue Q, String outFile2, BufferedWriter logWriter){ //every build method shares a BufferedWriter so that they can all write to logFile w/o overriding eachother
                try (BufferedWriter writer = new BufferedWriter(new FileWriter(outFile2));
		            /*BufferedWriter writer1 = new BufferedWriter(new FileWriter(logFile))*/){

                        logWriter.write("Entering buildQueue! \n");
                        while(!S.isEmpty())
                        {
                            int value = S.pop();
                            if(value!= -999)
                            {
                                Q.insertQ(value,writer);
                                logWriter.write("After pop stack, newNodes data is " + value + "\n");
                            }
                            
                            
                           
                        }
                        logWriter.write("Leaving buildQueue! \n");
                        writer.flush();
                        logWriter.flush();
                    }
                    catch(IOException e){
                        e.printStackTrace();
                    }
            }

}

class LLList
{
    listNode head;
    public LLList()
    {
        head = new listNode(-9999, null);
    }

        public listNode findSpot(int data, String logFile)
        {
            listNode Spot = head; //Start at the first node and traverse the list to find the sorted position of the new node
            while(Spot.next != null && Spot.next.data < data)
            {
                Spot = Spot.next; //if the next node has smaller data, move forward
            }
                return Spot; //This is the node which the new node should be inserted before
        }
            public void insertOneNode(int data)
            {
                listNode newNode = new listNode(data, null);
                listNode Spot = findSpot(data, null); //use the findSpot method to find the correct spot for the new node
                newNode.next = Spot.next; // new node goes into the next node
                Spot.next = newNode; // spot is connected to newNode
            
            }
                
                public void printList(BufferedWriter writer) throws IOException
                {
                        listNode current = head; //top is a the head dummy node, needs to be printed here
                        while(current!= null)
                    {
                        if(current.next == null)
                        {
                            writer.write(" (" + current.data + "," + "null" + ") \n");
                        }
                            else
                            {
                                writer.write(" (" + current.data + "," + current.next.data + ") \n");
                            }               
                        current = current.next;
                    }
                        writer.write("NULL");
                        writer.newLine();
                }

                
                public static void buildList(LLQueue Q, LLList L, String outFile3, BufferedWriter logWriter) //every build method shares a BufferedWriter so that they can all write to logFile w/o overriding eachother
                {
                    try (BufferedWriter writer = new BufferedWriter(new FileWriter(outFile3));
		            /*BufferedWriter writer1 = new BufferedWriter(new FileWriter(logFile))*/){
                    
                        logWriter.write("Entering buildList()\n" );
                        listNode newNode;
                        while((newNode = Q.deleteQ()) != null)
                        {

                            writer.write("delete a node from Q, newNode's data is " + newNode.data +"\n");
                            logWriter.write("delete a node from Q, newNode's data is " + newNode.data + "\n");
                            //listNode spot = L.findSpot(newNode.data, logFile);
                            L.insertOneNode(newNode.data);
                        }
                            logWriter.write("Leaving buildList()\n");
                            L.printList(writer);
                            L.printList(logWriter);
                    }
                
                     catch (IOException e) {
                        e.printStackTrace();
                    }         
                } 
            }          

class SanchezP_Project1_Main{
    
    public static void main(String[] args) throws IOException {     
        if (args.length != 5) {
            System.err.println("Usage: java SanchezP_Project1_Main <inputFile> <outputFile1> <outputFile2> <outputFile3> <logFile>");
            System.exit(1); // Exit with an error status
        }
        
        String inputFile = args[0];
        String outputFile1 = args[1];
        String outputFile2 = args[2];
        String outputFile3 = args[3];
        String logFile = args[4];

        LLStack stack = new LLStack();
        LLQueue queue = new LLQueue();
        LLList list = new LLList();
        try (BufferedWriter logWriter = new BufferedWriter(new FileWriter(args[4]))) { //every build method shares a BufferedWriter so that they can all write to logFile w/o overriding eachother
            LLStack.buildStack(stack, inputFile, outputFile1, logWriter);
            LLQueue.buildQueue(stack, queue, outputFile2, logWriter);
            LLList.buildList(queue, list, outputFile3, logWriter);
        }
    }
}
