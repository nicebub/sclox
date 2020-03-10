/*
 * Parser.c
 *
 *  Created on: Mar 9, 2020
 *      Author: scotty
 */

#include "Parser.h"

#include "Token.h"
#include "Expr.h"
#include "TokenType.h"

void init_parser(Parser* parser, TokenArray* tokens){
	parser->match = &parser_match;
	parser->isAtEnd = &parser_isAtEnd;
	parser->peek = &parser_peek;
	parser->advance = &parser_advance;
	parser->tokens = tokens;
	parser->current = 0;
}

Expr* parse(Parser* parser){
/*	try{*/
	Expr* expr = expression(parser);
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
	TokenType types[] = { FALSE, KNULL };
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
		new_Literal(lit,strdup("false"),3, FALSE);
		return (Expr*)lit;
	}
	types[0] = TRUE;
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
		new_Literal(lit,strdup("true"),3, TRUE);
		return (Expr*)lit;
	}
	types[0] = NIL;
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
		new_Literal(lit,strdup("nil"),3, NIL);
		return (Expr*)lit;
	}
	types[0] = NUMBER;
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
		new_Literal(lit,previous(parser)->literal,1,NUMBER);
		return (Expr*)lit;
	}
	types[0] = STRING;
	if(parser->match(parser,types)){
		Literal* lit = malloc(sizeof(Literal));
		new_Literal(lit,previous(parser)->literal,3,STRING);
		return (Expr*)lit;
	}
	types[0] = LEFT_PAREN;
	if(parser->match(parser,types)){
		Expr* expr = expression(parser);
		Grouping*  group;
		consume(parser, RIGHT_PAREN, "Expect ')' after expression.\n");
		group = malloc(sizeof(Grouping));
		new_Grouping(group,expr);
		return (Expr*)group;
	}
    return NULL;
/*	 throw error(peek(parser), "Expect expression.");*/

}

Token* consume(Parser* parser, TokenType type, const char* message){
    if(check(parser,type)){
	   Token* adv = parser->advance(parser);
		return adv;
    }

/*	 otherwise throw error(peek(),message);*/
    return NULL;
}

void parse_error(Parser* parser, Token* token, const char* message){
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
	return &parser->tokens->tokens[parser->current];
}

Token* previous(Parser* parser){
	return &parser->tokens->tokens[parser->current-1];
}
