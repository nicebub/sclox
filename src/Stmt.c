
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
#include "Stmt.h"
void new_Block (Block* inObj,StmtArray* statementsparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptBlock;    
    inObj->super.vtable.delete = &delete_Block;
    inObj->statements = statementsparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Block",strlen("Block"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyBlock;
    footer->functions.delete = &delete_Block;
    inObj->super.id = addtoStmtCounter();
}
void delete_Block(void* arg){
    Block* stmt = (Block*)arg;

if(getReferenceCount(arg) ==1){
/*    delete(stmt->statements);*/
    stmt->statements=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptBlock(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitBlockStmt(visitor,arg);
}
void* copyBlock(void* inblk){
    Block* inObj,* blk;
    mem_footer* footer;
    blk = (Block*) inblk;
    inObj = new(OBJECTIVE,sizeof(Block));
    inObj->super.vtable.accept = &acceptBlock;
    inObj->super.vtable.delete = &delete_Block;
    inObj->statements = copy(blk->statements);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Block",strlen("Block"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 1;
    footer->functions.copy = &copyBlock;
    footer->functions.delete = &delete_Block;
    inObj->super.id = addtoStmtCounter();
    return inObj;

}
void new_Expression (Expression* inObj,Expr* expressionparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptExpression;    
    inObj->super.vtable.delete = &delete_Expression;
    inObj->expression = expressionparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Expression",strlen("Expression"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyExpression;
    footer->functions.delete = &delete_Expression;
    inObj->super.id = addtoStmtCounter();
}
void delete_Expression(void* arg){
    Expression* stmt = (Expression*)arg;

if(getReferenceCount(arg)==1){
/*    delete(stmt->expression);*/
    stmt->expression=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptExpression(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitExpressionStmt(visitor,arg);
}
void* copyExpression(void* expression){
    Expression* inObj,*expr;
    mem_footer* footer;
    expr = (Expression*)expression;
    inObj = new(OBJECTIVE,sizeof(Expression));
    inObj->super.vtable.accept = &acceptExpression;
    inObj->super.vtable.delete = &delete_Expression;
    inObj->expression = copy(expr->expression);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Expression",strlen("Expression"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyExpression;
    footer->functions.delete = &delete_Expression;
    inObj->super.id = addtoStmtCounter();
    return inObj;
}

void new_Function (Function* inObj,Token* nameparam,TokenArray* paramsparam,StmtArray* bodyparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptFunction;    
    inObj->super.vtable.delete = &delete_Function;
    inObj->name = nameparam;
    inObj->params = paramsparam;
    inObj->body = bodyparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Function",strlen("Function"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyFunction;
    footer->functions.delete = &delete_Function;
    inObj->super.id = addtoStmtCounter();
}
void delete_Function(void* arg){
    Function* stmt = (Function*)arg;

if(getReferenceCount(arg) ==1){
/*    delete(stmt->name);*/
    stmt->name=NULL;

/*    delete(stmt->params);*/
    stmt->params=NULL;

/*    delete(stmt->body);*/
    stmt->body=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptFunction(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitFunctionStmt(visitor,arg);
}
void* copyFunction(void* inFunction){
    mem_footer* footer;
    Function* function,*inObj;
    function = (Function*)inFunction;
    inObj = new(OBJECTIVE,sizeof(Function));
    inObj->super.vtable.accept = &acceptFunction;
    inObj->super.vtable.delete = &delete_Function;
    inObj->name = copy(function->name);
    inObj->params = copy(function->params);
    inObj->body = copy(function->body);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Function",strlen("Function"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyFunction;
    footer->functions.delete = &delete_Function;
    inObj->super.id = addtoStmtCounter();
    return inObj;
}

void new_If (If* inObj,Expr* conditionparam,Stmt* thenBranchparam,Stmt* elseBranchparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptIf;    
    inObj->super.vtable.delete = &delete_If;
    inObj->condition = conditionparam;
    inObj->thenBranch = thenBranchparam;
    inObj->elseBranch = elseBranchparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"If",strlen("If"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyIf;
    footer->functions.delete = &delete_If;
    inObj->super.id = addtoStmtCounter();
}
void delete_If(void* arg){
    If* stmt = (If*)arg;

if(getReferenceCount(arg) ==1){
/*    delete(stmt->condition);*/
    stmt->condition=NULL;

/*    delete(stmt->thenBranch);*/
    stmt->thenBranch=NULL;

/*    delete(stmt->elseBranch);*/
    stmt->elseBranch=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
       releaseReference(arg);
    }
}
Object* acceptIf(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitIfStmt(visitor,arg);
}
void* copyIf(void* iff){
    If* inObj, *inIf;
    mem_footer* footer;
    inObj = new(OBJECTIVE,sizeof(If));
    inIf = (If*) iff;
    inObj->super.vtable.accept = &acceptIf;
    inObj->super.vtable.delete = &delete_If;
    inObj->condition = copy(inIf->condition);
    inObj->thenBranch = copy(inIf->thenBranch);
    if(inIf->elseBranch)
		  inObj->elseBranch = copy(inIf->elseBranch);
    else
	   inObj->elseBranch = NULL;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"If",strlen("If"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyIf;
    footer->functions.delete = &delete_If;
    inObj->super.id = addtoStmtCounter();
    return inObj;
}

void new_Print (Print* inObj,Expr* expressionparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptPrint;    
    inObj->super.vtable.delete = &delete_Print;
    inObj->expression = expressionparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Print",strlen("Print"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyPrint;
    footer->functions.delete = &delete_Print;
    inObj->super.id = addtoStmtCounter();
}
void delete_Print(void* arg){
    Print* stmt = (Print*)arg;

if(getReferenceCount(arg) ==1){
/*    delete(stmt->expression);*/
    stmt->expression=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptPrint(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitPrintStmt(visitor,arg);
}
void* copyPrint(void* inPrint){
    Print* inObj,*print;
    mem_footer* footer;
    inObj= new(OBJECTIVE,sizeof(Print));
    print = (Print*)inPrint;
    inObj->super.vtable.accept = &acceptPrint;
    inObj->super.vtable.delete = &delete_Print;
    inObj->expression = copy(print->expression);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Print",strlen("Print"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyPrint;
    footer->functions.delete = &delete_Print;
    inObj->super.id = addtoStmtCounter();
    return inObj;
}

void new_Return (Return* inObj,Token* keywordparam,Expr* valueparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptReturn;    
    inObj->super.vtable.delete = &delete_Return;
    inObj->keyword = keywordparam;
    inObj->value = valueparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Return",strlen("Return"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyReturn;
    footer->functions.delete = &delete_Return;
    inObj->super.id = addtoStmtCounter();
}
void delete_Return(void* arg){
    Return* stmt = (Return*)arg;

    if(getReferenceCount(arg)==1){
/*    delete(stmt->keyword);*/
    stmt->keyword=NULL;

/*    delete(stmt->value);*/
    stmt->value=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptReturn(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitReturnStmt(visitor,arg);
}
void* copyReturn(void* inRet){
    Return* inObj,*ret;
    mem_footer* footer;
    inObj = new(OBJECTIVE,sizeof(Return));
    ret = (Return*) inRet;
    inObj->super.vtable.accept = &acceptReturn;
    inObj->super.vtable.delete = &delete_Return;
    inObj->keyword = copy(ret->keyword);
    inObj->value = copy(ret->value);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Return",strlen("Return"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyReturn;
    footer->functions.delete = &delete_Return;
    inObj->super.id = addtoStmtCounter();
    return inObj;

}

void new_Var (Var* inObj,Token* nameparam,Expr* initializerparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptVar;    
    inObj->super.vtable.delete = &delete_Var;
    inObj->name = nameparam;
    inObj->initializer = initializerparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Var",strlen("Var"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyVar;
    footer->functions.delete = &delete_Var;
    inObj->super.id = addtoStmtCounter();
}
void delete_Var(void* arg){
    Var* stmt = (Var*)arg;

if(getReferenceCount(arg) ==1){
/*    delete(stmt->name);*/
    stmt->name=NULL;

/*    delete(stmt->initializer);*/
    stmt->initializer=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
       releaseReference(arg);
    }
}
Object* acceptVar(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitVarStmt(visitor,arg);
}
void* copyVar(void* inVar){
    Var* inObj, *var;
    mem_footer* footer;
    inObj= new(OBJECTIVE,sizeof(Var));
    var = (Var*)inVar;
    inObj->super.vtable.accept = &acceptVar;
    inObj->super.vtable.delete = &delete_Var;
    inObj->name = copy(var->name);
    inObj->initializer = copy(var->initializer);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Var",strlen("Var"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyVar;
    footer->functions.delete = &delete_Var;
    inObj->super.id = addtoStmtCounter();
    return inObj;
}

void new_While (While* inObj,Expr* conditionparam,Stmt* bodyparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptWhile;    
    inObj->super.vtable.delete = &delete_While;
    inObj->condition = conditionparam;
    inObj->body = bodyparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"While",strlen("While"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyWhile;
    footer->functions.delete = &delete_While;
    inObj->super.id = addtoStmtCounter();
}
void delete_While(void* arg){
    While* stmt = (While*)arg;

if(getReferenceCount(arg) ==1){
/*    delete(stmt->condition);*/
    stmt->condition=NULL;

/*    delete(stmt->body);*/
    stmt->body=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
       releaseReference(arg);
    }
}
Object* acceptWhile(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitWhileStmt(visitor,arg);
}

void* copyWhile(void* data){
    While* inObj,*wle;
    mem_footer* footer;
    inObj = new(OBJECTIVE,sizeof(While));
    wle = (While*)data;
    inObj->super.vtable.accept = &acceptWhile;
    inObj->super.vtable.delete = &delete_While;
    inObj->condition = copy(wle->condition);
    inObj->body = copy(wle->body);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"While",strlen("While"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyWhile;
    footer->functions.delete = &delete_While;
    inObj->super.id = addtoStmtCounter();
    return inObj;
}

void delete_Stmt(void* inStmt){
    Stmt* stmt;
    stmt = (Stmt*) inStmt;
    if(stmt)
	   stmt->vtable.delete(stmt);
}
short int addtoStmtCounter(void){
     Stmtcounter += 1;
     return Stmtcounter;
    }
