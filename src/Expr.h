
#ifndef _EXPR_H
#define _EXPR_H

#include <string.h>
#include "Token.h"
#include "Object.h"
#ifndef _STMTARRAY
#define _STMTARRAY
    typedef struct _StmtArray StmtArray;
    extern void delete_StmtArray(StmtArray* array);
#endif
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(ExprArray* array);
#endif


typedef struct _Expr Expr;
typedef struct _ExprVisitor ExprVisitor;

typedef struct _Expr_vtable Expr_vtable;
struct _Expr_vtable {
    Object* (*accept)(Expr* arg,ExprVisitor* visitor);
    void (*delete_Expr)(Expr* arg);

};

typedef struct _ExprVisitor_vtable ExprVisitor_vtable;
struct _ExprVisitor_vtable {
	Object* (*visitAssignExpr)(ExprVisitor* visitor,Expr* expr);
	Object* (*visitBinaryExpr)(ExprVisitor* visitor,Expr* expr);
	Object* (*visitCallExpr)(ExprVisitor* visitor,Expr* expr);
	Object* (*visitGroupingExpr)(ExprVisitor* visitor,Expr* expr);
	Object* (*visitLiteralExpr)(ExprVisitor* visitor,Expr* expr);
	Object* (*visitLogicalExpr)(ExprVisitor* visitor,Expr* expr);
	Object* (*visitUnaryExpr)(ExprVisitor* visitor,Expr* expr);
	Object* (*visitVariableExpr)(ExprVisitor* visitor,Expr* expr);

};
struct _ExprVisitor {

    
    ExprVisitor_vtable vtable;
    
};
static short int Exprcounter=0;
    
struct _Expr {

                 char instanceOf[20];
                 short int id;
                 short int owner_references;
                 Expr_vtable vtable;

};
typedef struct _Assign    Assign   ;
struct _Assign    {
	Expr super;
	Token* name;
	Expr* value;

};
void new_Assign   (Assign   * inObj,Token* nameparam,Expr* valueparam);
void delete_Assign   (Expr* arg);
Object* acceptAssign(Expr* arg, ExprVisitor* visitor);
typedef struct _Binary  Binary ;
struct _Binary  {
	Expr super;
	Expr* left;
	Token* operator;
	Expr* right;

};
void new_Binary (Binary * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam);
void delete_Binary (Expr* arg);
Object* acceptBinary(Expr* arg, ExprVisitor* visitor);
typedef struct _Call  Call ;
struct _Call  {
	Expr super;
	Expr* callee;
	Token* paren;
	ExprArray* arguments;

};
void new_Call (Call * inObj,Expr* calleeparam,Token* parenparam,ExprArray* argumentsparam);
void delete_Call (Expr* arg);
Object* acceptCall(Expr* arg, ExprVisitor* visitor);
typedef struct _Grouping  Grouping ;
struct _Grouping  {
	Expr super;
	Expr* expression;

};
void new_Grouping (Grouping * inObj,Expr* expressionparam);
void delete_Grouping (Expr* arg);
Object* acceptGrouping(Expr* arg, ExprVisitor* visitor);
typedef struct _Literal  Literal ;
struct _Literal  {
	Expr super;
	Object* value;

};
void new_Literal (Literal * inObj,Object* valueparam);
void delete_Literal (Expr* arg);
Object* acceptLiteral(Expr* arg, ExprVisitor* visitor);
typedef struct _Logical   Logical  ;
struct _Logical   {
	Expr super;
	Expr* left;
	Token* operator;
	Expr* right;

};
void new_Logical  (Logical  * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam);
void delete_Logical  (Expr* arg);
Object* acceptLogical(Expr* arg, ExprVisitor* visitor);
typedef struct _Unary  Unary ;
struct _Unary  {
	Expr super;
	Token* operator;
	Expr* right;

};
void new_Unary (Unary * inObj,Token* operatorparam,Expr* rightparam);
void delete_Unary (Expr* arg);
Object* acceptUnary(Expr* arg, ExprVisitor* visitor);
typedef struct _Variable  Variable ;
struct _Variable  {
	Expr super;
	Token* name;

};
void new_Variable (Variable * inObj,Token* nameparam);
void delete_Variable (Expr* arg);
Object* acceptVariable(Expr* arg, ExprVisitor* visitor);
void delete_Expr(Expr* expr);
short int addtoExprCounter(void);


Expr* getExprReference(Expr* arg);
void removeExprReference(Expr* arg);
#endif
