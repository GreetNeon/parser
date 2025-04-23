#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below
void error (char* msg){
	fprintf(stderr, "Error: %s\n", msg);
	exit(1);
}

void classDeclar(){
	return;
}

void memberDeclar(){
	return;
}

void classVarDeclar(){
	return;
}

void type(){
	return;
}

void subroutineDeclar(){
	return;
}

void paramList(){
	return;
}

void subroutineBody(){
	return;
}

void statement(){
	return;
}

void varDeclarStatement(){
	return;
}

void letStatement(){
	return;
}

void ifStatement(){
	return;
}

void whileStatement(){
	return;
}

void doStatement(){
	return;
}

void subroutineCall(){
	return;
}

void expressionList(){
	return;
}

void returnStatemnt(){
	return;
}

void expression(){
	return;
}

void relationalExpression(){
	return;
}

void ArithmeticExpression(){
	return;
}

void term(){
	return;
}

void factor(){
	return;
}

void Operand(){
	Token t = PeekNextToken();
	// integerConstant
	if (t.tp == INT){
		GetNextToken(); // consume the token
		// process integer
		return;
	// identifier [.identifier] [ [expression] | (expressionList) ]
	} else if (t.tp == ID){
		GetNextToken(); // consume the token
		// Can be a variable or a function call or indexing
		t = PeekNextToken();
		if (t.tp == SYMBOL){
			// Check for indexing
			if (strcmp(t.lx, "[") == 0){
				GetNextToken(); // consume the token
				expression();
				t = GetNextToken();
				if (strcmp(t.lx, "]") != 0){
					error("Close bracket expected");
				}
			// Check for function call
			} else if (strcmp(t.lx, "(") == 0){
				GetNextToken(); // consume the token
				expressionList();
				t = GetNextToken();
				if (strcmp(t.lx, ")") != 0){
					error("Close bracket expected");
				}
			// Check for dot operator (method call)
			} else if (strcmp(t.lx, ".") == 0){
				GetNextToken(); // consume the token
				t = GetNextToken(); // get the next token
				if (t.tp != ID){
					error("Identifier expected after '.'");
				}
			}
		}
		// process variable
		return;
	// (expression)
	} else if (t.tp == SYMBOL){
		GetNextToken(); // consume the token
		// Check for open bracket
		if (strcmp(t.lx, "(") == 0){
			expression();
			t = GetNextToken();
			if (strcmp(t.lx, ")") != 0){
				error("Close bracket expected");
			}
			return;
		}
	// stringLiteral
	} else if (t.tp == STRING){
		GetNextToken(); // consume the token
		// process string
		return;
	}
	// true | false | null | this
	else if (t.tp == RESWORD){
		if (strcmp(t.lx, "true") == 0 || strcmp(t.lx, "false") == 0 || strcmp(t.lx, "null") == 0 || strcmp(t.lx, "this") == 0){
			GetNextToken(); // consume the token
			// process boolean or null or this
			return;
		}
	}
	else {
		error("Operand expected");
	}
}


int InitParser (char* file_name)
{

	return 1;
}

ParserInfo Parse ()
{
	ParserInfo pi;

	// implement the function


	pi.er = none;
	return pi;
}


int StopParser ()
{
	return 1;
}

#ifndef TEST_PARSER
int main ()
{
	printf("Parser module\n");
	InitLexer("Main.jack");
	Token t = GetNextToken();
	if (t.tp == ERR){
		printf("%s\n", t.lx);
	} else {
		printf("Token: %s\n", t.lx);
	}
	return 1;
}
#endif
