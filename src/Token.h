/*
 * Token.h
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */

#ifndef TOKEN_H_
#define TOKEN_H_
#include "TokenType.h"
#include "Object.h"

typedef struct _Token Token;
typedef struct _TokenArray TokenArray;
struct _Token {
	TokenType type;
	char * lexeme;
	Object* literal;
	char * inString;
	int line;
	char * (*toString)(Token* token);
	void (*delete_token)(Token* token);

};

struct _TokenArray {
	Token * tokens;
	int size;
	int used;
	void (*tokens_add)(TokenArray* tokenArray,Token* token);
	void (*delete_tokenArray)(TokenArray* tokenArray);
};

void init_token(Token* token, TokenType type, char* lexeme, Object* literal, int line);

char * token_toString(Token* token);
void delete_Token(Token* token);

void init_tokenArray(TokenArray* tokenArray);
void tokens_add(TokenArray* tokenArray,Token* token);
void delete_tokenArray(TokenArray* tokenArray);

#endif /* TOKEN_H_ */
