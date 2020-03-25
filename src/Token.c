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

#define TOKEN_INIT_SIZE 5

void init_Token(Token* token, TokenType type, char* lexeme, Object* literal, int line){
    mem_footer* footer;
	init_Object(&token->super,NULL,type);
	token->lexeme = NULL;
	if(lexeme){
		token->lexeme = new(RAW,sizeof(char)*(strlen(lexeme)+1));
	    memset(token->lexeme,0,strlen(lexeme)+1);
		strncpy(token->lexeme,lexeme,strlen(lexeme));
	}
    footer = get_footer(token);
	token->id = getNextTokenID();
	footer->functions.owner_references = 1;
	if(literal)
		token->literal = getReference(literal);
	else
		token->literal = NULL;
	token->line = line;
	token->inString = NULL;
	token->vtable.toString = &token_toString;
	token->vtable.delete = &delete_Token;
    footer->functions.copy = &copy_Token;
    footer->functions.delete = &delete_Token;
    token->vtable.copy = &copy_Token;
    token->vtable.init = &init_Token;
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
	    temp_str = new(RAW,sizeof(char)*(strlen(fresh)+1));
	    memset(temp_str,0,strlen(fresh)+1);
	    strncpy(temp_str,fresh,strlen(fresh));
	    free(fresh);
	    fresh = temp_str;
	}
    asprintf(&token->inString,"%s %s %s",typeName[token->super.type], token->lexeme, fresh);
	temp_str = new(RAW,sizeof(char)*(strlen(token->inString)+1));
    memset(temp_str,0,strlen(token->inString));
    strncpy(temp_str,token->inString,strlen(token->inString));
    delete(token->inString);
    token->inString = temp_str;
	delete(fresh);
	fresh = NULL;
	return token->inString;
}

void delete_Token(void* ntoken){
	Token* token = (Token*) ntoken;
	if(token){
		if(getReferenceCount(token) <= 1){
			if(token->lexeme){
				delete(token->lexeme);
				token->lexeme = NULL;
			}
		delete(&token->literal);
		token->literal = NULL;
			if(token->inString){
				delete(token->inString);
				token->inString = NULL;
			}
			token = NULL;
		}

		else {
			releaseReference(token);
/*			token->owner_references -= 1;*/
		}
	}
}
void initializeTokenElement(Token** tok, void* value){
	Token* newtok = (Token*)value;
    if(!value){
	   *tok = NULL;
    }
/*		init_Token(tok,EEOF,NULL,NULL,0);*/
	else{
	    mem_footer* footer;
		*tok = new(OBJECTIVE,sizeof(Token));
		init_Token(*tok,newtok->super.type,newtok->lexeme,newtok->literal,newtok->line);
	    footer = get_footer(*tok);
	    footer->functions.allocated = 1;
	}
}

void* copy_Token(void* inToken){
	Token *newtok, *intok;
    char* lexeme;
    Object* lit;
    newtok = new(OBJECTIVE,sizeof(Token));
    intok = (Token*) inToken;
    lexeme = NULL;
    lit = NULL;
    if(intok->lexeme){
	   lexeme = new(RAW,sizeof(char)*(strlen(intok->lexeme)+1));
	   memset(lexeme,0,strlen(intok->lexeme)+1);
	   strncpy(lexeme,intok->lexeme,strlen(intok->lexeme));
	   newtok->lexeme = lexeme;
    }
    if(intok->literal){
	   lit = copy(intok->literal);
    }
    init_Token(newtok,intok->super.type,lexeme,lit,intok->line);
    if(intok->inString){
	   newtok->inString = new(RAW,sizeof(char)*(strlen(intok->inString)+1));
	   memset(newtok->inString,0,strlen(intok->inString)+1);
	   strncpy(newtok->inString,intok->inString,strlen(intok->inString));
    }
	return newtok;
}

