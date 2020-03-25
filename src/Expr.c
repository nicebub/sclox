
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
void new_Assign (Assign* inObj,Token* nameparam,Expr* valueparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptAssign;    
    inObj->super.vtable.delete = &delete_Assign;
    inObj->super.vtable.copy = &copyAssign;
    inObj->name = nameparam;
    inObj->value = valueparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Assign",strlen("Assign"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyAssign;
    footer->functions.delete = &delete_Assign;
    inObj->super.id = addtoExprCounter();
}
void delete_Assign(void* inArg){
    mem_footer* footer;
    Assign* arg,*expr;
    arg = (Assign*) inArg;
    expr = (Assign*)arg;
    footer = get_footer(arg);
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
    mem_footer* footer;
    Assign* temp,*assign;
    assign = (Assign*) expr;
    temp = new(OBJECTIVE,sizeof(Assign));
    new_Assign(temp,assign->name,assign->value);
    footer = get_footer(expr);
    footer->functions.allocated =1;
    temp->name = getReference(temp->name);
    temp->value = getReference(temp->value);
    return (void*)temp;
    
}
void new_Binary (Binary* inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptBinary;    
    inObj->super.vtable.delete = &delete_Binary;
    inObj->super.vtable.copy = &copyBinary;
    inObj->left = leftparam;
    inObj->operator = operatorparam;
    inObj->right = rightparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Binary",strlen("Binary"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyBinary;
    footer->functions.delete = &delete_Binary;
    inObj->super.id = addtoExprCounter();
}
void delete_Binary(void* inArg){
    Binary* arg,*expr;
    mem_footer* footer;
    arg = (Binary*) inArg;
    expr = (Binary*)arg;
    footer = get_footer(inArg);
if(getReferenceCount(inArg) ==1){
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
	   getReferenceCount(inArg);;
    }
}
Object* acceptBinary(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitBinaryExpr(visitor,arg);
}
void* copyBinary(void* expr){
    mem_footer* footer;
    Binary* temp,*bin;
    bin = (Binary*) expr;
    temp = new(OBJECTIVE,sizeof(Binary));
    new_Binary(temp,bin->left,bin->operator,bin->right);
    footer = get_footer(bin);
    footer->functions.allocated =1;
    temp->left = getReference(temp->left);
    temp->operator = getReference(temp->operator);
    temp->right = getReference(temp->right);
    return (void*)temp;
    
}

void new_Call (Call* inObj,Expr* calleeparam,Token* parenparam,ExprArray* argumentsparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptCall;    
    inObj->super.vtable.delete = &delete_Call;
    inObj->super.vtable.copy = &copyCall;
    inObj->callee = calleeparam;
    inObj->paren = parenparam;
    inObj->arguments = argumentsparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Call",strlen("Call"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyCall;
    footer->functions.delete = &delete_Call;
    inObj->super.id = addtoExprCounter();
}
void delete_Call(void* arg){
    Call* expr = (Call*)arg;

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
    mem_footer* footer;
    Call* temp,*bin;
    bin = (Call*) expr;
    temp = new(OBJECTIVE,sizeof(Call));
    new_Call(temp,bin->callee,bin->paren,bin->arguments);
    footer = get_footer(temp);
    footer->functions.allocated =1;
    temp->callee = getReference(temp->callee);
    temp->paren = getReference(temp->paren);
    temp->arguments = getReference(temp->arguments);
    return (void*)temp;
    
}

void new_Grouping (Grouping* inObj,Expr* expressionparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptGrouping;    
    inObj->super.vtable.delete = &delete_Grouping;
    inObj->super.vtable.copy = &copyGrouping;
    inObj->expression = expressionparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Grouping",strlen("Grouping"));
    footer = get_footer(inObj);
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyGrouping;
    footer->functions.delete = &delete_Grouping;
    inObj->super.id = addtoExprCounter();
}
void delete_Grouping(void* arg){
    Grouping* expr = (Grouping*)arg;

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
    mem_footer* footer;
    Grouping* temp,*bin;
    bin = (Grouping*) expr;
    temp = new(OBJECTIVE,sizeof(Grouping));
    new_Grouping(temp,bin->expression);
    footer = get_footer(temp);
    footer->functions.allocated =1;
    temp->expression = getReference(temp->expression);
    return (void*)temp;
    
}

void new_Literal (Literal* inObj,Object* valueparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptLiteral;    
    inObj->super.vtable.delete = &delete_Literal;
    inObj->value = valueparam;
    inObj->super.vtable.copy = &copyLiteral;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Literal",strlen("Literal"));
    footer = get_footer(inObj);
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyLiteral;
    footer->functions.delete = &delete_Literal;
    inObj->super.id = addtoExprCounter();
}
void delete_Literal(void* arg){
    Literal* expr = (Literal*)arg;

if(getReferenceCount(arg)==1){
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
    mem_footer* footer;
    Literal* temp,*bin;
    bin = (Literal*) expr;
    temp = new(OBJECTIVE,sizeof(Literal));
    new_Literal(temp,bin->value);
    footer = get_footer(temp);
    footer->functions.allocated =1;
    temp->value = getReference(temp->value);
    return (void*)temp;
    
}

void new_Logical (Logical* inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptLogical;    
    inObj->super.vtable.delete = &delete_Logical;
    inObj->super.vtable.copy = &copyLogical;
    inObj->left = leftparam;
    inObj->operator = operatorparam;
    inObj->right = rightparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Logical",strlen("Logical"));
    footer = get_footer(inObj);
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyLogical;
    footer->functions.delete = &delete_Logical;
    inObj->super.id = addtoExprCounter();
}
void delete_Logical(void* arg){
    Logical* expr = (Logical*)arg;

if(getReferenceCount(arg)==1){
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
    mem_footer* footer;
    Logical* temp,*bin;
    bin = (Logical*) expr;
    temp = new(OBJECTIVE,sizeof(Logical));
    new_Logical(temp,bin->left,bin->operator,bin->right);
    footer = get_footer(temp);
    footer->functions.allocated =1;
    temp->left = getReference(temp->left);
    temp->right = getReference(temp->right);
    temp->operator = getReference(temp->operator);
    return (void*)temp;
    
}

void new_Unary (Unary* inObj,Token* operatorparam,Expr* rightparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptUnary;
    inObj->super.vtable.delete = &delete_Unary;
    inObj->super.vtable.copy = &copyUnary;
    inObj->operator = operatorparam;
    inObj->right = rightparam;
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Unary",strlen("Unary"));
    footer = get_footer(inObj);
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyUnary;
    footer->functions.delete = &delete_Unary;
    inObj->super.id = addtoExprCounter();
}
void delete_Unary(void* arg){
    Unary* expr = (Unary*)arg;

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
    mem_footer* footer;
    Unary* temp,*bin;
    bin = (Unary*) expr;
    temp = new(OBJECTIVE,sizeof(Unary));
    new_Unary(temp,bin->operator,bin->right);
    footer = get_footer(temp);
    footer->functions.allocated =1;
    temp->right = getReference(temp->right);
    temp->operator = getReference(temp->operator);
    return (void*)temp;
    
}

void new_Variable (Variable* inObj,Token* nameparam){
    mem_footer* footer;
    inObj->super.vtable.accept = &acceptVariable;    
    inObj->super.vtable.delete = &delete_Variable;
    inObj->super.vtable.copy = &copyVariable;
    inObj->name = getReference(nameparam);
    memset((char*)&inObj->super.instanceOf,0,30);
    strncpy((char*)&inObj->super.instanceOf,"Variable",strlen("Variable"));
    footer = get_footer(inObj);
    footer->functions.owner_references=1;
    footer->functions.allocated = 0;
    footer->functions.copy = &copyVariable;
    footer->functions.delete = &delete_Variable;
    inObj->super.id = addtoExprCounter();
}
void delete_Variable(void* arg){
    Variable* expr = (Variable*)arg;

if(getReferenceCount(arg)==1){
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
    mem_footer* footer;
    Variable* temp,*bin;
    bin = (Variable*) expr;
    temp = new(OBJECTIVE,sizeof(Variable));
    new_Variable(temp,bin->name);
    footer = get_footer(temp);
    footer->functions.allocated =1;
    temp->name = getReference(temp->name);
    return (void*)temp;
    
}


void delete_Expr(Expr* expr){
    if(expr)
	   expr->vtable.delete(expr);
}
short int addtoExprCounter(void){
     Exprcounter += 1;
     return Exprcounter;
    }
