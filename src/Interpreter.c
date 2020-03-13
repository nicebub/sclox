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
#include "CException.h"
#include <string.h>
#include <stdio.h>
#include "Lox.h"
#include "Interpreter.h"
#include "Expr.h"
#include "TokenType.h"
#include "ReturnResult.h"
#include "Object.h"

void init_Interpreter(Interpreter* intprtr, Lox* lox){
    intprtr->lox = lox;
	intprtr->super.vtable.visitLiteralExpr = &visitLiteralExprInterpreter;
	intprtr->super.vtable.visitGroupingExpr = &visitGroupingExprInterpreter;
	intprtr->super.vtable.visitUnaryExpr = &visitUnaryExprInterpreter;
    intprtr->super.vtable.visitBinaryExpr = &visitBinaryExprInterpreter;
    intprtr->checkNumberOperand = &checkNumberOperand;
    intprtr->checkNumberOperands = &checkNumberOperands;
    intprtr->evaluate = &evaluate;
    intprtr->interpret = &interpret;
    intprtr->isEqual = &isEqual;
    intprtr->isTruthy = &isTruthy;
    intprtr->stringify = &stringify;
}

void interpret(Interpreter* intprtr, Expr* expression){
    CEXCEPTION_T e;
    Try{
	   volatile ReturnResult value;
	   value = evaluate(&intprtr->super, expression);
	   printf("%s\n", stringify(value));
    }
    Catch(e){
	   intprtr->lox->runtimeError(intprtr->lox,e);
    }
}

ReturnResult visitLiteralExprInterpreter(Visitor* visitor, Expr* expr){
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
ReturnResult visitGroupingExprInterpreter(Visitor* visitor, Expr* expr){
	return evaluate(visitor,((Grouping*)expr)->expression);
}
ReturnResult visitUnaryExprInterpreter(Visitor* visitor, Expr* expr){
	ReturnResult right,result;
	right = evaluate(visitor,((Unary*)expr)->right);

	switch(((Unary*)expr)->operator->type){
		case BANG:
		   result.type = BOOLEAN;
		   result.value.number = ! isTruthy(((Literal*)((Unary*)expr)->right)->value).value.number;
		   return result;
		case MINUS:
		   checkNumberOperand(((Unary*)expr)->operator,
						  ((Literal*)((Unary*)expr)->right)->value );
		   result.type = NUMBER;
		   result.value.number = - * (double*) ((Literal*)((Unary*)expr)->right)->value->value;
			return  result;
	    default: break;
	}
    result.type = KNULL;
	return result;
}

ReturnResult evaluate(Visitor* visitor, Expr* expr){
	return expr->vtable.accept(expr,visitor);
}
ReturnResult isTruthy(Object* obj){
	ReturnResult r;
    if(obj == NULL || obj->type == KNULL){
	   r.type = BOOLEAN;
	   r.value.number = 0;
	   return r;
    }
    if(obj->isBool){
	   r.type = BOOLEAN;
	   r.value.number = *((double*)obj->value);
    }
    r.type = BOOLEAN;
    r.value.number = 0;
    return r;
}

ReturnResult visitBinaryExprInterpreter(Visitor* visitor, Expr* expr){
    ReturnResult left, right, result;
    left = evaluate(visitor,((Binary*)expr)->left);
    right = evaluate(visitor,((Binary*)expr)->right);
    switch(((Binary*)expr)->operator->type){
	  case MINUS:
		  checkNumberOperands(((Binary*)expr)->operator,
			 ((Literal*)((Binary*)expr)->left)->value,
			 ((Literal*)((Binary*)expr)->right)->value);
		  result.type = NUMBER;
		  result.value.number = (double)left.value.number - (double)right.value.number;
		  return result;
	   case PLUS:
		  if(left.type == NUMBER && right.type == NUMBER){
			 result.type = NUMBER;
			 result.value.number = (double)left.value.number + (double)right.value.number;
			 return result;
		  }
		  if(left.type == STRING && right.type == STRING){
			 char* new_str;
			 new_str = NULL;
			 result.type = STRING;
			 new_str = malloc(sizeof(char)*(strlen(left.value.string)+strlen(right.value.string)+1));
			 strncpy(new_str,left.value.string,strlen(left.value.string));
			 strncat(new_str,right.value.string,strlen(right.value.string));
			 result.value.string = new_str;
			 return result;
		  }
		  Throw(4);
		  break;
	   case SLASH:
		  checkNumberOperands(((Binary*)expr)->operator,
			 ((Literal*)((Binary*)expr)->left)->value,
			 ((Literal*)((Binary*)expr)->right)->value);
		  result.type = NUMBER;
		  result.value.number = (double)left.value.number / (double)right.value.number;
		  return result;
	   case STAR:
		  checkNumberOperands(((Binary*)expr)->operator,
			 ((Literal*)((Binary*)expr)->left)->value,
			 ((Literal*)((Binary*)expr)->right)->value);
		  result.type = NUMBER;
		  result.value.number = (double)left.value.number * (double)right.value.number;
		  return result;
	   case GREATER:
		  checkNumberOperands(((Binary*)expr)->operator,
			 ((Literal*)((Binary*)expr)->left)->value,
			 ((Literal*)((Binary*)expr)->right)->value);
		  result.type = BOOLEAN;
		  result.value.number = (double)left.value.number > (double)right.value.number;
		  return result;
	   case GREATER_EQUAL:
		  checkNumberOperands(((Binary*)expr)->operator,
			 ((Literal*)((Binary*)expr)->left)->value,
			 ((Literal*)((Binary*)expr)->right)->value);
		  result.type = BOOLEAN;
		  result.value.number = (double)left.value.number >= (double)right.value.number;
		  return result;
	   case LESS:
		  checkNumberOperands(((Binary*)expr)->operator,
			 ((Literal*)((Binary*)expr)->left)->value,
			 ((Literal*)((Binary*)expr)->right)->value);
		  result.type = BOOLEAN;
		  result.value.number = (double)left.value.number < (double)right.value.number;
		  return result;
	   case LESS_EQUAL:
		  checkNumberOperands(((Binary*)expr)->operator,
			 ((Literal*)((Binary*)expr)->left)->value,
			 ((Literal*)((Binary*)expr)->right)->value);
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
		  return left.value.number == right.value.number;
	   case STRING:
	   {
		   size_t length = strlen(left.value.string);
		   if(length < strlen(right.value.string))
				   length = strlen(right.value.string);
		  return strncmp(left.value.string,right.value.string,length) == 0;
	   }
	   break;
	   default: break;
    }
    return 0;
}
void checkNumberOperand(Token* operator,Object* right){
    if(operator->type == NUMBER) return;
    Throw(2);
}
void checkNumberOperands(Token* operator, Object* left, Object* right){
	   if(left->type == NUMBER && right->type == NUMBER)
		  return;
    Throw(3);
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
