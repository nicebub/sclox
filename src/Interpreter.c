/*
 * Interpreter.c
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */
/*
typedef struct _exception{
    unsigned int id;
} exception;
#define CEXCEPTION_T exception
*/
#define CEXCEPTION_USE_CONFIG_FILE

#include "CException.h"
#include <string.h>
#include <stdio.h>
#include "Lox.h"
#include "Interpreter.h"
#include "Expr.h"
#include "TokenType.h"
#include "ReturnResult.h"
#include "Object.h"
#include "additions.h"

ReturnResult visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr);
ReturnResult visitGroupingExprInterpreter(ExprVisitor* visitor, Expr* expr);
ReturnResult visitUnaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
ReturnResult visitBinaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
ReturnResult visitExpressionStmt(StmtVisitor* visitor, Stmt* stmt);
ReturnResult visitPrintStmt(StmtVisitor* visitor, Stmt* stmt);
ReturnResult evaluate(ExprVisitor* visitor, Expr* expr);
ReturnResult isTruthy(ReturnResult obj);


void init_Interpreter(Interpreter* intprtr, Lox* lox){
    intprtr->lox = lox;
	intprtr->super.expr.vtable.visitLiteralExpr = &visitLiteralExprInterpreter;
	intprtr->super.expr.vtable.visitGroupingExpr = &visitGroupingExprInterpreter;
	intprtr->super.expr.vtable.visitUnaryExpr = &visitUnaryExprInterpreter;
    intprtr->super.expr.vtable.visitBinaryExpr = &visitBinaryExprInterpreter;
    intprtr->super.vtable.visitExpressionStmt =&visitExpressionStmt;
    intprtr->super.vtable.visitPrintStmt = &visitPrintStmt;
    intprtr->checkNumberOperand = &checkNumberOperand;
    intprtr->checkNumberOperands = &checkNumberOperands;
    intprtr->evaluate = &evaluate;
    intprtr->interpret = &interpret;
    intprtr->isEqual = &isEqual;
    intprtr->isTruthy = &isTruthy;
    intprtr->stringify = &stringify;
}

void interpret(Interpreter* intprtr, StmtArray* array){
    CEXCEPTION_T e;
    Try{
        int i;
/*	   volatile ReturnResult value;*/
	   for(i=0;i<array->used;i++){
		  Stmt* stmt = getStmtinArrayAt(array,i);
		   execute(intprtr,stmt);
	   }
    }
    Catch(e){
	   intprtr->lox->runtimeError(intprtr->lox,e);
    }
}

void execute(Interpreter* intprtr, Stmt* stmt){
    Expression * ex = (Expression*)stmt;
	ex->super.vtable.accept(stmt,&intprtr->super);
}

ReturnResult visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr){
	ReturnResult r;
    r.type = ((Literal*)expr)->value->type;
	switch(r.type){
	case TRUE:
	case FALSE:
	case NUMBER:
		r.value.number = *(double*)((Literal*)expr)->value->value;
		break;
	case NIL:
	case STRING:
		r.value.string = ((Literal*)expr)->value->value;
		break;
	default: break;
	}
	return r;
}
ReturnResult visitGroupingExprInterpreter(ExprVisitor* visitor, Expr* expr){
	return evaluate(visitor,((Grouping*)expr)->expression);
}
ReturnResult visitUnaryExprInterpreter(ExprVisitor* visitor, Expr* expr){
	ReturnResult right,result;
	right = evaluate(visitor,((Unary*)expr)->right);

	switch(((Unary*)expr)->operator->type){
		case BANG:
		   result.type = BOOLEAN;
		   result.value.number = ! isTruthy(right).value.number;
		   return result;
		case MINUS:
		   checkNumberOperand(((Unary*)expr)->operator,right);
		   result.type = NUMBER;
   		   result.value.number = - (double) right.value.number;
/*		   result.value.number = - * (double*) ((Literal*)((Unary*)expr)->right)->value->value;*/
			return  result;
	    default: break;
	}
    result.type = KNULL;
	return result;
}

ReturnResult evaluate(ExprVisitor* visitor, Expr* expr){
	return expr->vtable.accept(expr,visitor);
}
ReturnResult isTruthy(ReturnResult obj){
	ReturnResult r;
    if(obj.type == BOOLEAN || obj.type == FALSE || obj.type == TRUE || obj.type == NIL){
	   r.type = BOOLEAN;
	   r.value.number = (double)obj.value.number;
    }
    else{
	   r.type = BOOLEAN;
	   r.value.number = 0;
    }
    return r;
}

ReturnResult visitBinaryExprInterpreter(ExprVisitor* visitor, Expr* expr){
    CEXCEPTION_T e;
    ReturnResult left, right, result;
    left = evaluate(visitor,((Binary*)expr)->left);
    right = evaluate(visitor,((Binary*)expr)->right);
    switch(((Binary*)expr)->operator->type){
	  case MINUS:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result.type = NUMBER;
		  result.value.number = (double)left.value.number - (double)right.value.number;
		  return result;
	   case PLUS:
		  if(left.type == NUMBER && right.type == NUMBER){
			 result.type = NUMBER;
			 result.value.number = (double)left.value.number + (double)right.value.number;
			 return result;
		  }
		  if((left.type == STRING && right.type == STRING) ||(left.type == STRING && right.type == NUMBER) ||
			(left.type == NUMBER && right.type == STRING)){
			 char* new_str, *num_str;
			 new_str = NULL;
			 result.type = STRING;
			 switch(left.type){
				case STRING:
				    switch(right.type){
					   case STRING:
						  new_str = malloc(sizeof(char)*(strlen(left.value.string)+strlen(right.value.string)+1));
						  strncpy(new_str,left.value.string,strlen(left.value.string));
						  strncat(new_str,right.value.string,strlen(right.value.string));
						  result.value.string = new_str;
						  break;
					   default:
						  num_str = stringify(right);
						  new_str = malloc(sizeof(char)*(strlen(num_str)+strlen(left.value.string)+1));
						  strncpy(new_str,left.value.string,strlen(left.value.string));
						  strncat(new_str,num_str,strlen(num_str));
						  result.value.string = new_str;
						  free(num_str);
						  num_str = NULL;
						  break;
				    }
				    break;
				default:
				    num_str = stringify(left);
				    new_str = malloc(sizeof(char)*(strlen(num_str)+strlen(right.value.string)+1));
				    strncpy(new_str,num_str,strlen(num_str));
				    strncat(new_str,right.value.string,strlen(right.value.string));
				    result.value.string = new_str;
				    free(num_str);
				    num_str = NULL;
				    break;
			 }
			 return result;
		  }
		  e.id = 4;
		  e.token = ((Binary*)expr)->operator;
		  e.message = "Operands must be two numbers, two strings, or one of each";
		  Throw(e);
		  break;
	   case SLASH:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  if(right.value.number == (double)0){
			 e.id = 5;
			 e.token = ((Binary*)expr)->operator;
			 e.message = "Cannot divide by Zero";
			 Throw(e);
		  }
		  result.type = NUMBER;
		  result.value.number = (double)left.value.number / (double)right.value.number;
		  return result;
	   case STAR:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result.type = NUMBER;
		  result.value.number = (double)left.value.number * (double)right.value.number;
		  return result;
	   case GREATER:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result.type = BOOLEAN;
		  result.value.number = (double)left.value.number > (double)right.value.number;
		  return result;
	   case GREATER_EQUAL:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result.type = BOOLEAN;
		  result.value.number = (double)left.value.number >= (double)right.value.number;
		  return result;
	   case LESS:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result.type = BOOLEAN;
		  result.value.number = (double)left.value.number < (double)right.value.number;
		  return result;
	   case LESS_EQUAL:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result.type = BOOLEAN;
		  result.value.number = (double)left.value.number <= (double)right.value.number;
		  return result;
	   case BANG_EQUAL:
		  result.type = BOOLEAN;
		  result.value.number = !isEqual(left,right);
		  return result;
	   case EQUAL_EQUAL:
		  result.type = BOOLEAN;
		  result.value.number = isEqual(left,right);
		  return result;
	   default: break;
    }
    result.type = KNULL;
    return result;
}

int isEqual(ReturnResult left, ReturnResult right){
    if(left.type == KNULL && right.type == KNULL)
	   return 1;
    if(left.type == NIL && right.type == NIL)
	   return 1;
    if(left.type == KNULL || left.type == NIL)
	   return 0;
    if(right.type == KNULL || right.type == NIL)
	   return 0;
/*    if((left.type == FALSE || left.type==TRUE) && (right.type == FALSE || right.type==TRUE))
	   return strncmp(left.value.string,right.value.string,strlen(left.value.string)) == 0;*/
    switch(left.type){
	   case NUMBER:
	   case BOOLEAN:
	   case TRUE:
	   case FALSE:
		  switch(right.type){
			 case NUMBER:
			 case BOOLEAN:
			 case FALSE:
			 case TRUE:
				    return left.value.number == right.value.number;
				break;
			 default: return 0;break;
		  }
		  break;
	   case STRING:
		  switch(right.type){
			 case STRING:
				{
					size_t length = strlen(left.value.string);
					if(length < strlen(right.value.string))
							length = strlen(right.value.string);
				    return strncmp(left.value.string,right.value.string,length) == 0;
				}
				break;
			 
			 default: return 0; break;
		  }
	   break;
	   default: break;
    }
    return 0;
}
void checkNumberOperand(Token* operator,ReturnResult right){
    CEXCEPTION_T e;
    if(right.type == NUMBER) return;
    e.id = 2;
    e.token = operator;
    e.message = "Operand must be a number";
    Throw(e);
}
void checkNumberOperands(Token* operator, ReturnResult left, ReturnResult right){
    CEXCEPTION_T e;
    if(left.type == NUMBER && right.type == NUMBER)
		  return;
    e.id = 3;
    e.token = operator;
    e.message = "Operands must be numbers";
    Throw(e);
}
const static char* nil = "nil";

char* stringify(ReturnResult obj){
    char* text;
    text = NULL;
    if( obj.type == KNULL)
	   return (char*)nil;
    if(obj.type == FALSE)
	   return "false";
    if(obj.type == TRUE)
	   return "true";
    if(obj.type == BOOLEAN){
	   switch((int)obj.value.number){
		  case 1:
			 return "true";
		  case 0:
			 return "false";
		  default:
			 return "boolean error";
	   }
    }
    if(obj.type == NUMBER){
	   char* text;
	   text = NULL;
	   asprintf(&text,"%.1lf",(double)obj.value.number);
	   if(strlen(text)>=2 && text[strlen(text)-1]== '0'
		 && text[strlen(text)-2] == '.'){
		  free(text);
		  text = NULL;
		  asprintf(&text,"%.0lf",(double)obj.value.number);
		  return text;
	   }
	   free(text);
	   text = NULL;
	   asprintf(&text,"%lf",(double)obj.value.number);

	   return text;
    }
/*    asprintf(&text,"%lf",(double)obj.value.number);*/

    return obj.value.string;
}

ReturnResult visitExpressionStmt(StmtVisitor* visitor, Stmt* stmt){
	ReturnResult result;
	result.type = KNULL;
	evaluate(&visitor->expr,((Expression*)stmt)->expression);
	return result;
}

ReturnResult visitPrintStmt(StmtVisitor* visitor, Stmt* stmt){
	ReturnResult r,result;
	r = evaluate(&visitor->expr,((Print*)stmt)->expression);
	printf("%s\n",stringify(r));
	result.type = KNULL;
	return result;
}
