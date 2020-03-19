
#ifndef _STMT_H
#define _STMT_H

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
#include "Expr.h"


typedef struct _Stmt Stmt;
typedef struct _StmtVisitor StmtVisitor;

typedef struct _Stmt_vtable Stmt_vtable;
struct _Stmt_vtable {
    Object* (*accept)(Stmt* arg,StmtVisitor* visitor);
    void (*delete_Stmt)(Stmt* arg);

};

typedef struct _StmtVisitor_vtable StmtVisitor_vtable;
struct _StmtVisitor_vtable {
	Object* (*visitBlockStmt)(StmtVisitor* visitor,Stmt* stmt);
	Object* (*visitExpressionStmt)(StmtVisitor* visitor,Stmt* stmt);
	Object* (*visitIfStmt)(StmtVisitor* visitor,Stmt* stmt);
	Object* (*visitPrintStmt)(StmtVisitor* visitor,Stmt* stmt);
	Object* (*visitVarStmt)(StmtVisitor* visitor,Stmt* stmt);
	Object* (*visitWhileStmt)(StmtVisitor* visitor,Stmt* stmt);

};
struct _StmtVisitor {

    ExprVisitor expr;
    StmtVisitor_vtable vtable;
    
};
static short int Stmtcounter=0;
    
struct _Stmt {

                 char instanceOf[30];
                 short int id;
                 short int owner_references;
                 Stmt_vtable vtable;

};
typedef struct _Block  Block ;
struct _Block  {
	Stmt super;
	StmtArray* statements;

};
void new_Block (Block * inObj,StmtArray* statementsparam);
void delete_Block (Stmt* arg);
Object* acceptBlock(Stmt* arg, StmtVisitor* visitor);
typedef struct _Expression  Expression ;
struct _Expression  {
	Stmt super;
	Expr* expression;

};
void new_Expression (Expression * inObj,Expr* expressionparam);
void delete_Expression (Stmt* arg);
Object* acceptExpression(Stmt* arg, StmtVisitor* visitor);
typedef struct _If          If         ;
struct _If          {
	Stmt super;
	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;

};
void new_If         (If         * inObj,Expr* conditionparam,Stmt* thenBranchparam,Stmt* elseBranchparam);
void delete_If         (Stmt* arg);
Object* acceptIf(Stmt* arg, StmtVisitor* visitor);
typedef struct _Print  Print ;
struct _Print  {
	Stmt super;
	Expr* expression;

};
void new_Print (Print * inObj,Expr* expressionparam);
void delete_Print (Stmt* arg);
Object* acceptPrint(Stmt* arg, StmtVisitor* visitor);
typedef struct _Var  Var ;
struct _Var  {
	Stmt super;
	Token* name;
	Expr* initializer;

};
void new_Var (Var * inObj,Token* nameparam,Expr* initializerparam);
void delete_Var (Stmt* arg);
Object* acceptVar(Stmt* arg, StmtVisitor* visitor);
typedef struct _While       While      ;
struct _While       {
	Stmt super;
	Expr* condition;
	Stmt* body;

};
void new_While      (While      * inObj,Expr* conditionparam,Stmt* bodyparam);
void delete_While      (Stmt* arg);
Object* acceptWhile(Stmt* arg, StmtVisitor* visitor);
void delete_Stmt(Stmt* expr);
short int addtoStmtCounter(void);


Stmt* getStmtReference(Stmt* arg);
void removeStmtReference(Stmt* arg);
#endif
