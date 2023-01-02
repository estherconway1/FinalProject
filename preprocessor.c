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
  int registerPosition = 0;

  
    
  int counter = 0; //Holds the possition that we are up to in the array that holds the line (word[])
  while((c = getchar()) != EOF){
    //Add the current character to the array
    word[counter] = c;

    //If the character is a newline, check if it needs to be changed
    if (c == '\n'){
      //Examine the characters stored in the word[] array
      for (int i = 0; i < counter; i ++){
	
	//If the computer encounters a RST opcode edit to for the instuctions to reset the  register (AND register with #0)
	if (word[i] == 'R' && word[i + 1] == 'S' && word[i+2] == 'T'){
	  //Start looking for a register after the RST
	  int offset = i + 2;
	  
	  while(word[offset] != 'R' && word[offset] != '\n'){
	    offset ++;
	  }

	  //Save the register
	  register1[0] = word[offset];
	  register1[1] = word[offset + 1];
	  if ((register1[0] != 'R') || (register1[1] < 48  ||  register1[1] > 57)){
	    fprintf(stderr, "ERROR: OPERAND SHOULD BE A REGISTER\n");
	    fprintf(outStream, "%s", "ERROR: OPERAND SHOULD BE A REGISTER");
	    break;
	    
	  }
	  //Print the new instruction
	  fprintf(outStream, "%s%s%s%s%s", "AND\t", register1, ", ", register1, ", #0");

	  //Continue scanning for instructions that need to be changed
	  break;
	}

	//SET a register - more complicated than originally thought because operand can be an number of variable length
	if (word[i] == 'S' && word[i + 1] == 'E' && word[i+2] == 'T'){
	  
	  int offset = i + 2;
	  while(word[offset] != 'R'){
	    offset ++;
	  }
	  register1[0] = word[offset];
	  register1[1] = word[offset + 1];
	  offset++;

	  registerPosition = 0;
	  while(word[offset] != '#' && word[offset] != '\n'){
	    offset ++;
	    registerPosition ++;
	   }
	  register2[0] = word[offset];
	  register2[1] = word[offset + 1];
	  offset++;
	  	   
	  word[offset + 1];
	  fprintf(outStream, "%s%s%s%s%s\n", "AND\t", register1, ", ", register1, ", #0");
	  fprintf(outStream, "\t%s%s%s%s%s%s", "ADD\t", register1, ", ", register1, ", ",  register2);
	  register1[1] = ' ';
	  break;
	}

	//If the computer  encounters the opcode SUB replace with the instructions to subtract the registers (negate second register and add to the first)
	if (word[i] == 'S' && word[i + 1] == 'U' && word[i+2] == 'B'){
	  int offset = i + 2;
	  while(word[offset] != 'R'){
	    offset ++;
	  }
	  register1[0] = word[offset];
	  register1[1] = word[offset + 1];
	  offset ++;

	  while(word[offset] != 'R' && word[offset]!= '\n'){
	    offset += 1;
	  }

	  register2[0] = word[offset];
	  register2[1] = word[offset + 1];
	  offset ++;

	  while(word[offset] != 'R' && word[offset] != '\n' ){
	    offset += 1;
	  }

	  register3[0] = word[offset];
	  register3[1] = word[offset + 1];
	  
	  fprintf(outStream, "%s%s%s%s\n", "NOT\t", register3, ", ", register3);
	  fprintf(outStream, "\t%s%s%s%s%s\n", "ADD\t", register3, ", ", register3, ", #1");
	  fprintf(outStream, "\t%s%s%s%s%s%s", "ADD\t", register1, ", ", register2, ", ", register3);
	  
	  break;
	}
	
	//MLT
	if (word[i] == 'M' && word[i + 1] == 'L' && word[i+2] == 'T'){
	  int offset = i + 2;
	  while(word[offset] != 'R'){
	    offset ++;

	  }
	  register1[0] = word[offset];
	  register1[1] = word[offset + 1];
	  offset++;
	  if ((register1[0] != 'R') || (register1[1] < 48  ||  register1[1] > 57)){
	    fprintf(stderr, "ERROR: OPERAND SHOULD BE A REGISTER\n");
	    fprintf(outStream, "%s", "ERROR: OPERAND SHOULD BE A REGISTER");
	    break;   
	  }

	  
	  while(word[offset] != 'R'){
	    offset ++;

	  }
	  register2[0] = word[offset];
	  register2[1] = word[offset + 1];
	  offset++;
	  if ((register2[0] != 'R') || (register2[1] < 48  ||  register2[1] > 57)){
	    fprintf(stderr, "ERROR: OPERAND SHOULD BE A REGISTER\n");
	    fprintf(outStream, "%s", "ERROR: OPERAND SHOULD BE A REGISTER");
	    break;
	  }

	fprintf(outStream, "%c", word[i]);

	while(word[offset] != 'R'){
	    offset ++;

	  }
	  register3[0] = word[offset];
	  register3[1] = word[offset + 1];
	  offset++;
	  if ((register2[0] != 'R') || (register2[1] < 48  ||  register2[1] > 57)){
	    fprintf(stderr, "ERROR: OPERAND SHOULD BE A REGISTER\n");
	    fprintf(outStream, "%s", "ERROR: OPERAND SHOULD BE A REGISTER");
	    break;
	  }

	  fprintf(outStream, "\t%s\t%s\n", ".FILL", "#0");
	  fprintf(outStream, "\t%s\t%s%s\n", "ST", register2, ", #-2");
	  fprintf(outStream, "\t%s\t%s%s\n", "AND", register1, ", #0");
	  fprintf(outStream, "\t%s\t%s%s%s%s%s\n", "ADD", register1, ", ", register1, ", ", register3);
	  fprintf(outStream, "\t%s\t%s%s%s%s\n", "ADD", register2, ", ", register2, ", #-1");
	  fprintf(outStream, "\t%s\t%s\n", "BRp", "#-3");
	  fprintf(outStream, "\t%s\t%s%s\n", "LD", register2, ", #-7");
	  
	  

	  break;

	}

	fprintf(outStream, "%c", word[i]);
      
    }
      fprintf(outStream, "%c", '\n');
      counter = -1;
    }
      

    counter ++;
      
    
     
  }

    

  fclose(inStream);
  fclose(outStream);
  

}
