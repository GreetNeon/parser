#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below

ParserInfo pi;

void error (SyntaxErrors err){
	pi.er = err;
}

void classDeclar(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "class") == 0){
		GetNextToken(); // consume the token
	} else {
		error(classExpected);
		return;
	}
	t = PeekNextToken();
	if (t.tp != ID){
		error(idExpected);
		return;
	}
	GetNextToken(); // consume the token
	t = PeekNextToken();
	if (strcmp(t.lx, "{") != 0){
		error(openBraceExpected);
		return;
	}
	GetNextToken(); // consume the token
	while (1){
		t = PeekNextToken();
		if (strcmp(t.lx, "}") == 0){
			break;
		}
		if (t.tp == EOFile){
			error(closeBraceExpected);
			return;
		}
		memberDeclar();
	}
	return;
}

void memberDeclar(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0){
		classVarDeclar();// Process class variable declaration
	} else if(strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0){
		subroutineDeclar();// Process subroutine declaration
	} else {
		error(memberDeclarErr);
		return;
	}
	return;
}

void classVarDeclar(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0){
		GetNextToken(); // consume the token
	} else {
		error(memberDeclarErr);
		return;
	}
	type();
	while(1){
		t = PeekNextToken();
		if (t.tp != ID){
			error(idExpected);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, ",") == 0){
			GetNextToken(); // consume the token
		} else {
			break;
		}
	}
	t = PeekNextToken();
	if (strcmp(t.lx, ";") != 0){
		error(semicolonExpected);
		return;
	}
	GetNextToken(); // consume the token
	return;
}

void type(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0){
		GetNextToken(); // consume the token
	} else if (t.tp == ID){
		GetNextToken(); // consume the token
	} else {
		error(illegalType);
		return;
	}
	return;
}

void subroutineDeclar(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0){
		GetNextToken(); // consume the token
	} else {
		error(subroutineDeclarErr);
		return;
	}
	t = PeekNextToken();
	if (strcmp(t.lx, "void") == 0){
		GetNextToken(); // consume the token
	} else {
		type();
	}
	t = PeekNextToken();
	if (t.tp != ID){
		error(idExpected);
		return;
	}
	GetNextToken(); // consume the token
	t = PeekNextToken();
	if (strcmp(t.lx, "(") != 0){
		error(openParenExpected);
		return;
	}
	GetNextToken(); // consume the token
	paramList();
	t = PeekNextToken();
	if (strcmp(t.lx, ")") != 0){
		error(closeParenExpected);
		return;
	}
	GetNextToken(); // consume the token
	subroutineBody();
	return;
}

void paramList(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, ")") == 0){
		// empty parameter list
		return;
	}
	while(1){
		type();
		t = PeekNextToken();
		if (t.tp != ID){
			error(idExpected);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, ",") == 0){
			GetNextToken(); // consume the token
		} else {
			break;
		}
	}
	return;
}

void subroutineBody(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "{") == 0){
		GetNextToken(); // consume the token
		while (1){
			t = PeekNextToken();
			if (strcmp(t.lx, "}") == 0){
				break;
			}
			if (t.tp == EOFile){
				error(closeBraceExpected);
				return;
			}
			statement();
		}
		GetNextToken(); // consume the token
	} else {
		error(openBraceExpected);
	}
	return;
}

void statement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "let") == 0){
		letStatement();
	} else if (strcmp(t.lx, "if") == 0){
		ifStatement();
	} else if (strcmp(t.lx, "while") == 0){
		whileStatement();
	} else if (strcmp(t.lx, "do") == 0){
		doStatement();
	} else if (strcmp(t.lx, "return") == 0){
		returnStatemnt();
	} else if (strcmp(t.lx, "var") == 0){
		varDeclarStatement();
	} else {
		error(syntaxError);
	}
	return;
}

void varDeclarStatement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "var") == 0){
		GetNextToken(); // consume the token
		type();
		t = PeekNextToken();
		if (t.tp != ID){
			error(idExpected);
			return;
		}
		GetNextToken(); // consume the token
		while (1){
			t = PeekNextToken();
			if (strcmp(t.lx, ",") == 0){
				GetNextToken(); // consume the token
				t = PeekNextToken();
				if (t.tp != ID){
					error(idExpected);
					return;
				}
				GetNextToken(); // consume the token
			} else {
				break;
			}
		}
		if (strcmp(t.lx, ";") != 0){
			error(semicolonExpected);
			return;
		}
		GetNextToken(); // consume the token
	} else {
		error(syntaxError);
	}
	return;
}

void letStatement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "let") == 0){
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (t.tp != ID){
			error(idExpected);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "[") == 0){
			GetNextToken(); // consume the token
			expression();
			t = PeekNextToken();
			if (strcmp(t.lx, "]") != 0){
				error(closeBracketExpected);
				return;
			}
			GetNextToken(); // consume the token
			t = PeekNextToken();
		}
		if (strcmp(t.lx, "=") != 0){
			error(equalExpected);
			return;
		}
		GetNextToken(); // consume the token
		expression();
		t = PeekNextToken();
		if (strcmp(t.lx, ";") != 0){
			error(semicolonExpected);
			return;
		}
		GetNextToken(); // consume the token
	} else {
		error(syntaxError);
	} 
	return;
}

void ifStatement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "if") == 0){
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "(") != 0){
			error(openParenExpected);
			return;
		}
		GetNextToken(); // consume the token
		expression();
		t = PeekNextToken();
		if (strcmp(t.lx, ")") != 0){
			error(closeParenExpected);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "{") != 0){
			error(openBraceExpected);
			return;
		}
		GetNextToken(); // consume the token
		while (1){
			t = PeekNextToken();
			if (strcmp(t.lx, "}") == 0){
				break;
			}
			if (t.tp == EOFile){
				error(closeBraceExpected);
				return;
			}
			statement();
		}
		t = PeekNextToken();
		if (strcmp(t.lx, "else") == 0){
			GetNextToken(); // consume the token
			t = PeekNextToken();
			if (strcmp(t.lx, "{") != 0){
				error(openBraceExpected);
				return;
			}
			GetNextToken(); // consume the token
			while (1){
				t = PeekNextToken();
				if (strcmp(t.lx, "}") == 0){
					break;
				}
				if (t.tp == EOFile){
					error(closeBraceExpected);
					return;
				}
				statement();
			}
		}
	} else {
		error(syntaxError);
	}
	return;
}

void whileStatement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "while") == 0){
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "(") != 0){
			error(openParenExpected);
			return;
		}
		GetNextToken(); // consume the token
		expression();
		t = PeekNextToken();
		if (strcmp(t.lx, ")") != 0){
			error(closeParenExpected);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "{") != 0){
			error(openBraceExpected);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		while (strcmp(t.lx, "}") != 0){
			if (t.tp == EOFile){
				error(closeBraceExpected);
				return;
			}
			subroutineBody();
			t = PeekNextToken();
			
		}
		GetNextToken(); // consume the token
		// Process while statement
	} else {
		error(syntaxError);
	}
	return;
}

void doStatement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "do") == 0){
		GetNextToken(); // consume the token
		subroutineCall();
		t = PeekNextToken();
		if (strcmp(t.lx, ";") != 0){
			error(semicolonExpected);
		}
		GetNextToken(); // consume the token
		// Process do statement
	} else {
		error(syntaxError);
	}
	return;
}

void subroutineCall(){
	Token t = PeekNextToken();
	if (t.tp == ID){
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "(") == 0){
			GetNextToken(); // consume the token
			expressionList();
			t = GetNextToken();
			if (strcmp(t.lx, ")") != 0){
				error(closeParenExpected);
			}
		} else if (strcmp(t.lx, ".") == 0){
			GetNextToken(); // consume the token
			t = GetNextToken(); // get the next token
			if (t.tp != ID){
				error(idExpected);
				return;
			}
			t = PeekNextToken();
			if (strcmp(t.lx, "(") == 0){
				GetNextToken(); // consume the token
				expressionList();
				t = GetNextToken();
				if (strcmp(t.lx, ")") != 0){
					error(closeParenExpected);
				}
			}
		}
	} else {
		error(idExpected);
	}

	return;
}

void expressionList(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, ")") == 0){
		// empty expression list
	}
	else {
		expression();
		while (1){
			t = PeekNextToken();
			if (strcmp(t.lx, ",") == 0){
				GetNextToken(); // consume the token
				expression();
			} else {
				break;
			}
		}
	}
	return;
}

void returnStatemnt(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "return") == 0){
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, ";") == 0){
			GetNextToken(); // consume the token
			// Proces return statement with no expression
		} else {
			expression();
			t = GetNextToken();
			if (strcmp(t.lx, ";") != 0){
				error(semicolonExpected);
			}
			// Process return statement with expression
		}
	} else {
		error(syntaxError);
	}
	return;
}

void expression(){
	relationalExpression();
	while (1){
		Token t = PeekNextToken();
		if (strcmp(t.lx, "&") == 0 || strcmp(t.lx, "|") == 0){
			GetNextToken(); // consume the token
			relationalExpression();
		} else {
			break;
		}
	}
	// Process the expression
	return;
}

void relationalExpression(){
	ArithmeticExpression();
	while (1){
		Token t = PeekNextToken();
		if (strcmp(t.lx, "<") == 0 || strcmp(t.lx, ">") == 0 || strcmp(t.lx, "=") == 0){
			GetNextToken(); // consume the token
			ArithmeticExpression();

		} else {
			break;
		}
	}
	// Process the relational expression
	return;
}

void ArithmeticExpression(){
	term();
	while (1){
		Token t = PeekNextToken();
		if (strcmp(t.lx, "+") == 0 || strcmp(t.lx, "-") == 0){
			GetNextToken(); // consume the token
			term();
		} else {
			break;
		}
	}
	// Process the expression
	return;
}

void term(){
	factor();
	while (1){
		Token t = PeekNextToken();
		if (strcmp(t.lx, "*") == 0 || strcmp(t.lx, "/") == 0){
			GetNextToken(); // consume the token
			factor();
		} else {
			break;
		}
	}
	// Process the term
	return;
}

void factor(){
	Token t = PeekNextToken();
	if (strcpy(t.lx, "-") == 0 || strcmp(t.lx, "~") == 0){
		GetNextToken(); // consume the token
		// Process unary operator
		Operand();
	} else if (t.tp == INT || t.tp == ID || t.tp == STRING || t.tp == SYMBOL){
		GetNextToken(); // consume the token
		// Process operand
		Operand();
	} else {
		error(syntaxError);
	}
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
					error(closeBracketExpected);
				}
			// Check for function call
			} else if (strcmp(t.lx, "(") == 0){
				GetNextToken(); // consume the token
				expressionList();
				t = GetNextToken();
				if (strcmp(t.lx, ")") != 0){
					error(closeBracketExpected);
				}
			// Check for dot operator (method call)
			} else if (strcmp(t.lx, ".") == 0){
				GetNextToken(); // consume the token
				t = GetNextToken(); // get the next token
				if (t.tp != ID){
					error(idExpected);
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
				error(closeParenExpected);
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
		error(syntaxError);
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
