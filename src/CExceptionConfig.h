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

#define DEBUG_EXTRA
#include "Token.h"
typedef struct _exception exception;
typedef struct _return_exception Return_exception;
struct _exception{
    unsigned int id;
    Token * token;
    char * message;
    struct _return_exception *sub;
};

struct _return_exception {
	Object* value;
};

#define CEXCEPTION_T         exception

#define CEXCEPTION_NONE      (0x5A5A5A5A) 

#ifdef DEBGUG_EXTRA
#define CEXCEPTION_HOOK_START_TRY start_try(&CExceptionFrames[MY_ID].Exception);
#define CEXCEPTION_HOOK_HAPPY_TRY happy_try(&CExceptionFrames[MY_ID].Exception);
#define CEXCEPTION_HOOK_AFTER_TRY after_try(&CExceptionFrames[MY_ID].Exception);
#define CEXCEPTION_HOOK_START_CATCH start_catch(&CExceptionFrames[MY_ID].Exception);
#endif
void start_try(volatile CEXCEPTION_T *Exception);
void happy_try(volatile CEXCEPTION_T *Exception);
void after_try(volatile CEXCEPTION_T *Exception);
void start_catch(volatile CEXCEPTION_T *Exception);

#endif /* CExceptionConfig_h */
