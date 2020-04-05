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

typedef struct _Token_vtable Token_vtable;

struct _Token_vtable {
	char * (*toString)(Token* token);
	void (*delete)(void* token);
	Token* (*init)(Token* token, TokenType type, char * lexeme, Object* literal, int line);
	void* (*copy)(void* tok1);
	short int (*getNextID)(void);
}__attribute__((packed));

struct _Token {
	Object super;
	Token_vtable vtable;
	char * lexeme;
	Object* literal;
	char * inString;
	int line;
	short int id;
}__attribute__((packed));


Token* init_Token(Token* token, TokenType type, char* lexeme, Object* literal, int line);

char * token_toString(Token* token);
void delete_Token(void* token);
short int getNextTokenID(void);
void* copy_Token(void*);

#endif /* TOKEN_H_ */
