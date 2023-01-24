#include <stdio.h>
#include <string.h>

FILE* outStream; //The machine code file is named the part of the filename before the .asm extenstion + i.obj

//Searches for the position of next operand in the line of input
int findNextOperand(char line[], int position){
  position++;
  while(line[position] != 'x' && line[position] != '#' && line[position] != 'R'){
    //As it looks for the next operand, it makes sure that the instruction looks proper
    if ((line[position] != ',' && line[position] != ' ' && line[position] != 9) || (line[position] == ',' && line[position + 1] == ',')){ //9 is ASCII for tab
      fprintf(stderr, "warning: improper instruction, instruction skipped\n");  
      return '\0';  
    }
    position++;
  }
  return position;
}

//Figures out how long the operand is
int getOperandSize(char line[], int position){
  int size = 0;
  while (line[position] != '\n' && line[position] != '\t' && line[position] != ' ' && line[position] != ',' && line[position] != ';' &&  line[position] != '\0'){
    position++;
    size++;
  }
  
  return size;
}

//Stores the name of the operand in a string passed in from the caller function
void storeOperand(char line[], char operand[],  int position, int operandSize){
  for (int i = 0; i <= operandSize; i++){
    operand[i] = line[position];
    position++;
  }
  
  operand[operandSize] = '\0';
}


//Ensures that an operand is in a proper format
int checkOperand(char operand[], int operandSize, char type){           //type is the type of operand. 'R' is register, 'S' is register or number

  //Make sure the operand is the proper length
  if (operandSize < 2){
    fprintf(stderr, "warning: improper operand, instruction skipped\n");
    return 0;
  }

  if (operand[0] == 'R' || type == 'R' ){
    if (operandSize > 2){
      fprintf(stderr, "warning: improper operand, instruction skipped\n");
      return 0;
    }

  //If the operand must be a register, make sure it is actually a register
  if (type == 'R'){
    if (operand[0] != 'R'){
     fprintf(stderr, "warning: improper operand, instruction skipped\n");
     return 0;
  }
  }

  //If the operand could be a register or a number, make sure it a properly formatted register or number
  if (type == 'S' && operand[0] != 'x' && operand[0] != '#' && operand[0] != 'R'){
    fprintf(stderr, "warning: improper operand, instruction skipped\n");
    return 0;
    
  }

  if (operand[0] == 'R' && (operand[0] < '0' || operand[1] > '7')){
     fprintf(stderr, "warning: improper operand, instruction skipped\n");
    return 0;
  }

  //If any of the remaining characters is not a hexidecimal digits, the operand is not in the proper format
   for (int i = 1; i < operandSize; i++){    
   if (operand[i] != '0' && operand[i] != '1' && operand[i] != '2' && operand[i] != '3' && operand[i] != '4' && operand[i] != '5' && operand[i] != '6' && operand[i] != '7' && operand[i] != '7' && operand[i] != '8' && operand[i] != '9' && operand[i] != 'A' && operand[i] != 'B' && operand[i] != 'C' && operand[i] != 'D' && operand[i] != 'E' && operand[i] != 'F' && operand[i] != 'a' && operand[i] != 'b' && operand[i] != 'c' && operand[i] != 'd' && operand[i] != 'e' && operand[i] != 'f' && operand[i] != '-'){
     fprintf(stderr, "warning: improper operand, instruction skipped\n");
     return 0;
    }
    }
  }

  return 1;
}

int RST(char line[], int position){

  //Get the register to reset
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  int size = getOperandSize(line, position);
  
  char register1[size];
  storeOperand(line, register1, position, size);

  if (checkOperand(register1, size, 'R') == 0)
    {
      return 0;
    }

  //Print the new instruction:
  fprintf(outStream, "\t%s%s%s%s%s\n", "AND\t", register1, ", ", register1, ", #0");
  
  return 1;
  
}

int MLT(char line[], int position){
  
  //Get the first Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  int size = getOperandSize(line, position);
  
  char register1[size];

  storeOperand(line, register1, position, size);
  position++;

  if (checkOperand(register1, size, 'R') == 0)
    {
      return 0;
    }

  
  //Get the second Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  size = getOperandSize(line, position);
  
  char register2[size];

  storeOperand(line, register2, position, size);
  position++;

  if (checkOperand(register2, size, 'R') == 0)
    {
      return 0;
    }

  
  //Get the third Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  size = getOperandSize(line, position);
  
  char register3[size];

  storeOperand(line, register3, position, size);
  
  if (checkOperand(register3, size, 'R') == 0)
    {
      return 0;
    }

  //Print out the new instructions. Which are:
  
  //Allocate a memory location
  fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
  //Save the contents of the second operand register at that location
  fprintf(outStream, "\t%s\t%s%s\n", "ST", register2, ", #-2");
  
  //Clear the destination register
  fprintf(outStream, "\t%s\t%s%s%s%s\n", "AND", register1, ", ", register1, ", #0");
 
  //Repeatedly add, and decrement the register until it is 0
  fprintf(outStream, "\t%s\t%s%s%s%s%s\n", "ADD", register1, ", ", register1, ", ", register3);
  fprintf(outStream, "\t%s\t%s%s%s%s\n", "ADD", register2, ", ", register2, ", #-1");
  fprintf(outStream, "\t%s\t%s\n", "BRp", "#-3");
  
  if (register2[1] != register1[1]){
    //Restore the contents of the counter register
    fprintf(outStream, "\t%s\t%s%s\n", "LD", register2, ", #-7");
  }
 
  return 1;
}

int SET(char line[], int position){

  //Get the first Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  int size = getOperandSize(line, position);
  
  char register1[size];

  storeOperand(line, register1, position, size);
  position++;

  if (checkOperand(register1, size, 'R') == 0)
    {
      return 0;
    } 

  //Get the second Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  size = getOperandSize(line, position);
  
  char operand2[size];

  storeOperand(line, operand2, position, size);
  position++;

  if (checkOperand(operand2, size, 'S') == 0)
    {
      return 0;
    } 

  fprintf(outStream, "\t%s\t%s, %s, %s\n", "AND", register1, register1, "#0");
  fprintf(outStream, "\t%s\t%s, %s, %s\n", "ADD", register1, register1, operand2);

  return 1;
}


int SUB(char line[], int position){
  //Get the first Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  int size = getOperandSize(line, position);
  char register1[size];
  storeOperand(line, register1, position, size);
  position++;
  if (checkOperand(register1, size, 'R') == 0)
    {
      return 0;
    }


  //Get the second Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  size = getOperandSize(line, position);
  char register2[size];
  storeOperand(line, register2, position, size);
  position++;
  if (checkOperand(register2, size, 'R') == 0)
    {
      return 0;
    }


  //Get the third Operand
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  size = getOperandSize(line, position);
  char operand3[size];
  storeOperand(line, operand3, position, size);
  position++;

  if (checkOperand(operand3, size, 'S') == 0)
    {
      return 0;
    }
  
   

  if (operand3[0] == 'R'){
    fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
    fprintf(outStream, "\t%s\t%s%s\n", "ST", operand3, ", #-2");
    fprintf(outStream, "\t%s%s%s%s\n", "NOT\t", operand3, ", ", operand3);
    fprintf(outStream, "\t%s%s%s%s%s\n", "ADD\t", operand3, ", ", operand3, ", #1");
    fprintf(outStream, "\t%s%s%s%s%s%s\n", "ADD\t", register1, ", ", register2, ", ", operand3);
    if (operand3[1] != register1[1]){
      fprintf(outStream, "\t%s\t%s%s\n", "LD", operand3, ", #-6");
    }
  }

  if (operand3[0] == '#' || operand3[0] == 'x'){
    fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
    fprintf(outStream, "\t%s\t%s%s\n", "ST", "R0", ", #-2");
    fprintf(outStream, "\t%s\t%s%s%s\n", "AND", "R0, ", " R0, ", " #0");
    fprintf(outStream, "\t%s\t%s, %s, %s\n", "ADD", "R0", "R0", operand3);
 
    fprintf(outStream, "\t%s\t%s\n", "NOT", "R0, R0");
    fprintf(outStream, "\t%s\t%s%s%s%s\n", "ADD", "R0", ", ", "R0", ", #1");
    fprintf(outStream, "\t%s\t%s, %s, %s\n", "ADD", register1, register2, "R0");
    if (register1[1] != '0'){
      fprintf(outStream, "\t%s\t%s%s\n", "LD", "R0", ", #-8");
    }
  }
  
  return 1;
}

int NEG(char line[], int position){
 
  //Get the destination register
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  int size = getOperandSize(line, position);
  char register1[size];
  storeOperand(line, register1, position, size);
  position++;
  if (checkOperand(register1, size, 'R') == 0)
    {
      return 0;
    }

  
  //Get the register we're negating
  position = findNextOperand(line, position);
  if (position == '\0'){
    return 0;
  }
  size = getOperandSize(line, position);
  char register2[size];
  storeOperand(line, register2, position, size);
  if (checkOperand(register2, size, 'R') == 0)
    {
      return 0;
    }
  

  //Print the new instructions to the output file
  fprintf(outStream, "\t%s\t%s%s%s\n", "NOT", register1, ", ", register2);
  fprintf(outStream, "\t%s\t%s%s%s%s\n", "ADD", register1, ", ", register1, ", #1");

  
}

//Checks to see if there are any changes to make on the line of input. The instructions don't have to be proper, other functions take care of that
int findOpcodes(char line[], int lineSize){
  for (int i = 0; i < lineSize; i++){
    if (line[i] == 'R' && line[i + 1] == 'S' && line[i+2] == 'T'){
      RST(line, i + 2);
      return 1;
    }    

    if (line[i] == 'S' && line[i + 1] == 'E' && line[i + 2] == 'T'){
      SET(line, i + 2);
      return 1;
    }

    if (line[i] == 'S' && line[i + 1] == 'U' && line[i+2] == 'B'){
      SUB(line, i + 2);
      return 1;
    }

    if (line[i] == 'M' && line[i + 1] == 'L' && line[i+2] == 'T'){
      MLT(line, i + 2);
      return 1;
    }

    if (line[i] == 'N' && line[i + 1] == 'E' && line[i+2] == 'G'){
      NEG(line, i + 2);
      return 1;
    }
    
  }
  return 0;
}


int unitTests(void);  
int main(int argc, char *argv[]){
  if (unitTests() == 1){
    printf("%s\n", "All tests passed. IGNORE ERROR MESSAGES ABOVE THIS LINE");
  } else {
    printf("%s\n", "1 or more tests failed");
      }
  //Accepts input file name as a command line argument, should end in the .asm extension for the code to work as expected
  char * inputFile = argv[1];
  int nameLen = strlen(inputFile);
  nameLen = nameLen - 4;
  FILE* inStream = fopen(inputFile, "r");

  //The intermediary output file to be assembled by the assembler has the same name as the file without the .asm extension so that the .obj file name will look as expected (same as input + 'i')
  char outputFile[nameLen + 1];
  outputFile[nameLen + 1] = '\0';
  
  strncpy(outputFile, inputFile, nameLen);
  strcat(outputFile, "i.asm");
  
  outStream = fopen(outputFile, "w");
  
  char line[50];
  int position = 0;
  char c;

  int lineType;
  while((c = getc(inStream)) != EOF){
    line[position] = c;

    //When it reaches the end of the line
    if (c == '\n'){
      //Check to see if there are any changes that have to be made
      lineType = findOpcodes(line, position);
      fflush(stdout);
      if (lineType != 1){
	//If not, print out the line
	line[position] = '\0';
	fprintf(outStream, "%s\n", line);     
      }
      
      position = -1;
      
     }
  position++;
  }
  
  fclose(inStream);
  fclose(outStream);

  char outputCommand[8] = "lc3as ";
  
  strcat(outputCommand, outputFile);
  system(outputCommand);
  
}

int testGetOperandSize(void){
  char test0[2] = " ";
  if (getOperandSize(test0, 0) != 0)
    {
      return 0;
    }
  
  char test1[7] = "R1, ";
  if (getOperandSize(test1, 0) != 2)
    {
      printf("%s", "test1 failed");
      return 0;
    }
  
  char test2[25] = "R1, ; this is a comment ";
  if (getOperandSize(test2, 0) != 2)
    {
      return 0;
    }

  char test3[30] = "R1, R1, ; this is a comment ";
  if (getOperandSize(test3, 0) != 2)
    {
      return 0;
    }

  char test4[30] = "R1, R1 ; this is a comment ";
  if (getOperandSize(test4, 4) != 2)
    {
      return 0;
    }

  char test5[34] = "R1, R1, #3 ; this is a comment ";
  if (getOperandSize(test5, 4) != 2)
    {
      return 0;
    }
  
  char test6[40] = "R1, R1, xFFFC ; this is a comment ";
  if (getOperandSize(test6, 8) != 5)
    {
      return 0;
    }

  char test7[6] = "xFFFC";
  if (getOperandSize(test7, 0) != 5)
    {
      return 0;
    }
  return 1;
  
}
int testStoreOperand(void){
  char operand1[3];

  char test1[9] = "RST R1";
  storeOperand(test1, operand1, 4, 2);

  if (operand1[0] != 'R' && operand1[1] != '1'){
    return 0;
  }

  char test2[15] = "SET R1, #3";
  storeOperand(test2, operand1, 4, 2);

  if (operand1[0] != 'R' && operand1[1] != '1'){
    return 0;
  }

  storeOperand(test2, operand1, 8, 2);

  if (operand1[0] != '#' && operand1[1] != '3'){
    return 0;
  }

  char test3[40] = "SUB R1, R1, xFFFC ; this is a comment ";

  storeOperand(test3, operand1, 12, 5);

  if (operand1[0] != 'x' || operand1[1] != 'F' || operand1[2] != 'F' || operand1[3] != 'F' || operand1[4] != 'C'){
      return 0;
    }
  

  return 1;
  
  
}


int testCheckOperand(void){
  char testReg[3] = "R5";
   if (checkOperand(testReg, 2, 'R') != 1){
     return 0;    
  }
   
  char testDecPos[3] = "#1";
  if (checkOperand(testDecPos, 2, 'S') != 1){
    return 0;    
  }
  char testDecNeg[4] = "#-2";
  if (checkOperand(testDecNeg, 3, 'S') != 1){
    return 0;    
  }
  
  char testHex[6] = "xE36F";
  if (checkOperand(testHex, 5, 'S') != 1){
    return 0;    
  }
  
  char testRegB1[2] = "R";
  if (checkOperand(testRegB1, 1, 'R') != 0){
    return 0;
  }
  
  char testRegB2[2] = "3";
  if (checkOperand(testRegB2, 1, 'R') != 0){
    return 0;
  }
  
  char testDecB2[2] = "2";
  if (checkOperand(testDecB2, 1, 'S') != 0){
    return 0;
  }

  char testDash[2] = "-";
  if (checkOperand(testDash, 1, 'S') != 0){
    return 0;
  }
  if (checkOperand(testDash, 1, 'R') != 0){
    return 0;
  }
  char testX[2] = "x";
  if (checkOperand(testX, 1, 'R') != 0){
    return 0;
  }
  if (checkOperand(testX, 1, 'S') != 0){
    return 0;
  }
  char testDecB1[2] = "#";
  if (checkOperand(testDecB1, 1, 'R') != 0){
    return 0;
  }
  if (checkOperand(testDecB1, 1, 'S') != 0){
    return 0;
  }

  char testRegB3[3] = "RA";
  if (checkOperand(testRegB3, 1, 'R') != 0){
    return 0;
  }
  if (checkOperand(testRegB3, 1, 'S') != 0){
    return 0;
  }

  char testR9[3] = "R9";
  if (checkOperand(testR9, 1, 'R') != 0){
    return 0;
  }
  if (checkOperand(testR9, 1, 'S') != 0){
    return 0;
  }
  return 1;
  
}

int testFindOpcodes(void){
  outStream = fopen("/dev/null", "w");

  char testSUB[15] = "SUB\tR1, R2, R3";
  if (findOpcodes(testSUB, 14) != 1){
    return 0;
  }
  char testMLT[15] = "MLT\tR1, R3, R4";
  if (findOpcodes(testMLT, 14) != 1){
    return 0;
  }
  char testSET[12] = "SET\tR1, #4";
  if (findOpcodes(testSET, 11) != 1){
    return 0;
  }
  char testNEG[12] = "NEG\tR1, R1";
  if (findOpcodes(testNEG, 11) != 1){
    return 0;
  }
  return 1;

  char testNoOp[12] = "R1, R1";
  if (findOpcodes(testNoOp, 6) != 0){
    return 0;
  }

  char testTwoOp[12] = "NEG RST R0"; //Should work, then find next operand would catch
  if (findOpcodes(testTwoOp, 10) != 1){ 
    return 0;
  }
  return 1;
}


int testFindNextOperand(void){

  char testNonsense[12] = "NEGfbeo R0";
  if (findNextOperand(testNonsense, 3) != '\0'){
    printf("%s", "NONSENSE");
    return 0;
  }
  
  char testTwoOp[] = "NEG SUB R0";
  if (findNextOperand(testTwoOp, 3) != '\0'){
   
    return 0;
  }
  return 1;
}

//Tests to make sure that all the functions involved in getting the operands work together properly to find each operand in a line
int testGetOperand(void){

  //Test case where all 3 are registers
  char register1[3];
  char line[15] = "MLT R1, R2, R3";
  
  int position = 3;
  position = findNextOperand(line, position);
  int size = getOperandSize(line, position);
  
  storeOperand(line, register1, position, size);
  position++;

  if (register1[0] != 'R' || register1[1] != '1'){
     return 0;
  }

  position = findNextOperand(line, position);
  size = getOperandSize(line, position);
  storeOperand(line, register1, position, size);
  position++;
  if (register1[0] != 'R' || register1[1] != '2'){
    return 0;
  }

  position = findNextOperand(line, position);
  size = getOperandSize(line, position);

  storeOperand(line, register1, position, size);
  position++;
  if (register1[0] != 'R' || register1[1] != '3'){  
    return 0;
  }

  //Test case where two are registers and one is a number
  char line2[15] = "SUB R1, R2, #-1";
    
  position = 3;
  position = findNextOperand(line2, position);
  size = getOperandSize(line2, position);
  
  storeOperand(line2, register1, position, size);
  position++;

  if (register1[0] != 'R' || register1[1] != '1'){
    return 0;
  }

  position = findNextOperand(line2, position);
  size = getOperandSize(line2, position);
  storeOperand(line2, register1, position, size);
  position++;
  if (register1[0] != 'R' || register1[1] != '2'){
    return 0;
  }

  position = findNextOperand(line2, position);
  size = getOperandSize(line2, position);

  storeOperand(line2, register1, position, size);
  position++;
  if (register1[0] != '#' || register1[1] != '-' || register1[2] != '1'){
    return 0;
  }

  //Test case to make sure that the code catches the 2 opcode situation properly

  char testTwoOp[12] = "NEG RST R0";
  position = 3;
  position = findNextOperand(testTwoOp, position);
  size = getOperandSize(testTwoOp, position);
  storeOperand(testTwoOp, register1, position, size);

  if (checkOperand(register1, size, 'R') != 0){
    return 0;
  }

  return 1;
   
}

int unitTests(void){
  return testGetOperandSize()&& testStoreOperand() && testCheckOperand() && testFindOpcodes() && testGetOperand() && testFindNextOperand();
   
}
