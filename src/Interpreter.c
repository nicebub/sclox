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
#include "Environment.h"
#ifndef _STMTARRAY
#define _STMTARRAY
	typedef struct _StmtArray StmtArray;
	extern deleteStmtArray(StmtArray* array);
#endif

	#include "StmtArray.h"
static ReturnResult visitWhileStmt(StmtVisitor* visitor, Stmt* stmt);
static ReturnResult visitLogicalExpr(ExprVisitor* visitor, Expr* expr);
static ReturnResult visitIfStmt(StmtVisitor * visitor, Stmt* expr);
static ReturnResult visitBlockStmt(StmtVisitor * visitor, Stmt* expr);
static ReturnResult visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr);
static ReturnResult visitGroupingExprInterpreter(ExprVisitor* visitor, Expr* expr);
static ReturnResult visitUnaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
static ReturnResult visitBinaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
static ReturnResult visitAssignExpr(ExprVisitor * visitor,Expr* expr);
static ReturnResult visitExpressionStmt(StmtVisitor* visitor, Stmt* stmt);
static ReturnResult visitPrintStmt(StmtVisitor* visitor, Stmt* stmt);
static ReturnResult visitVarStmt(StmtVisitor* visitor,Stmt* stmt);
static ReturnResult visitVariableExpr(ExprVisitor* visitor, Expr* stmt);
ReturnResult evaluate(ExprVisitor* visitor, Expr* expr);
ReturnResult isTruthy(ReturnResult obj);

void init_Interpreter(Interpreter* intprtr, void* lox){
    intprtr->lox = lox;
    intprtr->environment = malloc(sizeof(Environment));
    init_Environment(intprtr->environment);
	intprtr->super.expr.vtable.visitLiteralExpr = &visitLiteralExprInterpreter;
	intprtr->super.expr.vtable.visitGroupingExpr = &visitGroupingExprInterpreter;
	intprtr->super.expr.vtable.visitUnaryExpr = &visitUnaryExprInterpreter;
    intprtr->super.expr.vtable.visitBinaryExpr = &visitBinaryExprInterpreter;
    intprtr->super.expr.vtable.visitVariableExpr = &visitVariableExpr;
    intprtr->super.expr.vtable.visitAssignExpr = &visitAssignExpr;
    intprtr->super.expr.vtable.visitLogicalExpr = &visitLogicalExpr;
    intprtr->super.vtable.visitExpressionStmt =&visitExpressionStmt;
    intprtr->super.vtable.visitWhileStmt = &visitWhileStmt;
    intprtr->super.vtable.visitIfStmt = &visitIfStmt;
    intprtr->super.vtable.visitBlockStmt = &visitBlockStmt;
    intprtr->super.vtable.visitPrintStmt = &visitPrintStmt;
    intprtr->super.vtable.visitVarStmt = &visitVarStmt;
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
	   ((Lox*)intprtr->lox)->runtimeError(intprtr->lox,e);
    }
    
}

void execute(Interpreter* intprtr, Stmt* stmt){
    Expression * ex = (Expression*)stmt;
	ex->super.vtable.accept(stmt,&intprtr->super);
}
void executeBlock(Interpreter* intrprtr ,StmtArray* array,Environment* newenv){
	CEXCEPTION_T e;
	Environment * previous;
	int i;
	previous = intrprtr->environment;
	Try{
		intrprtr->environment = newenv;

		for(i = 0; i<array->used;i++){
			execute(intrprtr,array->getElementInArrayAt(array,i));
		}
	}
	Catch(e){
		fprintf(stderr,"RuntimeError: caught in executeBlock.\n");
	}
	intrprtr->environment = previous;
}

ReturnResult visitWhileStmt(StmtVisitor* visitor, Stmt* stmt){
	ReturnResult r;
	while(isTruthy(evaluate(&visitor->expr,((While*)stmt)->condition)).value.number){
		execute((Interpreter*)visitor,((While*)stmt)->body);
	}

	r.type = KNULL;
	return r;
}

ReturnResult visitBlockStmt(StmtVisitor * visitor, Stmt* expr){
	Block* temp;
	Interpreter* intrprtr;
	Environment* env;
	ReturnResult r;
	temp= (Block*)expr;
	intrprtr = (Interpreter*) visitor;
	env = malloc(sizeof(Environment));
	init_EnvironmentwithEnclosing(env,intrprtr->environment);
	executeBlock(intrprtr ,temp->statements,env);
    deleteEnvironment(env);
    env = NULL;
	r.type = KNULL;
	return r;
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
    if(obj.type == BOOLEAN || obj.type == FALSE || obj.type == TRUE || obj.type == NUMBER){
	   r.type = BOOLEAN;
	   r.value.number = (double)obj.value.number;
    }
    else if(obj.type == NIL){
 	   r.type = BOOLEAN;
 	   r.value.number = 0;
    }
    else{
	   r.type = BOOLEAN;
	   r.value.number = 1;
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
						  memset(new_str,0,strlen(left.value.string)+strlen(right.value.string)+1);
						  strncpy(new_str,left.value.string,strlen(left.value.string));
						  strncat(new_str,right.value.string,strlen(right.value.string));
						  result.value.string = new_str;
						  break;
					   default:
						  num_str = stringify(right);
						  new_str = malloc(sizeof(char)*(strlen(num_str)+strlen(left.value.string)+1));
						  memset(new_str,0,strlen(num_str)+strlen(left.value.string)+1);
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
				    memset(new_str,0,strlen(num_str)+strlen(right.value.string)+1);
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
ReturnResult visitLogicalExpr(ExprVisitor* visitor, Expr* expr){
	ReturnResult left;
	Logical* log;
	log = (Logical*) expr;
	left = evaluate(visitor,log->left);
	if(log->operator->type == OR){
		if(isTruthy(left).value.number)
		    return left;
	       }
	else{
	    if(!isTruthy(left).value.number){
			return left;
		}
	}
	return evaluate(visitor,log->right);
}

ReturnResult visitIfStmt(StmtVisitor * visitor, Stmt* expr){
	ReturnResult r,a;
	If* stmt = (If*) expr;
    a = evaluate(&visitor->expr,stmt->condition);
    if(isTruthy(a).value.number){
		execute((Interpreter*)visitor,stmt->thenBranch);
	}
	else if(stmt->elseBranch != NULL){
		execute((Interpreter*)visitor,stmt->elseBranch);
	}
	r.type = KNULL;
	return r;
}


ReturnResult visitExpressionStmt(StmtVisitor* visitor, Stmt* stmt){
	ReturnResult result;
	result.type = KNULL;
	evaluate(&visitor->expr,((Expression*)stmt)->expression);
	return result;
}

ReturnResult visitPrintStmt(StmtVisitor* visitor, Stmt* stmt){
	ReturnResult r,result;
    char * val;
	r = evaluate(&visitor->expr,((Print*)stmt)->expression);
    val = stringify(r);
	printf("%s\n",val);
    if(r.type == NUMBER){
		  free(val);
		  val = NULL;
    }
	result.type = KNULL;
	return result;
}

ReturnResult visitVarStmt(StmtVisitor* visitor, Stmt* stmt){
	Interpreter* intprtr;
	ReturnResult *r,a;
	Var* vstmt;
	intprtr = (Interpreter*) visitor;
	r = NULL;
	vstmt = (Var*)stmt;
	if(vstmt->initializer != NULL){
		r = malloc(sizeof(ReturnResult));
		a = evaluate(&visitor->expr,vstmt->initializer);
		r->type = a.type;
		 if(r->type == NUMBER)
			    memcpy(&r->value,&a.value,sizeof(double));
		 else{
			r->value.string = strdup(a.value.string);
/*			r->value.string = malloc(sizeof(char)*(strlen(a.value.string)+1));
			memcpy(r->value.string,&a.value.string,strlen(a.value.string)+1);*/
		 }
	}
	intprtr->environment->defineEnv(intprtr->environment,strdup(vstmt->name->lexeme),r);
	a.type = NIL;
	return a;
}
ReturnResult visitAssignExpr(ExprVisitor * visitor,Expr* expr){
	Assign* assign = (Assign*) expr;
	ReturnResult *r,a;
	Interpreter* intprtr;
	r = malloc(sizeof(ReturnResult));
	intprtr= (Interpreter*)visitor;
	a = evaluate(visitor,assign->value);
	r->type = a.type;
    if(r->type == NUMBER)
		  memcpy(&r->value,&a.value,sizeof(double));
    else{
	   r->value.string = strdup(a.value.string);
	   /*	   memcpy(&r->value.string,&a.value.string,strlen(a.value.string)+1);*/
    }
	r->value = a.value;
	intprtr->environment->assign(intprtr->environment,assign->name,r);
	return a;
}

ReturnResult visitVariableExpr(ExprVisitor* visitor, Expr* expr){
	Environment* env;
	ReturnResult *r,a;
	env = (((Interpreter*)visitor)->environment);
	r = (env->get(env,((Variable*)expr)->name ));
    if(r){
	a.type = r->type;
	if(a.type == NUMBER)
	    memcpy(&a.value,&r->value,sizeof(double));
	else{
	    a.value.string = strdup(r->value.string);
/*	    memcpy(&a.value.string,&r->value.string,strlen(r->value.string)+1);*/
	}
	   return a;
    }
    a.type = KNULL;
    a.value.number=0;
	return a;
}
