
#ifndef _EXPR_H
#define _EXPR_H

#include <string.h>
#include "Token.h"
#include "Object.h"
#include "ReturnResult.h"
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
    ReturnResult (*accept)(Expr* arg,ExprVisitor* visitor);
    void (*delete_Expr)(Expr* arg);

};

typedef struct _ExprVisitor_vtable ExprVisitor_vtable;
struct _ExprVisitor_vtable {
	ReturnResult (*visitAssignExpr)(ExprVisitor* visitor,Expr* expr);
	ReturnResult (*visitBinaryExpr)(ExprVisitor* visitor,Expr* expr);
	ReturnResult (*visitCallExpr)(ExprVisitor* visitor,Expr* expr);
	ReturnResult (*visitGroupingExpr)(ExprVisitor* visitor,Expr* expr);
	ReturnResult (*visitLiteralExpr)(ExprVisitor* visitor,Expr* expr);
	ReturnResult (*visitLogicalExpr)(ExprVisitor* visitor,Expr* expr);
	ReturnResult (*visitUnaryExpr)(ExprVisitor* visitor,Expr* expr);
	ReturnResult (*visitVariableExpr)(ExprVisitor* visitor,Expr* expr);

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
ReturnResult acceptAssign(Expr* arg, ExprVisitor* visitor);
typedef struct _Binary  Binary ;
struct _Binary  {
	Expr super;
	Expr* left;
	Token* operator;
	Expr* right;

};
void new_Binary (Binary * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam);
void delete_Binary (Expr* arg);
ReturnResult acceptBinary(Expr* arg, ExprVisitor* visitor);
typedef struct _Call  Call ;
struct _Call  {
	Expr super;
	Expr* callee;
	Token* paren;
	ExprArray* arguments;

};
void new_Call (Call * inObj,Expr* calleeparam,Token* parenparam,ExprArray* argumentsparam);
void delete_Call (Expr* arg);
ReturnResult acceptCall(Expr* arg, ExprVisitor* visitor);
typedef struct _Grouping  Grouping ;
struct _Grouping  {
	Expr super;
	Expr* expression;

};
void new_Grouping (Grouping * inObj,Expr* expressionparam);
void delete_Grouping (Expr* arg);
ReturnResult acceptGrouping(Expr* arg, ExprVisitor* visitor);
typedef struct _Literal  Literal ;
struct _Literal  {
	Expr super;
	Object* value;

};
void new_Literal (Literal * inObj,Object* valueparam);
void delete_Literal (Expr* arg);
ReturnResult acceptLiteral(Expr* arg, ExprVisitor* visitor);
typedef struct _Logical   Logical  ;
struct _Logical   {
	Expr super;
	Expr* left;
	Token* operator;
	Expr* right;

};
void new_Logical  (Logical  * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam);
void delete_Logical  (Expr* arg);
ReturnResult acceptLogical(Expr* arg, ExprVisitor* visitor);
typedef struct _Unary  Unary ;
struct _Unary  {
	Expr super;
	Token* operator;
	Expr* right;

};
void new_Unary (Unary * inObj,Token* operatorparam,Expr* rightparam);
void delete_Unary (Expr* arg);
ReturnResult acceptUnary(Expr* arg, ExprVisitor* visitor);
typedef struct _Variable  Variable ;
struct _Variable  {
	Expr super;
	Token* name;

};
void new_Variable (Variable * inObj,Token* nameparam);
void delete_Variable (Expr* arg);
ReturnResult acceptVariable(Expr* arg, ExprVisitor* visitor);
void delete_Expr(Expr* expr);
short int addtoExprCounter(void);


Expr* getExprReference(Expr* arg);
void removeExprReference(Expr* arg);
#endif
