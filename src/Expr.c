#include "Token.h"
#include "Expr.h"
static void* visitBinaryExpr(Visitor* visitor,Expr* expr){return NULL;}
static void* visitGroupingExpr(Visitor* visitor,Expr* expr){return NULL;}
static void* visitLiteralExpr(Visitor* visitor,Expr* expr){return NULL;}
static void* visitUnaryExpr(Visitor* visitor,Expr* expr){return NULL;}
void new_Binary (Binary * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
	inObj->super.vtable.accept = &acceptBinary;
	inObj->left = leftparam;
	inObj->operator = operatorparam;
	inObj->right = rightparam;
}
void* acceptBinary(Expr *arg, Visitor* visitor){
	 return visitor->vtable.visitBinaryExpr(visitor,arg);
}
void new_Grouping (Grouping * inObj,Expr* expressionparam){
	inObj->super.vtable.accept = &acceptGrouping;
	inObj->expression = expressionparam;
}
void* acceptGrouping(Expr *arg, Visitor* visitor){
	 return visitor->vtable.visitGroupingExpr(visitor,arg);
}
void new_Literal (Literal * inObj,void* valueparam,int* valueTypeparam){
	inObj->super.vtable.accept = &acceptLiteral;
	inObj->value = valueparam;
	inObj->valueType = valueTypeparam;
}
void* acceptLiteral(Expr *arg, Visitor* visitor){
	 return visitor->vtable.visitLiteralExpr(visitor,arg);
}
void new_Unary (Unary * inObj,Token* operatorparam,Expr* rightparam){
	inObj->super.vtable.accept = &acceptUnary;
	inObj->operator = operatorparam;
	inObj->right = rightparam;
}
void* acceptUnary(Expr *arg, Visitor* visitor){
	 return visitor->vtable.visitUnaryExpr(visitor,arg);
}
