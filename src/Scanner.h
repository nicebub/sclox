/*
 * Scanner.h
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */

#ifndef SCANNER_H_
#define SCANNER_H_
#include "TokenType.h"
#include "Token.h"
#include "Lox.h"
typedef struct _Scanner Scanner;
struct _Scanner {
	Lox* lox;
	char * source;
	TokenArray tokens;
	int start;
	int current;
	int line;
	TokenArray * (*scanTokens)(Scanner* scanner);
	void (*delete_scanner)(Scanner* scanner);
	int (*isAtEnd)(Scanner* scanner);
	void (*scanToken)(Scanner* scanner);
	char (*advance)(Scanner* scanner);
	void (*addToken)(Scanner* scanner, TokenType type);
	void (*addTokenWithObject)(Scanner* scanner, TokenType type, char* literal);
	int (*match)(Scanner* scanner, const char expected);
	char (*peek)(Scanner* scanner);
	void (*string)(Scanner* scanner);
	int (*isDigit)(const char c);
	void (*number)(Scanner* scanner);
	char (*peekNext)(Scanner* scanner);
	void (*identifier)(Scanner* scanner);
	int (*isAlpha)(const char c);
	int (*isAlphaNumeric)(const char c);
	TokenType (*getTokenTypeFromString)(const char * inString);
	void (*cComment)(Scanner* scanner);
};


void init_scanner(Scanner* scanner, const char * source, Lox* lox);
TokenArray * scanTokens(Scanner* scanner);
void scanToken(Scanner* scanner);
void delete_scanner(Scanner* scanner);
int isAtEnd(Scanner* scanner);
char advance(Scanner* scanner);
void addToken(Scanner* scanner, TokenType type);
void addTokenWithObject(Scanner* scanner, TokenType type, char* literal);
int match(Scanner* scanner, const char expected);
char peek(Scanner* scanner);
void string(Scanner* scanner);
int isDigit(const char c);
void number(Scanner* scanner);
char peekNext(Scanner* scanner);
void identifier(Scanner* scanner);
int isAlpha(const char c);
int isAlphaNumeric(const char c);
TokenType getTokenTypeFromString(const char * inString);
void cComment(Scanner* scanner);
#endif /* SCANNER_H_ */
