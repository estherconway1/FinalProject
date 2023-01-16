#include <stdio.h>
int main(void){
  //char[9] file = "test1.asm";
  FILE* inStream = fopen("test1.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char c;

  char word[100];
  char register1[3];
>>>>>>> alt-history
    
  int counter = 0;
  while((c = getchar()) != EOF){
    word[counter] = c;
    if (c == '\n'){
      //printf("%s", word);
      for (int i = 0; i < counter; i ++){
	if (word[i] == 'R' && word[i + 1] == 'S' && word[i+2] == 'T'){
	  register1[0] = word[i+4];
	  register1[1] = word[i+5];
	  fprintf(outStream, "%s%s%s%s%s", "AND ", register1, ", ", register1, ", #0");  
	  break;
	}
	fprintf(outStream, "%c", word[i]);
    }
      fprintf(outStream, "%c", '\n');
      counter = -1;
    }
      

    counter ++;
>>>>>>> alt-history
      
    
 

    

  fclose(inStream);
  fclose(outStream);
  

}
