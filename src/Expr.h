#ifndef _EXPR_H
#define _EXPR_H
#include "Token.h"
typedef struct _Expr Expr;
typedef struct _Visitor Visitor;
typedef struct _Expr_vtable Expr_vtable;
struct _Expr_vtable {
	void* (*accept)(Expr* arg,Visitor* visitor);
};
typedef struct _Visitor_vtable Visitor_vtable;
struct _Visitor_vtable {
	 void* (*visitBinaryExpr)(Visitor* visitor,Expr* expr);
	 void* (*visitGroupingExpr)(Visitor* visitor,Expr* expr);
	 void* (*visitLiteralExpr)(Visitor* visitor,Expr* expr);
	 void* (*visitUnaryExpr)(Visitor* visitor,Expr* expr);
};
struct _Visitor {
	Visitor_vtable vtable;
};
struct _Expr {
	Expr_vtable vtable;
};
typedef struct _Binary  Binary ;
struct _Binary  {
	Expr super;
	Expr* left;
	Token* operator;
	Expr* right;
};
void new_Binary (Binary * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam);
void* acceptBinary(Expr* arg, Visitor* visitor);
static void* visitBinaryExpr( Visitor* visitor,Expr* arg);
typedef struct _Grouping  Grouping ;
struct _Grouping  {
	Expr super;
	Expr* expression;
};
void new_Grouping (Grouping * inObj,Expr* expressionparam);
void* acceptGrouping(Expr* arg, Visitor* visitor);
static void* visitGroupingExpr( Visitor* visitor,Expr* arg);
typedef struct _Literal  Literal ;
struct _Literal  {
	Expr super;
	void* value;
	int valueType;
	TokenType type;
};
void new_Literal (Literal * inObj,void* valueparam,int valueTypeparam,TokenType typeparam);
void* acceptLiteral(Expr* arg, Visitor* visitor);
static void* visitLiteralExpr( Visitor* visitor,Expr* arg);
typedef struct _Unary  Unary ;
struct _Unary  {
	Expr super;
	Token* operator;
	Expr* right;
};
void new_Unary (Unary * inObj,Token* operatorparam,Expr* rightparam);
void* acceptUnary(Expr* arg, Visitor* visitor);
static void* visitUnaryExpr( Visitor* visitor,Expr* arg);
#endif
