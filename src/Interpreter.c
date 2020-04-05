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
#include "LoxClass.h"
#include "LoxInstance.h"
#include "str.h"
#ifndef _STMTARRAY
#define _STMTARRAY
	typedef struct _StmtArray StmtArray;
	extern void delete_StmtArray(StmtArray* array);
#endif
static Object *NLLOBJ;
#include "StmtArray.h"

static Object* visitSuperExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitThisExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitSetExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitGetExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitClassStmtInterpreter(StmtVisitor* visitor, Stmt* stmt);
static Object* visitReturnStmtInterpreter(StmtVisitor* visitor, Stmt* stmt);
static Object* visitFunctionStmtInterpreter(StmtVisitor* visitor, Stmt* stmt);
static Object* visitCallExprInterpreter(ExprVisitor* visitor, Expr* stmt);
static Object* visitWhileStmtInterpreter(StmtVisitor* visitor, Stmt* stmt);
static Object* visitLogicalExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitIfStmtInterpreter(StmtVisitor * visitor, Stmt* expr);
static Object* visitBlockStmtInterpreter(StmtVisitor * visitor, Stmt* expr);
static Object* visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitGroupingExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitUnaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitBinaryExprInterpreter(ExprVisitor* visitor, Expr* expr);
static Object* visitAssignExprInterpreter(ExprVisitor * visitor,Expr* expr);
static Object* visitExpressionStmtInterpreter(StmtVisitor* visitor, Stmt* stmt);
static Object* visitPrintStmtInterpreter(StmtVisitor* visitor, Stmt* stmt);
static Object* visitVarStmtInterpreter(StmtVisitor* visitor,Stmt* stmt);
static Object* visitVariableExprInterpreter(ExprVisitor* visitor, Expr* stmt);
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
    temp = NULL;
    temp = strcopy(temp,"<native fn>");
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
	intprtr->super.vtable.visitFunctionStmt = &visitFunctionStmtInterpreter;
    intprtr->super.vtable.visitReturnStmt = &visitReturnStmtInterpreter;
	intprtr->super.expr.vtable.visitGroupingExpr = &visitGroupingExprInterpreter;
    intprtr->super.expr.vtable.visitGetExpr = &visitGetExprInterpreter;
    intprtr->super.expr.vtable.visitSetExpr = &visitSetExprInterpreter;
	intprtr->super.expr.vtable.visitThisExpr = &visitThisExprInterpreter;
	intprtr->super.expr.vtable.visitUnaryExpr = &visitUnaryExprInterpreter;
    intprtr->super.expr.vtable.visitBinaryExpr = &visitBinaryExprInterpreter;
    intprtr->super.expr.vtable.visitVariableExpr = &visitVariableExprInterpreter;
    intprtr->super.expr.vtable.visitAssignExpr = &visitAssignExprInterpreter;
    intprtr->super.expr.vtable.visitLogicalExpr = &visitLogicalExprInterpreter;
    intprtr->super.expr.vtable.visitSuperExpr = &visitSuperExprInterpreter;
    intprtr->super.vtable.visitExpressionStmt =&visitExpressionStmtInterpreter;
    intprtr->super.expr.vtable.visitCallExpr = &visitCallExprInterpreter;
    intprtr->super.vtable.visitWhileStmt = &visitWhileStmtInterpreter;
    intprtr->super.vtable.visitIfStmt = &visitIfStmtInterpreter;
    intprtr->super.vtable.visitBlockStmt = &visitBlockStmtInterpreter;
    intprtr->super.vtable.visitPrintStmt = &visitPrintStmtInterpreter;
    intprtr->super.vtable.visitVarStmt = &visitVarStmtInterpreter;
    intprtr->super.vtable.visitClassStmt = &visitClassStmtInterpreter;
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
    lcall->super.value.string = NULL;
    lcall->super.value.string = strcopy(lcall->super.value.string,"clock");
    setInstanceOf(&lcall->super,"LoxCallable");
/*    memset(&lcall->super.instanceOf,0,30);
    strncpy((char*)&lcall->super.instanceOf,"LoxCallable",strlen("LoxCallable"));*/
    lcall->vtable.arity = &global_clock_arity;
    lcall->vtable.call = &global_clock_call;
    lcall->vtable.toString = &global_toString;
    clockname = NULL;
    clockname = strcopy(clockname,"clock");
    intprtr->globals->defineEnv(intprtr->globals,clockname, (Object*)lcall);
    intprtr->locals = create_ExprIntHashMap(40);
}

void interpret(Interpreter* intprtr, StmtArray* array){
    CEXCEPTION_T e;
    e = create_exception(0,NULL,NULL,NULL);
    Try{
        int i;
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
    e = create_exception(0,NULL,NULL,NULL);
	previous = getReference(intrprtr->environment);
	Try{
		intrprtr->environment = getReference(newenv);

		for(i = 0; i<array->used;i++){
			execute(intrprtr,array->getElementInArrayAt(array,i));
		}
	}
	Catch(e){
	    if(e.id != 50){
			 intrprtr->environment = getReference(previous);
			 ((Lox*)intrprtr->lox)->runtimeError(intrprtr->lox,e);
	    }
	    else{
		   intrprtr->environment = getReference(previous);
		   Try{
		    Throw(e);
		   }
		   Catch(e){ Throw(e);}
	    }
	}
	intrprtr->environment = getReference(previous);
}
void resolve_Interpreter(Interpreter* intrprtr, Expr* expr, int* depth){
	intrprtr-> locals->super.super.vtable.add_to_hash((struct _HASH*)intrprtr->locals,copy(expr),copy(depth));
}


static Object* visitReturnStmtInterpreter(StmtVisitor* visitor, Stmt* stmt){
    CEXCEPTION_T e;
	 Return_exception *re;
	Return* ret;
	Object* value;
	ret = (Return*) stmt;
	value = NULL;
	if(ret->value != NULL)
		value = evaluate(&visitor->expr,ret->value);
	re = new(RAW,sizeof(Return_exception));
    e = create_exception(50,NULL,"",re);
	e.sub->value = (value);
    Try{
	Throw(e);
    }
    Catch(e){ Throw(e);}
	return NULL;
}

static Object* visitClassStmtInterpreter(StmtVisitor* visitor, Stmt* stmt){
	Object* superclass;
	Class* cls;
	Interpreter* interpreter;
    StrObjHashMap* methods;
    Environment* env;
    int *x , i ;
    LoxClass* klass;
    superclass = NULL;
    cls = (Class*) stmt;
    interpreter = (Interpreter*)visitor;
    env = interpreter->environment;
    if(cls->superclass != NULL){
    	superclass = evaluate((ExprVisitor*)visitor,(Expr*)cls->superclass);
    	if(!(strcmp(superclass->instanceOf,"LoxClass")==0)){
    		CEXCEPTION_T e;
	    e = create_exception(60,cls->superclass->name,"Superclass must be a class.",NULL);
		Try{
		 Throw(e);
		}
		Catch(e){ Throw(e);}
    		return NULL;
    	}
    }
    x = new(RAW, sizeof(int));
    *x = 0;
    env->defineEnv(env,cls->name->lexeme,(Object*)x);
    if(cls->superclass != NULL){
    	Environment* temp;
    	char* super;
	   super = NULL;
	   super = strcopy(super,"super");
    	temp = new(OBJECTIVE,sizeof(Environment));
    	init_EnvironmentwithEnclosing(temp,env);
    	env = temp;
    	env->defineEnv(env,super,superclass);
    }
    methods = new(OBJECTIVE,sizeof(StrObjHashMap));
    init_StrObjhm(methods,40);
    for(i=0;i <((Class*)stmt)->methods->used;i++){
    	LoxFunction* function;
	   Stmt* cur_stmt = getStmtinArrayAt(((Class*)stmt)->methods,i);
    	function = new(OBJECTIVE,sizeof(LoxFunction));
    	init_LoxFunctionWithClosureAndInitializer((LoxFunction*)function,(Function*)cur_stmt,env,strcmp(((Function*)cur_stmt)->name->lexeme,"init")==0);
    	methods->super.super.vtable.add_to_hash((struct _HASH*)methods,
    			((Function*)cur_stmt)->name->lexeme,function);

    }


    klass = new(OBJECTIVE,sizeof(LoxClass));
    init_LoxClassWithMethodsAndSuper(klass,((Class*)stmt)->name->lexeme,(LoxClass*)superclass,methods);
    if(superclass != NULL){
    	env = env->Enclosing;
    }
    env->assign(env,((Class*)stmt)->name,(Object*)klass);
    return NULL;
}


static Object* visitFunctionStmtInterpreter(StmtVisitor* visitor, Stmt* stmt){
	LoxFunction * func;
	Environment* env;
	Function* function;
	Interpreter* intprtr;
	func = new(OBJECTIVE,sizeof(LoxFunction));
	function = (Function*) stmt;
	intprtr = (Interpreter*) visitor;
	env = intprtr->environment;
	init_LoxFunctionWithClosureAndInitializer(func,function,env,0);
	env->defineEnv(env,getReference(function->name->lexeme),getReference(func));
    delete(func);
	return NULL;

}

static Object* visitCallExprInterpreter(ExprVisitor* visitor, Expr* expr){
    ObjectArray *arguments;
    Object* r,*callee, *res;
    int i;
    LoxFunction *function;
    Expr* argument;
    callee = evaluate(visitor,((Call*)expr)->callee);
    arguments = new(OBJECTIVE,sizeof(ObjectArray));
    init_ObjectArray(arguments);
    for(i = 0;i<((Call*)expr)->arguments->used;i++){
    	argument = ((Call*)expr)->arguments->getElementInArrayAt(((Call*)expr)->arguments,i);

    	r = getReference(evaluate(visitor,argument));
    	arguments->addElementToArray(arguments,(r));
    }
    if(!(strcmp(callee->instanceOf,"LoxCallable")==0) && !(strcmp(callee->instanceOf,"LoxFunction")==0)
	  && !(strcmp(callee->instanceOf,"LoxClass")==0)&& !(strcmp(callee->instanceOf,"LoxInstance")==0)){
	   CEXCEPTION_T e;
	   e = create_exception(20,((Call*)expr)->paren,"Can only call functions and classes.",NULL);
	   Try{
	    Throw(e);
	   }
	   Catch(e){ Throw(e);}
    }
    if(strcmp(callee->instanceOf,"LoxInstance")==0)
	   function = (LoxFunction*)((LoxInstance*)callee)->klass;
    else
	   function = ((LoxFunction*)(callee));
    if(arguments->used != function->super.vtable.arity((LoxCallable*)function)){
	   CEXCEPTION_T e;
	   char * new_str,*temp_str;
	   new_str = NULL;
	   asprintf(&new_str,"Expected %d arguments but got %d.",function->super.vtable.arity((LoxCallable*)function),arguments->size);
	   temp_str = NULL;
	   temp_str = strcopy(temp_str,new_str);
	   e = create_exception(21,((Call*)expr)->paren,temp_str,NULL);
	   free(new_str);
	   new_str = NULL;
	   Try{
	    Throw(e);
	   }
	   Catch(e){ Throw(e);}
    }
    res = function->super.vtable.call((LoxCallable*)function,(Interpreter*)visitor,getReference(arguments));
    delete(function);
    delete(arguments);
    function = NULL;
    return res;
}


Object* visitWhileStmtInterpreter(StmtVisitor* visitor, Stmt* stmt){
    Object *temp;
   Object* r = NULL;
    temp = (evaluate(&visitor->expr,((While*)stmt)->condition));
    r = isTruthy(temp);
	while(r->value.number){
		delete(r);
		execute((Interpreter*)visitor,((While*)stmt)->body);
	    temp = (evaluate(&visitor->expr,((While*)stmt)->condition));
	    r = isTruthy(temp);
	}
	return NULL;
}

Object* visitBlockStmtInterpreter(StmtVisitor * visitor, Stmt* expr){
	Block* temp;
	Interpreter* intrprtr;
	Environment* env;
	temp= (Block*)expr;
	intrprtr = (Interpreter*) visitor;
	env = new(OBJECTIVE,sizeof(Environment));
	init_EnvironmentwithEnclosing(env,intrprtr->environment);
	executeBlock(intrprtr ,temp->statements,env);
     deleteEnvironment(env);
    env = NULL;
	return NULL;
}
Object* visitLiteralExprInterpreter(ExprVisitor* visitor, Expr* expr){
    Object* r = NULL;
    r = getReference(((Literal*)expr)->value);
	return r;
}

Object* visitThisExprInterpreter(ExprVisitor* visitor, Expr* expr){
	return (Object*)lookUpVariable((Interpreter*)visitor,((This*)expr)->keyword,expr);
}

Object* visitGetExprInterpreter(ExprVisitor* visitor, Expr* expr){
    CEXCEPTION_T e;
	Object* obj;
	obj = evaluate(visitor,((Get*)expr)->object);
	if(strcmp(obj->instanceOf,"LoxInstance")==0){
		return ((LoxInstance*)obj)->get((LoxInstance*)obj,((Get*)expr)->name);
	}
    e = create_exception(52,((Get*)expr)->name,"Only instances have properties.",NULL);
	Try{
	 Throw(e);
	}
	Catch(e){ Throw(e);}
	return NULL;
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

static Object* visitSuperExprInterpreter(ExprVisitor* visitor, Expr* expr){
	LoxClass* superclass;
	LoxInstance* object;
	LoxFunction* method;
	int distance,distance2;
	char * super,*thisstr;
    super = NULL;
    thisstr = NULL;
    super = strcopy(super,"super");
    thisstr = strcopy(thisstr,"this");
	distance = *(int*)get_value_for_key((struct _HASH*)((Interpreter*)visitor)->locals,expr);
	superclass = (LoxClass*) ((Interpreter*)visitor)->environment->getAt(((Interpreter*)visitor)->environment,&distance,super);
	distance2 = distance -1;
	object = (LoxInstance*) ((Interpreter*)visitor)->environment->getAt(((Interpreter*)visitor)->environment, &distance2,thisstr);
	method = superclass->findMethod((LoxFunction*)superclass,((Super*)expr)->method->lexeme);
	if(method == NULL){
		CEXCEPTION_T e;
		char * str;
	    str = NULL;
	    str = new_str(strlen("Undefined property ''.")+strlen(((Super*)expr)->method->lexeme));
		asprintf(&str,"Undefined property '%s'.",((Super*)expr)->method->lexeme);
	    e = create_exception(70,((Super*)expr)->method,str,NULL);
		Try{
		 Throw(e);
		}
		Catch(e){ Throw(e);}
		return NULL;
	}
	return (Object*)method->bind(method,object);
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
			 char* anew_str, *num_str;
			 anew_str = NULL;
			 result->type = STRING;
			 switch(left->type){
				case STRING:
				    switch(right->type){
					   case STRING:
						  anew_str = NULL;
						  anew_str = new_str(strlen(left->value.string)+strlen(right->value.string));
						  strncpy(anew_str,left->value.string,strlen(left->value.string));
						  strncat(anew_str,right->value.string,strlen(right->value.string));
						  result->value.string = anew_str;
						  break;
					   default:
						  num_str = stringify(right);
						  anew_str = new_str(strlen(num_str)+strlen(left->value.string));
						  strncpy(anew_str,left->value.string,strlen(left->value.string));
						  strncat(anew_str,num_str,strlen(num_str));
						  result->value.string = anew_str;
						  delete(num_str);
						  num_str = NULL;
						  break;
				    }
				    break;
				default:
				    num_str = stringify(left);
				    anew_str = NULL;
				    anew_str = new_str(strlen(num_str)+strlen(right->value.string));
				    strncpy(anew_str,num_str,strlen(num_str));
				    strncat(anew_str,right->value.string,strlen(right->value.string));
				    result->value.string = anew_str;
				    delete(num_str);
				    num_str = NULL;
				    break;
			 }
			 return result;
		  }
		  e = create_exception(4,((Binary*)expr)->operator,"Operands must be two numbers, two strings, or one of each",NULL);
		  delete(result);
		  result = NULL;
		  Try{
		   Throw(e);
		  }
		  Catch(e){ Throw(e);}
		  break;
	   case SLASH:
		  checkNumberOperands(((Binary*)expr)->operator,left,right);
		  if(right->value.number == (double)0){
			 e = create_exception(5,((Binary*)expr)->operator,"Cannot divide by Zero",NULL);
			  delete(result);
			  result = NULL;
			 Try{
			  Throw(e);
			 }
			 Catch(e){ Throw(e);}
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
    e = create_exception(2,operator,"Operand must be a number",NULL);
    Try{
	Throw(e);
    }
    Catch(e){ Throw(e);}
}
void checkNumberOperands(Token* operator, Object* left, Object* right){
    CEXCEPTION_T e;
    if(left->type == NUMBER && right->type == NUMBER)
		  return;
    e = create_exception(3,operator,"Operand must be a number",NULL);
    Try{
	Throw(e);
    }
    Catch(e){ Throw(e);}
}
const static char* nil = "nil";

char* stringify(Object* obj){
    char* text;
    char* temp;
    text = NULL;
    temp = NULL;
    if(obj == NULL || obj->type == KNULL){
	   return strcopy(temp,nil);
    }
    if(obj->type == FALSE){
	   return strcopy(temp,"false");
    }
    if(obj->type == TRUE){
	   return strcopy(temp,"true");
    }
    if(obj->type == BOOLEAN){
	   switch((int)obj->value.number){
		  case 1:
			 return strcopy(temp,"true");
		  case 0:
			 return strcopy(temp,"false");
		  default:
			 return strcopy(temp,"boolean error");
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
		  temp_str = NULL;
		  temp_str = strcopy(temp_str,text);
		  free(text);
		  text = NULL;
		  return temp_str;
	   }
	   free(text);
	   text = NULL;
	   asprintf(&text,"%lf",(double)obj->value.number);
	   temp_str = NULL;
	   temp_str = strcopy(temp_str,text);
	   free(text);
	   text = NULL;
	   return temp_str;
    }

    return copy(obj->value.string);
}
Object* visitLogicalExprInterpreter(ExprVisitor* visitor, Expr* expr){
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

Object* visitSetExprInterpreter(ExprVisitor* visitor, Expr* expr){
	CEXCEPTION_T e;
	Object* obj, *value;
	obj = evaluate(visitor,((Set*)expr)->object);
	if(!(strcmp(obj->instanceOf,"LoxInstance")==0)){
	    e = create_exception(57,((Set*)expr)->name,"Only instances have fields.",NULL);
		Try{
		 Throw(e);
		}
		Catch(e){ Throw(e);}
		return NULL;
	}
	value = evaluate(visitor,((Set*)expr)->value);
	((LoxInstance*)obj)->set((LoxInstance*)obj,((Set*)expr)->name,value);
	return value;
}

Object* visitIfStmtInterpreter(StmtVisitor * visitor, Stmt* expr){
    Object* a,*truth;
    If* stmt;
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


Object* visitExpressionStmtInterpreter(StmtVisitor* visitor, Stmt* stmt){
	evaluate(&visitor->expr,((Expression*)stmt)->expression);
	return NLLOBJ;
}

Object* visitPrintStmtInterpreter(StmtVisitor* visitor, Stmt* stmt){
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

Object* visitVarStmtInterpreter(StmtVisitor* visitor, Stmt* stmt){
	Interpreter* intprtr;
    Object* r,*a;
	Var* vstmt;
    char* new_str;
	intprtr = (Interpreter*) visitor;
	vstmt = (Var*)stmt;
	a = NULL;
	r = NULL;
/*    r = new(OBJECTIVE,sizeof(Object));
	init_Object(r,"",KNULL);*/
	if(vstmt->initializer != NULL){
		a = evaluate(&visitor->expr,vstmt->initializer);
	}
    new_str = NULL;
    new_str = strcopy(new_str,vstmt->name->lexeme);
    intprtr->environment->defineEnv(intprtr->environment,new_str,copy(a));
/*	a = new(OBJECTIVE,sizeof(Object));
    init_Object(a,"nil",NIL);
	a->type = NIL*/;
	return NLLOBJ;
}
Object* visitAssignExprInterpreter(ExprVisitor * visitor,Expr* expr){
	Assign* assign;
	Object *a;
	int *distance;
	Interpreter* intprtr;
	assign = (Assign*) expr;
	intprtr= (Interpreter*)visitor;
	a = evaluate(visitor,assign->value);

	distance = (int*)intprtr->locals->super.super.vtable.get_value_for_key((struct _HASH*)intprtr->locals,expr);
	if(distance && (*distance != -1)){
		intprtr->environment->assignAt(intprtr->environment,distance,assign->name,copy(a));
	}
	else {
		intprtr->globals->assign(intprtr->globals,assign->name,copy(a));
	}
	return getReference(a);
}

Object* visitVariableExprInterpreter(ExprVisitor* visitor, Expr* expr){
	Environment* env;
    Object* r;
	Variable* nexpr;
	nexpr = (Variable*)expr;
	return lookUpVariable((Interpreter*)visitor,nexpr->name,expr);
    r = NULL;
	env = (((Interpreter*)visitor)->environment);
	r = (env->get(env,((Variable*)expr)->name ));
   	return r;
}
Object* lookUpVariable(Interpreter* intprtr,Token* name, Expr* expr){
	int *distance;
    distance = intprtr->locals->super.super.vtable.get_value_for_key((struct _HASH*)intprtr->locals,expr);
	if(distance != NULL){
	    return intprtr->environment->getAt(intprtr->environment,distance,name->lexeme);
	}
	else {
		return intprtr->globals->get(intprtr->globals,name);
	}
	return NULL;
}

