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
#include <assert.h>
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
#include "LoxFunction.h"
#ifndef _STMTARRAY
#define _STMTARRAY
	typedef struct _StmtArray StmtArray;
	extern delete_StmtArray(StmtArray* array);
#endif
static Object *NLLOBJ;
#include "StmtArray.h"

static Object* visitReturnStmt(StmtVisitor* visitor, Stmt* stmt);
static Object* visitFunctionStmt(StmtVisitor* visitor, Stmt* stmt);
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
    double x;
    gettimeofday(&tv,NULL);
    x =  ((double)tv.tv_sec)+ (((double)tv.tv_usec / (double)1000.0)/ (double)1000.0);
    obj = new(OBJECTIVE,sizeof(Object));
    init_Object(obj,&x,NUMBER);
    return obj;
    /*    return (double)system currentime in millisceons / 1000.0  */
}
static char* global_toString(LoxCallable* inloxcall){
    char * temp;
    temp = new(RAW,sizeof(char)*(strlen("<native fn>")+1));
    memset(temp,0,strlen("<native fn>")+1);
    strncpy(temp,"<native fn>",strlen("<native fn>"));
    return temp;
}
void init_Interpreter(Interpreter* intprtr, void* lox){
    LoxCallable* lcall;
    char* clockname;
    intprtr->lox = lox;
    intprtr->globals = new(OBJECTIVE,sizeof(Environment));
    init_Environment(intprtr->globals);
    intprtr->environment = intprtr->globals;
    intprtr->resolve = &resolve_Interpreter;
	intprtr->super.expr.vtable.visitLiteralExpr = &visitLiteralExprInterpreter;
	intprtr->super.vtable.visitFunctionStmt = &visitFunctionStmt;
    intprtr->super.vtable.visitReturnStmt = &visitReturnStmt;
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
    intprtr->evaluate = &evaluate;
    intprtr->executeBlock = &executeBlock;
    intprtr->checkNumberOperand = &checkNumberOperand;
    intprtr->checkNumberOperands = &checkNumberOperands;
    intprtr->interpret = &interpret;
    intprtr->isEqual = &isEqual;
    intprtr->isTruthy = &isTruthy;
    intprtr->stringify = &stringify;
    NLLOBJ = new(OBJECTIVE,sizeof(Object));
    init_Object(NLLOBJ,"",KNULL);
    lcall = new(OBJECTIVE,sizeof(LoxCallable));
    init_LoxCallable(lcall);
    lcall->super.type = FUN;
    lcall->super.value.string = new(RAW,sizeof(char)*(strlen("clock")+1));
    memset(lcall->super.value.string,0,strlen("clock")+1);
    strncpy(lcall->super.value.string,"clock",strlen("clock"));
/*    init_Object(&lcall->super,"clock",FUN);*/
    memset(&lcall->super.instanceOf,0,30);
    strncpy((char*)&lcall->super.instanceOf,"LoxCallable",strlen("LoxCallable"));
    lcall->vtable.arity = &global_clock_arity;
    lcall->vtable.call = &global_clock_call;
    lcall->vtable.toString = &global_toString;
    clockname = new(RAW,sizeof(char)*(strlen("clock")+1));
    memset(clockname,0,strlen("clock")+1);
    strncpy(clockname,"clock",strlen("clock"));
    intprtr->globals->defineEnv(intprtr->globals,clockname, (Object*)lcall);
}

void interpret(Interpreter* intprtr, StmtArray* array){
    CEXCEPTION_T e;
    e.id = 0;
    e.message = NULL;
    e.sub = NULL;
    e.token = NULL;
    Try{
        int i;
/*	   volatile Object* value;*/
	   for(i=0;i<array->used;i++){
		  Stmt* stmt = getStmtinArrayAt(array,i);
		   execute(intprtr,stmt);
	   }
    }
    Catch(e){
/*	   if(e.id != 50)*/
	   ((Lox*)intprtr->lox)->runtimeError(intprtr->lox,e);
/*	   Throw(e);*/
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
    e.id=0;
    e.message= NULL;
    e.sub=NULL;
    e.token=NULL;
	previous = getReference(intrprtr->environment);
	Try{
		intrprtr->environment = getReference(newenv);

		for(i = 0; i<array->used;i++){
			execute(intrprtr,array->getElementInArrayAt(array,i));
		}
	}
	Catch(e){
	    if(e.id != 50){
		   delete(intrprtr->environment);
			 intrprtr->environment = getReference(previous);
			 ((Lox*)intrprtr->lox)->runtimeError(intrprtr->lox,e);
	    }
	    else{
		   delete(intrprtr->environment);
		   intrprtr->environment = getReference(previous);
		   Throw(e);
	    }
/*		fprintf(stderr,"RuntimeError: caught in executeBlock.: %s\n",e.message);*/
	}
	delete(intrprtr->environment);
	intrprtr->environment = getReference(previous);
}
void resolve_Interpreter(Interpreter* intrprtr, Expr* expr, int depth){
/*	intrprtr->locals.put(intrprtr->locals,expr,depth);*/
}


static Object* visitReturnStmt(StmtVisitor* visitor, Stmt* stmt){
    CEXCEPTION_T e;
	 Return_exception *re;
	Return* ret;
	Object* value;
	ret = (Return*) stmt;
	value = NULL;
	if(ret->value != NULL)
		value = evaluate(&visitor->expr,ret->value);
	re = new(RAW,sizeof(Return_exception));
	e.id = 50;
	e.message = "";
	e.token = NULL;
    e.sub = re;
	e.sub->value = (value);
    Try{
	Throw(e);
    }
    Catch(e){ Throw(e);}
	return NULL;
}

static Object* visitFunctionStmt(StmtVisitor* visitor, Stmt* stmt){
	LoxFunction * func;
	Environment* env;
	Function* function;
	Interpreter* intprtr;
	func = new(OBJECTIVE,sizeof(LoxFunction));
	function = (Function*) stmt;
	intprtr = (Interpreter*) visitor;
	env = intprtr->environment;
	init_LoxFunctionWithClosure(func,function,env);
	env->defineEnv(env,getReference(function->name->lexeme),getReference(func));
    delete(func);
	return NULL;

}

static Object* visitCallExpr(ExprVisitor* visitor, Expr* expr){
    ObjectArray *arguments;
    Object* r,*callee, *res;
    int i;
    LoxFunction *function;
    Expr* argument;
    callee = evaluate(visitor,((Call*)expr)->callee);
    arguments = new(OBJECTIVE,sizeof(ObjectArray));
    init_ObjectArray(arguments);
   /* argument = ((Call*)expr)->arguments->getElementInArrayAt(arguments,0);*/
    for(i = 0;i<((Call*)expr)->arguments->used;i++){
    	argument = ((Call*)expr)->arguments->getElementInArrayAt(((Call*)expr)->arguments,i);

    	r = getReference(evaluate(visitor,argument));
/*	   assert(r->value.number != -1);*/
    	arguments->addElementToArray(arguments,(r));
    }
    if(!(strcmp(callee->instanceOf,"LoxCallable")==0) && !(strcmp(callee->instanceOf,"LoxFunction")==0)){
	   CEXCEPTION_T e;
	   e.id=20;
	   e.message = "Can only call functions and classes.";
	   e.token = ((Call*)expr)->paren;
	   e.sub = NULL;
	   Throw(e);
    }
   function = ((LoxFunction*)(callee));
    if(arguments->used != function->super.vtable.arity((LoxCallable*)function)){
	   CEXCEPTION_T e;
	   char * new_str,*temp_str;
	   new_str = NULL;
	   e.id= 21;
	   asprintf(&new_str,"Expected %d arguments but got %d.",function->super.vtable.arity((LoxCallable*)function),arguments->size);
	   temp_str = new(RAW,sizeof(char)*(strlen(new_str)+1));
	   memset(temp_str,0,strlen(new_str)+1);
	   strncpy(temp_str,new_str,strlen(new_str));
	   e.message = temp_str;
	   e.token = ((Call*)expr)->paren;
	   e.sub = NULL;
	   free(new_str);
	   new_str = NULL;
	   Throw(e);
    }
    res = function->super.vtable.call((LoxCallable*)function,(Interpreter*)visitor,getReference(arguments));
    delete(function);
    delete(arguments);
    function = NULL;
    return res;
}


Object* visitWhileStmt(StmtVisitor* visitor, Stmt* stmt){
    Object *temp;
   Object* r = NULL;
    temp = evaluate(&visitor->expr,((While*)stmt)->condition);
    r = isTruthy(temp);
	while(r->value.number){
		delete(temp);
		delete(r);
		execute((Interpreter*)visitor,((While*)stmt)->body);
	    temp = evaluate(&visitor->expr,((While*)stmt)->condition);
	    r = isTruthy(evaluate(&visitor->expr,((While*)stmt)->condition));
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
	env = new(OBJECTIVE,sizeof(Environment));
	init_EnvironmentwithEnclosing(env,intrprtr->environment);
	executeBlock(intrprtr ,temp->statements,env);
   deleteEnvironment(env);
    env = NULL;
/*	r->type = KNULL;*/
	return NULL;
}
Object* visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr){
    Object* r = NULL;
/*    r = copy(((Literal*)expr)->value);*/
    r = getReference(((Literal*)expr)->value);
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
    Object* right,*result,*truth;
	right = evaluate(visitor,((Unary*)expr)->right);
	result = NULL;
	result = new(OBJECTIVE,sizeof(Object));
	init_Object(result,NULL,KNULL);
	switch(((Unary*)expr)->operator->super.type){
		case BANG:
		   result->type = BOOLEAN;
		   truth = isTruthy(right);
		   result->value.number = ! (double)truth->value.number;
		   delete(truth);
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
    r = new(OBJECTIVE,sizeof(Object));
    init_Object(r,NULL,KNULL);
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
    result =new(OBJECTIVE,sizeof(Object));
    init_Object(result,"",KNULL);
    switch(((Binary*)expr)->operator->super.type){
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
						  new_str = new(RAW,sizeof(char)*(strlen(left->value.string)+strlen(right->value.string)+1));
						  memset(new_str,0,strlen(left->value.string)+strlen(right->value.string)+1);
						  strncpy(new_str,left->value.string,strlen(left->value.string));
						  strncat(new_str,right->value.string,strlen(right->value.string));
						  result->value.string = new_str;
						  break;
					   default:
						  num_str = stringify(right);
						  new_str = new(RAW,sizeof(char)*(strlen(num_str)+strlen(left->value.string)+1));
						  memset(new_str,0,strlen(num_str)+strlen(left->value.string)+1);
						  strncpy(new_str,left->value.string,strlen(left->value.string));
						  strncat(new_str,num_str,strlen(num_str));
						  result->value.string = new_str;
						  delete(num_str);
						  num_str = NULL;
						  break;
				    }
				    break;
				default:
				    num_str = stringify(left);
				    new_str = new(RAW,sizeof(char)*(strlen(num_str)+strlen(right->value.string)+1));
				    memset(new_str,0,strlen(num_str)+strlen(right->value.string)+1);
				    strncpy(new_str,num_str,strlen(num_str));
				    strncat(new_str,right->value.string,strlen(right->value.string));
				    result->value.string = new_str;
				    delete(num_str);
				    num_str = NULL;
				    break;
			 }
			 return result;
		  }
		  e.id = 4;
		  e.token = ((Binary*)expr)->operator;
		  e.message = "Operands must be two numbers, two strings, or one of each";
		  e.sub = NULL;
		  delete(result);
		  result = NULL;
		  Throw(e);
		  break;
	   case SLASH:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  if(right->value.number == (double)0){
			 e.id = 5;
			 e.token = ((Binary*)expr)->operator;
			 e.message = "Cannot divide by Zero";
			  e.sub = NULL;
			  delete(result);
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
    e.sub = NULL;
    Throw(e);
}
void checkNumberOperands(Token* operator, Object* left, Object* right){
    CEXCEPTION_T e;
    if(left->type == NUMBER && right->type == NUMBER)
		  return;
    e.id = 3;
    e.token = operator;
    e.message = "Operands must be numbers";
    e.sub = NULL;
    Throw(e);
}
const static char* nil = "nil";

char* stringify(Object* obj){
    char* text;
    char* temp;
    text = NULL;
    if(obj == NULL || obj->type == KNULL){
	   temp = new(RAW,sizeof(char)*(strlen(nil)+1));
	   memset(temp,0,strlen(nil)+1);
	   strncpy(temp,nil,strlen(nil));
	   return (char*)temp;
    }
    if(obj->type == FALSE){
	   temp = new(RAW,sizeof(char)*(strlen("false")+1));
	   memset(temp,0,strlen("false")+1);
	   strncpy(temp,"false",strlen("false"));
	   return (char*)temp;
    }
    if(obj->type == TRUE){
	   temp = new(RAW,sizeof(char)*(strlen("true")+1));
	   memset(temp,0,strlen("true")+1);
	   strncpy(temp,"true",strlen("true"));
	   return (char*)temp;
    }
    if(obj->type == BOOLEAN){
	   switch((int)obj->value.number){
		  case 1:
			 temp = new(RAW,sizeof(char)*(strlen("true")+1));
				memset(temp,0,strlen("true")+1);
				strncpy(temp,"true",strlen("true"));
				return (char*)temp;
		  case 0:
			 temp = new(RAW,sizeof(char)*(strlen("false")+1));
			 memset(temp,0,strlen("false")+1);
			 strncpy(temp,"false",strlen("false"));
			 return (char*)temp;
		  default:
			 temp = new(RAW,sizeof(char)*(strlen("boolean error")+1));
			 memset(temp,0,strlen("boolean error")+1);
			 strncpy(temp,"boolean error",strlen("boolean error"));
			 return (char*)temp;
	   }
    }
    if(obj->type == FUN){
	   return ((LoxFunction*)obj)->super.vtable.toString((LoxCallable*)((LoxFunction*)obj)) ;
    }
    if(obj->type == NUMBER){
	   char* text, *temp_str;
	   text = NULL;
	   asprintf(&text,"%.1lf",(double)obj->value.number);
	   if(strlen(text)>=2 && text[strlen(text)-1]== '0'
		 && text[strlen(text)-2] == '.'){
		  free(text);
		  text = NULL;
		  asprintf(&text,"%.0lf",(double)obj->value.number);
		  temp_str = new(RAW,sizeof(char)*(strlen(text)+1));
		  memset(temp_str,0,strlen(text)+1);
		  strncpy(temp_str,text,strlen(text));
		  free(text);
		  text = NULL;
		  return temp_str;
	   }
	   free(text);
	   text = NULL;
	   asprintf(&text,"%lf",(double)obj->value.number);
	   temp_str = new(RAW,sizeof(char)*(strlen(text)+1));
	   memset(temp_str,0,strlen(text)+1);
	   strncpy(temp_str,text,strlen(text));
	   free(text);
	   text = NULL;
	   return temp_str;
    }
/*    asprintf(&text,"%lf",(double)obj.value.number);*/

    return copy(obj->value.string);
}
Object* visitLogicalExpr(ExprVisitor* visitor, Expr* expr){
	Object* left, *truth;
	Logical* log;
	log = (Logical*) expr;
	left = evaluate(visitor,log->left);
	if(log->operator->super.type == OR){
	    truth = isTruthy(left);
	    if(truth->value.number){
		   delete(truth);
		    return left;
	    }
	       }
	else{
	    truth = isTruthy(left);
	    if(!truth->value.number){
		   delete(truth);
			return left;
		}
	}
	return evaluate(visitor,log->right);
}

Object* visitIfStmt(StmtVisitor * visitor, Stmt* expr){
    Object* a,*truth;
    If* stmt;
/*    r = NULL;
    r = new(OBJECTIVE,sizeof(Object));
    init_Object(r,"",KNULL);*/
    stmt = (If*) expr;
    a = evaluate(&visitor->expr,stmt->condition);
    truth = isTruthy(a);
    if(truth->value.number ){
	   delete(a);
	   delete(truth);
		execute((Interpreter*)visitor,stmt->thenBranch);
	}
	else if(stmt->elseBranch != NULL){
		delete(a);
		delete(truth);
		execute((Interpreter*)visitor,stmt->elseBranch);
	}
	return NLLOBJ;
}


Object* visitExpressionStmt(StmtVisitor* visitor, Stmt* stmt){
	evaluate(&visitor->expr,((Expression*)stmt)->expression);
	return NLLOBJ;
}

Object* visitPrintStmt(StmtVisitor* visitor, Stmt* stmt){
    Object* result;
    char * val;
    result = NULL;
    result = evaluate(&visitor->expr,((Print*)stmt)->expression);
    val = stringify(result);
    printf("%s\n",val);
    delete(val);
    val = NULL;
    if(result)
	   return getReference(result);
    return NLLOBJ;
}

Object* visitVarStmt(StmtVisitor* visitor, Stmt* stmt){
	Interpreter* intprtr;
    Object* r,*a;
	Var* vstmt;
    char* new_str;
	intprtr = (Interpreter*) visitor;
	vstmt = (Var*)stmt;
	a = NULL;
	r = NULL;
    r = new(OBJECTIVE,sizeof(Object));
	init_Object(r,"",KNULL);
	if(vstmt->initializer != NULL){
		a = evaluate(&visitor->expr,vstmt->initializer);
/*		r->type = a->type;
		 if(r->type == NUMBER)
			 r->value.number = a->value.number;
		 else if(r->type == STRING){
			temp_str = new(RAW,sizeof(char)*(strlen(a->value.string)+1));
			memset(temp_str,0,strlen(a->value.string)+1);
			strncpy(temp_str,a->value.string,strlen(a->value.string));
			r->value.string = temp_str;
		 }
		 else {
			r = resize(OBJECTIVE,r,sizeof(LoxFunction));
			 r->value.callable = memcpy(new(OBJECTIVE,sizeof(LoxCallable)),a->value.callable,sizeof(LoxCallable));
		 }*/
	}
    new_str = new(RAW,sizeof(char)*(strlen(vstmt->name->lexeme)+1));
    memset(new_str,0,strlen(vstmt->name->lexeme)+1);
    strncpy(new_str,vstmt->name->lexeme,strlen(vstmt->name->lexeme));
    intprtr->environment->defineEnv(intprtr->environment,new_str,copy(a));
	a = new(OBJECTIVE,sizeof(Object));
    init_Object(a,"nil",NIL);
	a->type = NIL;
	return a;
}
Object* visitAssignExpr(ExprVisitor * visitor,Expr* expr){
	Assign* assign;
	Object* r,*a;
	Interpreter* intprtr;
    char * new_str;
	assign = (Assign*) expr;
	r = new(OBJECTIVE,sizeof(Object));
	init_Object(r,"",KNULL);
	intprtr= (Interpreter*)visitor;
	a = evaluate(visitor,assign->value);
	r->type = a->type;
    if(r->type == NUMBER)
 	   r->value.number = a->value.number;
    else if(r->type == STRING){
	   new_str = new(RAW,sizeof(char)*(strlen(a->value.string)+1));
	   memset(new_str,0,strlen(a->value.string)+1);
	   strncpy(new_str,a->value.string,strlen(a->value.string));
	   r->value.string = new_str;
	   /*	   memcpy(&r->value.string,&a.value.string,strlen(a.value.string)+1);*/
    }
    else{
    	r->value.callable = memcpy(new(OBJECTIVE,sizeof(LoxCallable)),a->value.callable,sizeof(LoxCallable));
    }

/*	r->value = a->value;*/
    intprtr->environment->assign(intprtr->environment,assign->name,copy(r));
	return getReference(r);
}

Object* visitVariableExpr(ExprVisitor* visitor, Expr* expr){
	Environment* env;
    Object* r,*a;
    a = NULL;
    r = NULL;
/*    a = new(OBJECTIVE,sizeof(Object));
    init_Object(a,"",KNULL);*/
	env = (((Interpreter*)visitor)->environment);
	r = (env->get(env,((Variable*)expr)->name ));
/*    if(r){*/
/*	   r = getObjectReference(r);*/
/*    	if(r->type == NUMBER)
    		a->value.number = r->value.number;
    	else if(r->type == STRING){
	    a->value.string = new(RAW,sizeof(char*)*(strlen(r->value.string)+1));
	    memset(a->value.string,0,strlen(r->value.string)+1);
    		strncpy(a->value.string ,r->value.string,strlen(r->value.string));
    	}
    	else {
	    if(strcmp(r->instanceOf,"LoxFunction")==0){
		   a = resize(OBJECTIVE,a,sizeof(LoxFunction));
		   init_LoxFunction((LoxFunction*)a,copy(((LoxFunction*)r)->declaration));
		   ((LoxFunction*)a)->closure = ((LoxFunction*)r)->closure;*/
/*
		   ((LoxFunction*)a)->declaration->body = ((LoxFunction*)r)->declaration->body;
		   ((LoxFunction*)a)->declaration->name = ((LoxFunction*)r)->declaration->name;
		   ((LoxFunction*)a)->declaration->params =((LoxFunction*)r)->declaration->params;
		   ((LoxFunction*)a)->declaration->super = ((LoxFunction*)r)->declaration->super;*/
/*	    }
	    else if(strcmp(r->instanceOf,"LoxCallable")==0){
		   a = resize(OBJECTIVE,a,sizeof(LoxCallable));
		   init_LoxCallable((LoxCallable*)a);
		   ((LoxCallable*)a)->super = ((LoxCallable*)r)->super;
		   ((LoxCallable*)a)->vtable = ((LoxCallable*)r)->vtable;
	    }*/
    	/*	a->value.callable = memcpy(malloc(sizeof(LoxCallable)),r->value.callable,sizeof(LoxCallable));*/
/*    	}
	   a->type = r->type;
	   memset((char*)a->instanceOf,0,30);
	   strncpy((char*)a->instanceOf,r->instanceOf,strlen(r->instanceOf));*/
	   a = (r);
    	return a;
/*    }*/
/*    a->type = KNULL;
    a->value.number=0;
	return NLLOBJ;*/
}
