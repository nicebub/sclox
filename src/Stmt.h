
#ifndef _STMT_H
#define _STMT_H

#include <string.h>
#include "Token.h"
#include "Object.h"
#include "TokenArray.h"
#include "Expr.h"
#ifndef _STMTARRAY
#define _STMTARRAY
    typedef struct _StmtArray StmtArray;
    extern void delete_StmtArray(void* array);
#endif
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(void* array);
#endif


typedef struct _Stmt Stmt;
typedef struct _StmtVisitor StmtVisitor;

typedef struct _Stmt_vtable Stmt_vtable;
struct _Stmt_vtable {
    Object* (*accept)(Stmt* arg,StmtVisitor* visitor);
    void (*delete)(void* arg);
    void* (*copy)(void*);

};
typedef struct _StmtVisitor_vtable StmtVisitor_vtable;
struct _StmtVisitor_vtable {
    Object* (*visitBlockStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitClassStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitExpressionStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitFunctionStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitIfStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitPrintStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitReturnStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitVarStmt)(StmtVisitor* visitor,Stmt* stmt);
    Object* (*visitWhileStmt)(StmtVisitor* visitor,Stmt* stmt);

};
struct _StmtVisitor {
    ExprVisitor expr;
    StmtVisitor_vtable vtable;
};
static short int Stmtcounter=0;
    
struct _Stmt {
    char instanceOf[30];
    short int id;
    Stmt_vtable vtable;
};

typedef struct _Block Block;
struct _Block {
    Stmt super;
    StmtArray* statements;

};
void new_Block(Block*  inObj,StmtArray* statements);
void delete_Block(void* arg);
Object* acceptBlock(Stmt* arg, StmtVisitor* visitor);
void* copyBlock(void*);

typedef struct _Class Class;
struct _Class {
    Stmt super;
    Token* name;
    Variable* superclass;
    StmtArray* methods;

};
void new_Class(Class*  inObj,Token* name,Variable* superclass,StmtArray* methods);
void delete_Class(void* arg);
Object* acceptClass(Stmt* arg, StmtVisitor* visitor);
void* copyClass(void*);

typedef struct _Expression Expression;
struct _Expression {
    Stmt super;
    Expr* expression;

};
void new_Expression(Expression*  inObj,Expr* expression);
void delete_Expression(void* arg);
Object* acceptExpression(Stmt* arg, StmtVisitor* visitor);
void* copyExpression(void*);

typedef struct _Function Function;
struct _Function {
    Stmt super;
    Token* name;
    TokenArray* params;
    StmtArray* body;

};
void new_Function(Function*  inObj,Token* name,TokenArray* params,StmtArray* body);
void delete_Function(void* arg);
Object* acceptFunction(Stmt* arg, StmtVisitor* visitor);
void* copyFunction(void*);

typedef struct _If If;
struct _If {
    Stmt super;
    Expr* condition;
    Stmt* thenBranch;
    Stmt* elseBranch;

};
void new_If(If*  inObj,Expr* condition,Stmt* thenBranch,Stmt* elseBranch);
void delete_If(void* arg);
Object* acceptIf(Stmt* arg, StmtVisitor* visitor);
void* copyIf(void*);

typedef struct _Print Print;
struct _Print {
    Stmt super;
    Expr* expression;

};
void new_Print(Print*  inObj,Expr* expression);
void delete_Print(void* arg);
Object* acceptPrint(Stmt* arg, StmtVisitor* visitor);
void* copyPrint(void*);

typedef struct _Return Return;
struct _Return {
    Stmt super;
    Token* keyword;
    Expr* value;

};
void new_Return(Return*  inObj,Token* keyword,Expr* value);
void delete_Return(void* arg);
Object* acceptReturn(Stmt* arg, StmtVisitor* visitor);
void* copyReturn(void*);

typedef struct _Var Var;
struct _Var {
    Stmt super;
    Token* name;
    Expr* initializer;

};
void new_Var(Var*  inObj,Token* name,Expr* initializer);
void delete_Var(void* arg);
Object* acceptVar(Stmt* arg, StmtVisitor* visitor);
void* copyVar(void*);

typedef struct _While While;
struct _While {
    Stmt super;
    Expr* condition;
    Stmt* body;

};
void new_While(While*  inObj,Expr* condition,Stmt* body);
void delete_While(void* arg);
Object* acceptWhile(Stmt* arg, StmtVisitor* visitor);
void* copyWhile(void*);

void delete_Stmt(Stmt* stmt);
short int addtoStmtCounter(void);

#endif

