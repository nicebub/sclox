/*
 * Parser.h
 *
 *  Created on: Mar 9, 2020
 *      Author: scotty
 */

#ifndef PARSER_H_
#define PARSER_H_

#define CEXCEPTION_USE_CONFIG_FILE
#include "CException.h"
#include "TokenType.h"
#include "Token.h"
#include "Expr.h"
#include "Lox.h"
#include "Stmt.h"
#include "additions.h"
#include "TokenArray.h"
#include "StmtArray.h"
#include "ExprArray.h"
typedef struct _Parser Parser;
struct _Parser {
	TokenArray* tokens;
	Lox* lox;
	int current;
	int (*match)(Parser* parser,TokenType* types);
	int (*isAtEnd)(Parser* parser);
	Token* (*peek)(Parser* parser);
	Token* (*advance)(Parser* parser);
	CEXCEPTION_T (*error)(Parser* parser, Token* token, const char* message);
};


void init_parser(Parser* parser, TokenArray* tokens,Lox* lox);

volatile StmtArray* parse(Parser* parser);
Stmt* function(Parser* parser, char* kind);
Expr* call(Parser* parser);
Expr* finishCall(Parser* parser, Expr* expr);
Expr* or(Parser* parser);
Stmt* whileStatement(Parser* parser);
Stmt* forStatement(Parser* parser);
Stmt* ifStatement(Parser* parser);
StmtArray* block(Parser* parser);
Stmt* declaration(Parser* parser);
Stmt* varDeclaration(Parser* parser);
Stmt* statement(Parser* parser);
Stmt* expressionStatement(Parser* parser);
Stmt* printStatement(Parser* parser);

Expr* and(Parser* parser);
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
CEXCEPTION_T parse_error(Parser* parser, Token* token, const char* message);

void delete_parser(Parser* parser);

#endif /* PARSER_H_ */
