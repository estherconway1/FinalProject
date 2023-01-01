#include <stdio.h>
int main(void){
  //char[9] file = "test1.asm";
  FILE* inStream = fopen("test1.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char c;

  char check[2];

  char word[5];

  char code[4] = "RST";
  
  int counter = 0;
  while((c = getc(inStream)) != EOF){
    //check[2] = c;
    word[counter] = c;
    if (counter == 6){
      counter = 1;
      printf("%s", word);
    }

    counter++;
    //if (word[2] == code[2]){
    //  printf("%s", word);
    //  fprintf(outStream, "%s", word);
     
    // }
    
     
  }

    

  fclose(inStream);
  fclose(outStream);
  

}
