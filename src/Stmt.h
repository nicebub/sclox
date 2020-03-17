
#ifndef _STMT_H
#define _STMT_H

#include <string.h>
#include "Token.h"
#include "Object.h"
#include "ReturnResult.h"
#include "Expr.h"

#ifndef _STMTARRAY
#define _STMTARRAY
	typedef struct _StmtArray StmtArray;
	extern void deleteStmtArray(StmtArray* array);
#endif

typedef struct _Stmt Stmt;
typedef struct _StmtVisitor StmtVisitor;

typedef struct _Stmt_vtable Stmt_vtable;
struct _Stmt_vtable {
    ReturnResult (*accept)(Stmt* arg,StmtVisitor* visitor);
    void (*delete_Stmt)(Stmt* arg);

};

typedef struct _StmtVisitor_vtable StmtVisitor_vtable;
struct _StmtVisitor_vtable {
	ReturnResult (*visitBlockStmt)(StmtVisitor* visitor,Stmt* stmt);
	ReturnResult (*visitExpressionStmt)(StmtVisitor* visitor,Stmt* stmt);
	ReturnResult (*visitPrintStmt)(StmtVisitor* visitor,Stmt* stmt);
	ReturnResult (*visitVarStmt)(StmtVisitor* visitor,Stmt* stmt);

};
struct _StmtVisitor {
	ExprVisitor expr;
	StmtVisitor_vtable vtable;

};
static short int Stmtcounter=0;
    
struct _Stmt {

                 char instanceOf[20];
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
ReturnResult acceptBlock(Stmt* arg, StmtVisitor* visitor);
typedef struct _Expression  Expression ;
struct _Expression  {
	Stmt super;
	Expr* expression;

};
void new_Expression (Expression * inObj,Expr* expressionparam);
void delete_Expression (Stmt* arg);
ReturnResult acceptExpression(Stmt* arg, StmtVisitor* visitor);
typedef struct _Print  Print ;
struct _Print  {
	Stmt super;
	Expr* expression;

};
void new_Print (Print * inObj,Expr* expressionparam);
void delete_Print (Stmt* arg);
ReturnResult acceptPrint(Stmt* arg, StmtVisitor* visitor);
typedef struct _Var  Var ;
struct _Var  {
	Stmt super;
	Token* name;
	Expr* initializer;

};
void new_Var (Var * inObj,Token* nameparam,Expr* initializerparam);
void delete_Var (Stmt* arg);
ReturnResult acceptVar(Stmt* arg, StmtVisitor* visitor);
void delete_Stmt(Stmt* expr);
short int addtoStmtCounter(void);


Stmt* getStmtReference(Stmt* arg);
void removeStmtReference(Stmt* arg);
#endif
