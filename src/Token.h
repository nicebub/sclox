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
#include "List.h"

typedef struct _Token Token;
struct _Token {
	TokenType type;
	char * lexeme;
	Object* literal;
	char * inString;
	int line;
	char * (*toString)(Token* token);
	void (*delete_token)(Token* token);

};

declareArrayType(Token);

void init_Token(Token* token, TokenType type, char* lexeme, Object* literal, int line);

char * token_toString(Token* token);
void delete_Token(Token* token);


#endif /* TOKEN_H_ */
