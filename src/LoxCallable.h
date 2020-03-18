/*
 * LoxCallable.h
 *
 *  Created on: Mar 18, 2020
 *      Author: scotty
 */

#ifndef LOXCALLABLE_H_
#define LOXCALLABLE_H_

#include "StmtArray.h"
#include "ExprArray.h"
#include "ReturnResultArray.h"
typedef struct _LoxCallable LoxCallable;
struct _LoxCallable{
	ReturnResult (*call)(Interpreter* intrprtr, ReturnResultArray* arguments);
};

#endif /* LOXCALLABLE_H_ */
