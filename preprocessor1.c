#include <stdio.h>
int main(void){
  //Open the input and output streams
  FILE* inStream = fopen("test2.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char c;            // Holds the current character from the input stream
  char word[100];    // Holds the current line
  char register1[3]; // Holds the 1st operand in the instructions we're editing
  char register2[3]; // Holds the 2nd operand
  char register3[3]; // Holds the 3rd operand
  int lineWritten = 0;

  
    
  int counter = 0; //Holds the possition that we are up to in the array that holds the line (word[])
  while((c = getchar()) != EOF){
    //Add the current character to the array
    word[counter] = c;

    //If the character is a newline, check if it needs to be changed
    if (c == '\n'){
      //printf("%s", word);
      for (int i = 0; i < counter; i ++){
	
	//If it encounters a RST opcode edit to for the instuctions to reset the  register (AND register with #0)
	if (word[i] == 'R' && word[i + 1] == 'S' && word[i+2] == 'T' && lineWritten == 0){
	  lineWritten = 1;
	  int offset = i + 2;
	  
	  while(word[offset] != 'R' && word[offset] != '\n'){
	    offset ++;
	  }

	  if ((register1[0] != 'R') || (register1[1] != '0' && register1[1] != '1' && register1[1] != '2' && register1[1] != '3' && register1[1] != '4' && register1[1] != '5' && register1[1] != '6' && register1[1] != '7')){
	     fprintf(stderr, "ERROR: OPERAND SHOULD BE A REGISTER\n");
	    
	    }
	   
	  register1[0] = word[offset];
	  register1[1] = word[offset + 1];
	  word[offset + 1];
	  fprintf(outStream, "%s%s%s%s%s", "AND\t", register1, ", ", register1, ", #0");
	  register1[1] = ' ';
	  break;
	}

	//SET a register - more complicated than originally thought because operand can be an number of variable length
	if (word[i] == 'S' && word[i + 1] == 'E' && word[i+2] == 'T' && lineWritten == 0){
	  lineWritten = 1;
	  
	  int offset = i + 2;
	  while(word[offset] != 'R'){
	    offset ++;
	  }
	  register1[0] = word[offset];
	  register1[1] = word[offset + 1];
	  offset++;

	  if ((offset > counter) || (register1[0] != 'R') || (register1[1] != '0' && register1[1] != '1' && register1[1] != '2' && register1[1] != '3' && register1[1] != '4' && register1[1] != '5' && register1[1] != '6' && register1[1] != '7')){
	     fprintf(stderr, "ERROR: OPERAND SHOULD BE A REGISTER\n");
	     //break;
	    }

	   while(word[offset] != '#' && word[offset] != '\n'){
	    offset ++;
	   }

	    register2[0] = word[offset];
	    register2[1] = word[offset + 1];
	    offset++;

	    if ((offset > counter) || (register2[0] != '#') || (register2[1] != '0' && register2[1] != '1' && register2[1] != '2' && register2[1] != '3' && register2[1] != '4' && register1[1] != '5' && register2[1] != '6' && register2[1] != '7')){
	     fprintf(stderr, "ERROR: OPERAND SHOULD BE A NUMBER\n");
	    break;
	    }

	  	   
	  word[offset + 1];
	  fprintf(outStream, "%s%s%s%s%s\n", "AND\t", register1, ", ", register1, ", #0");
	  fprintf(outStream, "\t%s%s%s%s%s%s", "ADD\t", register1, ", ", register1, ", ",  register2);
	  register1[1] = ' ';
	  break;
	}

	//SUB two registers or a register and a decimal number
	if (word[i] == 'S' && word[i + 1] == 'U' && word[i+2] == 'B' && lineWritten == 0){
	  lineWritten = 1;
	  int offset = i + 2;
	  while(word[offset] != 'R'){
	    offset ++;
	  }
	  register1[0] = word[offset];
	  register1[1] = word[offset + 1];
	  offset ++;

	  while(word[offset] != 'R'){
	    offset += 1;
	  }

	  register2[0] = word[offset];
	  register2[1] = word[offset + 1];
	  offset ++;

	  while(word[offset] != 'R' && word[offset] != '#' ){
	    offset += 1;
	  }

	  register3[0] = word[offset];
	  register3[1] = word[offset + 1];
	  
	  fprintf(outStream, "%s%s%s%s\n", "NOT\t", register3, ", ", register3);
	  fprintf(outStream, "\t%s%s%s%s%s\n", "ADD\t", register3, ", ", register3, ", #1");
	  fprintf(outStream, "\t%s%s%s%s%s%s", "ADD\t", register1, ", ", register2, ", ", register3);
	  
	  break;
	}

	fprintf(outStream, "%c", word[i]);
      }
    }
      fprintf(outStream, "%c", '\n');
      counter = -1;
    }
      

    counter ++;
      
    
     
  }

    

  fclose(inStream);
  fclose(outStream);
  

}
