/*
 * Parser.h
 *
 *  Created on: Mar 9, 2020
 *      Author: scotty
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "TokenType.h"
#include "Token.h"
#include "Expr.h"

typedef struct _Parser Parser;
/*
typedef struct _Parser_vtable Parser_vtable;
struct _Parser_vtable{
	int (*match)(Parser* parser,TokenType* types);
	int (*isAtEnd)(Parser* parser);
	int (*peek)(Parser* parser);
};
*/
struct _Parser {
/*	Parser_vtable vtable;*/
	TokenArray* tokens;
	int current;
	int (*match)(Parser* parser,TokenType* types);
	int (*isAtEnd)(Parser* parser);
	Token* (*peek)(Parser* parser);
	Token* (*advance)(Parser* parser);
};


void init_parser(Parser* parser, TokenArray* tokens);

Expr* parse(Parser* parser);
Expr* expression(Parser* parser);
Expr* equality(Parser* parser);
Expr* comparison(Parser* parser);
Expr* addition(Parser* parser);
Expr* multiplication(Parser* parser);
Expr* unary(Parser* parser);
Expr* primary(Parser* parser);

int parser_match(Parser* parser,TokenType* types);
int check(Parser* parser, TokenType type);
int parser_isAtEnd(Parser* parser);
Token* parser_peek(Parser* parser);
Token* parser_advance(Parser* parser);
Token* previous(Parser* parser);
void synchronize(Parser* parser);


Token* consume(Parser* parser, TokenType type, const char* message);
void parse_error(Parser* parser, Token* token, const char* message);

#endif /* PARSER_H_ */
