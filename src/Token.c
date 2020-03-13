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

#define TOKEN_INIT_SIZE 5

void init_token(Token* token, TokenType type, char* lexeme, Object* literal, int line){
	token->type = type;
	token->lexeme = lexeme?strndup(lexeme,strlen(lexeme)):NULL;
/*	token->literal = literal?strdup(literal):NULL;*/
/*	if(!literal) token->literal = NULL;
	else*/
	token->literal = literal;
/*	asprintf(&token->lexeme,"%s",lexeme);
	asprintf(&token->literal,"%s",literal);*/
	token->line = line;
	token->inString = NULL;
	token->toString = &token_toString;
	token->delete_token = &delete_Token;
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
/*	TODO	delete_Object(token->literal);
		token->literal = NULL;*/
		if(token->inString){
			free(token->inString);
			token->inString = NULL;
		}
	}
}

void init_tokenArray(TokenArray* tokenArray){
	tokenArray->tokens = NULL;
	tokenArray->size = 0;
	tokenArray->used = 0;
	tokenArray->tokens_add = &tokens_add;
	tokenArray->delete_tokenArray = &delete_tokenArray;
}

void tokens_add(TokenArray* tokenArray,Token* token){
/*	Token* temp_token;
	temp_token = NULL;*/
	int i;
	int init_start=0, init_finish=0;
	if(!tokenArray->tokens){
		tokenArray->tokens = malloc(sizeof(Token) * TOKEN_INIT_SIZE);
		init_finish = TOKEN_INIT_SIZE;
		tokenArray->size = TOKEN_INIT_SIZE;
	}
	else if(tokenArray->used == tokenArray->size){
		tokenArray->tokens = realloc(tokenArray->tokens, sizeof(Token)*(tokenArray->size+TOKEN_INIT_SIZE));
		init_start = tokenArray->used;
		init_finish = tokenArray->size = tokenArray->size+TOKEN_INIT_SIZE;
	}
	for(i=init_start;i<init_finish;i++)
		init_token(&tokenArray->tokens[i],EEOF,NULL,NULL,0);

	init_token(&tokenArray->tokens[tokenArray->used],
			token->type,token->lexeme,token->literal,token->line);
/*
	tokenArray->tokens[tokenArray->used].lexeme = token->lexeme;
	tokenArray->tokens[tokenArray->used].line = token->line;
	tokenArray->tokens[tokenArray->used].literal = token->literal;
	tokenArray->tokens[tokenArray->used].type = token->type;
	*/
	tokenArray->used++;
}

void delete_tokenArray(TokenArray* tokenArray){
	if(tokenArray){
		int i;
		for(i =0; i <tokenArray->used;i++){
			delete_Token(&tokenArray->tokens[i]);
/*			tokenArray->tokens[i] = NULL;*/
		}
	}
}
