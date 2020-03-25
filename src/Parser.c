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
#include "ExprArray.h"
#include "Lox.h"
void init_Parser(Parser* parser, TokenArray* tokens, Lox* lox){
	parser->lox = lox;
	parser->match = &parser_match;
	parser->isAtEnd = &parser_isAtEnd;
	parser->peek = &parser_peek;
	parser->advance = &parser_advance;
	parser->error = &parse_error;
	parser->tokens = getReference(tokens);

	parser->current = 0;
}

volatile StmtArray* parse(Parser* parser){
	StmtArray *statements;
	statements = new(OBJECTIVE,sizeof(StmtArray));
	init_StmtArray(statements);

	while(!parser->isAtEnd(parser)){
		statements->addElementToArray(statements,(declaration(parser)));
	}
	if(statements)
		return (StmtArray*)statements;
	return NULL;
}

Stmt* declaration(Parser* parser){
    char* fun_str;
	CEXCEPTION_T e;
	TokenType toks[] = { FUN, KNULL };
    fun_str = new(RAW,sizeof(char)*(strlen("function") +1));
    strcpy(fun_str,"function");
	Try {
		if(parser->match(parser,toks))
			return (function(parser,fun_str));
		toks[0] = VAR;
		if(parser->match(parser,toks))
			return (varDeclaration(parser));
		return (statement(parser));
	}
	Catch(e) {
		synchronize(parser);
		return NULL;
	}
    return NULL;
}

Stmt* function(Parser* parser, char* kind){
	Function* func;
	TokenArray* parameters;
	StmtArray* body;
	Token* name;
	char* str_val;
	TokenType comma[] = { COMMA, KNULL };
	str_val = NULL;
    str_val = new(RAW,sizeof(char)*(strlen("Expect  name.")+strlen(kind)+1));
	asprintf(&str_val,"Expect %s name.",kind);
	name = (consume(parser,IDENTIFIER, str_val));
/*	delete(str_val);*/
	str_val = NULL;
	str_val = new(RAW,sizeof(char)*(strlen("Expect '(' after  name.")+strlen(kind)+1));
	asprintf(&str_val,"Expect '(' after %s name.",kind);
	consume(parser,LEFT_PAREN,str_val);
/*	delete(str_val);*/
	str_val = NULL;
	parameters = new(OBJECTIVE,sizeof(TokenArray));
	init_TokenArray(parameters);
	if(!check(parser,RIGHT_PAREN)){
		do{
			if(parameters->size >= 255){
				parser->error(parser,parser->peek(parser),"Cannot have more than 255 parameters");
			}
			parameters->addElementToArray(parameters,(consume(parser,IDENTIFIER,"Expect parameter name.")));
		}while(parser->match(parser,comma));
	}
	consume(parser,RIGHT_PAREN,"Expect ')' after parameters.");
    str_val = new(RAW,sizeof(char)*(strlen("Expect '{' before %s body.")+1));
	asprintf(&str_val,"Expect '{' before %s body.",kind);
	consume(parser,LEFT_BRACE, str_val);
/*	delete(str_val);*/
	str_val = NULL;
	body = (block(parser));
	func = new(OBJECTIVE,sizeof(Function));
	new_Function(func,name,parameters,body);
    setAllocated(func,1);
	return (Stmt*)func;
}

Stmt* varDeclaration(Parser* parser){
	Token* name;
	Expr * initializer;
	Var* result;
	TokenType toks[] = { EQUAL, KNULL };
	initializer = NULL;
	name =(consume(parser,IDENTIFIER, "Expect variable name."));

	if(parser->match(parser,toks))
		initializer = expression(parser);
	consume(parser,SEMICOLON, "Expect ';' after variable declaration.");
	result = new(OBJECTIVE,sizeof(Var));
	new_Var(result,name,initializer);
	return (Stmt*)result;
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
    p[0] = RETURN;
	if(parser->match(parser,p)){
		return returnStatement(parser);
	}
    p[0] = WHILE;
	if(parser->match(parser,p)){
		return whileStatement(parser);
	}
	p[0] = LEFT_BRACE;
	if(parser->match(parser,p)){
		Block* tempblock;
		tempblock = new(OBJECTIVE,sizeof(Block));
		new_Block(tempblock,block(parser));
	    return (Stmt*)tempblock;
	}
	return expressionStatement(parser);

}

Stmt* whileStatement(Parser* parser){
	Expr* condition;
	Stmt* body;
	While* w;
	consume(parser,LEFT_PAREN,"Expect '(' after 'while'.");
	condition = expression(parser);
	consume(parser,RIGHT_PAREN,"Expect ')' after condition.");
	body = statement(parser);
	w = new(OBJECTIVE,sizeof(While));
	new_While(w,condition,body);
	return (Stmt*)w;
}

Stmt* returnStatement(Parser* parser){
	Token* keyword;
	Expr* value;
	Return* ret;
	keyword = previous(parser);
	value = NULL;
	if(!check(parser,SEMICOLON)){
		value = expression(parser);
	}
	consume(parser,SEMICOLON,"Expect ';' after return value.");
	ret = new(OBJECTIVE,sizeof(Return));
	new_Return(ret,keyword,value);
	return (Stmt*)ret;
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
    		initializer = (varDeclaration(parser));
    	}
    	else{
    		initializer = (expressionStatement(parser));
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
    	array = new(OBJECTIVE,sizeof(StmtArray));
    	init_StmtArray(array);
    	expression = new(OBJECTIVE,sizeof(Expression));
    	new_Expression(expression,increment);
    	array->addElementToArray(array,body);
    	array->addElementToArray(array,(Stmt*)expression);
    	block = new(OBJECTIVE,sizeof(Block));
    	new_Block(block,array);
    	body = (Stmt*)block;
    }
    if(condition == NULL){
    	Literal* lit;
    	Object* obj;
	   int * num;
    	lit = new(OBJECTIVE,sizeof(Literal));
    	obj = new(OBJECTIVE,sizeof(Object));
	   num = new(RAW,sizeof(int));
	   *num = 1;
    	init_Object(obj,num,TRUE);
    	new_Literal(lit,obj);
/*    	delete(num);*/
    	num=NULL;

    	condition = (Expr*)lit;
    }
    wstmt = new(OBJECTIVE,sizeof(While));
    new_While(wstmt,condition,body);
    body = (Stmt*)wstmt;
    if(initializer != NULL){
    	Block* block;
    	StmtArray *array;
    	array = (StmtArray*)new(OBJECTIVE,sizeof(StmtArray));
    	init_StmtArray(array);
    	array->addElementToArray(array,initializer);
    	array->addElementToArray(array,body);
    	block = new(OBJECTIVE,sizeof(Block));
    	new_Block(block,array);
    	body = (Stmt*)block;
    }
	return body;
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
	r = new(OBJECTIVE,sizeof(If));
	new_If((If*)r,condition,thenBranch,elseBranch);
	return (Stmt*)r;
}

StmtArray* block(Parser* parser){
	StmtArray* array;
	array = new(OBJECTIVE,sizeof(StmtArray));
	init_StmtArray(array);
	while(!check(parser,RIGHT_BRACE) && !parser->isAtEnd(parser)){
		array->addElementToArray(array,(declaration(parser)));
	}
	consume(parser,RIGHT_BRACE, "Expect '}' after block.");
	return array;
}


Stmt* printStatement(Parser* parser){
	Print* result;
	Expr* value = expression(parser);
	consume(parser,SEMICOLON, "Expect ';' after value.");
	result = new(OBJECTIVE,sizeof(Print));
	new_Print(result,value);
	return (Stmt*)result;
}
Stmt* expressionStatement(Parser* parser){
	Expression* ex;
	Expr* expr = expression(parser);
	consume(parser,SEMICOLON, "Expect ';' after expression.");
	ex = new(OBJECTIVE,sizeof(Expression));
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
		newy = new(OBJECTIVE,sizeof(Logical));
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
		newy = new(OBJECTIVE,sizeof(Logical));
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
			assign = new(OBJECTIVE,sizeof(Assign));
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
		Binary* binary = new(OBJECTIVE,sizeof(Binary));
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
		Binary* binary = new(OBJECTIVE,sizeof(Binary));
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
		Binary* binary = new(OBJECTIVE,sizeof(Binary));
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
		Binary* binary = new(OBJECTIVE,sizeof(Binary));
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
		Unary* un = new(OBJECTIVE,sizeof(Unary));
		new_Unary(un,operator,right);
		return (Expr*)un;
	}
	return 	call(parser);
}

Expr* call(Parser* parser){
	Expr* expr;
	TokenType toks[] = { LEFT_PAREN, KNULL };
	expr = (Expr*)primary(parser);
	while(1){
		if(parser->match(parser,toks)){
			expr = finishCall(parser,expr);
		}
		else{
			break;
		}
	}
	return expr;
}
Expr* finishCall(Parser* parser, Expr* callee){
	ExprArray * arguments;
	Token* paren;
	Call* caller;
	TokenType comma[] = { COMMA, KNULL };
	arguments = new(OBJECTIVE,sizeof(ExprArray));
	init_ExprArray(arguments);
	if(!check(parser,RIGHT_PAREN)) {
		do {
			if(arguments->size >= 255) {
				parser->error(parser,parser->peek(parser),"Cannot have more than 255 arguments.");
			}
		    arguments->addElementToArray(arguments,(expression(parser)));

		}while (parser->match(parser,comma));
	}
	paren = consume(parser,RIGHT_PAREN, "Expect ')' after arguments.");
	caller = new(OBJECTIVE,sizeof(Call));
	new_Call(caller,callee,paren,arguments);
	return (Expr*)caller;
}

Expr* primary(Parser* parser){
	Object * obj;
	TokenType types[] = { FALSE, KNULL };
	if(parser->match(parser,types)){
		double* val;
		Literal* lit = new(OBJECTIVE,sizeof(Literal));
		obj = new(OBJECTIVE,sizeof(Object));
	    val = new(RAW,sizeof(double));
	    *val = 0;
		init_Object(obj,val,FALSE);
		new_Literal(lit,obj);
/*		delete(val);*/
		val = NULL;
		return (Expr*)lit;
	}
	types[0] = TRUE;
	if(parser->match(parser,types)){
	    double* val;
		Literal* lit = new(OBJECTIVE,sizeof(Literal));
		obj = new(OBJECTIVE,sizeof(Object));
	     val = new(RAW,sizeof(double));
	     *val = 1;
	     init_Object(obj,val,TRUE);
	     new_Literal(lit,obj);
/*	     delete(val);*/
	     val = NULL;
		return (Expr*)lit;
	}
	types[0] = NIL;
	if(parser->match(parser,types)){
		Literal* lit = new(OBJECTIVE,sizeof(Literal));
		obj = new(OBJECTIVE,sizeof(Object));
		init_Object(obj,"nil",NIL);
		new_Literal(lit,obj);
		return (Expr*)lit;
	}
	types[0] = NUMBER;
	if(parser->match(parser,types)){
		Literal* lit = new(OBJECTIVE,sizeof(Literal));
		new_Literal(lit,previous(parser)->literal);
		return (Expr*)lit;
	}
	types[0] = STRING;
	if(parser->match(parser,types)){
		Literal* lit = new(OBJECTIVE,sizeof(Literal));
		new_Literal(lit,previous(parser)->literal);
		return (Expr*)lit;
	}
	types[0] = IDENTIFIER;
	if(parser->match(parser,types)){
		Variable* var = new(OBJECTIVE,sizeof(Variable));
		new_Variable(var,previous(parser));
		return (Expr*)var;
	}

	types[0] = LEFT_PAREN;
	if(parser->match(parser,types)){
		Expr* expr = expression(parser);
		Grouping*  group;
		consume(parser, RIGHT_PAREN, "Expect ')' after expression.");
		group = new(OBJECTIVE,sizeof(Grouping));
		new_Grouping(group,expr);
		return (Expr*)group;
	}
	Throw(parser->error(parser,parser->peek(parser),"Expect expression."));
	/* shouldn't reach this return statement */
    return NULL;

}

Token* consume(Parser* parser, TokenType type, const char* message){
    if(check(parser,type))
		return parser->advance(parser);

    Throw(parser->error(parser,parser->peek(parser),message));
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
		if(previous(parser)->super.type == SEMICOLON)
			return;
		switch(parser->peek(parser)->super.type){
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
	return parser->peek(parser)->super.type == type;
}

Token* parser_advance(Parser* parser){
	if(!parser->isAtEnd(parser))
		parser->current++;
	return previous(parser);
}

int parser_isAtEnd(Parser* parser){
	return parser->peek(parser)->super.type == EEOF;
}

Token* parser_peek(Parser* parser){
	return getTokeninArrayAt(parser->tokens,parser->current);
}

Token* previous(Parser* parser){
	return getTokeninArrayAt(parser->tokens,parser->current-1);
}


void delete_parser(Parser* parser){
	if(parser){
		/*delete_TokenArray(parser->tokens);*/
		parser->tokens = NULL;
	}
}
