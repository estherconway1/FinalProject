#include <stdio.h>

FILE* outStream;

int findNextOperand(char line[], int position){
  while(line[position] != 'x' && line[position] != '#' && line[position] != 'R'){
    position++;
  }
  return position;
}


int getOperandSize(char line[], int position){
  int size = 0;
  while (line[position] != '\n' && line[position] != '\t' && line[position] != ' ' && line[position] != ','){
    position++;
    size++;
  }
  return size;
}


void storeOperand(char line[], char operand[],  int position, int operandSize){

  for (int i = 0; i <= operandSize; i++){
    operand[i] = line[position];
    position++;
  }
  
  operand[operandSize] = '\0';
   
}

int RST(char line[], int position){

  //Get the register to reset
  position = findNextOperand(line, position);
  int size = getOperandSize(line, position);
  
  char register1[size];
  storeOperand(line, register1, position, size);

  //Print the new instruction:
  fprintf(outStream, "\t%s%s%s%s%s\n", "AND\t", register1, ", ", register1, ", #0");
  
  return 1;
  
}

int MLT(char line[], int position){
  
  //Get the first Operand
  position = findNextOperand(line, position);
  int size = getOperandSize(line, position);
  
  char register1[size];

  storeOperand(line, register1, position, size);
  position++;

  //Get the second Operand
  position = findNextOperand(line, position);
  size = getOperandSize(line, position);
  
  char register2[size];

  storeOperand(line, register2, position, size);
  position++;

  //Get the third Operand
  position = findNextOperand(line, position);
  size = getOperandSize(line, position);
  
  char register3[size];

  storeOperand(line, register3, position, size);

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
  
  //Restore the contents of the counter register
  fprintf(outStream, "\t%s\t%s%s\n", "LD", register2, ", #-7");
 
  return 1;
}

int SET(char line[], int position){

  //Get the first Operand
  position = findNextOperand(line, position);
  int size = getOperandSize(line, position);
  
  char register1[size];

  storeOperand(line, register1, position, size);
  position++;

  //Get the second Operand
  position = findNextOperand(line, position);
  size = getOperandSize(line, position);
  
  char operand2[size];

  storeOperand(line, operand2, position, size);
  position++;

  fprintf(outStream, "\t%s\t%s, %s, %s\n", "AND", register1, register1, "#0");
  fprintf(outStream, "\t%s\t%s, %s, %s\n", "ADD", register1, register1, operand2);
}


int SUB(char line[], int position){
  //Get the first Operand
  position = findNextOperand(line, position);
  int size = getOperandSize(line, position);
  
  char register1[size];

  storeOperand(line, register1, position, size);
  position++;

  //Get the second Operand
  position = findNextOperand(line, position);
  size = getOperandSize(line, position);
  
  char register2[size];

  storeOperand(line, register2, position, size);
  position++;

  //Get the third Operand
  position = findNextOperand(line, position);
  size = getOperandSize(line, position);

  
  
  char operand3[size];

  storeOperand(line, operand3, position, size);
  position++;

  if (operand3[0] == 'R'){
    fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
    fprintf(outStream, "\t%s\t%s%s\n", "ST", operand3, ", #-2");
    fprintf(outStream, "\t%s%s%s%s\n", "NOT\t", operand3, ", ", operand3);
    fprintf(outStream, "\t%s%s%s%s%s\n", "ADD\t", operand3, ", ", operand3, ", #1");
    fprintf(outStream, "\t%s%s%s%s%s%s\n", "ADD\t", register1, ", ", register2, ", ", operand3);
    fprintf(outStream, "\t%s\t%s%s\n", "LD", operand3, ", #-6");
  }

  
  if (operand3[0] == '#' || operand3[0] == 'x'){
    fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
    fprintf(outStream, "\t%s\t%s%s\n", "ST", "R0", ", #-2");
    fprintf(outStream, "\t%s\t%s%s%s\n", "AND", "R0, ", " R0, ", " #0");
    fprintf(outStream, "\t%s\t%s, %s, %s\n", "ADD", "R0", "R0", operand3);
 
    fprintf(outStream, "\t%s\t%s\n", "NOT", "R0, R0");
    fprintf(outStream, "\t%s\t%s%s%s%s\n", "ADD", "R0", ", ", "R0", ", #1");
    fprintf(outStream, "\t%s\t%s, %s, %s\n", "ADD", register1, register2, "R0");
    fprintf(outStream, "\t%s\t%s%s\n", "LD", "R0", ", #-8");
  }
  
  return 1;
}

int NEG(char line[], int position){
  //Get the destination register
  position = findNextOperand(line, position);
  int size = getOperandSize(line, position);
  
  char register1[size];

  storeOperand(line, register1, position, size);
  position++;

  
  //Get the register we're negating
  position = findNextOperand(line, position);
  size = getOperandSize(line, position);
  
  char register2[size];

  storeOperand(line, register2, position, size);
  

  //Print the new instructions to the output file
  fprintf(outStream, "\t%s\t%s%s%s\n", "NOT", register1, ", ", register2);
  fprintf(outStream, "\t%s\t%s%s%s%s\n", "ADD", register1, ", ", register1, ", #1");

  
  
}

int findOpcodes(char line[], int lineSize){
   
  for (int i = 0; i < lineSize; i ++){
    
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


int main(int argc, char *argv[]){
  
  FILE* inStream = fopen(argv[1], "r");
  outStream = fopen("output.asm", "w");
  
  char line[50];
  int position = 0;
  char c;

  int lineType;
  while((c = getc(inStream)) != EOF){
    
    line[position] = c;
    
    if (c == '\n'){
      lineType = findOpcodes(line, position);
      if (lineType != 1){
	line[position] = '\0';
	fprintf(outStream, "%s\n", line);     
      }
      
      position = -1;
      
     }
  position++;
  }

  fclose(inStream);
  fclose(outStream);
}
