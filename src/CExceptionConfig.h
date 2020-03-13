/*
//  CExceptionConfig.h
//  sclox
//
//  Created by Scott Lorberbaum on 3/12/20.
//  Copyright © 2020 Scott Lorberbaum. All rights reserved.
//
*/
#ifndef CExceptionConfig_h
#define CExceptionConfig_h
#include "Token.h"
typedef struct _exception exception;
struct _exception{
    unsigned int id;
    Token * token;
    char * message;
};

#define CEXCEPTION_T         exception

#define CEXCEPTION_NONE      (0x5A5A5A5A) 


#endif /* CExceptionConfig_h */
