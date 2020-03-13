
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
}
void delete_Binary (Expr* arg){
	Binary * expr = (Binary *)arg;

    delete_Expr(expr->left);
    expr->left=NULL;
    delete_Token(expr->operator);
    expr->operator=NULL;
    delete_Expr(expr->right);
    expr->right=NULL;

	free(expr);
	expr=NULL;
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
}
void delete_Grouping (Expr* arg){
	Grouping * expr = (Grouping *)arg;

    delete_Expr(expr->expression);
    expr->expression=NULL;

	free(expr);
	expr=NULL;
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
}
void delete_Literal (Expr* arg){
	Literal * expr = (Literal *)arg;

    delete_Object(&expr->value);
    expr->value=NULL;

	free(expr);
	expr=NULL;
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
}
void delete_Unary (Expr* arg){
	Unary * expr = (Unary *)arg;

    delete_Token(expr->operator);
    expr->operator=NULL;
    delete_Expr(expr->right);
    expr->right=NULL;

	free(expr);
	expr=NULL;
}

ReturnResult acceptUnary(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitUnaryExpr(visitor,arg);
}
void delete_Expr(Expr* expr){
	expr->vtable.delete_Expr(expr);
}
