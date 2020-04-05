/*
 * Token.c
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "memory.h"
#include "Token.h"
#include "TokenType.h"
#include "Object.h"
#include "TokenArray.h"
#include "str.h"
#define TOKEN_INIT_SIZE 5

Token* init_Token(Token* token, TokenType type, char* lexeme, Object* literal, int line){
	init_Object(&token->super,NULL,type);
	token->lexeme = NULL;
	if(lexeme){
	    token->lexeme = strcopy(token->lexeme,lexeme);
	}
	token->id = getNextTokenID();
	if(literal)
		token->literal = getReference(literal);
	else
		token->literal = NULL;
	token->line = line;
	token->inString = NULL;
	token->vtable.toString = &token_toString;
	token->vtable.delete = &delete_Token;
    token->vtable.copy = &copy_Token;
    token->vtable.init = &init_Token;
    setAllocated(token,0);
    setCopyConstructor(token,&copy_Token);
    setDestructor(token,&delete_Token);
    return token;
}



short int getNextTokenID(void){
	Tcounter += 1;
	return Tcounter;
}
char * token_toString(Token* token){
	char* fresh, *temp_str;
	fresh = NULL;
	if(token->inString)
		delete(token->inString);
	token->inString = NULL;
	if(token->super.type == NUMBER){
	    asprintf(&fresh,"%.2f",(double)token->literal->value.number);
	    temp_str = NULL;
	    temp_str = strcopy(temp_str,fresh);
	    free(fresh);
	    fresh = temp_str;
	}
    asprintf(&token->inString,"%s %s %s",typeName[token->super.type], token->lexeme, fresh);
    temp_str = NULL;
    temp_str = strcopy(temp_str, token->inString);
    delete(token->inString);
    token->inString = temp_str;
	delete(fresh);
	fresh = NULL;
	return token->inString;
}

void delete_Token(void* ntoken){
	if((Token*)ntoken){
		if(getReferenceCount(ntoken) <= 1){
			if(((Token*)ntoken)->lexeme){
				delete(((Token*)ntoken)->lexeme);
				((Token*)ntoken)->lexeme = NULL;
			}
		delete(&((Token*)ntoken)->literal);
		((Token*)ntoken)->literal = NULL;
			if(((Token*)ntoken)->inString){
				delete(((Token*)ntoken)->inString);
				((Token*)ntoken)->inString = NULL;
			}
			ntoken = NULL;
		}

		else {
			releaseReference(ntoken);
		}
	}
}
void initializeTokenElement(Token** tok, void* value){
    if(!value){
	   *tok = NULL;
    }
	else{
		*tok = new(OBJECTIVE,sizeof(Token));
		init_Token(*tok,((Token*)value)->super.type,
			 ((Token*)value)->lexeme,
				 ((Token*)value)->literal,((Token*)value)->line);
	    setAllocated(*tok,1);
	}
}

void* copy_Token(void* inToken){
    Token *newtok;
    newtok = new(OBJECTIVE,sizeof(Token));
    newtok->lexeme = NULL;
    if(((Token*)inToken)->lexeme){
	   newtok->lexeme =strcopy(newtok->lexeme,((Token*)inToken)->lexeme);
    }
    if(((Token*)inToken)->literal){
	   newtok->literal = copy(((Token*)inToken)->literal);
    }
    init_Token(newtok,((Token*)inToken)->super.type,newtok->lexeme,newtok->literal,((Token*)inToken)->line);
    if(((Token*)inToken)->inString){
	   newtok->inString = NULL;
	   newtok->inString = strcopy(newtok->inString,((Token*)inToken)->inString);
    }
	return newtok;
}

