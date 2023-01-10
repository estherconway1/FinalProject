void findOpcodes(line, lineSize);

int main(void){
	FILE* inStream = fopen("test2.asm", "r");
 	FILE* outStream = fopen("output.asm", "w");
	
	char line[50];

	int counter = 0;
	while((c = getchar()) != EOF){
		line[counter] = c;
		if (c == '\n'){
			findOpcodes(word, counter);
			fprintf(outStream, "%c", '\n');
			counter = -1

		}
		counter++;

	}
}

