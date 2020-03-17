
#include <string.h>
#include "Token.h"
#include "Object.h"
#include "ReturnResult.h"
#include "Expr.h"
#include "Stmt.h"



static ReturnResult visitBlockStmt( StmtVisitor* visitor,Stmt* arg);

static ReturnResult visitBlockStmt(StmtVisitor* visitor,Stmt* stmt){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Block (Block * inObj,StmtArray* statementsparam){
    inObj->super.vtable.accept = &acceptBlock;
    
    inObj->super.vtable.delete_Stmt = &delete_Block ;
	inObj->statements = statementsparam;
	strcpy((char*)&inObj->super.instanceOf,"Block");
	inObj->super.owner_references=1;
	inObj->super.id = addtoStmtCounter();
}
void delete_Block (Stmt* arg){
	Block * expr = (Block *)arg;

extern void deleteStmtArray(StmtArray* array);

if(expr->super.owner_references ==1){
    deleteStmtArray(expr->statements);
    expr->statements=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptBlock(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitBlockStmt(visitor,arg);
}

static ReturnResult visitExpressionStmt( StmtVisitor* visitor,Stmt* arg);

static ReturnResult visitExpressionStmt(StmtVisitor* visitor,Stmt* stmt){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Expression (Expression * inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptExpression;
    
    inObj->super.vtable.delete_Stmt = &delete_Expression ;
	inObj->expression = expressionparam;
	strcpy((char*)&inObj->super.instanceOf,"Expression");
	inObj->super.owner_references=1;
	inObj->super.id = addtoStmtCounter();
}
void delete_Expression (Stmt* arg){
	Expression * expr = (Expression *)arg;

if(expr->super.owner_references ==1){
    delete_Expr(expr->expression);
    expr->expression=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
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
	strcpy((char*)&inObj->super.instanceOf,"Print");
	inObj->super.owner_references=1;
	inObj->super.id = addtoStmtCounter();
}
void delete_Print (Stmt* arg){
	Print * expr = (Print *)arg;

if(expr->super.owner_references ==1){
    delete_Expr(expr->expression);
    expr->expression=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
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
	strcpy((char*)&inObj->super.instanceOf,"Var");
	inObj->super.owner_references=1;
	inObj->super.id = addtoStmtCounter();
}
void delete_Var (Stmt* arg){
	Var * expr = (Var *)arg;

if(expr->super.owner_references ==1){
    delete_Token(expr->name);
    expr->name=NULL;
    delete_Expr(expr->initializer);
    expr->initializer=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptVar(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitVarStmt(visitor,arg);
}
void delete_Stmt(Stmt* expr){
	expr->vtable.delete_Stmt(expr);
}
short int addtoStmtCounter(void){
     Stmtcounter += 1;
     return Stmtcounter;
    }

Stmt* getStmtReference(Stmt* arg){
    arg->owner_references += 1;
    return arg;
}
void removeStmtReference(Stmt* arg){
    if(arg->owner_references > 1)
        arg->owner_references -= 1;
}

