#include <stdio.h>

FILE* outStream;

int getOperandSize(char line[], int position){
  int size = 0;
  while (line[position] != '\n' && line[position] != '\t' && line[position] != ' '){
    position++;
    size++;
  }
  return size;
}


int RST(char line[], int position){
  
  char register1[3] = "  ";
  
  while(line[position] != 'R' && line[position] != '\n'){
    position++;
  }

  register1[0] = line[position];
  position++;
  register1[1] = line[position];

  fprintf(outStream, "\t%s%s%s%s%s\n", "AND\t", register1, ", ", register1, ", #0");

  return 1;

}
int SET(char line[], int position){
  char register1[3] = "  ";

   while(line[position] != 'R'){
    position++;
   }

  register1[0] = line[position];
  position++;
  register1[1] = line[position];

  while(line[position] != 'x' && line[position] != '#'){
    position++;
   }

  int operandSize = getOperandSize(line, position);

  char operand2[operandSize];
  printf("%d", operandSize);


  for (int i = 0; i <= operandSize; i++){
    operand2[i] = line[position];
    printf("%c", line[position]);
    position++;
    
  }
   operand2[operandSize] = '\0';

 fprintf(outStream, "\t%s\t%s, %s, %s\n", "AND", register1, register1, "#0");
 fprintf(outStream, "\t%s\t%s, %s, %s\n", "ADD", register1, register1, operand2);
   
  return 1;
}
int SUB(char line[], int position){
  char register1[3] = "  ";
  char register2[3] = "  ";

  char thirdOperandType = '0';

  while(line[position] != 'R'){
    position++;
   }

  register1[0] = line[position];
  position++;
  register1[1] = line[position];

  while(line[position] != 'R'){
    position++;
   }

  register2[0] = line[position];
  position++;
  register2[1] = line[position];

  while(line[position] != 'x' && line[position] != '#' && line[position] != 'R'){
    position++;
   }

  if (line[position] == '#' || line[position] == 'x'){
    thirdOperandType = 'x';
  }

  if (line[position] == 'R'){
    thirdOperandType = 'R';
  }

  int operandSize = getOperandSize(line, position);

  char operand3[operandSize];
  printf("%d", operandSize);


  for (int i = 0; i <= operandSize; i++){
    operand3[i] = line[position];
    printf("%c", line[position]);
    position++;
    
  }
   operand3[operandSize] = '\0';
   
  
  if (thirdOperandType == 'R'){
    fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
    fprintf(outStream, "\t%s\t%s%s\n", "ST", operand3, ", #-2");
    fprintf(outStream, "\t%s%s%s%s\n", "NOT\t", operand3, ", ", operand3);
    fprintf(outStream, "\t%s%s%s%s%s\n", "ADD\t", operand3, ", ", operand3, ", #1");
    fprintf(outStream, "\t%s%s%s%s%s%s\n", "ADD\t", register1, ", ", register2, ", ", operand3);
    fprintf(outStream, "\t%s\t%s%s\n", "LD", operand3, ", #-6");
  }
 
  if (thirdOperandType == '#'){
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
int MLT(char line[], int position){
  char register1[3] = "  ";
  char register2[3] = "  ";
  char register3[3] = "  ";

  while(line[position] != 'R'){
    position++;
 }

  register1[0] = line[position];
  position++;
  register1[1] = line[position];

  while(line[position] != 'R'){
    position++;
 }

  register2[0] = line[position];
  position++;
  register2[1] = line[position];

  while(line[position] != 'R'){
    position++;
 }

  register3[0] = line[position];
  position++;
  register3[1] = line[position];

 //Print out the new instructions. Which are:
 //Allocate a memory location
 fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
 //Save the contents of the second operand register at that location
 fprintf(outStream, "\t%s\t%s%s\n", "ST", register2, ", #-2");
 //Clear the destination register
 fprintf(outStream, "\t%s\t%s%s%s%s\n", "AND", register1, ", ", register1, 
", #0");
 
 //Repeatedly add, and decrement the register until it is 0
 fprintf(outStream, "\t%s\t%s%s%s%s%s\n", "ADD", register1, ", ", 
register1, ", ", register3);
 fprintf(outStream, "\t%s\t%s%s%s %s\n", "ADD", register2, ",", register2, 
", #-1");
 fprintf(outStream, "\t%s\t%s\n", "BRp", "#-3");
 //Restore the contents of the counter register
 fprintf(outStream, "\t%s\t%s%s\n", "LD", register2, ", #-7"); 
  return 1;
}


int findOpcodes(char line[], int lineSize){

  
  
  
  for (int i = 0; i < lineSize; i ++){

    
    
    if (line[i] == 'R' && line[i + 1] == 'S' && line[i+2] == 'T'){
      RST(line, i + 2);
      return 1;
    }
    

    if (line[i] == 'S' && line[i + 1] == 'E' && line[i + 2] == 'T'){
      SET(line, i + 2);
      return 2;
      
    }
    

    if (line[i] == 'S' && line[i + 1] == 'U' && line[i+2] == 'B'){
      SUB(line, i + 2);
      return 3;
    }

    
    if (line[i] == 'M' && line[i + 1] == 'L' && line[i+2] == 'T'){
      MLT(line, i + 2);
      return 4;
    }

      
  }
  return 5;

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
      if (lineType == 5){
	for (int i = 0; i < position; i++){
	  putc(line[i], outStream);
	}
	fprintf(outStream, "%s", "\n");
      }
      position = -1;
     }
  position++;
  }

  fclose(inStream);
  fclose(outStream);
}

