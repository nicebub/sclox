/*
 * TokenType.h
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */

#ifndef TOKENTYPE_H_
#define TOKENTYPE_H_
#include <stdlib.h>

typedef struct _keywordMap keywordMap;



typedef enum _TokenType {
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	BOOLEAN, IDENTIFIER, STRING, NUMBER,


	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, KNULL,

	EEOF
} TokenType;
struct _keywordMap {
	TokenType keyword;
	char * keywordName;
};

const static char * typeName[] = { "LEFT_PAREN", "RIGHT_PAREN","LEFT_BRACE","RIGHT_BRACE",
						"COMMA","DOT","MINUS","PLUS","SEMICOLON","SLASH","STAR",
						"BANG","BANG_EQUAL","EQUAL","EQUAL_EQUAL",
						"GREATER","GREATER_EQUAL","LESS","LESS_EQUAL",
					"BOOLEAN","IDENTIFIER","STRING","NUMBER",
						"AND","CLASS","ELSE","FALSE","FUN","FOR","IF","NIL","OR",
						"PRINT","RETURN","SUPER","THIS","TRUE","VAR","WHILE",
						"EOF" };


const static keywordMap kmap[] = {
		{ AND, "and"}, { CLASS, "class"}, { ELSE, "else" }, {FALSE, "false" } ,
		{ FUN, "fun" }, { FOR, "for" }, { IF, "if" }, { NIL, "nil" }, { OR, "or" },
		{ PRINT, "print" }, { RETURN, "return" }, { SUPER, "super" }, { THIS, "this" },
		{ TRUE, "true" }, { VAR, "var" }, { WHILE, "while" }, { KNULL, "null" }
};

#endif /* TOKENTYPE_H_ */
