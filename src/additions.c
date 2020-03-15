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

