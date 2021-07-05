#include <stdio.h>
#include <stdlib.h>

const char* WHITESPACE = " \t\n\v\f\r";

int main(int argc, char* argv[]) {
	// open code file
	FILE* codeFile = fopen(argv[1], "r");
	if (!codeFile) {
		fprintf(stderr, "No file specified");
		exit(1);
	}
	// get code file length
	fseek(codeFile, 0, SEEK_END);
	long codeFileLength = ftell(codeFile);
	fseek(codeFile, 0, SEEK_SET);
	// store code into string and close file
	char* codeText = malloc(codeFileLength);
	fread(codeText, codeFileLength, 1, codeFile);
	fclose(codeFile);

	puts(codeText);

	return 0;
}