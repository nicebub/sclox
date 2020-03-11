/*
 * ReturnResult.h
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#ifndef RETURNRESULT_H_
#define RETURNRESULT_H_

#include "TokenType.h"

typedef struct _ReturnResult ReturnResult;
typedef union _ResultValue ResultValue;

union _ResultValue {
	double number;
	char* string;
};

struct _ReturnResult {
	ResultValue value;
	TokenType type;
};

#endif /* RETURNRESULT_H_ */
