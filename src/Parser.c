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
#include "Stmt.h"
#include "additions.h"
#include "TokenArray.h"
#include "StmtArray.h"
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

volatile StmtArray* parse(Parser* parser){
	StmtArray *statements;
	statements = malloc(sizeof(StmtArray));
	init_StmtArray(statements);

	while(!parser->isAtEnd(parser)){
		statements->addElementToArray(statements,declaration(parser));
	}
	if(statements)
		return (StmtArray*)statements;
	return NULL;
}

Stmt* declaration(Parser* parser){
	CEXCEPTION_T e;
	TokenType toks[] = { VAR, KNULL };
	Try {
		if(parser->match(parser,toks))
			return varDeclaration(parser);
		return statement(parser);
	}
	Catch(e) {
		synchronize(parser);
		return NULL;
	}
    return NULL;
}

Stmt* varDeclaration(Parser* parser){
	Token* name;
	Expr * initializer;
	Var* result;
	TokenType toks[] = { EQUAL, KNULL };
	initializer = NULL;
	name = consume(parser,IDENTIFIER, "Expect variable name.");

	if(parser->match(parser,toks))
		initializer = expression(parser);
	consume(parser,SEMICOLON, "Expect ';' after variable declaration.");
	result = malloc(sizeof(Var));
	new_Var(result,name,initializer);
	return (Stmt*)result;
}
Stmt* whileStatement(Parser* parser){
	Expr* condition;
	Stmt* body;
	While* w;
	consume(parser,LEFT_PAREN,"Expect '(' after 'while'.");
	condition = expression(parser);
	consume(parser,RIGHT_PAREN,"Expect ')' after condition.");
	body = statement(parser);
	w = malloc(sizeof(While));
	new_While(w,condition,body);
	return (Stmt*)w;
}

Stmt* forStatement(Parser* parser){
    Stmt* initializer, *body;
	Expr* condition, *increment;
	StmtArray * array;
	While* wstmt;
    TokenType p[] = {SEMICOLON, KNULL };
	consume(parser,LEFT_PAREN,"Expect '('  after 'for'.");
    if(parser->match(parser,p)){
    	initializer = NULL;
    }
    else{
    	p[0] = VAR;
    	if(parser->match(parser,p)){
    		initializer = varDeclaration(parser);
    	}
    	else{
    		initializer = expressionStatement(parser);
    	}
    }
    condition = NULL;
    if(!check(parser,SEMICOLON)){
    	condition = expression(parser);
    }
    consume(parser,SEMICOLON,"Expect ';' after loop condition.");
    increment = NULL;
    if(!check(parser,RIGHT_PAREN)){
    	increment = expression(parser);
    }
    consume(parser,RIGHT_PAREN,"Expect ')' after for clauses.");
    body = statement(parser);
    if(increment != NULL){
    	Block* block;
    	Expression* expression;
    	array = malloc(sizeof(StmtArray));
    	init_StmtArray(array);
    	expression = malloc(sizeof(Expression));
    	new_Expression(expression,increment);
    	array->addElementToArray(array,body);
    	array->addElementToArray(array,(Stmt*)expression);
    	block = malloc(sizeof(Block));
    	new_Block(block,array);
    	body = (Stmt*)block;
    }
    if(condition == NULL){
    	Literal* lit;
    	Object* obj;
	   int * num;
    	lit = malloc(sizeof(Literal));
    	obj = malloc(sizeof(Object));
	   num = malloc(sizeof(int));
	   *num = 1;
    	init_Object(obj,num,TRUE);
    	new_Literal(lit,obj);

    	condition = (Expr*)lit;
    }
    wstmt = malloc(sizeof(While));
    new_While(wstmt,condition,body);
    body = (Stmt*)wstmt;
    if(initializer != NULL){
    	Block* block;
    	StmtArray *array;
    	array = (StmtArray*)malloc(sizeof(StmtArray));
    	init_StmtArray(array);
    	array->addElementToArray(array,initializer);
    	array->addElementToArray(array,body);
    	block = malloc(sizeof(Block));
    	new_Block(block,array);
    	body = (Stmt*)block;
    }
	return body;
}
Stmt* statement(Parser* parser){
    TokenType p[] = {FOR, KNULL };
    if(parser->match(parser,p)){
    	return forStatement(parser);
    }
    p[0] = IF;
    if(parser->match(parser,p)){
    	return ifStatement(parser);
    }
    p[0] = PRINT;
	if(parser->match(parser,p)){
		return printStatement(parser);
	}
    p[0] = WHILE;
	if(parser->match(parser,p)){
		return whileStatement(parser);
	}
	p[0] = LEFT_BRACE;
	if(parser->match(parser,p)){
		Block* tempblock;
		tempblock = malloc(sizeof(Block));
		new_Block(tempblock,block(parser));
	    return (Stmt*)tempblock;
	}
	return expressionStatement(parser);

}
Stmt* ifStatement(Parser* parser){
	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;
	If* r;
	TokenType tok[] = { ELSE, KNULL };
	consume(parser,LEFT_PAREN, "Expect '(' after  'if'.");
	condition = expression(parser);
	consume(parser, RIGHT_PAREN,"Expect ')' after if condition.");
	thenBranch = statement(parser);
	elseBranch =  NULL;
	if(parser->match(parser,tok)){
		elseBranch = statement(parser);
	}
	r = malloc(sizeof(If));
	new_If((If*)r,condition,thenBranch,elseBranch);
	return (Stmt*)r;
}

StmtArray* block(Parser* parser){
	StmtArray* array;
/*	TokenType p[] = {RIGHT_BRACE, KNULL };*/
	array = malloc(sizeof(StmtArray));
	init_StmtArray(array);
	while(!check(parser,RIGHT_BRACE) && !parser->isAtEnd(parser)){
		array->addElementToArray(array,declaration(parser));
	}
	consume(parser,RIGHT_BRACE, "Expect '}' after block.");
	return array;
}


Stmt* printStatement(Parser* parser){
	Print* result;
	Expr* value = expression(parser);
	consume(parser,SEMICOLON, "Expect ';' after value.");
	result = malloc(sizeof(Print));
	new_Print(result,value);
	return (Stmt*)result;
}
Stmt* expressionStatement(Parser* parser){
	Expression* ex;
	Expr* expr = expression(parser);
	consume(parser,SEMICOLON, "Expect ';' after expression.");
	ex = malloc(sizeof(Expression));
	new_Expression(ex,expr);
	return (Stmt*)ex;
}
Expr* or(Parser* parser){
	Logical* expr;
	TokenType tok[] = { OR, KNULL };
	expr = NULL;
	expr = (Logical*)and(parser);
	while(parser->match(parser,tok)){
		Expr* right;
	    Logical* newy;
		Token* operator;
		operator = previous(parser);
		right = and(parser);
		newy = malloc(sizeof(Logical));
		new_Logical(newy,(Expr*)expr,operator,right);
	    expr = newy;
	}
	return (Expr*)expr;
}
Expr* and(Parser* parser){
	Logical* expr;
	TokenType tok[] = { AND, KNULL };
	expr = NULL;
	expr = (Logical*)equality(parser);
	while(parser->match(parser,tok)){
		Token* operator;
		Expr* right;
		Logical* newy;
		operator = previous(parser);
		right = equality(parser);
		newy = malloc(sizeof(Logical));
		new_Logical(newy,(Expr*)expr,operator,right);
	    expr = newy;
	}
	return (Expr*)expr;
}
Expr* assignment(Parser* parser){
	TokenType toks[] = {EQUAL, KNULL };
	Expr* expr;
	expr = or(parser);

	if(parser->match(parser,toks)){
		Token* equals;
		Expr* value;
		equals = previous(parser);
		value = assignment(parser);
		if(strcmp(expr->instanceOf,"Variable")==0){
			Token* name = ((Variable*)expr)->name;
			Assign* assign;
			assign = malloc(sizeof(Assign));
			new_Assign(assign,name,value);
			return (Expr*)assign;
		}
		parser->error(parser,equals,"Invalid assignment target.");
	}
	return expr;
}

Expr* expression(Parser* parser){
	return assignment(parser);
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
	types[0] = IDENTIFIER;
	if(parser->match(parser,types)){
		Variable* var = malloc(sizeof(Variable));
/*		obj = malloc(sizeof(Object));*/
/*		init_Object(obj,previous(parser)->literal,STRING);*/
		new_Variable(var,previous(parser));
		return (Expr*)var;
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
