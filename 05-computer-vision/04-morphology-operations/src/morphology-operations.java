import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.util.Scanner;

class Morphology{
    int numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin;
    int rowFrameSize, colFrameSize, extraRows, extraCols, rowSize, colSize;
    int[][] zeroFramedAry;
    int[][] morphAry;
    int[][] tempAry;
    int[][] structAry;

        Morphology(int numImgRows, int numImgCols, int imgMin, int imgMax, int numStructRows, int numStructCols, int structMin, int structMax, int rowOrigin, int colOrigin){
            this.numImgRows = numImgRows;
            this.numImgCols = numImgCols;
            this.imgMin = imgMin;
            this.imgMax = imgMax;
            this.numStructRows = numStructRows;
            this.numStructCols = numStructCols;
            this.structMin = structMin;
            this.structMax = structMax;
            this.rowOrigin = rowOrigin;
            this.colOrigin = colOrigin;
            rowFrameSize = (numStructRows/2);
            colFrameSize = (numStructCols/2);
            extraRows = (rowFrameSize * 2);
            extraCols = (colFrameSize * 2);
            rowSize = (numImgRows + extraRows);
            colSize = (numImgCols + extraCols);
            zeroFramedAry = new int[rowSize][colSize];
            morphAry = new int[rowSize][colSize];
            tempAry = new int[rowSize][colSize];
            structAry = new int[numStructRows][numStructCols];

        }

        void zero2DAry(int[][] ary, int nRows, int nCols){
            for(int i = 0; i<nRows; i++){
                for(int j=0; j<nCols; j++){
                    ary[i][j] = 0;
                }
            }
        }
        
        void loadImg(String input, int[][] zeroFramedAry) throws IOException{
            Scanner scanner = new Scanner(new File(input));
            numImgRows = scanner.nextInt();
            numImgCols = scanner.nextInt();
            imgMin = scanner.nextInt();
            imgMax = scanner.nextInt();
            for(int i=rowFrameSize; i<numImgRows + rowFrameSize; i++){
                for(int j=colFrameSize; j<numImgCols + colFrameSize; j++){
                    zeroFramedAry[i][j] = scanner.nextInt();
                }
            }
            scanner.close();
        }

        void loadStruct(String structElement, int[][] structAry) throws IOException{
            Scanner scanner = new Scanner(new File(structElement));
            numStructRows = scanner.nextInt();
            numStructCols = scanner.nextInt();
            structMin = scanner.nextInt();
            structMax = scanner.nextInt();
            //rowOrigin = scanner.nextInt();
            //colOrigin = scanner.nextInt();
            rowOrigin = scanner.nextInt();
            colOrigin = scanner.nextInt();
            for(int i = 0; i<numStructRows; i++){
                for(int j = 0; j<numStructCols; j++){
                    structAry[i][j] = scanner.nextInt();
                }
            }
            scanner.close();
        }

        void computeDilation(int[][] inAry, int[][] outAry){
            for(int i = rowFrameSize; i < numImgRows + rowFrameSize; i++){
                for(int j = colFrameSize; j < numImgCols + colFrameSize; j++){
                    if(inAry[i][j] > 0){
                        onePixelDilation(i, j, inAry, outAry);
                    }
                }
            }
        }

        void computeErosion(int[][] inAry, int[][] outAry){
            for(int i = rowFrameSize; i<numImgRows + rowFrameSize; i++){
                for(int j = colFrameSize; j<numImgCols + colFrameSize; j++){
                    if(inAry[i][j] > 0){
                        onePixelErosion(i, j, inAry, outAry);
                    }
                }
            }
        }
        
        void onePixelDilation(int i, int j, int[][] inAry, int[][] outAry){
            int iOffset = i - (rowOrigin);
            int jOffset = j - (colOrigin);
            for(int k = 0; k < numStructRows; k++){
                for(int l = 0; l < numStructCols; l++){
                    if(structAry[k][l] > 0){
                        outAry[iOffset + k][jOffset + l] = 1;
                    }
                }
            }
        }
        
        void onePixelErosion(int i, int j, int[][] inAry, int[][] outAry){
            int iOffset = i - (rowOrigin);
            int jOffset = j - (colOrigin);
            Boolean matchFlag = true;
            int k =0;
            while((matchFlag) && (k < numStructRows)){
                int l =0;
                while((matchFlag) && (l < numStructCols)){
                    if((structAry[k][l] > 0) && (inAry[iOffset + k][jOffset + l]) <= 0){
                        matchFlag = false;
                    }
                    l++;
                }
                k++;
            }
            if(matchFlag == true){
                outAry[i][j] = 1;
            }
                else{
                    outAry[i][j] = 0;
                }
        }

        void computeClosing(int[][] zeroFramedAry,int[][] morphAry ,int[][] tempAry){
            computeDilation(zeroFramedAry, tempAry);
            computeErosion(tempAry, morphAry);
        }

        void computeOpening(int[][] zeroFramedAry, int[][] morphAry, int[][] tempAry){
            computeErosion(zeroFramedAry, tempAry);
            computeDilation(tempAry, morphAry);
        }

        void AryToFile(int[][] inAry, BufferedWriter writer) throws IOException{
            writer.write( "\n" + numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + "\n");
            for(int i =rowOrigin; i<rowSize; i++){
                for(int j=colOrigin; j<colSize; j++){
                    writer.write(inAry[i][j] + " "); // + " " = convert integer to string for printing, avoid ascii errors
                }
                writer.newLine();
            }
        }

        void prettyPrint(int[][] ary, BufferedWriter writer) throws IOException{
            writer.write("\n" + numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + "\n");
            for(int i =rowFrameSize; i<numImgRows + rowFrameSize; i++){
                for(int j=colFrameSize; j<numImgCols + colFrameSize; j++){
                    if(ary[i][j] > 0){
                        writer.write("1 ");
                    }
                        else{
                            writer.write(". ");
                        }
                        /* int temp = ary[i][j];
                        String W = Integer.toString(temp);
                        int WW = W.length();
                        WW++; */
                }
                    writer.newLine();
            }
                
        }

        void prettyPrintStruct(int[][] ary, BufferedWriter writer) throws IOException{
            writer.write("\n" + numStructRows +  " " + numStructCols + " " + structMin + " " + structMax + "\n");
            for(int i =0; i<numStructRows; i++){
                for(int j =0; j<numStructCols; j++){
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

        void process1(BufferedWriter writer) throws IOException{
            String fileName = "dilationOutFile.txt";
            writer.write("\n Open: " + fileName + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeDilation(zeroFramedAry, morphAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer);
        }

        void process2(BufferedWriter writer) throws IOException{
            String fileName = "erosionOutFile.txt";
            writer.write("\n Open: " + fileName + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeErosion(zeroFramedAry, morphAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer);            
        }

        void process3(BufferedWriter writer) throws IOException{
            String fileName = "openingOutFile.txt";
            writer.write("\n Open: " + fileName + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeOpening(zeroFramedAry, morphAry, tempAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer);  
        }

        void process4(BufferedWriter writer) throws IOException{
            String fileName = "closingOutFile.txt";
            writer.write("\n Open: " + fileName + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeClosing(zeroFramedAry, morphAry, tempAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer);  
        }

        void process5(BufferedWriter writer) throws IOException{
            String fileName = "dilationOutFile.txt";
            writer.write("\n Open: " + fileName + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeDilation(zeroFramedAry, morphAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer);
            
            String fileName2 = "erosionOutFile.txt";
            writer.write("\n Open: " + fileName2 + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeErosion(zeroFramedAry, morphAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer); 
            
            String fileName3 = "openingOutFile.txt";
            writer.write("\n Open: " + fileName3 + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeOpening(zeroFramedAry, morphAry, tempAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer);  

            String fileName4 = "closingOutFile.txt";
            writer.write("\n Open: " + fileName4 + ". \n");
            zero2DAry(morphAry, rowSize, colSize);
            computeClosing(zeroFramedAry, morphAry, tempAry);
            AryToFile(morphAry, writer);
            prettyPrint(morphAry, writer);  
        }

}


class SanchezP_Project4_Main {
   
    public static void main(String[] args) throws IOException{
        if(args.length != 4){
            System.err.println("Your command line must include 4 parameters: inFile, structFile, Operation choice and a prettyPrintFile");
            System.exit(1);            
        }

        String inFile = args[0];
        String structFile = args[1];
        int choice = Integer.parseInt(args[2]);
        String printFile = args[3];

        Scanner scanner = new Scanner(new File(inFile));
        Scanner scanner2 = new Scanner(new File(structFile));
        BufferedWriter writer = new BufferedWriter(new FileWriter(printFile));

        int numImgRows = scanner.nextInt();
        int numImgCols = scanner.nextInt();
        int imgMin = scanner.nextInt();
        int imgMax = scanner.nextInt();
        int numStructRows = scanner2.nextInt();
        int numStructCols = scanner2.nextInt();
        int structMin = scanner2.nextInt();
        int structMax = scanner2.nextInt();
        int rowOrigin = scanner2.nextInt() - 1;
        int colOrigin = scanner2.nextInt() - 1;



        
        Morphology morphology = new Morphology(numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin);
        morphology.zero2DAry(morphology.zeroFramedAry, morphology.rowSize, morphology.colSize);
        morphology.loadImg(inFile, morphology.zeroFramedAry);
        writer.write("\n *** Below is the input Framed array ***\n");
        morphology.prettyPrint(morphology.zeroFramedAry, writer);
        morphology.zero2DAry(morphology.structAry, numStructRows, numStructCols);
        morphology.loadStruct(structFile, morphology.structAry);
        writer.write("\n *** Below is the structuring element array ***\n");
        morphology.prettyPrintStruct(morphology.structAry, writer);  

        if(choice == 1){
            morphology.process1(writer);
        }
        else if(choice == 2){
            morphology.process2(writer);
        }
        else if(choice == 3){
            morphology.process3(writer);
        }
        else if(choice == 4){
            morphology.process4(writer);
        }
        else if(choice == 5){
            morphology.process5(writer);
        }

        scanner.close();
        scanner2.close();
        writer.close();
    }
    
}
