#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

	// open code file
	/*if (argv[1][0] == 0) {
		fprintf(stderr, "No input file specified");
	}
	if (argv[2][0] == 0) {
		fprintf(stderr, "No output file specified");
	}
	FILE* codeFile = fopen(argv[1], "r");
	if (!codeFile) {
		fprintf(stderr, "Invalid input file");
		return 1;
	}*/
	FILE* elfFile = fopen(argv[2], "wb");
	if (!elfFile) {
		fprintf(stderr, "No output file specified");
		return 1;
	}

	// get code file length
	/*fseek(codeFile, 0, SEEK_END);
	long codeFileLength = ftell(codeFile);
	fseek(codeFile, 0, SEEK_SET);
	// store code into string and close file
	char* codeText = malloc(codeFileLength);
	fread(codeText, 1, codeFileLength, codeFile);
	fclose(codeFile);*/

	//puts(codeText);

	char elfHeader[0x40] = {
		0x7F, 'E', 'L', 'F', // magic number
		0x2, // 64 bit
		0x1, // little endian
		0x1, // version
		0x0, 0x0, // platform (unused)
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // unused

		0x3, 0x0, // executable
		0x3e, 0x0, // x86_64
		0x1, 0x0, 0x0, 0x0, // version again?
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // TODO: entry point memory address
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // TODO: program header table
		0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // TODO: section header table
		0x0, 0x0, 0x0, 0x0, // TODO: flags
		0x40, 0x0, // header size
		0x0, 0x0, // TODO: program header table entry
		0x0, 0x0, // TODO: number of entries in section header table
		0x0, 0x0, // TODO: index of section header table with section names
	};
	char sectionHeader[0x40] = {
		
	};
	fwrite(elfHeader, 1, 0x40, elfFile);

	fclose(elfFile);

	return 0;
}