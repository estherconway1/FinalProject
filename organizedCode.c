#include <stdio.h>


int RST(char line[], int position, FILE* outStream){
  //FILE* outStream = fopen("output.asm", "a");
  
  char register1[3];
  
  while(line[position] != 'R' && line[position] != '\n'){
    position++;
  }

  register1[0] = line[position];
  position++;
  register1[1] = line[position];

  fprintf(outStream, "%s%s%s%s%s\n", "AND\t", register1, ", ", register1, ", #0");

  fclose(outStream);
  return 1;

}
int SET(char line[], int position){
  return 1;
}
int SUB(char line[], int position){
  return 1;
}
int MLT(char line[], int position){
  return 1;
}


int findOpcodes(char line[], int lineSize, FILE* outStream){

  //Iterate over the line
  for (int i = 0; i < lineSize; i ++){
    
    
    if (line[i] == 'R' && line[i + 1] == 'S' && line[i+2] == 'T'){
      RST(line, i + 2, outStream);
      return 1;
    }
    

    if (line[i] == 'S' && line[i + 1] == 'E' && line[i + 2] == 'T'){
      SET(line, i);
      return 2;
      
    }
    

    if (line[i] == 'S' && line[i + 1] == 'U' && line[i+2] == 'B'){
      SUB(line, i);
      return 3;
    }

    
    if (line[i] == 'M' && line[i + 1] == 'L' && line[i+2] == 'T'){
      MLT(line, i);
      return 4;
    }

      
  }
  return 5;

}

int main(void){
  FILE* outStream = fopen("output.asm", "w");
  
  char line[50];
  int position = 0;
  char c;

  int lineType;
  while((c = getchar()) != EOF){ //Reads the next character from stdin
    line[position] = c;
    
    if (c == '\n'){
      lineType = findOpcodes(line, position, outStream);
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

  fclose(outStream);
}

