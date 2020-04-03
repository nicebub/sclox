
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
#include "Expr.h"
void new_Assign (Assign* inObj,Token* nameparam,Expr* valueparam){
    inObj->super.vtable.accept = &acceptAssign;    
    inObj->super.vtable.delete = &delete_Assign;
    inObj->super.vtable.copy = &copyAssign;
    inObj->name = nameparam;
    inObj->value = valueparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Assign",strlen("Assign"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyAssign);
    setDestructor(inObj,&delete_Assign);
    inObj->super.id = addtoExprCounter();
}
void delete_Assign(void* inArg){
    Assign* arg, *expr;
    arg = (Assign*) inArg;
    expr = (Assign*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->name);*/
    expr->name=NULL;

/*    delete(expr->value);*/
    expr->value=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptAssign(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitAssignExpr(visitor,arg);
}

void* copyAssign(void* expr){
    Assign* temp, *assignm;
    assignm = (Assign*)expr;
    temp = new(OBJECTIVE, sizeof(Assign));
new_Assign(temp,assignm->name,assignm->value);
temp->name = getReference(temp->name);
        
temp->value = getReference(temp->value);
        

    setAllocated(temp,1);
    temp->super.id = assignm->super.id;
    return (void*)temp;
}
void new_Binary (Binary* inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptBinary;    
    inObj->super.vtable.delete = &delete_Binary;
    inObj->super.vtable.copy = &copyBinary;
    inObj->left = leftparam;
    inObj->operator = operatorparam;
    inObj->right = rightparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Binary",strlen("Binary"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyBinary);
    setDestructor(inObj,&delete_Binary);
    inObj->super.id = addtoExprCounter();
}
void delete_Binary(void* inArg){
    Binary* arg, *expr;
    arg = (Binary*) inArg;
    expr = (Binary*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->left);*/
    expr->left=NULL;

/*    delete(expr->operator);*/
    expr->operator=NULL;

/*    delete(expr->right);*/
    expr->right=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptBinary(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitBinaryExpr(visitor,arg);
}

void* copyBinary(void* expr){
    Binary* temp, *binarym;
    binarym = (Binary*)expr;
    temp = new(OBJECTIVE, sizeof(Binary));
new_Binary(temp,binarym->left,binarym->operator,binarym->right);
temp->left = getReference(temp->left);
        
temp->operator = getReference(temp->operator);
        
temp->right = getReference(temp->right);
        

    setAllocated(temp,1);
    temp->super.id = binarym->super.id;
    return (void*)temp;
}
void new_Call (Call* inObj,Expr* calleeparam,Token* parenparam,ExprArray* argumentsparam){
    inObj->super.vtable.accept = &acceptCall;    
    inObj->super.vtable.delete = &delete_Call;
    inObj->super.vtable.copy = &copyCall;
    inObj->callee = calleeparam;
    inObj->paren = parenparam;
    inObj->arguments = argumentsparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Call",strlen("Call"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyCall);
    setDestructor(inObj,&delete_Call);
    inObj->super.id = addtoExprCounter();
}
void delete_Call(void* inArg){
    Call* arg, *expr;
    arg = (Call*) inArg;
    expr = (Call*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->callee);*/
    expr->callee=NULL;

/*    delete(expr->paren);*/
    expr->paren=NULL;

/*    delete(expr->arguments);*/
    expr->arguments=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptCall(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitCallExpr(visitor,arg);
}

void* copyCall(void* expr){
    Call* temp, *callm;
    callm = (Call*)expr;
    temp = new(OBJECTIVE, sizeof(Call));
new_Call(temp,callm->callee,callm->paren,callm->arguments);
temp->callee = getReference(temp->callee);
        
temp->paren = getReference(temp->paren);
        
temp->arguments = getReference(temp->arguments);
        

    setAllocated(temp,1);
    temp->super.id = callm->super.id;
    return (void*)temp;
}
void new_Get (Get* inObj,Expr* objectparam,Token* nameparam){
    inObj->super.vtable.accept = &acceptGet;    
    inObj->super.vtable.delete = &delete_Get;
    inObj->super.vtable.copy = &copyGet;
    inObj->object = objectparam;
    inObj->name = nameparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Get",strlen("Get"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyGet);
    setDestructor(inObj,&delete_Get);
    inObj->super.id = addtoExprCounter();
}
void delete_Get(void* inArg){
    Get* arg, *expr;
    arg = (Get*) inArg;
    expr = (Get*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->object);*/
    expr->object=NULL;

/*    delete(expr->name);*/
    expr->name=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptGet(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitGetExpr(visitor,arg);
}

void* copyGet(void* expr){
    Get* temp, *getm;
    getm = (Get*)expr;
    temp = new(OBJECTIVE, sizeof(Get));
new_Get(temp,getm->object,getm->name);
temp->object = getReference(temp->object);
        
temp->name = getReference(temp->name);
        

    setAllocated(temp,1);
    temp->super.id = getm->super.id;
    return (void*)temp;
}
void new_Grouping (Grouping* inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptGrouping;    
    inObj->super.vtable.delete = &delete_Grouping;
    inObj->super.vtable.copy = &copyGrouping;
    inObj->expression = expressionparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Grouping",strlen("Grouping"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyGrouping);
    setDestructor(inObj,&delete_Grouping);
    inObj->super.id = addtoExprCounter();
}
void delete_Grouping(void* inArg){
    Grouping* arg, *expr;
    arg = (Grouping*) inArg;
    expr = (Grouping*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->expression);*/
    expr->expression=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptGrouping(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitGroupingExpr(visitor,arg);
}

void* copyGrouping(void* expr){
    Grouping* temp, *groupingm;
    groupingm = (Grouping*)expr;
    temp = new(OBJECTIVE, sizeof(Grouping));
new_Grouping(temp,groupingm->expression);
temp->expression = getReference(temp->expression);
        

    setAllocated(temp,1);
    temp->super.id = groupingm->super.id;
    return (void*)temp;
}
void new_Literal (Literal* inObj,Object* valueparam){
    inObj->super.vtable.accept = &acceptLiteral;    
    inObj->super.vtable.delete = &delete_Literal;
    inObj->super.vtable.copy = &copyLiteral;
    inObj->value = valueparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Literal",strlen("Literal"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyLiteral);
    setDestructor(inObj,&delete_Literal);
    inObj->super.id = addtoExprCounter();
}
void delete_Literal(void* inArg){
    Literal* arg, *expr;
    arg = (Literal*) inArg;
    expr = (Literal*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->value);*/
    expr->value=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptLiteral(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitLiteralExpr(visitor,arg);
}

void* copyLiteral(void* expr){
    Literal* temp, *literalm;
    literalm = (Literal*)expr;
    temp = new(OBJECTIVE, sizeof(Literal));
new_Literal(temp,literalm->value);
temp->value = getReference(temp->value);
        

    setAllocated(temp,1);
    temp->super.id = literalm->super.id;
    return (void*)temp;
}
void new_Logical (Logical* inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptLogical;    
    inObj->super.vtable.delete = &delete_Logical;
    inObj->super.vtable.copy = &copyLogical;
    inObj->left = leftparam;
    inObj->operator = operatorparam;
    inObj->right = rightparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Logical",strlen("Logical"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyLogical);
    setDestructor(inObj,&delete_Logical);
    inObj->super.id = addtoExprCounter();
}
void delete_Logical(void* inArg){
    Logical* arg, *expr;
    arg = (Logical*) inArg;
    expr = (Logical*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->left);*/
    expr->left=NULL;

/*    delete(expr->operator);*/
    expr->operator=NULL;

/*    delete(expr->right);*/
    expr->right=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptLogical(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitLogicalExpr(visitor,arg);
}

void* copyLogical(void* expr){
    Logical* temp, *logicalm;
    logicalm = (Logical*)expr;
    temp = new(OBJECTIVE, sizeof(Logical));
new_Logical(temp,logicalm->left,logicalm->operator,logicalm->right);
temp->left = getReference(temp->left);
        
temp->operator = getReference(temp->operator);
        
temp->right = getReference(temp->right);
        

    setAllocated(temp,1);
    temp->super.id = logicalm->super.id;
    return (void*)temp;
}
void new_Set (Set* inObj,Expr* objectparam,Token* nameparam,Expr* valueparam){
    inObj->super.vtable.accept = &acceptSet;    
    inObj->super.vtable.delete = &delete_Set;
    inObj->super.vtable.copy = &copySet;
    inObj->object = objectparam;
    inObj->name = nameparam;
    inObj->value = valueparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Set",strlen("Set"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copySet);
    setDestructor(inObj,&delete_Set);
    inObj->super.id = addtoExprCounter();
}
void delete_Set(void* inArg){
    Set* arg, *expr;
    arg = (Set*) inArg;
    expr = (Set*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->object);*/
    expr->object=NULL;

/*    delete(expr->name);*/
    expr->name=NULL;

/*    delete(expr->value);*/
    expr->value=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptSet(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitSetExpr(visitor,arg);
}

void* copySet(void* expr){
    Set* temp, *setm;
    setm = (Set*)expr;
    temp = new(OBJECTIVE, sizeof(Set));
new_Set(temp,setm->object,setm->name,setm->value);
temp->object = getReference(temp->object);
        
temp->name = getReference(temp->name);
        
temp->value = getReference(temp->value);
        

    setAllocated(temp,1);
    temp->super.id = setm->super.id;
    return (void*)temp;
}
void new_Super (Super* inObj,Token* keywordparam,Token* methodparam){
    inObj->super.vtable.accept = &acceptSuper;    
    inObj->super.vtable.delete = &delete_Super;
    inObj->super.vtable.copy = &copySuper;
    inObj->keyword = keywordparam;
    inObj->method = methodparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Super",strlen("Super"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copySuper);
    setDestructor(inObj,&delete_Super);
    inObj->super.id = addtoExprCounter();
}
void delete_Super(void* inArg){
    Super* arg, *expr;
    arg = (Super*) inArg;
    expr = (Super*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->keyword);*/
    expr->keyword=NULL;

/*    delete(expr->method);*/
    expr->method=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptSuper(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitSuperExpr(visitor,arg);
}

void* copySuper(void* expr){
    Super* temp, *superm;
    superm = (Super*)expr;
    temp = new(OBJECTIVE, sizeof(Super));
new_Super(temp,superm->keyword,superm->method);
temp->keyword = getReference(temp->keyword);
        
temp->method = getReference(temp->method);
        

    setAllocated(temp,1);
    temp->super.id = superm->super.id;
    return (void*)temp;
}
void new_This (This* inObj,Token* keywordparam){
    inObj->super.vtable.accept = &acceptThis;    
    inObj->super.vtable.delete = &delete_This;
    inObj->super.vtable.copy = &copyThis;
    inObj->keyword = keywordparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"This",strlen("This"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyThis);
    setDestructor(inObj,&delete_This);
    inObj->super.id = addtoExprCounter();
}
void delete_This(void* inArg){
    This* arg, *expr;
    arg = (This*) inArg;
    expr = (This*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->keyword);*/
    expr->keyword=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptThis(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitThisExpr(visitor,arg);
}

void* copyThis(void* expr){
    This* temp, *thism;
    thism = (This*)expr;
    temp = new(OBJECTIVE, sizeof(This));
new_This(temp,thism->keyword);
temp->keyword = getReference(temp->keyword);
        

    setAllocated(temp,1);
    temp->super.id = thism->super.id;
    return (void*)temp;
}
void new_Unary (Unary* inObj,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptUnary;    
    inObj->super.vtable.delete = &delete_Unary;
    inObj->super.vtable.copy = &copyUnary;
    inObj->operator = operatorparam;
    inObj->right = rightparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Unary",strlen("Unary"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyUnary);
    setDestructor(inObj,&delete_Unary);
    inObj->super.id = addtoExprCounter();
}
void delete_Unary(void* inArg){
    Unary* arg, *expr;
    arg = (Unary*) inArg;
    expr = (Unary*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->operator);*/
    expr->operator=NULL;

/*    delete(expr->right);*/
    expr->right=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptUnary(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitUnaryExpr(visitor,arg);
}

void* copyUnary(void* expr){
    Unary* temp, *unarym;
    unarym = (Unary*)expr;
    temp = new(OBJECTIVE, sizeof(Unary));
new_Unary(temp,unarym->operator,unarym->right);
temp->operator = getReference(temp->operator);
        
temp->right = getReference(temp->right);
        

    setAllocated(temp,1);
    temp->super.id = unarym->super.id;
    return (void*)temp;
}
void new_Variable (Variable* inObj,Token* nameparam){
    inObj->super.vtable.accept = &acceptVariable;    
    inObj->super.vtable.delete = &delete_Variable;
    inObj->super.vtable.copy = &copyVariable;
    inObj->name = nameparam;
        memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Variable",strlen("Variable"));
    setAllocated(inObj,0);
    setCopyConstructor(inObj,&copyVariable);
    setDestructor(inObj,&delete_Variable);
    inObj->super.id = addtoExprCounter();
}
void delete_Variable(void* inArg){
    Variable* arg, *expr;
    arg = (Variable*) inArg;
    expr = (Variable*) arg;

if(getReferenceCount(arg) ==1){
/*    delete(expr->name);*/
    expr->name=NULL;
/*    delete(expr);*/
    expr=NULL;
}
    else{
        releaseReference(arg);
    }
}
Object* acceptVariable(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitVariableExpr(visitor,arg);
}

void* copyVariable(void* expr){
    Variable* temp, *variablem;
    variablem = (Variable*)expr;
    temp = new(OBJECTIVE, sizeof(Variable));
new_Variable(temp,variablem->name);
temp->name = getReference(temp->name);
        

    setAllocated(temp,1);
    temp->super.id = variablem->super.id;
    return (void*)temp;
}

void delete_Expr(Expr* expr){
    if(expr)        expr->vtable.delete(expr);
}
short int addtoExprCounter(void){
     Exprcounter += 1;
     return Exprcounter;
    }
