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

static short int Tcounter = 0;
typedef struct _Token Token;
struct _Token {
	TokenType type;
	char * lexeme;
	Object* literal;
	char * inString;
	int line;
	short int id;
	short int owner_references;
	char * (*toString)(Token* token);
	void (*delete_token)(Token* token);

};


void init_Token(Token* token, TokenType type, char* lexeme, Object* literal, int line);

char * token_toString(Token* token);
void delete_Token(Token* token);
short int getNextTokenID(void);

Token* getTokenReference(Token* tok);
Token* releaseTokenReference(Token* tok);
Token copyToken(Token*);

#endif /* TOKEN_H_ */
