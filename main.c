#include <stdio.h>
#include <stdlib.h>

const char* WHITESPACE = " \t\n\v\f\r";

int main(int argc, char* argv[]) {
	// open code file
	if (argv[1][0] == 0) {
		fprintf(stderr, "No input file specified");
	}
	if (argv[2][0] == 0) {
		fprintf(stderr, "No output file specified");
	}
	FILE* codeFile = fopen(argv[1], "r");
	if (!codeFile) {
		fprintf(stderr, "Invalid input file");
		return 1;
	}
	/*FILE* elfFile = fopen(argv[1], "wb");
	if (!elfFile) {
		fprintf(stderr, "No output file specified");
		return 1;
	}*/

	// get code file length
	fseek(codeFile, 0, SEEK_END);
	long codeFileLength = ftell(codeFile);
	fseek(codeFile, 0, SEEK_SET);
	// store code into string and close file
	char* codeText = malloc(codeFileLength);
	fread(codeText, 1, codeFileLength, codeFile);
	fclose(codeFile);

	puts(codeText);

	//fclose(elfFile);

	return 0;
}