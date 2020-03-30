/*
 * Resolver.c
 *
 *  Created on: Mar 25, 2020
 *      Author: scotty
 */
#include <stdio.h>
#include "Resolver.h"
#include "Stmt.h"
#include "StrObjHashMap.h"
#include "Lox.h"
#include "TokenArray.h"

#include <stddef.h>
static Object* visitReturnStmtResolver(StmtVisitor* visitor, Stmt* stmt);
static Object* visitFunctionStmtResolver(StmtVisitor* visitor, Stmt* stmt);
static Object* visitCallExprResolver(ExprVisitor* visitor, Expr* stmt);
static Object* visitWhileStmtResolver(StmtVisitor* visitor, Stmt* stmt);
static Object* visitLogicalExprResolver(ExprVisitor* visitor, Expr* expr);
static Object* visitIfStmtResolver(StmtVisitor * visitor, Stmt* expr);
static Object* visitBlockStmtResolver(StmtVisitor * visitor, Stmt* expr);
static Object* visitLiteralExprResolver(ExprVisitor* visitor, Expr* expr);
static Object* visitGroupingExprResolver(ExprVisitor* visitor, Expr* expr);
static Object* visitUnaryExprResolver(ExprVisitor* visitor, Expr* expr);
static Object* visitBinaryExprResolver(ExprVisitor* visitor, Expr* expr);
static Object* visitAssignExprResolver(ExprVisitor * visitor,Expr* expr);
static Object* visitExpressionStmtResolver(StmtVisitor* visitor, Stmt* stmt);
static Object* visitPrintStmtResolver(StmtVisitor* visitor, Stmt* stmt);
static Object* visitVarStmtResolver(StmtVisitor* visitor,Stmt* stmt);
static Object* visitVariableExprResolver(ExprVisitor* visitor, Expr* stmt);

StmtVisitor_vtable Resolver_vtable = {
    visitBlockStmtResolver,
    visitExpressionStmtResolver,
    visitFunctionStmtResolver,
    visitIfStmtResolver,
    visitPrintStmtResolver,
    visitReturnStmtResolver,
    visitVarStmtResolver,
    visitWhileStmtResolver

};


Resolver* init_Resolver(Resolver* r, Interpreter* i){
	int size = 40;
	r->interpreter = getReference(i);
	r->scopes = new(OBJECTIVE,sizeof(HashMapStack));
	init_HashMapStack(r->scopes,&size);
	r->super.vtable.visitBlockStmt = &visitBlockStmtResolver;
    r->super.vtable.visitExpressionStmt = &visitExpressionStmtResolver;
    r->super.vtable.visitFunctionStmt = &visitFunctionStmtResolver;
    r->super.vtable.visitIfStmt = &visitIfStmtResolver;
    r->super.vtable.visitPrintStmt = &visitPrintStmtResolver;
    r->super.vtable.visitReturnStmt = &visitReturnStmtResolver;
    r->super.vtable.visitVarStmt = &visitVarStmtResolver;
    r->super.vtable.visitWhileStmt = &visitWhileStmtResolver;
    r->super.expr.vtable.visitAssignExpr = &visitAssignExprResolver;
    r->super.expr.vtable.visitBinaryExpr = &visitBinaryExprResolver;
    r->super.expr.vtable.visitCallExpr = &visitCallExprResolver;
    r->super.expr.vtable.visitGroupingExpr = &visitGroupingExprResolver;
    r->super.expr.vtable.visitLiteralExpr = &visitLiteralExprResolver;
    r->super.expr.vtable.visitLogicalExpr = &visitLogicalExprResolver;
    r->super.expr.vtable.visitUnaryExpr = &visitUnaryExprResolver;
    r->super.expr.vtable.visitVariableExpr = &visitVariableExprResolver;
    r->resolve = &resolve;
    r->resolve_stmt = &resolve_stmt;
    r->beginScope =&beginScope;
    r->endScope = &endScope;
    r->declare = &declare;
    r->define_resolver = &define_resolver;
    r->resolveLocal = &resolveLocal;
    r->resolveFunction = &resolveFunction;
    r->currentFunction = FT_NONE;
/*	r->super.vtable = */
/*	r->super.expr =*/
	return r;
}

char* toStringResolverValue(void * value){
	char * temp, *num;
	int val;
	num = NULL;
	val = *(int*)value;
	if(val){
		asprintf(&num,"%d",*(int*)value);
		temp = new(RAW,sizeof(char)*(strlen(num)+1));
		memset(temp,0,strlen(num)+1);
		strncpy(temp,num,strlen(num));
		free(num);
		num = NULL;
		return temp;
	}
	return NULL;
}
static Object* visitBlockStmtResolver(StmtVisitor * visitor, Stmt* stmt){
	beginScope((Resolver*)visitor);
	resolve((Resolver*)visitor,((Block*)stmt)->statements);
	endScope((Resolver*)visitor);
	return NULL;
}
void resolve(Resolver* resolver, StmtArray* statements){
	int i;
	Stmt* statement;
	for(i=0;i<statements->used;i++){
		statement = statements->Stmts[i];
		resolve_stmt(resolver,statement);
	}
}
void resolve_stmt(Resolver* resolver, Stmt* stmt){
	stmt->vtable.accept(stmt,(StmtVisitor*)resolver);
}
#define INIT_SIZE 40
void beginScope(Resolver* resolver){
	StrObjHashMap* temp;
	temp = (StrObjHashMap*)create_StrObjHashMap(INIT_SIZE);
	resolver->scopes->super.push((Stack*)resolver->scopes,temp);
}

void endScope(Resolver* resolver){
	resolver->scopes->super.pop((Stack*)resolver->scopes);
}

static Object* visitVarStmtResolver(StmtVisitor* visitor,Stmt* stmt){
	declare((Resolver*)visitor,((Var*)stmt)->name);
	if(((Var*)stmt)->initializer != NULL){
		resolve_stmt((Resolver*)visitor,(Stmt*)((Var*)stmt)->initializer);
	}
	define_resolver((Resolver*)visitor,((Var*)stmt)->name);
	return NULL;
}

void resolve_expr(Resolver* resolver, Expr* expr){
	expr->vtable.accept(expr,&resolver->super.expr);
}


void declare(Resolver* resolver, Token* name){
	StrObjHashMap* scope;
	int  *x;
	if(resolver->scopes->super.used == 0) return;
	scope = resolver->scopes->super.top((Stack*)resolver->scopes);
    x = new(RAW,sizeof(int));
	*x = 0;
    if(scope->super.super.vtable.get_value_for_key((struct _HASH*)scope,name->lexeme)){
	   ((Lox*)resolver->interpreter->lox)->parse_error(resolver->interpreter->lox,name,"Variable with this name already declared in this scope.");
    }
	add_to_hash((struct _HASH*)scope,name->lexeme,x);


}
void define_resolver(Resolver* resolver, Token* name){
	StrObjHashMap* map;
	int *x;
	if(resolver->scopes->super.used == 0) return;
    x = new(RAW,sizeof(int));
	*x = 1;
	map = resolver->scopes->super.top((Stack*)resolver->scopes);
	add_to_hash((struct _HASH*)map,name->lexeme,x);
}

static Object* visitVariableExprResolver(ExprVisitor* visitor, Expr* expr){
	Resolver* resolver;
	resolver = (Resolver*) visitor;
	if((resolver->scopes->super.used != 0)){
		StrObjHashMap* map;
	    int * x;
		map =(StrObjHashMap*)resolver->scopes->super.top((Stack*)resolver->scopes);
	    x = (int*)get_value_for_key((struct _HASH*)map,((Variable*)expr)->name->lexeme);
	    if(x){
		if( *x == 0){
			Lox* lox;
			lox = (Lox*)resolver->interpreter->lox;
			lox->parse_error(lox,((Variable*)expr)->name,"Cannot read local variable in its own initializer.");
		}
	   }
	    resolveLocal(resolver,expr,((Variable*)expr)->name);
	}
	return NULL;
}

void resolveLocal(Resolver* resolver,Expr* expr, Token* name){
	int i;
	for(i = resolver->scopes->super.used -1; i>=0;i--){
		StrObjHashMap* map;
		map = ((StrObjHashMap**)resolver->scopes->super.values)[i];
	    
	    if(get_value_for_key((struct _HASH*)map,name->lexeme)){
			Interpreter* interpreter;
		    int * x;
			interpreter = resolver->interpreter;
		    x = new(RAW,sizeof(int));
		    *x = resolver->scopes->super.used-1-i;
		    interpreter-> resolve(interpreter,expr,x);
		   return ;
		}
	}
}
static Object* visitAssignExprResolver(ExprVisitor * visitor,Expr* expr){
	resolve_expr((Resolver*)visitor,((Assign*)expr)->value);
	resolveLocal((Resolver*)visitor,expr,((Assign*)expr)->name);
	return NULL;
}

static Object* visitFunctionStmtResolver(StmtVisitor* visitor, Stmt* stmt){
	declare((Resolver*)visitor,((Function*)stmt)->name);
	define_resolver((Resolver*)visitor,((Function*)stmt)->name);
	resolveFunction((Resolver*)visitor,(Function*)stmt, FT_FUNCTION);
	return NULL;
}

void resolveFunction(Resolver* resolver, Function* function, FunctionType type){
	int i;
	FunctionType enclosingFunction = resolver->currentFunction;
	resolver->currentFunction = type;
	beginScope(resolver);
	for(i=0;i<function->params->used;i++){
		declare(resolver,(Token*)getTokeninArrayAt(function->params,i));
		define_resolver(resolver,(Token*)getTokeninArrayAt(function->params,i));
	}
	resolve(resolver,function->body);
	endScope(resolver);
	resolver->currentFunction = enclosingFunction;
}
static Object* visitExpressionStmtResolver(StmtVisitor* visitor, Stmt* stmt){
	Expression* expr;
	Resolver* resolver;
	resolver = (Resolver*) visitor;
	expr = (Expression*)stmt;
	resolve_expr(resolver,expr->expression);
	return NULL;

}

static Object* visitIfStmtResolver(StmtVisitor * visitor, Stmt* expr){
	If* iff;
	Resolver* resolver;
	resolver = (Resolver*) visitor;
	iff = (If*)expr;
	resolve_expr(resolver,iff->condition);
	resolve_stmt(resolver,iff->thenBranch);
	if(iff->elseBranch != NULL)
		resolve_stmt(resolver,iff->elseBranch);
	return NULL;
}
static Object* visitPrintStmtResolver(StmtVisitor* visitor, Stmt* stmt){
	Print* print;
	Resolver* resolver;
	resolver = (Resolver*) visitor;
	print = (Print*) stmt;
	resolve_expr(resolver,print->expression);
	return NULL;
}

static Object* visitReturnStmtResolver(StmtVisitor* visitor, Stmt* stmt){
	Return * ret;
	Resolver* resolver;
	resolver =(Resolver*) visitor;
	ret = (Return*) stmt;
	if(resolver->currentFunction == FT_NONE){
		((Lox*)resolver->interpreter->lox)->parse_error(resolver->interpreter->lox,ret->keyword,"Cannot return from top-level code.");
	}
	if(ret->value != NULL){
		resolve_expr(resolver,ret->value);
	}
	return NULL;
}
static Object* visitWhileStmtResolver(StmtVisitor* visitor, Stmt* stmt){
	While* wle;
	Resolver* resolver;
	resolver= (Resolver*) visitor;
	wle = (While*)stmt;
	resolve_expr(resolver,wle->condition);
	resolve_stmt(resolver,wle->body);
	return NULL;
}

static Object* visitBinaryExprResolver(ExprVisitor* visitor, Expr* expr){
	Binary* binary;
	Resolver* resolver;
	resolver = (Resolver*) visitor;
	binary = (Binary*)expr;
	resolve_expr(resolver,binary->left);
	resolve_expr(resolver,binary->right);
	return NULL;
}
static Object* visitCallExprResolver(ExprVisitor* visitor, Expr* stmt){
	int i;
	Call* call;
	Resolver* resolver;
	resolver = (Resolver*) visitor;
	call= (Call*) stmt;
	resolve_expr(resolver,call->callee);
	for(i=0;i<call->arguments->used;i++){
		resolve_expr(resolver,call->arguments->Exprs[i]);
	}
	return NULL;
}
static Object* visitGroupingExprResolver(ExprVisitor* visitor, Expr* expr){
	Grouping* group;
	Resolver* resolver;
	resolver = (Resolver*) visitor;
	group = (Grouping*)expr;
	resolve_expr(resolver,group->expression);
	return NULL;
}

static Object* visitLiteralExprResolver(ExprVisitor* visitor, Expr* expr){
	return NULL;
}

static Object* visitLogicalExprResolver(ExprVisitor* visitor, Expr* expr){
	Logical* logical;
	Resolver* resolver;
	resolver = (Resolver*)visitor;
	logical = (Logical*)expr;
	resolve_expr(resolver,logical->left);
	resolve_expr(resolver,logical->right);
	return NULL;
}

static Object* visitUnaryExprResolver(ExprVisitor* visitor, Expr* expr){
	Unary* un;
	Resolver* resolver;
	resolver = (Resolver*)visitor;
	un= (Unary*)expr;
	resolve_expr(resolver,un->right);
	return NULL;
}

