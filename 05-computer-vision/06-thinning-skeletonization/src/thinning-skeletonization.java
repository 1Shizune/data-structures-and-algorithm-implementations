import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.util.Scanner;

class Thinning{
    int numRows, numCols, minVal, maxVal, changeCount, cycleCount;
    int[][] aryOne;
    int[][] aryTwo;

    Thinning(int numRows, int numCols, int minVal, int maxVal, int changeCount, int cycleCount){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.changeCount = changeCount;
        this.cycleCount = cycleCount;
        aryOne = new int[numRows+2][numCols+2];
        aryTwo = new int[numRows+2][numCols+2];

        for(int i=0; i<=numRows; i++){
            for(int j=0; j<=numCols; j++){
                aryOne[i][j] = 0;
                aryTwo[i][j] = 0;
            }
        }

    }

    void loadImage(int[][] aryOne, String inFile) throws IOException{
        Scanner scanner = new Scanner(new File(inFile));
        this.numRows = scanner.nextInt();
        this.numCols = scanner.nextInt();
        this.minVal = scanner.nextInt();
        this.maxVal = scanner.nextInt();

        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                aryOne[i][j] = scanner.nextInt();
            }
        }
        scanner.close();
    }

    int countNonZeroNeighbors(int[][] ary, int i, int j){
        int count = 0;
        for(int k = -1; k<=1; k++){
            for(int l = -1; l<=1; l++){
                if(k == 0 && l==0){
                    continue;
                }              
                if(ary[i+k][j+l] != 0){
                    count++;
                }
            }
        }
        return count;
    }

    void copyArys(int[][] aryOne, int[][] aryTwo){
        for(int i =1; i<=numRows; i++){
            for(int j =1; j<=numCols; j++){
                aryOne[i][j] = aryTwo[i][j];
            }
        }
    }

    void thinning(int[][] aryOne, int[][] aryTwo, BufferedWriter writer) throws IOException{
        writer.write("*** Entering thinning() before thinning 4 sides, aryOne is below ***\n");
        prettyPrint(aryOne, writer);
        changeCount = 0;

        NorthThinning(aryOne, aryTwo, writer);
        writer.write("*** In thinning: after NorthThinning; below is aryTwo, changeCount = " + changeCount + "; cycleCount= " + cycleCount + "***\n");
        prettyPrint(aryTwo, writer);
        copyArys(aryOne, aryTwo);

        SouthThinning(aryOne, aryTwo, writer);
        writer.write("*** In thinning: after SouthThinning; below is aryTwo, changeCount = " + changeCount + "; cycleCount= " + cycleCount + "***\n");
        prettyPrint(aryTwo, writer);
        copyArys(aryOne, aryTwo);

        WestThinning(aryOne, aryTwo, writer);
        writer.write("*** In thinning: after WestThinning; below is aryTwo, changeCount = " + changeCount + "; cycleCount= " + cycleCount + "***\n");
        prettyPrint(aryTwo, writer);
        copyArys(aryOne, aryTwo);


        EastThinning(aryOne, aryTwo, writer);
        writer.write("*** In thinning: after EastThinning; below is aryTwo, changeCount = " + changeCount + "; cycleCount= " + cycleCount + "***\n");
        prettyPrint(aryTwo, writer);
        copyArys(aryOne, aryTwo);

        writer.write("*** Leaving thinning() cycleCount = " + cycleCount + " changeCount = " + changeCount + " ***\n");
    }

    boolean checkConnector(int[][] Ary, int i, int j){
        int a,b,c,d,e,f,g,h;
        a= Ary[i-1][j-1];
        b = Ary[i-1][j];
        c = Ary[i-1][j+1];
        d = Ary[i][j-1];
        e = Ary[i][j+1];
        f = Ary[i+1][j-1];
        g = Ary[i+1][j];
        h = Ary[i+1][j+1];
        if(b == 0 && g == 0){
            return true;
        }
        else if(d == 0 && e == 0){
            return true;
        }
        else if(a > 0 && b == 0 && d == 0){
            return true;
        }
        else if(b == 0 && c > 0 && e == 0){
            return true;
        }
        else if(d == 0 && f > 0 && g == 0){
            return true;
        }
        else if(e == 0 && g == 0 && h > 0){
            return true;
        }
            else{
                return false;
            }
    }
    
    void NorthThinning(int[][] aryOne, int[][] aryTwo, BufferedWriter writer) throws IOException{
        writer.write("*** Entering NorthThinning(); cycleCount = " + cycleCount + "; changeCount = " + changeCount + ". ***\n");
        int nonZeroCount;
        boolean isConnector;
        for(int i = 1; i<=numRows; i++){
            for(int j = 1; j<=numCols; j++){
                
                aryTwo[i][j] = aryOne[i][j];
                if(aryOne[i][j] > 0 && aryOne[i-1][j] == 0){
                    nonZeroCount = countNonZeroNeighbors(aryOne, i, j);
                    isConnector = checkConnector(aryOne, i, j);
                    if(nonZeroCount >= 4 && isConnector == false){
                        aryTwo[i][j] = 0;
                        changeCount++;
                    }
                }
            }
        }
    }

    void SouthThinning(int[][] aryOne, int[][] aryTwo, BufferedWriter writer) throws IOException{
        writer.write("*** Entering SouthThinning(); cycleCount = " + cycleCount + "; changeCount = " + changeCount + ". ***\n");
        int nonZeroCount;
        boolean isConnector;
        for(int i = 1; i<=numRows; i++){
            for(int j = 1; j<=numCols; j++){
                
                aryTwo[i][j] = aryOne[i][j];
                if(aryOne[i][j] > 0 && aryOne[i+1][j] == 0){
                    nonZeroCount = countNonZeroNeighbors(aryOne, i, j);
                    isConnector = checkConnector(aryTwo, i, j);
                    if(nonZeroCount >= 4 && isConnector == false){
                        aryTwo[i][j] = 0;
                        changeCount++;
                    }
                }
            }
        }
    }

    void EastThinning(int[][] aryOne, int[][] aryTwo, BufferedWriter writer) throws IOException{
        writer.write("*** Entering EastThinning(); cycleCount = " + cycleCount + "; changeCount = " + changeCount + ". ***\n");
        int nonZeroCount;
        boolean isConnector;
        for(int i = 1; i<=numRows; i++){
            for(int j = 1; j<=numCols; j++){
                
                aryTwo[i][j] = aryOne[i][j];
                if(aryOne[i][j] > 0 && aryOne[i][j+1] == 0){
                    nonZeroCount = countNonZeroNeighbors(aryOne, i, j);
                    isConnector = checkConnector(aryTwo, i, j);
                    if(nonZeroCount >= 4 && isConnector == false){
                        aryTwo[i][j] = 0;
                        changeCount++;
                    }
                }
            }
        }
    }

    void WestThinning(int[][] aryOne, int[][] aryTwo, BufferedWriter writer) throws IOException{
        writer.write("*** Entering WestThinning(); cycleCount = " + cycleCount + "; changeCount = " + changeCount + ". ***\n");
        int nonZeroCount;
        boolean isConnector;
        for(int i = 1; i<=numRows; i++){
            for(int j = 1; j<=numCols; j++){
                
                aryTwo[i][j] = aryOne[i][j];
                if(aryOne[i][j] > 0 && aryOne[i][j-1] == 0){
                    nonZeroCount = countNonZeroNeighbors(aryOne, i, j);
                    isConnector = checkConnector(aryTwo, i, j);
                    if(nonZeroCount >= 4 && isConnector == false){
                        aryTwo[i][j] = 0;
                        changeCount++;
                    }
                }
            }
        }
    }

    void prettyPrint(int[][] ary, BufferedWriter writer) throws IOException{
        writer.write("\n" + numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        for(int i =0; i<numRows + 2; i++){
            for(int j=0; j<numCols + 2; j++){
                if(ary[i][j] > 0){
                        writer.write("1 ");
                }
                    else{
                        writer.write(". ");
                    }
            }
                    writer.newLine();
        }
                
    }


}

class SanchezP_Project6_Main {
    public static void main(String[] args) throws IOException{
        if(args.length != 3){
            System.err.println("Your command line must include 3 parameters: inFile, outFile, and a logFile");
            System.exit(1); 
        }

        String inFile = args[0];
        String outFile = args[1];
        String logFile = args[2];

        Scanner scanner = new Scanner(new File(inFile));
        BufferedWriter writer = new BufferedWriter(new FileWriter(outFile));
        BufferedWriter writer2 = new BufferedWriter(new FileWriter(logFile));
    
        int numRows, numCols, minVal, maxVal;
        numRows = scanner.nextInt();
        numCols = scanner.nextInt();
        minVal = scanner.nextInt();
        maxVal = scanner.nextInt();
        //int changeCount = 1;
        int cycleCount = 0;
        Thinning thinning = new Thinning(numRows, numCols, minVal, maxVal, 0, 0);
        
        thinning.changeCount = 1;
        writer.write(numRows + " " + numCols + " " + minVal + " " + maxVal + " \n");
        thinning.loadImage(thinning.aryOne, inFile);
        writer.write("*** In main(), before thinning, changeCount = " + thinning.changeCount + ", cycleCount = " + thinning.changeCount + " ***\n");
        thinning.prettyPrint(thinning.aryOne, writer);
        while(thinning.changeCount > 0){
            thinning.changeCount = 0;
            thinning.thinning(thinning.aryOne, thinning.aryTwo, writer2);
            cycleCount++;
            writer.write("*** In main(), inside iteration; changeCount = " + thinning.changeCount + ", cycleCount = " + cycleCount + " ***\n");
            thinning.prettyPrint(thinning.aryOne, writer);
            writer.flush();
            writer2.flush();
        }


        writer.write("*** In main(), the final skeleton, changeCount = " + thinning.changeCount + ", cycleCount = " + cycleCount + " ***\n");
        thinning.prettyPrint(thinning.aryOne, writer);

        scanner.close();
        writer.close();
        writer2.close();
    }
}
