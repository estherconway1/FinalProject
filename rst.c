#include <stdio.h>
int main(void){
  //char[9] file = "test1.asm";
  FILE* inStream = fopen("test1.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char c;

  char check[2]; 

  char word[7];

  char code[4] = "RST";
  
  int counter = 0;
  while((c = getc(inStream)) != EOF){
    
    char *ptr = &c;
    //fprintf(outStream, "%c", *ptr);
    
    word[counter] = c;

    if (((*ptr) == ' ' || ((*ptr) == '\n') || ((*ptr) == '\t'))){
      for(int i=0; i < counter; i++){
	if(word[i] == 'R' && word[i + 1] == 'S' && word[i + 2] == 'T'){
	  printf("%s", "AND");
	}
      }
      counter = 0;
    }

    counter ++;
    
      
      
    
     
  }

    

  fclose(inStream);
  fclose(outStream);
  

}
