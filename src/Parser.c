/*
 * Parser.c
 *
 *  Created on: Mar 9, 2020
 *      Author: scotty
 */
#define CEXCEPTION_USE_CONFIG_FILE
#include "CException.h"
#include <stdio.h>
#include "Parser.h"

#include "Token.h"
#include "Expr.h"
#include "TokenType.h"
#include "string.h"
void init_parser(Parser* parser, TokenArray* tokens, Lox* lox){
	parser->lox = lox;
	parser->match = &parser_match;
	parser->isAtEnd = &parser_isAtEnd;
	parser->peek = &parser_peek;
	parser->advance = &parser_advance;
	parser->error = &parse_error;
	parser->tokens = tokens;
	parser->current = 0;
}

volatile Expr* parse(Parser* parser){
/*	try{*/
	volatile CEXCEPTION_T e;
	volatile Expr* expr ;
    expr = NULL;
	Try {
		expr = expression(parser);
	}
	Catch(e) {
		printf("Caught an exception during parse\n");
		return NULL;

	}
/*	 }*/
/*	catch {
	 return NULL;
	 }*/
	if(expr)
		return expr;
	return NULL;
}

Expr* expression(Parser* parser){
	return equality(parser);
}

Expr* equality(Parser* parser){
	Expr* expr = comparison(parser);

	TokenType types[] = { BANG_EQUAL, EQUAL_EQUAL, KNULL };
	while(parser->match(parser,types)){
		Token* operator = previous(parser);
		Expr* right = comparison(parser);
		Binary* binary = malloc(sizeof(Binary));
		new_Binary(binary,expr, operator, right);
		expr = (Expr*)binary;
	}
	return expr;
}

Expr* comparison(Parser* parser){
	Expr* expr = addition(parser);
	TokenType types[] = {GREATER,GREATER_EQUAL, LESS, LESS_EQUAL,KNULL };

	while(parser->match(parser,types)){
		Token* operator = previous(parser);
		Expr* right = addition(parser);
		Binary* binary = malloc(sizeof(Binary));
		new_Binary(binary,expr,operator,right);
		expr = (Expr*)binary;
	}
	return expr;
}

Expr* addition(Parser* parser){
	Expr* expr = multiplication(parser);
	TokenType types[] = {MINUS,PLUS, KNULL};

	while(parser->match(parser,types)){
		Token* operator = previous(parser);
		Expr* right = multiplication(parser);
		Binary* binary = malloc(sizeof(Binary));
		new_Binary(binary,expr,operator,right);
		expr = (Expr*)binary;
	}
	return expr;
}

Expr* multiplication(Parser* parser){
	Expr* expr = unary(parser);
	TokenType types[] = {SLASH,STAR, KNULL};

	while(parser->match(parser,types)){
		Token* operator = previous(parser);
		Expr* right = unary(parser);
		Binary* binary = malloc(sizeof(Binary));
		new_Binary(binary,expr,operator,right);
		expr = (Expr*)binary;
	}
	return expr;
}

Expr* unary(Parser* parser){
	TokenType types[] = { BANG, MINUS, KNULL };
	if(parser->match(parser, types)){
		Token* operator = previous(parser);
		Expr* right = unary(parser);
		Unary* un = malloc(sizeof(Unary));
		new_Unary(un,operator,right);
		return (Expr*)un;
	}
	return primary(parser);
}

Expr* primary(Parser* parser){
	Object * obj;
	TokenType types[] = { FALSE, KNULL };
	if(parser->match(parser,types)){
		double* val;
		Literal* lit = malloc(sizeof(Literal));
		obj = malloc(sizeof(Object));
	    val = malloc(sizeof(double));
	    *val = 0;
		init_Object(obj,val,FALSE);
/*		init_Object(obj,strdup("false"),FALSE);*/
		new_Literal(lit,obj);
		return (Expr*)lit;
	}
	types[0] = TRUE;
	if(parser->match(parser,types)){
	    double* val;
		Literal* lit = malloc(sizeof(Literal));
		obj = malloc(sizeof(Object));
	     val = malloc(sizeof(double));
	     *val = 1;
	     init_Object(obj,val,TRUE);
/*		init_Object(obj,strdup("true"),TRUE);*/
		new_Literal(lit,obj);
		return (Expr*)lit;
	}
	types[0] = NIL;
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
		obj = malloc(sizeof(Object));
		init_Object(obj,strdup("nil"),NIL);
		new_Literal(lit,obj);
		return (Expr*)lit;
	}
	types[0] = NUMBER;
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
/*		obj = malloc(sizeof(Object));*/
/*		init_Object(obj,previous(parser)->literal-> ,NUMBER);*/
		new_Literal(lit,previous(parser)->literal);
		return (Expr*)lit;
	}
	types[0] = STRING;
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
/*		obj = malloc(sizeof(Object));*/
/*		init_Object(obj,previous(parser)->literal,STRING);*/
		new_Literal(lit,previous(parser)->literal);
		return (Expr*)lit;
	}
	types[0] = LEFT_PAREN;
	if(parser->match(parser,types)){
		Expr* expr = expression(parser);
		Grouping*  group;
		consume(parser, RIGHT_PAREN, "Expect ')' after expression.");
		group = malloc(sizeof(Grouping));
		new_Grouping(group,expr);
		return (Expr*)group;
	}
	Throw(parser->error(parser,parser->peek(parser),"Expect expression."));
	/* shouldn't reach this return statement */
    return NULL;
/*	 throw error(peek(parser), "Expect expression.");*/

}

Token* consume(Parser* parser, TokenType type, const char* message){
    if(check(parser,type))
		return parser->advance(parser);

    Throw(parser->error(parser,parser->peek(parser),message));
/*	 otherwise throw error(peek(),message);*/
    return NULL;
}

CEXCEPTION_T parse_error(Parser* parser, Token* token, const char* message){
    CEXCEPTION_T e;
	parser->lox->parse_error(parser->lox,token,message);
    e.id = 1;
	return e;
/*	 Lox.error(token,message);
	 return new ParseError() exception*/
}


void synchronize(Parser* parser){
	parser->advance(parser);
	while(!parser->isAtEnd(parser)){
		if(previous(parser)->type == SEMICOLON)
			return;
		switch(parser->peek(parser)->type){
		case CLASS:
		case FUN:
		case VAR:
		case FOR:
		case IF:
		case WHILE:
		case PRINT:
		case RETURN:
			return;
		default: break;
		}
		parser->advance(parser);
	}
}

int parser_match(Parser* parser, TokenType* types){
	TokenType* type_ptr;
	type_ptr = types;
	while(*type_ptr != KNULL){
		if(check(parser,*type_ptr)){
			parser->advance(parser);
			return 1;
		}
		type_ptr++;
	}
	return 0;
}

int check(Parser* parser, TokenType type){
	if(parser->isAtEnd(parser))
		return 0;
	return parser->peek(parser)->type == type;
}

Token* parser_advance(Parser* parser){
	if(!parser->isAtEnd(parser))
		parser->current++;
	return previous(parser);
}

int parser_isAtEnd(Parser* parser){
	return parser->peek(parser)->type == EEOF;
}

Token* parser_peek(Parser* parser){
	return getTokeninArrayAt(parser->tokens,parser->current);
}

Token* previous(Parser* parser){
	return getTokeninArrayAt(parser->tokens,parser->current-1);
}

/*
 * TODO  - CHALLENGES
 * In C, a block is a statement form that allows you to pack a series of
 *  statements where a single one is expected. The comma operator is an
 *  analogous syntax for expressions. A comma-separated series of expressions
 *   can be given where a single expression is expected (except inside a
 *   function call’s argument list). At runtime, the comma operator evaluates
 *   the left operand and discards the result. Then it evaluates and returns
 *   the right operand.
 *
 * Add support for comma expressions. Give them the same precedence and
 *   associativity as in C. Write the grammar, and then implement the necessary
 *    parsing code.
 *
 * Likewise, add support for the C-style conditional or “ternary” operator ?:.
 *    What precedence level is allowed between the ? and :? Is the whole operator
 *    left-associative or right-associative?
 *
 * Add error productions to handle each binary operator appearing without a
 *    left-hand operand. In other words, detect a binary operator appearing at the
 *    beginning of an expression. Report that as an error, but also parse and
 *    discard a right-hand operand with the appropriate precedence.
 *
 */
