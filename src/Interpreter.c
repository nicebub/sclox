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
#include "CExceptionConfig.h"
#include "CException.h"
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include "Lox.h"
#include "Interpreter.h"
#include "Expr.h"
#include "TokenType.h"
#include "Object.h"
#include "additions.h"
#include "Environment.h"
#include "StmtArray.h"
#include "ExprArray.h"
#include "TokenArray.h"
#include "ObjectArray.h"
#include "LoxCallable.h"
#ifndef _STMTARRAY
#define _STMTARRAY
	typedef struct _StmtArray StmtArray;
	extern delete_StmtArray(StmtArray* array);
#endif

	#include "StmtArray.h"
static Object* visitCallExpr(ExprVisitor* visitor, Expr* stmt);
static Object* visitWhileStmt(StmtVisitor* visitor, Stmt* stmt);
static Object* visitLogicalExpr(ExprVisitor* visitor, Expr* expr);
static Object* visitIfStmt(StmtVisitor * visitor, Stmt* expr);
static Object* visitBlockStmt(StmtVisitor * visitor, Stmt* expr);
static Object* visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitGroupingExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitUnaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitBinaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitAssignExpr(ExprVisitor * visitor,Expr* expr);
static Object* visitExpressionStmt(StmtVisitor* visitor, Stmt* stmt);
static Object* visitPrintStmt(StmtVisitor* visitor, Stmt* stmt);
static Object* visitVarStmt(StmtVisitor* visitor,Stmt* stmt);
static Object* visitVariableExpr(ExprVisitor* visitor, Expr* stmt);
Object* evaluate(ExprVisitor* visitor, Expr* expr);
Object* isTruthy(Object* obj);

static int global_clock_arity(LoxCallable* inloxcall){
    return 0;
}
static Object* global_clock_call(LoxCallable* inLoxcall,Interpreter* intprtr, ObjectArray* arguments){
    Object* obj;
    struct timeval tv;
    struct timezone tz;
    double x;
    gettimeofday(&tv,&tz);
    x =  ((double)tv.tv_sec)+ (((double)tv.tv_usec / (double)1000.0)/ (double)1000.0);
    obj = malloc(sizeof(Object));
    init_Object(obj,&x,NUMBER);
    return obj;
    /*    return (double)system currentime in millisceons / 1000.0  */
}
static char* global_toString(LoxCallable* inloxcall){
    return "<native fn>";
}
void init_Interpreter(Interpreter* intprtr, void* lox){
    LoxCallable* lcall;
    intprtr->lox = lox;
    intprtr->globals = malloc(sizeof(Environment));
    init_Environment(intprtr->globals);
    intprtr->environment = intprtr->globals;
	intprtr->super.expr.vtable.visitLiteralExpr = &visitLiteralExprInterpreter;
	intprtr->super.expr.vtable.visitGroupingExpr = &visitGroupingExprInterpreter;
	intprtr->super.expr.vtable.visitUnaryExpr = &visitUnaryExprInterpreter;
    intprtr->super.expr.vtable.visitBinaryExpr = &visitBinaryExprInterpreter;
    intprtr->super.expr.vtable.visitVariableExpr = &visitVariableExpr;
    intprtr->super.expr.vtable.visitAssignExpr = &visitAssignExpr;
    intprtr->super.expr.vtable.visitLogicalExpr = &visitLogicalExpr;
    intprtr->super.vtable.visitExpressionStmt =&visitExpressionStmt;
    intprtr->super.expr.vtable.visitCallExpr = &visitCallExpr;
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
    lcall = malloc(sizeof(LoxCallable));
    init_Object(&lcall->super,"clock",FUN);
    strcpy((char*)&lcall->super.instanceOf,"LoxCallable");
    lcall->arity = &global_clock_arity;
    lcall->call = &global_clock_call;
    lcall->toString = &global_toString;
    intprtr->globals->defineEnv(intprtr->globals,"clock", (Object*)lcall);
}

void interpret(Interpreter* intprtr, StmtArray* array){
    CEXCEPTION_T e;
    Try{
        int i;
/*	   volatile Object* value;*/
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
	    ((Lox*)intrprtr->lox)->runtimeError(intrprtr->lox,e);
/*		fprintf(stderr,"RuntimeError: caught in executeBlock.: %s\n",e.message);*/
	}
	intrprtr->environment = previous;
}

static Object* visitCallExpr(ExprVisitor* visitor, Expr* expr){
    ObjectArray *arguments;
    Object* r,*callee;
    int i;
    LoxCallable *function;
    Expr* argument;
    callee = evaluate(visitor,((Call*)expr)->callee);
    arguments = malloc(sizeof(ObjectArray));
    init_ObjectArray(arguments);
   /* argument = ((Call*)expr)->arguments->getElementInArrayAt(arguments,0);*/
    for(i = 0;i<((Call*)expr)->arguments->used;i++){
    	argument = ((Call*)expr)->arguments->getElementInArrayAt(((Call*)expr)->arguments,i);

    	r = getObjectReference(evaluate(visitor,argument));
    	arguments->addElementToArray(arguments,r);
    }
    if(!(strcmp(callee->instanceOf,"LoxCallable")==0)){
	   CEXCEPTION_T e;
	   e.id=20;
	   e.message = "Can only call functions and classes.";
	   e.token = ((Call*)expr)->paren;
	   Throw(e);
    }
   function = ((LoxCallable*)(callee));
    if(arguments->size != function->arity(function)){
	   CEXCEPTION_T e;
	   e.id= 21;
	   e.message = "Expected function.arity(function) arguments but got arguments.size.";
	   e.token = ((Call*)expr)->paren;
	   Throw(e);
    }
    return function->call(function,(Interpreter*)visitor,arguments);
}


Object* visitWhileStmt(StmtVisitor* visitor, Stmt* stmt){
/*    Object* r = NULL;*/
	while(isTruthy(evaluate(&visitor->expr,((While*)stmt)->condition))->value.number){
		execute((Interpreter*)visitor,((While*)stmt)->body);
	}

/*	r->type = KNULL;*/
	return NULL;
}

Object* visitBlockStmt(StmtVisitor * visitor, Stmt* expr){
	Block* temp;
	Interpreter* intrprtr;
	Environment* env;
/*  Object* r = NULL;*/
	temp= (Block*)expr;
	intrprtr = (Interpreter*) visitor;
	env = malloc(sizeof(Environment));
	init_EnvironmentwithEnclosing(env,intrprtr->environment);
	executeBlock(intrprtr ,temp->statements,env);
    deleteEnvironment(env);
    env = NULL;
/*	r->type = KNULL;*/
	return NULL;
}
Object* visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr){
    Object* r = NULL;
    r = copyObject(((Literal*)expr)->value);
/*    r->type = ((Literal*)expr)->value->type;
	switch(r->type){
	case TRUE:
	case FALSE:
	case NUMBER:
		   r->value.number = (double)((Literal*)expr)->value->value.number;
		break;
	case NIL:
	case STRING:
		r->value.string = ((Literal*)expr)->value->value.string;
		break;
	default: break;
	}*/
	return r;
}
Object* visitGroupingExprInterpreter(ExprVisitor* visitor, Expr* expr){
	return evaluate(visitor,((Grouping*)expr)->expression);
}
Object* visitUnaryExprInterpreter(ExprVisitor* visitor, Expr* expr){
    Object* right,*result;
	right = evaluate(visitor,((Unary*)expr)->right);
	result = NULL;
	result = malloc(sizeof(Object));
	init_Object(result,NULL,KNULL);
	switch(((Unary*)expr)->operator->type){
		case BANG:
		   result->type = BOOLEAN;
		   result->value.number = ! (double)isTruthy(right)->value.number;
		   return result;
		case MINUS:
		   checkNumberOperand(((Unary*)expr)->operator,right);
		   result->type = NUMBER;
   		   result->value.number = - (double) right->value.number;
			return  result;
	    default: break;
	}
	return result;
}

Object* evaluate(ExprVisitor* visitor, Expr* expr){
	return expr->vtable.accept(expr,visitor);
}
Object* isTruthy(Object* obj){
    Object* r;
    r = NULL;
    r = malloc(sizeof(Object));
    init_Object(r,"",KNULL);
    if(obj->type == BOOLEAN || obj->type == FALSE || obj->type == TRUE || obj->type == NUMBER){
	   r->type = BOOLEAN;
	   r->value.number = (double)obj->value.number;
    }
    else if(obj->type == NIL){
 	   r->type = BOOLEAN;
 	   r->value.number = 0;
    }
    else{
	   r->type = BOOLEAN;
	   r->value.number = 1;
    }
    return r;
}

Object* visitBinaryExprInterpreter(ExprVisitor* visitor, Expr* expr){
    CEXCEPTION_T e;
    Object* left, *right, *result;
    left = evaluate(visitor,((Binary*)expr)->left);
    right = evaluate(visitor,((Binary*)expr)->right);
    result = NULL;
    result =malloc(sizeof(Object));
    init_Object(result,"",KNULL);
    switch(((Binary*)expr)->operator->type){
	  case MINUS:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result->type = NUMBER;
		  result->value.number = (double)left->value.number - (double)right->value.number;
		  return result;
	   case PLUS:
		  if(left->type == NUMBER && right->type == NUMBER){
			 result->type = NUMBER;
			 result->value.number = (double)left->value.number + (double)right->value.number;
			 return result;
		  }
		  if((left->type == STRING && right->type == STRING) ||(left->type == STRING && right->type == NUMBER) ||
			(left->type == NUMBER && right->type == STRING)){
			 char* new_str, *num_str;
			 new_str = NULL;
			 result->type = STRING;
			 switch(left->type){
				case STRING:
				    switch(right->type){
					   case STRING:
						  new_str = malloc(sizeof(char)*(strlen(left->value.string)+strlen(right->value.string)+1));
						  memset(new_str,0,strlen(left->value.string)+strlen(right->value.string)+1);
						  strncpy(new_str,left->value.string,strlen(left->value.string));
						  strncat(new_str,right->value.string,strlen(right->value.string));
						  result->value.string = new_str;
						  break;
					   default:
						  num_str = stringify(right);
						  new_str = malloc(sizeof(char)*(strlen(num_str)+strlen(left->value.string)+1));
						  memset(new_str,0,strlen(num_str)+strlen(left->value.string)+1);
						  strncpy(new_str,left->value.string,strlen(left->value.string));
						  strncat(new_str,num_str,strlen(num_str));
						  result->value.string = new_str;
						  free(num_str);
						  num_str = NULL;
						  break;
				    }
				    break;
				default:
				    num_str = stringify(left);
				    new_str = malloc(sizeof(char)*(strlen(num_str)+strlen(right->value.string)+1));
				    memset(new_str,0,strlen(num_str)+strlen(right->value.string)+1);
				    strncpy(new_str,num_str,strlen(num_str));
				    strncat(new_str,right->value.string,strlen(right->value.string));
				    result->value.string = new_str;
				    free(num_str);
				    num_str = NULL;
				    break;
			 }
			 return result;
		  }
		  e.id = 4;
		  e.token = ((Binary*)expr)->operator;
		  e.message = "Operands must be two numbers, two strings, or one of each";
		  delete_Object(&result);
		  result = NULL;
		  Throw(e);
		  break;
	   case SLASH:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  if(right->value.number == (double)0){
			 e.id = 5;
			 e.token = ((Binary*)expr)->operator;
			 e.message = "Cannot divide by Zero";
			  delete_Object(&result);
			  result = NULL;
			 Throw(e);
			 return result;
		  }
		  result->type = NUMBER;
		  result->value.number = (double)left->value.number / (double)right->value.number;
		  return result;
	   case STAR:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result->type = NUMBER;
		  result->value.number = (double)left->value.number * (double)right->value.number;
		  return result;
	   case GREATER:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result->type = BOOLEAN;
		  result->value.number = (double)left->value.number > (double)right->value.number;
		  return result;
	   case GREATER_EQUAL:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result->type = BOOLEAN;
		  result->value.number = (double)left->value.number >= (double)right->value.number;
		  return result;
	   case LESS:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result->type = BOOLEAN;
		  result->value.number = (double)left->value.number < (double)right->value.number;
		  return result;
	   case LESS_EQUAL:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  result->type = BOOLEAN;
		  result->value.number = (double)left->value.number <= (double)right->value.number;
		  return result;
	   case BANG_EQUAL:
		  result->type = BOOLEAN;
		  result->value.number = !isEqual(left,right);
		  return result;
	   case EQUAL_EQUAL:
		  result->type = BOOLEAN;
		  result->value.number = isEqual(left,right);
		  return result;
	   default: break;
    }
    return result;
}

int isEqual(Object* left, Object* right){
    if(left->type == KNULL && right->type == KNULL)
	   return 1;
    if(left->type == NIL && right->type == NIL)
	   return 1;
    if(left->type == KNULL || left->type == NIL)
	   return 0;
    if(right->type == KNULL || right->type == NIL)
	   return 0;
/*    if((left.type == FALSE || left.type==TRUE) && (right.type == FALSE || right.type==TRUE))
	   return strncmp(left.value.string,right.value.string,strlen(left.value.string)) == 0;*/
    switch(left->type){
	   case NUMBER:
	   case BOOLEAN:
	   case TRUE:
	   case FALSE:
		  switch(right->type){
			 case NUMBER:
			 case BOOLEAN:
			 case FALSE:
			 case TRUE:
				    return left->value.number == right->value.number;
				break;
			 default: return 0;break;
		  }
		  break;
	   case STRING:
		  switch(right->type){
			 case STRING:
				{
					size_t length = strlen(left->value.string);
					if(length < strlen(right->value.string))
							length = strlen(right->value.string);
				    return strncmp(left->value.string,right->value.string,length) == 0;
				}
				break;
			 
			 default: return 0; break;
		  }
	   break;
	   default: break;
    }
    return 0;
}
void checkNumberOperand(Token* operator,Object* right){
    CEXCEPTION_T e;
    if(right->type == NUMBER) return;
    e.id = 2;
    e.token = operator;
    e.message = "Operand must be a number";
    Throw(e);
}
void checkNumberOperands(Token* operator, Object* left, Object* right){
    CEXCEPTION_T e;
    if(left->type == NUMBER && right->type == NUMBER)
		  return;
    e.id = 3;
    e.token = operator;
    e.message = "Operands must be numbers";
    Throw(e);
}
const static char* nil = "nil";

char* stringify(Object* obj){
    char* text;
    text = NULL;
    if( obj->type == KNULL)
	   return (char*)nil;
    if(obj->type == FALSE)
	   return "false";
    if(obj->type == TRUE)
	   return "true";
    if(obj->type == BOOLEAN){
	   switch((int)obj->value.number){
		  case 1:
			 return "true";
		  case 0:
			 return "false";
		  default:
			 return "boolean error";
	   }
    }
    if(obj->type == NUMBER){
	   char* text;
	   text = NULL;
	   asprintf(&text,"%.1lf",(double)obj->value.number);
	   if(strlen(text)>=2 && text[strlen(text)-1]== '0'
		 && text[strlen(text)-2] == '.'){
		  free(text);
		  text = NULL;
		  asprintf(&text,"%.0lf",(double)obj->value.number);
		  return text;
	   }
	   free(text);
	   text = NULL;
	   asprintf(&text,"%lf",(double)obj->value.number);

	   return text;
    }
/*    asprintf(&text,"%lf",(double)obj.value.number);*/

    return obj->value.string;
}
Object* visitLogicalExpr(ExprVisitor* visitor, Expr* expr){
	Object* left;
	Logical* log;
	log = (Logical*) expr;
	left = evaluate(visitor,log->left);
	if(log->operator->type == OR){
		if(isTruthy(left)->value.number)
		    return left;
	       }
	else{
	    if(!isTruthy(left)->value.number){
			return left;
		}
	}
	return evaluate(visitor,log->right);
}

Object* visitIfStmt(StmtVisitor * visitor, Stmt* expr){
    Object* r,*a;
    If* stmt;
    r = NULL;
    r = malloc(sizeof(Object));
    init_Object(r,"",KNULL);
    stmt = (If*) expr;
    a = evaluate(&visitor->expr,stmt->condition);
    if(isTruthy(a)->value.number ){
		execute((Interpreter*)visitor,stmt->thenBranch);
	}
	else if(stmt->elseBranch != NULL){
		execute((Interpreter*)visitor,stmt->elseBranch);
	}
	return r;
}


Object* visitExpressionStmt(StmtVisitor* visitor, Stmt* stmt){
    Object* result = NULL;
    result = malloc(sizeof(Object));
    init_Object(result,"",KNULL);
	evaluate(&visitor->expr,((Expression*)stmt)->expression);
	return result;
}

Object* visitPrintStmt(StmtVisitor* visitor, Stmt* stmt){
    Object* result;
    char * val;
    result = NULL;
    result = evaluate(&visitor->expr,((Print*)stmt)->expression);
    val = stringify(result);
	printf("%s\n",val);
    if(result->type == NUMBER){
		  free(val);
		  val = NULL;
    }
	result->type = KNULL;
	return getObjectReference(result);
}

Object* visitVarStmt(StmtVisitor* visitor, Stmt* stmt){
	Interpreter* intprtr;
    Object* r,*a;
	Var* vstmt;
	intprtr = (Interpreter*) visitor;
	vstmt = (Var*)stmt;
	a = NULL;
	r = NULL;
    r = malloc(sizeof(Object));
	init_Object(r,"",KNULL);
	if(vstmt->initializer != NULL){
		a = evaluate(&visitor->expr,vstmt->initializer);
		r->type = a->type;
		 if(r->type == NUMBER)
			 r->value.number = a->value.number;
		 else if(r->type == STRING){
			r->value.string = strdup(a->value.string);
		 }
		 else {
			 r->value.callable = memcpy(malloc(sizeof(LoxCallable)),a->value.callable,sizeof(LoxCallable));
		 }
	}
    intprtr->environment->defineEnv(intprtr->environment,strdup(vstmt->name->lexeme),r);
	a = malloc(sizeof(Object));
    init_Object(a,"nil",NIL);
	a->type = NIL;
	return a;
}
Object* visitAssignExpr(ExprVisitor * visitor,Expr* expr){
	Assign* assign;
	Object* r,*a;
	Interpreter* intprtr;
	assign = (Assign*) expr;
	r = malloc(sizeof(Object));
	init_Object(r,"",KNULL);
	intprtr= (Interpreter*)visitor;
	a = evaluate(visitor,assign->value);
	r->type = a->type;
    if(r->type == NUMBER)
 	   r->value.number = a->value.number;
    else if(r->type == STRING){
	   r->value.string = strdup(a->value.string);
	   /*	   memcpy(&r->value.string,&a.value.string,strlen(a.value.string)+1);*/
    }
    else{
    	r->value.callable = memcpy(malloc(sizeof(LoxCallable)),a->value.callable,sizeof(LoxCallable));
    }

/*	r->value = a->value;*/
    intprtr->environment->assign(intprtr->environment,assign->name,r);
	return getObjectReference(a);
}

Object* visitVariableExpr(ExprVisitor* visitor, Expr* expr){
	Environment* env;
    Object* r,*a;
    a = r = NULL;
    a = malloc(sizeof(Object));
    init_Object(a,"",KNULL);
	env = (((Interpreter*)visitor)->environment);
	r = (env->get(env,((Variable*)expr)->name ));
    if(r){
    	a->type = r->type;
    	if(a->type == NUMBER)
    		a->value.number = r->value.number;
    	else if(a->type == STRING){
    		a->value.string = strdup(r->value.string);
    	}
    	else {
    		a->value.callable = memcpy(malloc(sizeof(LoxCallable)),r->value.callable,sizeof(LoxCallable));
    	}
    	return a;
    }
    a->type = KNULL;
    a->value.number=0;
	return a;
}
