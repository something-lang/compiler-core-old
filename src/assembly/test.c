//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

// TODO: this code only works correctly on x86_64 processors, and can only compile for x86_64 processors.
// 


const char
	MAGIC_NUMBER[4] = {0x7F, 'E', 'L', 'F'},
	ARCH_64BIT = 2, // 2 for true, 1 for false
	ARCH_ENDIANNESS = 1; // 1 for little, 2 for big

char* generateElfHeader(size_t* elfHeaderSize) {
	*elfHeaderSize = ARCH_64BIT == 2 ? 0x40 : 0x34;
	char* elfHeader = malloc(*elfHeaderSize);
	uint64_t* tmp = malloc(sizeof(uint64_t));

	memcpy(elfHeader + 0x0, &MAGIC_NUMBER, 4);
	memset(elfHeader + 0x4, ARCH_64BIT, 1);
	memset(elfHeader + 0x5, ARCH_ENDIANNESS, 1);
	memset(elfHeader + 0x6, 0x01, 1); // elf version
	memset(elfHeader + 0x7, 0x03, 1); // platform
	memset(elfHeader + 0x8, 0x00, 1); // extra platform information
	memset(elfHeader + 0x9, 0x00, 7); // unused space

	memcpy(elfHeader + 0x10, (*(uint16_t*)tmp = 0x0003, tmp), 2); // file type
	memcpy(elfHeader + 0x12, (*(uint16_t*)tmp = 0x003e, tmp), 2); // architecture
	memcpy(elfHeader + 0x14, (*(uint32_t*)tmp = 0x00000001, tmp), 4); // version again?

	size_t offset = 0;
	if (ARCH_64BIT == 2) {
		// 64 bit
		memcpy(elfHeader + 0x18, (*(uint64_t*)tmp = 0x0000000000000000, tmp), 8); // TODO: entry point
		memcpy(elfHeader + 0x20, (uint64_t*)elfHeaderSize, 8); // program header
		memcpy(elfHeader + 0x28, (*(uint64_t*)tmp = 0x0000000000000000, tmp), 8); // TODO: section header
		offset += 0x30 - 0x24;
	} else {
		// 32 bit
		memcpy(elfHeader + 0x18, (*(uint32_t*)tmp = 0x00000000, tmp), 4); // TODO: entry point
		memcpy(elfHeader + 0x1c, (uint32_t*)elfHeaderSize, 4); // program header
		memcpy(elfHeader + 0x20, (*(uint32_t*)tmp = 0x00000000, tmp), 4); // TODO: section header
	}

	memcpy(elfHeader + offset + 0x24, (*(uint16_t*)tmp = 0x0000, tmp), 2); // flags (unused on x86_64) TODO: other architectures

	memcpy(elfHeader + offset + 0x28, (uint16_t*)elfHeaderSize, 2); // elf header size
	memcpy(elfHeader + offset + 0x2a, (uint16_t*)tmp, 2); // TODO: program header entry size
	memcpy(elfHeader + offset + 0x2c, (uint16_t*)tmp, 2); // TODO: program header entry amount
	memcpy(elfHeader + offset + 0x2e, (uint16_t*)tmp, 2); // TODO: section header entry size
	memcpy(elfHeader + offset + 0x30, (uint16_t*)tmp, 2); // TODO: section header entry amount

	memcpy(elfHeader + offset + 0x30, (uint16_t*)tmp, 2); // TODO: section header entry names entry index 

	return elfHeader;
}

char* generateProgramHeader(size_t* programHeaderSize) {
	*programHeaderSize = ARCH_64BIT == 2 ? 0x38 : 0x20;
	char* programHeader = malloc(*programHeaderSize);
	memcpy(programHeader + 0x0, MAGIC_NUMBER, 4);
	//uint32_t
	/*{
		, // type of segment
		ARCH_64BIT, // 32/64 bit
		ARCH_ENDIANNESS, // little/big endian
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
	};*/

	return programHeader;
}

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

	size_t elfHeaderSize;
	char* elfHeader = generateElfHeader(&elfHeaderSize);
	size_t programHeaderSize;
	char* programHeader = generateProgramHeader(&programHeaderSize);
	fwrite(elfHeader, 1, elfHeaderSize, elfFile);
	//fwrite(programHeader, 1, programHeaderSize, elfFile);

	fclose(elfFile);

	return 0;
}