
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
#include "Stmt.h"


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

if(expr->super.owner_references ==1){
    delete_StmtArray(expr->statements);
    expr->statements=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

Object* acceptBlock(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitBlockStmt(visitor,arg);
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

Object* acceptExpression(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitExpressionStmt(visitor,arg);
}
void new_If         (If         * inObj,Expr* conditionparam,Stmt* thenBranchparam,Stmt* elseBranchparam){
    inObj->super.vtable.accept = &acceptIf;
    
    inObj->super.vtable.delete_Stmt = &delete_If         ;
	inObj->condition = conditionparam;
	inObj->thenBranch = thenBranchparam;
	inObj->elseBranch = elseBranchparam;
	strcpy((char*)&inObj->super.instanceOf,"If");
	inObj->super.owner_references=1;
	inObj->super.id = addtoStmtCounter();
}
void delete_If         (Stmt* arg){
	If         * expr = (If         *)arg;

if(expr->super.owner_references ==1){
    delete_Expr(expr->condition);
    expr->condition=NULL;
    delete_Stmt(expr->thenBranch);
    expr->thenBranch=NULL;
    delete_Stmt(expr->elseBranch);
    expr->elseBranch=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

Object* acceptIf(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitIfStmt(visitor,arg);
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

Object* acceptPrint(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitPrintStmt(visitor,arg);
}
void new_Var (Var * inObj,Token* nameparam,Expr* initializerparam){
    inObj->super.vtable.accept = &acceptVar;
    
    inObj->super.vtable.delete_Stmt = &delete_Var ;
	inObj->name = getTokenReference(nameparam);
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

Object* acceptVar(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitVarStmt(visitor,arg);
}
void new_While      (While      * inObj,Expr* conditionparam,Stmt* bodyparam){
    inObj->super.vtable.accept = &acceptWhile;
    
    inObj->super.vtable.delete_Stmt = &delete_While      ;
	inObj->condition = conditionparam;
	inObj->body = bodyparam;
	strcpy((char*)&inObj->super.instanceOf,"While");
	inObj->super.owner_references=1;
	inObj->super.id = addtoStmtCounter();
}
void delete_While      (Stmt* arg){
	While      * expr = (While      *)arg;

if(expr->super.owner_references ==1){
    delete_Expr(expr->condition);
    expr->condition=NULL;
    delete_Stmt(expr->body);
    expr->body=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

Object* acceptWhile(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitWhileStmt(visitor,arg);
}
void delete_Stmt(Stmt* expr){
	if(expr)		expr->vtable.delete_Stmt(expr);
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

