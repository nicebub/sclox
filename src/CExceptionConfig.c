#define CEXCEPTION_USE_CONFIG_FILE
#include "CExceptionConfig.h"
#include "CException.h"
#include <stdio.h>

void start_try(volatile CEXCEPTION_T *Exception){
    fprintf(stderr,"start try exception: %d \n",Exception->id);
    if(Exception->message)
	   fprintf(stderr,"token lexeme %s\n",Exception->message);
    if(Exception->token && Exception->token->lexeme)
	   fprintf(stderr,"token lexeme %s\n",Exception->token->lexeme);
    
}
void happy_try(volatile CEXCEPTION_T *Exception){
	fprintf(stderr,"happy try exception: %d \n",Exception->id);
	if(Exception->message)
	    fprintf(stderr,"token lexeme %s\n",Exception->message);
	if(Exception->token && Exception->token->lexeme)
	    fprintf(stderr,"token lexeme %s\n",Exception->token->lexeme);

}
void after_try(volatile CEXCEPTION_T *Exception){
	fprintf(stderr,"after try exception: %d \n",Exception->id);
	if(Exception->message)
	    fprintf(stderr,"token lexeme %s\n",Exception->message);
	if(Exception->token && Exception->token->lexeme)
	    fprintf(stderr,"token lexeme %s\n",Exception->token->lexeme);
}
void start_catch(volatile CEXCEPTION_T *Exception){
	fprintf(stderr,"start catch exception: %d \n",Exception->id);
	if(Exception->message)
	    fprintf(stderr,"token lexeme %s\n",Exception->message);
	if(Exception->token && Exception->token->lexeme)
	    fprintf(stderr,"token lexeme %s\n",Exception->token->lexeme);
}
CEXCEPTION_T create_exception(unsigned int id, Token* token, char* message, struct _return_exception* sub){
	CEXCEPTION_T e;
    e.id = id;
	e.token = token;
	e.message = message;
    e.sub = sub;
    return e;
}
