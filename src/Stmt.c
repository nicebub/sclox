
#include "Token.h"
#include "Object.h"
#include "ReturnResult.h"
#include "Expr.h"
#include "Stmt.h"



static ReturnResult visitExpressionStmt( StmtVisitor* visitor,Stmt* arg);

static ReturnResult visitExpressionStmt(StmtVisitor* visitor,Stmt* stmt){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Expression (Expression * inObj,Expr* expressionparam){
	inObj->super.vtable.accept = &acceptExpression;
	inObj->super.vtable.delete_Stmt = &delete_Expression ;
	inObj->expression = expressionparam;
}
void delete_Expression (Stmt* arg){
	Expression * expr = (Expression *)arg;

    delete_Expr(expr->expression);
    expr->expression=NULL;

	free(expr);
	expr=NULL;
}

ReturnResult acceptExpression(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitExpressionStmt(visitor,arg);
}

static ReturnResult visitPrintStmt( StmtVisitor* visitor,Stmt* arg);

static ReturnResult visitPrintStmt(StmtVisitor* visitor,Stmt* stmt){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Print (Print * inObj,Expr* expressionparam){
	inObj->super.vtable.accept = &acceptPrint;
	inObj->super.vtable.delete_Stmt = &delete_Print ;
	inObj->expression = expressionparam;
}
void delete_Print (Stmt* arg){
	Print * expr = (Print *)arg;

    delete_Expr(expr->expression);
    expr->expression=NULL;

	free(expr);
	expr=NULL;
}

ReturnResult acceptPrint(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitPrintStmt(visitor,arg);
}

static ReturnResult visitVarStmt( StmtVisitor* visitor,Stmt* arg);

static ReturnResult visitVarStmt(StmtVisitor* visitor,Stmt* stmt){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Var (Var * inObj,Token* nameparam,Expr* initializerparam){
	inObj->super.vtable.accept = &acceptVar;
	inObj->super.vtable.delete_Stmt = &delete_Var ;
	inObj->name = nameparam;
	inObj->initializer = initializerparam;
}
void delete_Var (Stmt* arg){
	Var * expr = (Var *)arg;

    delete_Token(expr->name);
    expr->name=NULL;
    delete_Expr(expr->initializer);
    expr->initializer=NULL;

	free(expr);
	expr=NULL;
}

ReturnResult acceptVar(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitVarStmt(visitor,arg);
}
void delete_Stmt(Stmt* expr){
	expr->vtable.delete_Stmt(expr);
}