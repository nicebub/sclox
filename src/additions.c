#include "additions.h"
#include "Stmt.h"
#include "StmtArray.h"
void initializeStmtElement(Stmt** statement, void* values){
    if(!values){
	   statement = NULL;
	   return;
    }
    *statement = (Stmt*) values;
}

void initializeExprElement(Expr** exprs, void * values){
	if(!values){
		exprs = NULL;
		return;
	}
	*exprs = (Expr*) values;
}
void initializeObjectElement(Object** results, void * values){
	if(!values){
		results = NULL;
		return;
	}
	*results = (Object*)values;

}

TokenArray*  copyTokenArray(TokenArray * arr){
	TokenArray* newarr;
	int i;
	newarr = malloc(sizeof(TokenArray));
	init_TokenArray(newarr);
	for(i=0;i<arr->used;i++){
		newarr->addElementToArray(newarr, getTokeninArrayAt(arr,i));
	}
	return newarr;
}
TokenArray* getTokenArrayReference(TokenArray* arr){
	arr->owner_references++;
	return arr;
}
TokenArray* releaseTokenArrayReference(TokenArray* arr){
	arr->owner_references--;
	return NULL;
}
