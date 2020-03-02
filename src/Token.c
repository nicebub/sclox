/*
 * Token.c
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */

#include "Token.h"
#include "TokenType.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TOKEN_INIT_SIZE 5

void init_token(Token* token, TokenType type, char* lexeme, char* literal, int line){
	token->type = type;
	token->lexeme = lexeme?strdup(lexeme):NULL;
	token->literal = literal?strdup(literal):NULL;
/*	asprintf(&token->lexeme,"%s",lexeme);
	asprintf(&token->literal,"%s",literal);*/
	token->line = line;
	token->inString = NULL;
	token->toString = &token_toString;
	token->delete_token = &delete_token;
}

char * token_toString(Token* token){
	if(token->inString)
		free(token->inString);
	token->inString = NULL;
	asprintf(&token->inString,"%s %s %s",typeName[token->type], token->lexeme, token->literal);
	return token->inString;
}

void delete_token(Token* token){
	if(token){
		if(token->lexeme){
			free(token->lexeme);
			token->lexeme = NULL;
		}
		if(token->literal){
			free(token->literal);
			token->literal = NULL;
		}
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

	tokenArray->tokens[tokenArray->used].lexeme = token->lexeme;
	tokenArray->tokens[tokenArray->used].line = token->line;
	tokenArray->tokens[tokenArray->used].literal = token->literal;
	tokenArray->tokens[tokenArray->used].type = token->type;
	tokenArray->used++;
}

void delete_tokenArray(TokenArray* tokenArray){
	if(tokenArray){
		int i;
		for(i =0; i <tokenArray->used;i++){
			delete_token(&tokenArray->tokens[i]);
/*			tokenArray->tokens[i] = NULL;*/
		}
	}
}
