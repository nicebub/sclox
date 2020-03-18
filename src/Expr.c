
#include <string.h>
#include "Token.h"
#include "Object.h"
#include "ReturnResult.h"
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


void new_Assign   (Assign   * inObj,Token* nameparam,Expr* valueparam){
    inObj->super.vtable.accept = &acceptAssign;
    
    inObj->super.vtable.delete_Expr = &delete_Assign   ;
	inObj->name = nameparam;
	inObj->value = valueparam;
	strcpy((char*)&inObj->super.instanceOf,"Assign");
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Assign   (Expr* arg){
	Assign   * expr = (Assign   *)arg;

if(expr->super.owner_references ==1){
    delete_Token(expr->name);
    expr->name=NULL;
    delete_Expr(expr->value);
    expr->value=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptAssign(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitAssignExpr(visitor,arg);
}
void new_Binary (Binary * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptBinary;
    
    inObj->super.vtable.delete_Expr = &delete_Binary ;
	inObj->left = leftparam;
	inObj->operator = operatorparam;
	inObj->right = rightparam;
	strcpy((char*)&inObj->super.instanceOf,"Binary");
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
void new_Call (Call * inObj,Expr* calleeparam,Token* parenparam,ExprArray* argumentsparam){
    inObj->super.vtable.accept = &acceptCall;
    
    inObj->super.vtable.delete_Expr = &delete_Call ;
	inObj->callee = calleeparam;
	inObj->paren = parenparam;
	inObj->arguments = argumentsparam;
	strcpy((char*)&inObj->super.instanceOf,"Call");
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Call (Expr* arg){
	Call * expr = (Call *)arg;

if(expr->super.owner_references ==1){
    delete_Expr(expr->callee);
    expr->callee=NULL;
    delete_Token(expr->paren);
    expr->paren=NULL;
    delete_ExprArray(expr->arguments);
    expr->arguments=NULL;

	free(expr);
	expr=NULL;
}

    else{
        expr->super.owner_references--;
    }
}

ReturnResult acceptCall(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitCallExpr(visitor,arg);
}
void new_Grouping (Grouping * inObj,Expr* expressionparam){
    inObj->super.vtable.accept = &acceptGrouping;
    
    inObj->super.vtable.delete_Expr = &delete_Grouping ;
	inObj->expression = expressionparam;
	strcpy((char*)&inObj->super.instanceOf,"Grouping");
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
void new_Literal (Literal * inObj,Object* valueparam){
    inObj->super.vtable.accept = &acceptLiteral;
    
    inObj->super.vtable.delete_Expr = &delete_Literal ;
	inObj->value = valueparam;
	strcpy((char*)&inObj->super.instanceOf,"Literal");
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
void new_Logical  (Logical  * inObj,Expr* leftparam,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptLogical;
    
    inObj->super.vtable.delete_Expr = &delete_Logical  ;
	inObj->left = leftparam;
	inObj->operator = operatorparam;
	inObj->right = rightparam;
	strcpy((char*)&inObj->super.instanceOf,"Logical");
	inObj->super.owner_references=1;
	inObj->super.id = addtoExprCounter();
}
void delete_Logical  (Expr* arg){
	Logical  * expr = (Logical  *)arg;

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

ReturnResult acceptLogical(Expr *arg, ExprVisitor* visitor){
    return visitor->vtable.visitLogicalExpr(visitor,arg);
}
void new_Unary (Unary * inObj,Token* operatorparam,Expr* rightparam){
    inObj->super.vtable.accept = &acceptUnary;
    
    inObj->super.vtable.delete_Expr = &delete_Unary ;
	inObj->operator = operatorparam;
	inObj->right = rightparam;
	strcpy((char*)&inObj->super.instanceOf,"Unary");
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
void new_Variable (Variable * inObj,Token* nameparam){
    inObj->super.vtable.accept = &acceptVariable;
    
    inObj->super.vtable.delete_Expr = &delete_Variable ;
	inObj->name = nameparam;
	strcpy((char*)&inObj->super.instanceOf,"Variable");
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
	if(expr)		expr->vtable.delete_Expr(expr);
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

