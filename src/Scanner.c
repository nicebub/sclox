/*
 * Scanner.c
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Scanner.h"
#include "Token.h"
#include "TokenType.h"
#include "Lox.h"

void init_scanner(Scanner* scanner, const char * source, Lox* lox){
/*	asprintf(&scanner->source,"%s", source);*/
	scanner->source = strdup(source);
	init_tokenArray(&scanner->tokens);
	scanner->lox = lox;
	scanner->start = scanner->current = 0;
	scanner->line=1;
	scanner->scanTokens = &scanTokens;
	scanner->delete_scanner = &delete_scanner;
	scanner->isAtEnd = &isAtEnd;
	scanner->scanToken = &scanToken;
	scanner->advance = &advance;
	scanner->addToken = &addToken;
	scanner->addTokenWithObject = &addTokenWithObject;
	scanner->match = &match;
	scanner->peek = &peek;
	scanner->string = &string;
	scanner->isDigit = &isDigit;
	scanner->number = &number;
	scanner->peekNext = &peekNext;
	scanner->identifier = &identifier;
	scanner->isAlpha = &isAlpha;
	scanner->isAlphaNumeric =&isAlphaNumeric;
	scanner->getTokenTypeFromString = &getTokenTypeFromString;
}
TokenArray * scanTokens(Scanner* scanner){
	Token temp_token;

	while(!isAtEnd(scanner)) {
		scanner->start = scanner->current;
		scanToken(scanner);
	}
/*	Token *temp_token = malloc(sizeof(Token));*/
	init_token(&temp_token,EEOF,"",NULL,scanner->line);

	tokens_add(&scanner->tokens,&temp_token);
	return &scanner->tokens;
}

void delete_scanner(Scanner* scanner){
	if(scanner){
		if(scanner->source){
			free(scanner->source);
			scanner->source = NULL;
		}
		delete_tokenArray(&scanner->tokens);
/*		scanner->tokens = NULL;*/
	}
}

int isAtEnd(Scanner* scanner){
	return scanner->current >= strlen(scanner->source);
}

void scanToken(Scanner* scanner){
	char c = advance(scanner);
	switch(c){
		case ' ':
		case '\r':
		case '\t':
					break;
		case '\n':
					scanner->line++;
					break;
      	case '(': addToken(scanner,LEFT_PAREN); break;
		case ')': addToken(scanner,RIGHT_PAREN); break;
		case '{': addToken(scanner,LEFT_BRACE); break;
		case '}': addToken(scanner,RIGHT_BRACE); break;
		case ',': addToken(scanner,COMMA); break;
		case '.': addToken(scanner,DOT); break;
		case '-': addToken(scanner,MINUS); break;
		case '+': addToken(scanner,PLUS); break;
		case ';': addToken(scanner,SEMICOLON); break;
		case '*': addToken(scanner,STAR); break;
		case '!': addToken(scanner,match(scanner,'=') ? BANG_EQUAL : BANG); break;
	    case '=': addToken(scanner,match(scanner,'=') ? EQUAL_EQUAL : EQUAL); break;
	    case '<': addToken(scanner,match(scanner,'=') ? LESS_EQUAL : LESS); break;
	    case '>': addToken(scanner,match(scanner,'=') ? GREATER_EQUAL : GREATER); break;
	    case '/':
	        if (match(scanner,'/')) {
	          while (peek(scanner) != '\n' && !isAtEnd(scanner))
	        	  advance(scanner);
	        }
	        else if(match(scanner,'*')) {
	        	cComment(scanner);
	        }
	        else
	          addToken(scanner,SLASH);
	        break;
	    case '"': string(scanner); break;
	    default:
	        if (isDigit(c)) {
	          number(scanner);
	        }
	        else if (isAlpha(c) ) {
	                 identifier(scanner);
	        }
	        else
	          scanner->lox->error(scanner->lox,scanner->line, "Unexpected character.");

	        break;
	}
}

void identifier(Scanner* scanner){
	char * new_str;
	TokenType type;
	new_str = NULL;
	while(isAlphaNumeric(peek(scanner)))
		advance(scanner);
	new_str = calloc((scanner->current-scanner->start)+1,sizeof(char));
	strncpy(new_str,&scanner->source[scanner->start],scanner->current-scanner->start);
    new_str[scanner->current-scanner->start] = '\0';
	type = getTokenTypeFromString(new_str);
	if(type == KNULL)
		type = IDENTIFIER;
		addToken(scanner,type);
    free(new_str);
    new_str = NULL;
}

int isAlpha(const char c){
	return (c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| (c == '_');
}
int isAlphaNumeric(const char c){
	return isAlpha(c) || isDigit(c);
}

char advance(Scanner* scanner){
	scanner->current++;
	return scanner->source[ scanner->current - 1 ];
}

void addToken(Scanner* scanner, TokenType type){
	addTokenWithObject(scanner,type,NULL);
}
void addTokenWithObject(Scanner* scanner, TokenType type, Object* literal){
	char * text;
	Token temp_token;
/*	Token* temp_token;*/
	text = NULL;
/*	temp_token = NULL; */
	text = calloc((scanner->current-scanner->start)+1,sizeof(char));
	strncpy(text,&scanner->source[scanner->start],scanner->current-scanner->start);
    text[scanner->current-scanner->start] = '\0';
/*	temp_token = malloc(sizeof(Token));*/
	init_token(&temp_token,type,text,literal,scanner->line);
	tokens_add(&scanner->tokens,&temp_token);
	free(text);
	text = NULL;
}

int match(Scanner* scanner, const char expected){
	if(isAtEnd(scanner)) return 0;
	if(scanner->source[scanner->current] != expected) return 0;
	scanner->current++;
	return 1;
}

char peek(Scanner* scanner){
	if(isAtEnd(scanner))
		return '\0';
	return scanner->source[scanner->current];
}

void string(Scanner* scanner){
	char * new_str;
	Object* obj;
	while(peek(scanner) != '"' && !isAtEnd(scanner)){
		if(peek(scanner) == '\n') scanner->line++;
		advance(scanner);

	}
	if(isAtEnd(scanner)){
		scanner->lox->error(scanner->lox,scanner->line,"Unterminated string.");
		return;
	}

	advance(scanner);
	new_str = NULL;
	new_str = calloc(((scanner->current-1)-(scanner->start +1))+1,sizeof(char));
	strncpy(new_str,&scanner->source[scanner->start+1],(scanner->current-1)-(scanner->start +1));
    new_str[(scanner->current-1)-(scanner->start +1)]='\0';
	obj = malloc(sizeof(Object));
	init_Object(obj,new_str,STRING);
	addTokenWithObject(scanner,STRING,obj);
/*	free(new_str);
	new_str = NULL;*/

}

int isDigit(const char c){
	return c>= '0' && c <= '9';
}

void number(Scanner* scanner){
	double *x;
	char * new_str;
	Object* obj;
/*	double y = 0;
	double mult = 10;
	double * worker = NULL;*/
	while(isDigit(peek(scanner)))
		advance(scanner);
	if((peek(scanner) == '.' && isDigit(peekNext(scanner)))){
		advance(scanner);
		while(isDigit(peek(scanner)))
			advance(scanner);
	}
/*	char * str_ptr;*/
	new_str = calloc((scanner->current-scanner->start)+1,sizeof(char));
	strncpy(new_str,&scanner->source[scanner->start],scanner->current - scanner->start);
    new_str[scanner->current-scanner->start]='\0';
/*	str_ptr = new_str;*/
/*	x = atof(new_str);*/
/*	worker = x;
	while(str_ptr++){
		if(str_ptr == '.'){
			worker = y;
			mult = (doube)1/(double)10;
			continue;
		}
		*worker +=
	}*/
	x = malloc(sizeof(double));
	sscanf(new_str,"%lf",x);
	obj = malloc(sizeof(Object));
	init_Object(obj,x, NUMBER);
	addTokenWithObject(scanner,NUMBER,obj);
	free(new_str);
	new_str = NULL;
}

char peekNext(Scanner* scanner){
	if(scanner->current + 1 >= strlen(scanner->source))
		return '\0';
	return scanner->source[scanner->current+1];
}

TokenType getTokenTypeFromString(const char * inString){
	int which = 0;
	while(kmap[which++].keyword != KNULL){
	    size_t length = strlen(inString);
	    if(strlen(kmap[which-1].keywordName) > length)
		   length = strlen(kmap[which-1].keywordName);
		if(strncmp(kmap[which-1].keywordName,inString,length)==0)
			return kmap[which-1].keyword;
/*		printf("%s\n",kmap[which-1].keywordName);*/
	}
	return KNULL;
}

void cComment(Scanner* scanner){
	int nested = 1;
	while(1){
       while (peek(scanner) != '*' && !isAtEnd(scanner)){
    	   if(peek(scanner) == '\n')
    		   scanner->line++;
    	   else if(peek(scanner) == '/' && peekNext(scanner) == '*'){
 /*   		   printf("found nested \/\*\n"); */
    		   nested++;
    		   advance(scanner);
    		   advance(scanner);
    		   continue;
    	   }
    	   advance(scanner);
       }
/*       printf("lookahead 1 : %c\n",peek(scanner));*/
       advance(scanner);
       if(isAtEnd(scanner))
    		   return;
    	if(peek(scanner) == '/'){
/*           printf("lookahead 2 : %c\n",peek(scanner));*/
           advance(scanner);
           nested--;
           if(!nested)
        	   return;
       }
	}
}
