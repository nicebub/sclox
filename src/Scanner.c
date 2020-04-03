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
#include "str.h"
void init_Scanner(Scanner* scanner, const char * source, Lox* lox){
    scanner->tokens = new(OBJECTIVE,sizeof(TokenArray));
	init_TokenArray(scanner->tokens);
	scanner->source = strdup(source);
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
	Token *temp_token;

	while(!isAtEnd(scanner)) {
		scanner->start = scanner->current;
		scanToken(scanner);
	}
    temp_token = new(OBJECTIVE,sizeof(Token));
	init_Token(temp_token,EEOF,"",NULL,scanner->line);

	addElementToTokenArray(scanner->tokens,temp_token);
	return scanner->tokens;
}

void delete_scanner(Scanner* scanner){
	if(scanner){
		if(scanner->source){
			free(scanner->source);
			scanner->source = NULL;
		}
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
      	case '(': addToken(scanner,LEFT_PAREN,"("); break;
		case ')': addToken(scanner,RIGHT_PAREN,")"); break;
		case '{': addToken(scanner,LEFT_BRACE,"{"); break;
		case '}': addToken(scanner,RIGHT_BRACE,"}"); break;
		case ',': addToken(scanner,COMMA,","); break;
		case '.': addToken(scanner,DOT,"."); break;
		case '-': addToken(scanner,MINUS,"-"); break;
		case '+': addToken(scanner,PLUS,"+"); break;
		case ';': addToken(scanner,SEMICOLON,";"); break;
		case '*': addToken(scanner,STAR,"*"); break;
		case '!': if(match(scanner,'='))
					addToken(scanner,BANG_EQUAL,"!=");
		else
			addToken(scanner,BANG,"!");
			break;
	    case '=':
	    	if(match(scanner,'='))
	    		addToken(scanner, EQUAL_EQUAL,"==");
	    	else
	    		addToken(scanner, EQUAL,"=");
	    		break;
	    case '<':
	    	if(match(scanner,'='))
				addToken(scanner, LESS_EQUAL,"<=");
	    	else
	    		addToken(scanner,LESS, "<");
	    	break;
	    case '>':
	    	if(match(scanner,'='))
	    		addToken(scanner,GREATER_EQUAL,">=");
	    	else
	    		addToken(scanner,GREATER,">");
	    	break;
	    case '/':
	        if (match(scanner,'/')) {
	          while (peek(scanner) != '\n' && !isAtEnd(scanner))
	        	  advance(scanner);
	        }
	        else if(match(scanner,'*')) {
	        	cComment(scanner);
	        }
	        else
	          addToken(scanner,SLASH,"/");
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
	char * anew_str;
	TokenType type;
	anew_str = NULL;
	while(isAlphaNumeric(peek(scanner)))
		advance(scanner);
    anew_str = new_str(scanner->current-scanner->start);
	strncpy(anew_str,&scanner->source[scanner->start],scanner->current-scanner->start);
    anew_str[scanner->current-scanner->start] = '\0';
	type = getTokenTypeFromString(anew_str);
	if(type == KNULL)
		type = IDENTIFIER;
		addToken(scanner,type,(anew_str));
    anew_str = NULL;
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

void addToken(Scanner* scanner, TokenType type,char* lexeme){
	addTokenWithObject(scanner,type,lexeme,NULL);
}
void addTokenWithObject(Scanner* scanner, TokenType type, char* lexeme,Object* literal){
	Token *temp_token;
    temp_token = new(OBJECTIVE,sizeof(Token));
	init_Token(temp_token,type,lexeme,literal,scanner->line);
	addElementToTokenArray(scanner->tokens,temp_token);
	lexeme = NULL;
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
	char * anew_str;
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
	anew_str = NULL;
    anew_str = new_str((scanner->current-1)-(scanner->start +1));
	strncpy(anew_str,&scanner->source[scanner->start+1],(scanner->current-1)-(scanner->start +1)+1);
    anew_str[(scanner->current-1)-(scanner->start +1)]='\0';
	obj = new(OBJECTIVE,sizeof(Object));
	init_Object(obj,anew_str,STRING);
	addTokenWithObject(scanner,STRING,anew_str,obj);
	anew_str = NULL;

}

int isDigit(const char c){
	return c>= '0' && c <= '9';
}

void number(Scanner* scanner){
	double *x;
	char * anew_str;
	Object* obj;
	while(isDigit(peek(scanner)))
		advance(scanner);
	if((peek(scanner) == '.' && isDigit(peekNext(scanner)))){
		advance(scanner);
		while(isDigit(peek(scanner)))
			advance(scanner);
	}
    anew_str =NULL;
    anew_str = new_str((scanner->current-scanner->start));
	strncpy(anew_str,&scanner->source[scanner->start],scanner->current - scanner->start);
    anew_str[scanner->current-scanner->start]='\0';
	x = new(RAW,sizeof(double));
	sscanf(anew_str,"%lf",x);
	obj = new(OBJECTIVE,sizeof(Object));
	init_Object(obj,x, NUMBER);
	addTokenWithObject(scanner,NUMBER,anew_str,obj);
	anew_str = NULL;
	x = NULL;
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
    		   nested++;
    		   advance(scanner);
    		   advance(scanner);
    		   continue;
    	   }
    	   advance(scanner);
       }
       advance(scanner);
       if(isAtEnd(scanner))
    		   return;
    	if(peek(scanner) == '/'){
           advance(scanner);
           nested--;
           if(!nested)
        	   return;
       }
	}
}
