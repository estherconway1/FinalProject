#include <stdio.h>
int main(void){
  //char[9] file = "test1.asm";
  FILE* inStream = fopen("test2.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char c;
  char word[100];
  char register1[3];
  char register2[3];
  char register3[3];

  
    
  int counter = 0;
  while((c = getchar()) != EOF){
    word[counter] = c;

    if (c == '\n'){
      //printf("%s", word);
      for (int i = 0; i < counter; i ++){
	if (word[i] == 'S' && word[i + 1] == 'U' && word[i+2] == 'B'){
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
	  
	  fprintf(outStream, "%s%s%s%s\n", "NOT\t", register2, ", ", register2);
	  fprintf(outStream, "\t%s%s%s%s%s\n", "ADD\t", register2, ", ", register2, ", #1");
	  fprintf(outStream, "\t%s%s%s%s%s%s", "ADD\t", register1, ", ", register2, ", ", register3);
	  
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
