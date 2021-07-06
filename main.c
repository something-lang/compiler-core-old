#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false,
	true } bool;

const char* WHITESPACE = " \t\n\v\f\r";

typedef enum {
	SYMBOL,               //0
	STRING,               //1
	NUMBER,               //2
	OPERATOR,             //3
	COMMA,                //4
	SEMI_COLON,           //5
	BRACKET_CIRCLE_LEFT,  //6
	BRACKET_CIRCLE_RIGHT, //7
	BRACKET_SQUARE_LEFT,  //8
	BRACKET_SQUARE_RIGHT, //9
	BRACKET_CURLY_LEFT,   //10
	BRACKET_CURLY_RIGHT   //11
} TOKEN_TYPE;

typedef struct {
	TOKEN_TYPE type;
	char* value;
	unsigned length;
} TOKEN;

struct {
	char* src;
	unsigned i; //Index into src
	char c;     //current character

	char* tokencache;
	bool running;
} lexer;

bool is_seperator(char c) {
	char* SEPERATORS = "(),;[]"; // what about comments?
	// shouldn't seperators arrayh
	// i was thinking // and /* would count as a toke
	// the seportaors array will tell me when to stop adding to a token
	unsigned i, length = strlen(SEPERATORS);
	for (i = 0; i < length; i++)
		if (SEPERATORS[i] == c)
			return true;
	return false;
}
bool is_number(char c) {
	//TODO: Add suport for binary and cirtain number sufixes

	//Is the number a decimal or a hexadecimal
	return isdigit(c); // || isxdigit(c);
}

TOKEN_TYPE get_char_type(char c) {
	switch (c) {
		case ',':
			return COMMA;
		case ';':
			return SEMI_COLON;
		case '(':
			return BRACKET_CIRCLE_LEFT;
		case ')':
			return BRACKET_CIRCLE_RIGHT;
		case '[':
			return BRACKET_SQUARE_LEFT;
		case ']':
			return BRACKET_SQUARE_RIGHT;
		case '{':
			return BRACKET_CURLY_LEFT;
		case '}':
			return BRACKET_SQUARE_RIGHT;
		default:
			if (is_number(c))
				return NUMBER;
			else if (isalnum(c))
				return SYMBOL;
			else
				return OPERATOR;
			break;
	}
}
TOKEN_TYPE get_type(char* c) {
	if (false)
		return STRING;
	else if (is_number(c))
		return NUMBER;
	else if (isalnum(c))
		return SYMBOL;
	else
		return OPERATOR;
}

const char* token_type_name(TOKEN_TYPE t) {
	switch (t) {
		case SYMBOL:
			return "Symbol";
		case STRING:
			return "String";
		case NUMBER:
			return "Number";
		case OPERATOR:
			return "Operator";
		case COMMA:
			return "Comma";
		case SEMI_COLON:
			return "Semi-Colon";
		case BRACKET_CIRCLE_LEFT:
			return "Left Circle Bracket";
		case BRACKET_CIRCLE_RIGHT:
			return "Right Circle Bracket";
		case BRACKET_SQUARE_LEFT:
			return "Left Square Bracket";
		case BRACKET_SQUARE_RIGHT:
			return "Right Square Bracket";
		case BRACKET_CURLY_LEFT:
			return "Left Curly Bracket";
		case BRACKET_CURLY_RIGHT:
			return "Right Curly Bracket";
		default:
			return "(unknown)";
	}
}
TOKEN create_char_token(TOKEN_TYPE type, char value) {
	TOKEN token;
	token.length = 1;
	token.type = type;
	token.value = malloc(sizeof(char));
	token.value[0] = value;

	printf("Token[%s]: %s\n", token_type_name(token.type), token.value);
	return token;
}

TOKEN create_token(TOKEN_TYPE type, char* value, unsigned length) {
	TOKEN token;
	token.value = value;
	token.length = length;
	token.type = type;

	printf("Token[%s]: %s\n", token_type_name(token.type), token.value);
	return token;
}

void init_lexer(char* src) {
	lexer.src = src;
	lexer.i = 0;
	lexer.c = lexer.src[lexer.i];
	lexer.running = false;
	lexer.tokencache = "";

	printf("Initalised Lexer: %s\n", src);
}
char lexer_peek() {
	return lexer.src[lexer.i + 1];
}

bool lexer_finished() {
	return lexer.i == strlen(lexer.src);
}

void lex_next_token() {
	if (lexer_finished()) {
		printf("The lexer has ran out of characters\n");
		exit(1);
	}
	unsigned length = 0;
	TOKEN token;
	do {
		length++;
		TOKEN_TYPE type = get_char_type(lexer.c),

					  future = get_char_type(lexer_peek());

		printf("Character[%s]: %c\n", token_type_name(type), lexer.c);

		switch (type) {
			case NUMBER:
			case SYMBOL:
				//strncat(lexer.tokencache, &lexer.c, 1);
				if (future == NUMBER || future == SYMBOL) {
					lexer.running = true;
				} else {
					lexer.running = false;
					//TOKEN_TYPE cache_type = get_type(lexer.tokencache);
					//printf("Cache[%s]: %d\n", token_type_name(cache_type), lexer.tokencache);
				}
				//printf("Cache: %d\n", lexer.tokencache);
				break;

			default:
				token = create_char_token(type, lexer.c);
				break;
		}

		do {
			lexer.i++;
			if (lexer_finished()) {
				lexer.running = false;
			} else {
				lexer.c = lexer.src[lexer.i];
			}
		} while (lexer.c == ' ');
	} while (lexer.running);

	lexer.tokencache = "";
}

// code format right now is just number values and functions like "add(3,5,multiply(5,8,3,2,1,5,76,6,)) // yes code"
// nested comment support like /* /* nesting */ */
/*
char* text - content of the document
unsigned int length - length of the text
*/

int main(int argc, char* argv[]) {
	init_lexer("h(w){int a=1+1;}");
	while (!lexer_finished())
		lex_next_token();

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
	}
	FILE* elfFile = fopen(argv[1], "wb");
	if (!elfFile) {
		fprintf(stderr, "No output file specified");
		return 1;
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

	fclose(elfFile);*/

	return 0;
}