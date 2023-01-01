#include <stdio.h>
int main(void){
  //char[9] file = "test1.asm";
  FILE* inStream = fopen("test1.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char c;
  char word[30];
  char rstCode[4] = "RST";
    
  int counter = 0;
  while((c = getc(inStream)) != EOF){
    word[counter] = c;

    if (c == '\n'){
      //printf("%s", word);
      for (int i = 0; i < counter; i ++){
	if (word[i] == 'R' && word[i + 1] == 'S' && word[i+2] == 'T'){
	  printf("%s", "AND ");
	  printf("%c", word[i+4]);
	  printf("%c", word[i+5]);
	  printf("%s", ", #0");
	  
	    
	  break;
	}
	printf("%c", word[i]);
    }
      printf("%c", '\n');
      counter = -1;
    }
      

    counter ++;
      
    
     
  }

    

  fclose(inStream);
  fclose(outStream);
  

}
