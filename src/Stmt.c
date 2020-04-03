
#include <string.h>
#include "Token.h"
#include "TokenArray.h"
#include "Object.h"
#ifndef _STMTARRAY
#define _STMTARRAY
    typedef struct _StmtArray StmtArray;
    extern void delete_StmtArray(void* array);
#endif
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(void* array);
#endif
#include "Stmt.h"
void new_Block (Block* inObj,StmtArray* statementsparam){
    inObj->super.vtable.accept = &acceptBlock;    
    inObj->super.vtable.delete = &delete_Block;
    inObj->super.vtable.copy = &copyBlock;
    inObj->statements = statementsparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Block",strlen("Block"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyBlock);
    setDestructor(inObj,&delete_Block);
    inObj->super.id = addtoStmtCounter();
}
void delete_Block(void* inArg){
    Block* arg, *stmt;
    arg = (Block*) inArg;
    stmt = (Block*) arg;

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

void* copyBlock(void* expr){
    Block* temp, *blockm;
    blockm = (Block*)expr;
    temp = new(OBJECTIVE, sizeof(Block));
new_Block(temp,blockm->statements);
temp->statements = getReference(temp->statements);
        

    setAllocated(temp,1);
    temp->super.id = blockm->super.id;
    return (void*)temp;
}
void new_Class (Class* inObj,Token* nameparam,Variable* superclassparam,StmtArray* methodsparam){
    inObj->super.vtable.accept = &acceptClass;    
    inObj->super.vtable.delete = &delete_Class;
    inObj->super.vtable.copy = &copyClass;
    inObj->name = nameparam;
    inObj->superclass = superclassparam;
    inObj->methods = methodsparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Class",strlen("Class"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyClass);
    setDestructor(inObj,&delete_Class);
    inObj->super.id = addtoStmtCounter();
}
void delete_Class(void* inArg){
    Class* arg, *stmt;
    arg = (Class*) inArg;
    stmt = (Class*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(stmt->name);*/
    stmt->name=NULL;

/*    delete(stmt->superclass);*/
    stmt->superclass=NULL;

/*    delete(stmt->methods);*/
    stmt->methods=NULL;
/*    delete(stmt);*/
    stmt=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptClass(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitClassStmt(visitor,arg);
}

void* copyClass(void* expr){
    Class* temp, *classm;
    classm = (Class*)expr;
    temp = new(OBJECTIVE, sizeof(Class));
new_Class(temp,classm->name,classm->superclass,classm->methods);
temp->name = getReference(temp->name);
        
temp->superclass = getReference(temp->superclass);
        
temp->methods = getReference(temp->methods);
        

    setAllocated(temp,1);
    temp->super.id = classm->super.id;
    return (void*)temp;
}
void new_Expression (Expression* inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptExpression;    
    inObj->super.vtable.delete = &delete_Expression;
    inObj->super.vtable.copy = &copyExpression;
    inObj->expression = expressionparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Expression",strlen("Expression"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyExpression);
    setDestructor(inObj,&delete_Expression);
    inObj->super.id = addtoStmtCounter();
}
void delete_Expression(void* inArg){
    Expression* arg, *stmt;
    arg = (Expression*) inArg;
    stmt = (Expression*) arg;

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
Object* acceptExpression(Stmt *arg, StmtVisitor* visitor){
    return visitor->vtable.visitExpressionStmt(visitor,arg);
}

void* copyExpression(void* expr){
    Expression* temp, *expressionm;
    expressionm = (Expression*)expr;
    temp = new(OBJECTIVE, sizeof(Expression));
new_Expression(temp,expressionm->expression);
temp->expression = getReference(temp->expression);
        

    setAllocated(temp,1);
    temp->super.id = expressionm->super.id;
    return (void*)temp;
}
void new_Function (Function* inObj,Token* nameparam,TokenArray* paramsparam,StmtArray* bodyparam){
    inObj->super.vtable.accept = &acceptFunction;    
    inObj->super.vtable.delete = &delete_Function;
    inObj->super.vtable.copy = &copyFunction;
    inObj->name = nameparam;
    inObj->params = paramsparam;
    inObj->body = bodyparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Function",strlen("Function"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyFunction);
    setDestructor(inObj,&delete_Function);
    inObj->super.id = addtoStmtCounter();
}
void delete_Function(void* inArg){
    Function* arg, *stmt;
    arg = (Function*) inArg;
    stmt = (Function*) arg;

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

void* copyFunction(void* expr){
    Function* temp, *functionm;
    functionm = (Function*)expr;
    temp = new(OBJECTIVE, sizeof(Function));
new_Function(temp,functionm->name,functionm->params,functionm->body);
temp->name = getReference(temp->name);
        
temp->params = getReference(temp->params);
        
temp->body = getReference(temp->body);
        

    setAllocated(temp,1);
    temp->super.id = functionm->super.id;
    return (void*)temp;
}
void new_If (If* inObj,Expr* conditionparam,Stmt* thenBranchparam,Stmt* elseBranchparam){
    inObj->super.vtable.accept = &acceptIf;    
    inObj->super.vtable.delete = &delete_If;
    inObj->super.vtable.copy = &copyIf;
    inObj->condition = conditionparam;
    inObj->thenBranch = thenBranchparam;
    inObj->elseBranch = elseBranchparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"If",strlen("If"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyIf);
    setDestructor(inObj,&delete_If);
    inObj->super.id = addtoStmtCounter();
}
void delete_If(void* inArg){
    If* arg, *stmt;
    arg = (If*) inArg;
    stmt = (If*) arg;

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

void* copyIf(void* expr){
    If* temp, *ifm;
    ifm = (If*)expr;
    temp = new(OBJECTIVE, sizeof(If));
new_If(temp,ifm->condition,ifm->thenBranch,ifm->elseBranch);
temp->condition = getReference(temp->condition);
        
temp->thenBranch = getReference(temp->thenBranch);
        
temp->elseBranch = getReference(temp->elseBranch);
        

    setAllocated(temp,1);
    temp->super.id = ifm->super.id;
    return (void*)temp;
}
void new_Print (Print* inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptPrint;    
    inObj->super.vtable.delete = &delete_Print;
    inObj->super.vtable.copy = &copyPrint;
    inObj->expression = expressionparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Print",strlen("Print"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyPrint);
    setDestructor(inObj,&delete_Print);
    inObj->super.id = addtoStmtCounter();
}
void delete_Print(void* inArg){
    Print* arg, *stmt;
    arg = (Print*) inArg;
    stmt = (Print*) arg;

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

void* copyPrint(void* expr){
    Print* temp, *printm;
    printm = (Print*)expr;
    temp = new(OBJECTIVE, sizeof(Print));
new_Print(temp,printm->expression);
temp->expression = getReference(temp->expression);
        

    setAllocated(temp,1);
    temp->super.id = printm->super.id;
    return (void*)temp;
}
void new_Return (Return* inObj,Token* keywordparam,Expr* valueparam){
    inObj->super.vtable.accept = &acceptReturn;    
    inObj->super.vtable.delete = &delete_Return;
    inObj->super.vtable.copy = &copyReturn;
    inObj->keyword = keywordparam;
    inObj->value = valueparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Return",strlen("Return"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyReturn);
    setDestructor(inObj,&delete_Return);
    inObj->super.id = addtoStmtCounter();
}
void delete_Return(void* inArg){
    Return* arg, *stmt;
    arg = (Return*) inArg;
    stmt = (Return*) arg;

if(getReferenceCount(arg) ==1){
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

void* copyReturn(void* expr){
    Return* temp, *returnm;
    returnm = (Return*)expr;
    temp = new(OBJECTIVE, sizeof(Return));
new_Return(temp,returnm->keyword,returnm->value);
temp->keyword = getReference(temp->keyword);
        
temp->value = getReference(temp->value);
        

    setAllocated(temp,1);
    temp->super.id = returnm->super.id;
    return (void*)temp;
}
void new_Var (Var* inObj,Token* nameparam,Expr* initializerparam){
    inObj->super.vtable.accept = &acceptVar;    
    inObj->super.vtable.delete = &delete_Var;
    inObj->super.vtable.copy = &copyVar;
    inObj->name = nameparam;
    inObj->initializer = initializerparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Var",strlen("Var"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyVar);
    setDestructor(inObj,&delete_Var);
    inObj->super.id = addtoStmtCounter();
}
void delete_Var(void* inArg){
    Var* arg, *stmt;
    arg = (Var*) inArg;
    stmt = (Var*) arg;

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

void* copyVar(void* expr){
    Var* temp, *varm;
    varm = (Var*)expr;
    temp = new(OBJECTIVE, sizeof(Var));
new_Var(temp,varm->name,varm->initializer);
temp->name = getReference(temp->name);
        
temp->initializer = getReference(temp->initializer);
        

    setAllocated(temp,1);
    temp->super.id = varm->super.id;
    return (void*)temp;
}
void new_While (While* inObj,Expr* conditionparam,Stmt* bodyparam){
    inObj->super.vtable.accept = &acceptWhile;    
    inObj->super.vtable.delete = &delete_While;
    inObj->super.vtable.copy = &copyWhile;
    inObj->condition = conditionparam;
    inObj->body = bodyparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"While",strlen("While"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyWhile);
    setDestructor(inObj,&delete_While);
    inObj->super.id = addtoStmtCounter();
}
void delete_While(void* inArg){
    While* arg, *stmt;
    arg = (While*) inArg;
    stmt = (While*) arg;

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

void* copyWhile(void* expr){
    While* temp, *whilem;
    whilem = (While*)expr;
    temp = new(OBJECTIVE, sizeof(While));
new_While(temp,whilem->condition,whilem->body);
temp->condition = getReference(temp->condition);
        
temp->body = getReference(temp->body);
        

    setAllocated(temp,1);
    temp->super.id = whilem->super.id;
    return (void*)temp;
}

void delete_Stmt(Stmt* stmt){
    if(stmt)        stmt->vtable.delete(stmt);
}
short int addtoStmtCounter(void){
     Stmtcounter += 1;
     return Stmtcounter;
    }
