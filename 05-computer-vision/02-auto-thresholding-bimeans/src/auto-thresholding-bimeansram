import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.File;
import java.util.Scanner;

class BiMeans{
    int numRows, numCols, minVal, maxVal;
    int biGaussThrVal;
    int histHeight;
    int maxHeight;
    int[] histAry;
    int[] GaussAry;
    int[] bestFitGaussAry;
    char[][] GaussGraph;
    char[][] gapGraph;
    char[][] progressGraph;

        BiMeans(int numRows, int numCols, int minVal, int maxVal, int biGaussThrVal, int histHeight, int maxHeight){
            this.numRows = numRows;
            this.numCols = numCols;
            this.minVal = minVal;
            this.maxVal = maxVal;
            this.biGaussThrVal = biGaussThrVal;
            this.histHeight = histHeight;
            this.maxHeight = 0;
            
            histAry = new int[maxVal+1];
            for(int i =0; i<=maxVal; i++){
                histAry[i] = 0;
            }

            GaussAry = new int[maxVal+1];
            for(int i =0; i<=maxVal; i++){
                GaussAry[i] = 0;
            }           

            bestFitGaussAry = new int[maxVal+1];
            for(int i =0; i<=maxVal; i++){
                bestFitGaussAry[i] = 0;
            }               

            GaussGraph = new char[maxVal+1][histHeight+1];
            for(int i=0; i<=maxVal; i++){
                for(int j = 0; j<=histHeight; j++){
                    GaussGraph[i][j] = ' ';
                }
            }
            
            gapGraph = new char[maxVal+1][histHeight+1];
            for(int i=0; i<=maxVal; i++){
                for(int j = 0; j<=histHeight; j++){
                    gapGraph[i][j] = ' ';
                }
            }
            
            progressGraph = new char[maxVal+1][histHeight+1];
            for(int i=0; i<=maxVal; i++){
                for(int j = 0; j<=histHeight; j++){
                    progressGraph[i][j] = ' ';
                }
            }

        }

        int loadHist(String inputFile) throws IOException{
            Scanner scanner = new Scanner(new File(inputFile));
            this.numRows = scanner.nextInt();
            this.numCols = scanner.nextInt();
            this.minVal = scanner.nextInt();
            this.maxVal = scanner.nextInt();
 
            int max = 0;
            for(int i =0; i<= this.maxVal; i++){
                this.histAry[i] = 0;
            }

            while(scanner.hasNextInt()){
                int index = scanner.nextInt();
                int value = scanner.nextInt();
                this.histAry[index] = value;
                
                if(value > max){
                    max = value;
                }
            }

            scanner.close();
            this.maxHeight = max;
            return max;
        }

        void printHist(BufferedWriter writer, int[] histAry) throws IOException{
            writer.write(numRows + " " + " " + numCols + " " + minVal + " " + maxVal + "\n");
            for(int i = 0; i<=maxVal; i++){
                writer.write(i + " " + histAry[i] + "\n");
            }
        
            writer.write("\n");
        }

        void printBestFitGauss(int[] GaussAry, BufferedWriter writer) throws IOException{
            writer.write(numRows + " " + " " + numCols + " " + minVal + " " + maxVal + "\n");
            for(int i = 0; i<=maxVal; i++){
                writer.write(i + " " + GaussAry[i] + "\n");
            }            
            writer.write("\n");
        }
        
        void dispHist(BufferedWriter writer, int[] histAry) throws IOException{
            int temp;
            writer.write("\n" + numRows + " " + " " + numCols + " " + minVal + " " + maxVal + "\n");
            for(int i =0; i<=maxVal; i++){
                temp = histAry[i];
                writer.write(i + " " + "(" + histAry[i] + "): ");
                while(temp>0){
                    writer.write("+");
                    temp--;
                }
                writer.write("\n");
            }  
            writer.write("\n");
        }

        void copyArys(int[] ary1, int[] ary2){
            int length = ary1.length;
            for(int i=0; i<length; i++){
                ary2[i] = ary1[i];
            }
        }

        void setZero1D(int[] ary){
            int length = ary.length;
            for(int i =0; i<length; i++){
                ary[i] = 0;
            }
        }

        void setBlanks(char[][] graph){
            for(int i =0; i<graph.length; i++){
                for(int j=0; j<graph[i].length; j++){
                    graph[i][j] = ' ';
                }
            }
        }

        int biGaussian(int[] histAry, int[] GaussAry, int maxHeight, int minVal, int maxVal, char[][] progressGraph, BufferedWriter logWriter, BufferedWriter progressWriter) throws IOException{
            logWriter.write("\n *** Entering biGaussian method() ***\n" );
            double sum1, sum2, total, minSumDiff;
            int offset = (int) ((maxVal - minVal) / 10.0);
            int dividePt = offset;
            int bestThr = dividePt;
            minSumDiff = 9999;

            while(dividePt < (maxVal - offset)){
                setZero1D(GaussAry);
                sum1 = fitGauss(0, dividePt, histAry, GaussAry, maxHeight, logWriter);
                sum2 = fitGauss(dividePt, maxVal, histAry, GaussAry, maxHeight, logWriter);
                total = sum1 + sum2;

                if(total < minSumDiff){
                    minSumDiff = total;
                    bestThr = dividePt;
                    copyArys(GaussAry, bestFitGaussAry);
                }

                logWriter.write("*** In biGaussian(): dividePt= " + dividePt + ", sum1= " + sum1 + ", sum2= " + sum2 + ", total= " + total + ", minSumDiff= " + minSumDiff + ", and bestThr= " + bestThr + ". \n");

                if((dividePt % 4) == 0){
                    progressWriter.write("\n*** Below is the progress graph: dividePt= " + dividePt + " *** \n");
                    
                    setBlanks(progressGraph);
                    plotProgressGraph(histAry, GaussAry, progressGraph, progressWriter);
                    dispProgressGraph(progressGraph, progressWriter);  
                }
                dividePt++;
        
            }

            logWriter.write("\n *** Leaving biGaussian(): minSumDiff = " + minSumDiff + " and bestThr = " + bestThr + " *** \n");
            logWriter.write("\n");        
            return bestThr;
        }

        double fitGauss(int leftIndex, int rightIndex, int[] histAry, int[] GaussAry, int maxHeight, BufferedWriter logWriter) throws IOException{
            double mean, var, Gval;
            double sum = 0.0;
            mean = computeMean(leftIndex, rightIndex, maxHeight, histAry, logWriter);
            var = computeVar(leftIndex, rightIndex, mean, histAry, logWriter);

            int index = leftIndex;
            while(index <= rightIndex){
                Gval = modifiedGauss(index, mean, var, maxHeight);
                sum+= Math.abs(Gval - (double)histAry[index]);
                GaussAry[index] = (int) Gval;
                index++; 
            }
            logWriter.write("\n *** Leaving fitGauss method, sum is: " + sum  + " *** \n");
            logWriter.write("\n");
            return sum;
        }


        
        void plotProgressGraph(int[] histAry, int[] GaussAry, char[][] progressGraph, BufferedWriter writer) throws IOException{
            writer.write("\n *** Entering plotProgressGraph() ***\n");
            setBlanks(progressGraph);
            int index = 0;

            while(index <= maxVal){
                if(histAry[index] > 0){
                    int i =0;
                    while(i < histAry[index]){
                        progressGraph[index][i] = '+';
                        i++;   
                    }
                        progressGraph[index][GaussAry[index]] = '@';     
                }
                    index++;
            }
                writer.write("\n");
        }

        void dispProgressGraph(char[][] progressGraph, BufferedWriter progressWriter) throws IOException{
            progressWriter.write("\n" + numRows + " " + " " + numCols + " " + minVal + " " + maxVal + "\n");

            for(int i =0; i<progressGraph.length; i++){
                for(int j =0; j<progressGraph[i].length; j++){
                    progressWriter.write(progressGraph[i][j]);
                }
                progressWriter.write("\n");
            }
            progressWriter.write("\n");
        }
        
        void plotGaussGraph(int[] bestFitGaussAry, char[][] GaussGraph, BufferedWriter writer) throws IOException{
            writer.write("\n *** Entering plotGaussGraph() ***\n");
            setBlanks(GaussGraph);
            int index = 0;
            while(index <= maxVal){
                if(bestFitGaussAry[index] > 0){
                    int i = 0;
                    while(i < bestFitGaussAry[index]){
                        GaussGraph[index][i] = '*';
                        i++;
                    }
                }
                index++;
            }
            writer.write("\n *** Leaving plotGaussGraph() ***\n");
            writer.write("\n");
        }

        void dispGaussGraph(char[][] GaussGraph, BufferedWriter writer) throws IOException{
            writer.write("\n *** 2D display of best fit gaussians ***\n");
            writer.write("\n" + numRows + " " + " " + numCols + " " + minVal + " " + maxVal + "\n");            
            for(int i=0; i<GaussGraph.length; i++){
                for(int j =0; j<GaussGraph[i].length; j++){
                    writer.write(GaussGraph[i][j]);
                }
                writer.write("\n");
            }
            writer.write("\n");
        }
        
        
        void plotGapGraph(int[] histAry, int[] bestFitGaussAry, char[][] gapGraph, BufferedWriter logWriter) throws IOException{
            logWriter.write("\n *** Entering plotGapGraph() ***\n");
            setBlanks(GaussGraph);
            setBlanks(gapGraph);
            int index = 0;
            int end1, end2;
            while(index <= maxVal){
                if(bestFitGaussAry[index] <= histAry[index]){
                    end1 = bestFitGaussAry[index];
                    end2 = histAry[index];
                }
                else{
                    end1 = histAry[index];
                    end2 = bestFitGaussAry[index];
                }
                
                int i = end1;
                while(i <= end2){
                    gapGraph[index][i] = '@';
                    i++;
                }
                index++;
            }

            logWriter.write("\n *** Leaving plotGapGraph() ***\n");
            logWriter.write("\n");
        }
        
        void dispGapGraph(char[][] gapGraph, BufferedWriter writer) throws IOException{
            writer.write("\n" + numRows + " " + " " + numCols + " " + minVal + " " + maxVal + "\n");

            for(int i =0; i<gapGraph.length; i++){
                for(int j=0; j < gapGraph[i].length; j++){
                    writer.write(gapGraph[i][j]);
                }
                    writer.write("\n");
            }
            writer.write("\n");
        }
        
        
        double computeMean(int leftIndex, int rightIndex, int maxHeight, int[] histAry, BufferedWriter logWriter) throws IOException{
            logWriter.write("\n *** Entering computeMean() ***\n");
            maxHeight = 0;
            int sum = 0;
            int numPixels = 0;
            int index = leftIndex;
            while(index < rightIndex){
                sum += (histAry[index] * index);
                numPixels += histAry[index];
                if(histAry[index] > maxHeight){
                    maxHeight = histAry[index];
                }
                index++;
            }
            double result = ((double)(sum))/((double)(numPixels));
            logWriter.write("\n *** Leaving computeMean(), maxHeight = " + maxHeight + ", result = " + result + " ***\n");
            logWriter.write("\n");
            return result;
        }

        double computeVar(int leftIndex, int rightIndex, double mean, int[] histAry, BufferedWriter logWriter) throws IOException{
            logWriter.write("\n *** Entering computeVar() ***\n");
            double sum = 0.0;
            int numPixels = 0;
            int index = leftIndex;

            while(index < rightIndex){
                sum += ( ((double) histAry[index]) * Math.pow(((double)(index - mean)), 2) );
                numPixels += histAry[index];
                index++;
            }

            double result = sum / (double) numPixels;
            logWriter.write("\n *** Leaving computeVar(), returning result = " + result + " ***\n");
            logWriter.write("\n");
            return result;
        }


        double modifiedGauss(int x, double mean, double var, int maxHeight){
            return (double) maxHeight * Math.exp(-((x - mean) * (x - mean)) / (2 * var));
        }

}




class SanchezP_Project2_Main {
    public static void main(String[] args) throws IOException {
        if (args.length != 5) {
            System.err.println("Your command line must include 5 parameters: inFile, histFile, GaussFile, progressFile and logFile");
            System.exit(1);
        }

        String inFile = args[0];
        String histFile = args[1];
        String GaussFile = args[2];
        String progressFile = args[3];
        String logFile = args[4];

        try (
            BufferedWriter histWriter = new BufferedWriter(new FileWriter(histFile));
            BufferedWriter GaussWriter = new BufferedWriter(new FileWriter(GaussFile));
            BufferedWriter progressWriter = new BufferedWriter(new FileWriter(progressFile));
            BufferedWriter logWriter = new BufferedWriter(new FileWriter(logFile));
            Scanner scanner = new Scanner(new File(inFile))
        ) {
            int numRows = scanner.nextInt();
            int numCols = scanner.nextInt();
            int minVal = scanner.nextInt();
            int maxVal = scanner.nextInt();

            BiMeans biMeans = new BiMeans(numRows, numCols, minVal, maxVal, 0, 0, 0);

            int histHeight = biMeans.loadHist(inFile);
            biMeans.GaussGraph = new char[maxVal + 1][histHeight + 1];
            biMeans.gapGraph = new char[maxVal + 1][histHeight + 1];
            biMeans.progressGraph = new char[maxVal + 1][histHeight + 1];

            histWriter.write(" *** Below is the input histogram ***\n");
            biMeans.printHist(histWriter, biMeans.histAry);
            histWriter.write(" *** Below is the graphic display of the input histogram ***\n");
            biMeans.dispHist(histWriter, biMeans.histAry);

            int biGaussThrVal = biMeans.biGaussian(biMeans.histAry, biMeans.GaussAry, histHeight, minVal, maxVal, biMeans.progressGraph, logWriter, progressWriter);

            GaussWriter.write(" *** The selected threshold value is: " + biGaussThrVal + " ***\n");
            GaussWriter.write(" *** Below is the best fitted Gaussians ***\n");
            biMeans.printBestFitGauss(biMeans.bestFitGaussAry, GaussWriter);

            biMeans.plotGaussGraph(biMeans.bestFitGaussAry, biMeans.GaussGraph, logWriter);
            GaussWriter.write(" *** Below is the graphic display of the bestFitGaussAry ***\n");
            biMeans.dispGaussGraph(biMeans.GaussGraph, GaussWriter);

            biMeans.plotGapGraph(biMeans.histAry, biMeans.bestFitGaussAry, biMeans.gapGraph, logWriter);
            GaussWriter.write(" *** Below displays the gaps between the histogram and the best fitted Gaussians ***\n");
            biMeans.dispGapGraph(biMeans.gapGraph, GaussWriter);

        }
    }
}
