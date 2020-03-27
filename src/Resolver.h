/*
 * Resolver.h
 *
 *  Created on: Mar 25, 2020
 *      Author: scotty
 */

#ifndef RESOLVER_H_
#define RESOLVER_H_
#include "Stmt.h"
#include "Expr.h"
#include "Interpreter.h"
#include "HashMapStack.h"

typedef struct _Resolver Resolver;
struct _Resolver {
	StmtVisitor super;
	Interpreter* interpreter;
	HashMapStack* scopes;
};


Resolver* init_Resolver(Resolver* r, Interpreter* i);
void resolve(Resolver* r, StmtArray* statements);
void resolve_stmt(Resolver* r, Stmt* stmt);
void beginScope(Resolver* resolver);
void endScope(Resolver* resolver);
void declare(Resolver* visitor,Token* name);
void define_resolver(Resolver* resolver, Token* name);
void resolveLocal(Resolver* resolver,Expr* expr, Token* name);
void resolveFunction(Resolver* resolver, Function* function);

#endif /* RESOLVER_H_ */
