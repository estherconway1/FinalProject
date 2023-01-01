#include <stdio.h>
int main(void){
  //char[9] file = "test1.asm";
  FILE* inStream = fopen("test1.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char c;

  char check[2]; 

  char word[10];

  char reg[3];

  

  char code[4] = "RST";
  
  int counter = 0;
  while((c = getc(inStream)) != EOF){
    //printf("%c", c);
    
    word[counter] = c;

    if ((c == '\n')){
      if (word[0] == 'R' && word[1] == 'S' && word[2] == 'T'){
	printf("%s", word);
	fprintf(outStream, "%s", "AND");
	//fprintf(outStream, "%c", word[4]);
	//fprintf(outStream, "%c", word[5]);
	//fprintf(outStream, "%s", ", #0\n");
	//counter = 0;
	}
      //else{
      // for(int i = 0; i <= counter; i++){
      //     fprintf(outStream, "%c", word[i]);
    counter = 0;
    }
    counter ++;
    
    }
      
    
 

    

  fclose(inStream);
  fclose(outStream);
  

}
