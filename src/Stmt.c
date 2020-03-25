
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
    inObj->super.vtable.accept = &acceptBlock;
    inObj->super.vtable.delete = &delete_Block;
    inObj->statements = statementsparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Block",strlen("Block"));
    setCopyConstructor(inObj,&copyBlock);
    setDestructor(inObj,&delete_Block);
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
    Block* inObj;
    inObj = new(OBJECTIVE,sizeof(Block));
    new_Block(inObj,copy(((Block*)inblk)->statements));
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Block",strlen("Block"));
    setAllocated(inObj,1);
    return inObj;

}
void new_Expression (Expression* inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptExpression;
    inObj->super.vtable.delete = &delete_Expression;
    inObj->expression = expressionparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Expression",strlen("Expression"));
    setCopyConstructor(inObj,&copyExpression);
    setDestructor(inObj,&delete_Expression);
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
    expr = (Expression*)expression;
    inObj = new(OBJECTIVE,sizeof(Expression));
    new_Expression(inObj, copy(expr->expression));
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Expression",strlen("Expression"));
    setAllocated(inObj,1);
    return inObj;
}

void new_Function (Function* inObj,Token* nameparam,TokenArray* paramsparam,StmtArray* bodyparam){
    inObj->super.vtable.accept = &acceptFunction;
    inObj->super.vtable.delete = &delete_Function;
    inObj->name = nameparam;
    inObj->params = paramsparam;
    inObj->body = bodyparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Function",strlen("Function"));
    setCopyConstructor(inObj,&copyFunction);
    setDestructor(inObj,&delete_Function);
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
    Function* function,*inObj;
    function = (Function*)inFunction;
    inObj = new(OBJECTIVE,sizeof(Function));
    new_Function(inObj,copy(function->name),copy(function->params),copy(function->body));
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Function",strlen("Function"));
    setAllocated(inObj,1);
    return inObj;
}

void new_If (If* inObj,Expr* conditionparam,Stmt* thenBranchparam,Stmt* elseBranchparam){
    inObj->super.vtable.accept = &acceptIf;
    inObj->super.vtable.delete = &delete_If;
    inObj->condition = conditionparam;
    inObj->thenBranch = thenBranchparam;
    inObj->elseBranch = elseBranchparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"If",strlen("If"));
    setCopyConstructor(inObj,&copyIf);
    setDestructor(inObj,&delete_If);
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
    inObj = new(OBJECTIVE,sizeof(If));
    inIf = (If*) iff;
    new_If(inObj,copy(inIf->condition),copy(inIf->thenBranch),NULL);
    if(inIf->elseBranch)
		  inObj->elseBranch = copy(inIf->elseBranch);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"If",strlen("If"));
    setAllocated(inObj,1);
    return inObj;
}

void new_Print (Print* inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptPrint;
    inObj->super.vtable.delete = &delete_Print;
    inObj->expression = expressionparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Print",strlen("Print"));
    setCopyConstructor(inObj,&copyPrint);
    setDestructor(inObj,&delete_Print);
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
    inObj= new(OBJECTIVE,sizeof(Print));
    print = (Print*)inPrint;
    new_Print(inObj,copy(print->expression));
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Print",strlen("Print"));
    setAllocated(inObj,1);
    return inObj;
}

void new_Return (Return* inObj,Token* keywordparam,Expr* valueparam){
    inObj->super.vtable.accept = &acceptReturn;
    inObj->super.vtable.delete = &delete_Return;
    inObj->keyword = keywordparam;
    inObj->value = valueparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Return",strlen("Return"));
    setCopyConstructor(inObj,&copyReturn);
    setDestructor(inObj,&delete_Return);
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
    inObj = new(OBJECTIVE,sizeof(Return));
    ret = (Return*) inRet;
    new_Return(inObj,copy(ret->keyword),copy(ret->value));
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Return",strlen("Return"));
    setAllocated(inObj,1);
    return inObj;

}

void new_Var (Var* inObj,Token* nameparam,Expr* initializerparam){
    inObj->super.vtable.accept = &acceptVar;
    inObj->super.vtable.delete = &delete_Var;
    inObj->name = nameparam;
    inObj->initializer = initializerparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Var",strlen("Var"));
    setCopyConstructor(inObj,&copyVar);
    setDestructor(inObj,&delete_Var);
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
    inObj= new(OBJECTIVE,sizeof(Var));
    var = (Var*)inVar;
    new_Var(inObj,copy(var->name),copy(var->initializer));
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Var",strlen("Var"));
    setAllocated(inObj,1);
    return inObj;
}

void new_While (While* inObj,Expr* conditionparam,Stmt* bodyparam){
    inObj->super.vtable.accept = &acceptWhile;
    inObj->super.vtable.delete = &delete_While;
    inObj->condition = conditionparam;
    inObj->body = bodyparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"While",strlen("While"));
    setCopyConstructor(inObj,&copyWhile);
    setDestructor(inObj,&delete_While);
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
    inObj = new(OBJECTIVE,sizeof(While));
    wle = (While*)data;
    new_While(inObj,copy(wle->condition),copy(wle->body));
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"While",strlen("While"));
    setAllocated(inObj,1);
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
