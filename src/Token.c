/*
 * Token.c
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Token.h"
#include "TokenType.h"
#include "Object.h"
#include "TokenArray.h"

#define TOKEN_INIT_SIZE 5

void init_Token(Token* token, TokenType type, char* lexeme, Object* literal, int line){
	token->type = type;
	token->lexeme = lexeme?strndup(lexeme,strlen(lexeme)):NULL;
/*	token->literal = literal?strdup(literal):NULL;*/
/*	if(!literal) token->literal = NULL;
	else*/
	token->id = getNextTokenID();
	token->owner_references = 1;
	token->literal = literal;
/*	asprintf(&token->lexeme,"%s",lexeme);
	asprintf(&token->literal,"%s",literal);*/
	token->line = line;
	token->inString = NULL;
	token->toString = &token_toString;
	token->delete_token = &delete_Token;
}

Token* getTokenReference(Token* tok){
	tok->owner_references += 1;
	return tok;
}
void releaseTokenReference(Token* tok){
	tok->owner_references -= 1;
}


short int getNextTokenID(void){
	Tcounter += 1;
	return Tcounter;
}
char * token_toString(Token* token){
	char* fresh;
	fresh = NULL;
	if(token->inString)
		free(token->inString);
	token->inString = NULL;
	if(token->type == NUMBER){
		asprintf(&fresh,"%.2f",*(double*)token->literal->value);
	}
	asprintf(&token->inString,"%s %s %s",typeName[token->type], token->lexeme, fresh);
	free(fresh);
	fresh = NULL;
	return token->inString;
}

void delete_Token(Token* token){
	if(token){
		if(token->lexeme){
			free(token->lexeme);
			token->lexeme = NULL;
		}
/*		delete_Object(&token->literal);*/
/*		token->literal = NULL;*/
		if(token->inString){
			free(token->inString);
			token->inString = NULL;
		}
	}
}
void initializeTokenElement(Token* tok, void* values){
	Token* newtok = (Token*)values;
	if(!values)
		init_Token(tok,EEOF,NULL,NULL,0);
	else{
		init_Token(tok,newtok->type,newtok->lexeme,newtok->literal,newtok->line);
	}
}
