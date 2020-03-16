
#include "Token.h"
#include "Object.h"
#include "ReturnResult.h"
#include "Expr.h"



static ReturnResult visitBinaryExpr( ExprVisitor* visitor,Expr* arg);

static ReturnResult visitBinaryExpr(ExprVisitor* visitor,Expr* expr){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Binary (Binary * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptBinary;
    
    inObj->super.vtable.delete_Expr = &delete_Binary ;
	inObj->left = leftparam;
	inObj->operator = operatorparam;
	inObj->right = rightparam;
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Binary (Expr* arg){
	Binary * expr = (Binary *)arg;

if(expr->super.owner_references ==1){
    delete_Expr(expr->left);
    expr->left=NULL;
    delete_Token(expr->operator);
    expr->operator=NULL;
    delete_Expr(expr->right);
    expr->right=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptBinary(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitBinaryExpr(visitor,arg);
}

static ReturnResult visitGroupingExpr( ExprVisitor* visitor,Expr* arg);

static ReturnResult visitGroupingExpr(ExprVisitor* visitor,Expr* expr){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Grouping (Grouping * inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptGrouping;
    
    inObj->super.vtable.delete_Expr = &delete_Grouping ;
	inObj->expression = expressionparam;
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Grouping (Expr* arg){
	Grouping * expr = (Grouping *)arg;

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

ReturnResult acceptGrouping(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitGroupingExpr(visitor,arg);
}

static ReturnResult visitLiteralExpr( ExprVisitor* visitor,Expr* arg);

static ReturnResult visitLiteralExpr(ExprVisitor* visitor,Expr* expr){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Literal (Literal * inObj,Object* valueparam){
    inObj->super.vtable.accept = &acceptLiteral;
    
    inObj->super.vtable.delete_Expr = &delete_Literal ;
	inObj->value = valueparam;
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Literal (Expr* arg){
	Literal * expr = (Literal *)arg;

if(expr->super.owner_references ==1){
    delete_Object(&expr->value);
    expr->value=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptLiteral(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitLiteralExpr(visitor,arg);
}

static ReturnResult visitUnaryExpr( ExprVisitor* visitor,Expr* arg);

static ReturnResult visitUnaryExpr(ExprVisitor* visitor,Expr* expr){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Unary (Unary * inObj,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptUnary;
    
    inObj->super.vtable.delete_Expr = &delete_Unary ;
	inObj->operator = operatorparam;
	inObj->right = rightparam;
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Unary (Expr* arg){
	Unary * expr = (Unary *)arg;

if(expr->super.owner_references ==1){
    delete_Token(expr->operator);
    expr->operator=NULL;
    delete_Expr(expr->right);
    expr->right=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptUnary(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitUnaryExpr(visitor,arg);
}

static ReturnResult visitVariableExpr( ExprVisitor* visitor,Expr* arg);

static ReturnResult visitVariableExpr(ExprVisitor* visitor,Expr* expr){
    ReturnResult r; r.value.string=NULL; return r;
}
void new_Variable (Variable * inObj,Token* nameparam){
    inObj->super.vtable.accept = &acceptVariable;
    
    inObj->super.vtable.delete_Expr = &delete_Variable ;
	inObj->name = nameparam;
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Variable (Expr* arg){
	Variable * expr = (Variable *)arg;

if(expr->super.owner_references ==1){
    delete_Token(expr->name);
    expr->name=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptVariable(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitVariableExpr(visitor,arg);
}
void delete_Expr(Expr* expr){
	expr->vtable.delete_Expr(expr);
}
short int addtoExprCounter(void){
     Exprcounter += 1;
     return Exprcounter;
    }

Expr* getExprReference(Expr* arg){
    arg->owner_references += 1;
    return arg;
}
void removeExprReference(Expr* arg){
    if(arg->owner_references > 1)
        arg->owner_references -= 1;
}

