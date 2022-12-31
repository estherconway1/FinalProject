#include <stdio.h>
int main(void){
  //char[9] file = "test1.asm";
  FILE* inStream = fopen("test1.asm", "r");
  FILE* outStream = fopen("output.asm", "w");

  char data;

  int counter = 0;
  while((fscanf(inStream, "%s", &data)) != EOF){

    int output = data;
    fprintf(outStream, "%s", &data);
    if (output == "\n"){
      fprintf(outStream, "%s", "\n");
    }
  
  counter++;
  }

  //fclose(inStream);
  fclose(outStream);
  

}
