#include "additions.h"
#include "Stmt.h"
#include "StmtArray.h"
void initializeStmtElement(Stmt** statement, void* values){
    if(!values){
	   statement = NULL;
	   return;
    }
    *statement = (Stmt*)getReference((Stmt*) values);
}

void initializeExprElement(Expr** exprs, void * values){
	if(!values){
		exprs = NULL;
		return;
	}
	*exprs = (Expr*)getReference((Expr*) values);
}
void initializeObjectElement(Object** results, void * values){
	if(!values){
		results = NULL;
		return;
	}
	*results = (Object*)getReference((Object*) values);

}
void initializememoryElement(Object** objs, void * values){

}
