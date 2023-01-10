
void RST();
void SET();
void SUB();
void MLT();


void findOpcodes(char[] line, int lineSize){

  //Iterate over the line
  for (int i = 0; i < lineSize; i ++){
    
    
    if (line[i] == 'R' && line[i + 1] == 'S' && line[i+2] == 'T'){
      
      RST(line, i);
    }
    

    if (line[i] == 'S' && line[i + 1] == 'E' && line[i + 2] == 'T'){
      SET(line, i);
    }
    

    if (line[i] == 'S' && line[i + 1] == 'U' && line[i+2] == 'B'){
      SUB(line, i);
    }

    
    if (line[i] == 'M' && line[i + 1] == 'L' && line[i+2] == 'T'){
      MLT(line, i);
    }

      
  }

  


  
}
int main(void){
	FILE* inStream = fopen("test2.asm", "r");
 	FILE* outStream = fopen("output.asm", "w");
	
	char line[50];
	int linePosition = 0;
	
	while((c = getchar()) != EOF){
	  line[counter] = c;
	  if (c == '\n'){
	    findOpcodes(line, linePosition);
	    fprintf(outStream, "%c", '\n');
	    linePosition = -1
	      }
	  counter++;
	}
}

