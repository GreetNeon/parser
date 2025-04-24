#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below

ParserInfo pi;

void error (SyntaxErrors err, Token t){
	pi.er = err;
	pi.tk = t;
}

void classDeclar(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "class") == 0){
		GetNextToken(); // consume the token
	} else {
		error(classExpected, t);
		return;
	}
	t = PeekNextToken();
	if (t.tp != ID){
		error(idExpected, t);
		return;
	}
	GetNextToken(); // consume the token
	t = PeekNextToken();
	if (strcmp(t.lx, "{") != 0){
		error(openBraceExpected, t);
		return;
	}
	GetNextToken(); // consume the token
	while (1){
		t = PeekNextToken();
		if (strcmp(t.lx, "}") == 0){
			GetNextToken(); // consume the token
			break;
		}
		if (t.tp == EOFile){
			error(closeBraceExpected, t);
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
		error(memberDeclarErr, t);
		return;
	}
	return;
}

void classVarDeclar(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0){
		GetNextToken(); // consume the token
	} else {
		error(memberDeclarErr, t);
		return;
	}
	type();
	while(1){
		t = PeekNextToken();
		if (t.tp != ID){
			error(idExpected, t);
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
		error(semicolonExpected, t);
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
		error(illegalType, t);
		return;
	}
	return;
}

void subroutineDeclar(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0){
		GetNextToken(); // consume the token
	} else {
		error(subroutineDeclarErr, t);
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
		error(idExpected, t);
		return;
	}
	GetNextToken(); // consume the token
	t = PeekNextToken();
	if (strcmp(t.lx, "(") != 0){
		error(openParenExpected, t);
		return;
	}
	GetNextToken(); // consume the token
	paramList();
	t = PeekNextToken();
	if (strcmp(t.lx, ")") != 0){
		error(closeParenExpected, t);
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
			error(idExpected, t);
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
	int i = 0;
	if (strcmp(t.lx, "{") == 0){
		GetNextToken(); // consume the token
		while (i < 30){
			printf("subroutineBody current token %s\n", t.lx);
			t = PeekNextToken();
			if (strcmp(t.lx, "}") == 0){
				GetNextToken(); // consume the token
				break;
			}
			if (t.tp == EOFile){
				error(closeBraceExpected, t);
				return;
			}
			statement();
			i++;
		}
		GetNextToken(); // consume the token
	} else {
		error(openBraceExpected, t);
	}
	return;
}

void statement(){
	Token t = PeekNextToken();
	printf("statement current token %s\n", t.lx);
	if (strcmp(t.lx, "let") == 0){
		printf("let statement\n");
		letStatement();
	} else if (strcmp(t.lx, "if") == 0){
		printf("if statement\n");
		ifStatement();
	} else if (strcmp(t.lx, "while") == 0){
		printf("while statement\n");
		whileStatement();
	} else if (strcmp(t.lx, "do") == 0){
		printf("do statement\n");
		doStatement();
	} else if (strcmp(t.lx, "return") == 0){
		printf("return statement\n");
		returnStatemnt();
	} else if (strcmp(t.lx, "var") == 0){
		printf("var statement\n");
		varDeclarStatement();
	} else {
		printf("syntax error in statement\n");
		error(syntaxError, t);
	}
	printf("statement returns\n");
	return;
}

void varDeclarStatement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "var") == 0){
		GetNextToken(); // consume the token
		type();
		t = PeekNextToken();
		if (t.tp != ID){
			error(idExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		while (1){
			t = PeekNextToken();
			if (strcmp(t.lx, ",") == 0){
				GetNextToken(); // consume the token
				t = PeekNextToken();
				if (t.tp != ID){
					error(idExpected, t);
					return;
				}
				GetNextToken(); // consume the token
			} else {
				break;
			}
		}
		if (strcmp(t.lx, ";") != 0){
			error(semicolonExpected, t);
			return;
		}
		GetNextToken(); // consume the token
	} else {
		error(syntaxError, t);
	}
	return;
}

void letStatement(){
	Token t = PeekNextToken();
	printf("letStatement current token %s\n", t.lx);
	if (strcmp(t.lx, "let") == 0){
		GetNextToken(); // consume the token
		t = PeekNextToken();
		printf("current token %s\n", t.lx);
		if (t.tp != ID){
			error(idExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "[") == 0){
			GetNextToken(); // consume the token
			expression();
			t = PeekNextToken();
			if (strcmp(t.lx, "]") != 0){
				error(closeBracketExpected, t);
				return;
			}
			GetNextToken(); // consume the token
			t = PeekNextToken();
		}
		if (strcmp(t.lx, "=") != 0){
			printf("current token not equal %s\n", t.lx);

			error(equalExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		printf("preexpression token %s\n", t.lx);
		expression();
		t = PeekNextToken();
		if (strcmp(t.lx, ";") != 0){
			error(semicolonExpected, t);
			return;
		}
		GetNextToken(); // consume the token
	} else {
		error(syntaxError, t);
	} 
	return;
}

void ifStatement(){
	Token t = PeekNextToken();
	if (strcmp(t.lx, "if") == 0){
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "(") != 0){
			error(openParenExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		expression();
		t = PeekNextToken();
		if (strcmp(t.lx, ")") != 0){
			error(closeParenExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "{") != 0){
			error(openBraceExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		while (1){
			t = PeekNextToken();
			if (strcmp(t.lx, "}") == 0){
				GetNextToken(); // consume the token
				break;
			}
			if (t.tp == EOFile){
				error(closeBraceExpected, t);
				return;
			}
			printf("if statement current token %s\n", t.lx);
			statement();
		}
		t = PeekNextToken();
		if (strcmp(t.lx, "else") == 0){
			GetNextToken(); // consume the token
			t = PeekNextToken();
			if (strcmp(t.lx, "{") != 0){
				error(openBraceExpected, t);
				return;
			}
			GetNextToken(); // consume the token
			while (1){
				t = PeekNextToken();
				if (strcmp(t.lx, "}") == 0){
					GetNextToken(); // consume the token
					break;
				}
				if (t.tp == EOFile){
					error(closeBraceExpected, t);
					return;
				}
				statement();
			}
		}
	} else {
		error(syntaxError, t);
	}
	return;
}

void whileStatement(){
	Token t = PeekNextToken();
	int i = 0;
	if (strcmp(t.lx, "while") == 0){
		printf("while statement\n");
		GetNextToken(); // consume the token
		t = PeekNextToken();
		if (strcmp(t.lx, "(") != 0){
			error(openParenExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		expression();
		t = PeekNextToken();
		if (strcmp(t.lx, ")") != 0){
			error(closeParenExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		printf("while statement current token %s\n", t.lx);
		if (strcmp(t.lx, "{") != 0){
			error(openBraceExpected, t);
			return;
		}
		GetNextToken(); // consume the token
		t = PeekNextToken();
		while (strcmp(t.lx, "}" ) != 0 && i < 10){
			printf("while loop current token %s\n", t.lx);
			printf("%d\n", i);
			if (t.tp == EOFile){
				error(closeBraceExpected, t);
				return;
			}
			statement();
			printf("while loop current token %s\n", t.lx);
			t = PeekNextToken();
			i++;
		}
		GetNextToken(); // consume the token
		// Process while statement
	} else {
		error(syntaxError, t);
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
			error(semicolonExpected, t);
		}
		GetNextToken(); // consume the token
		// Process do statement
	} else {
		error(syntaxError, t);
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
				error(closeParenExpected, t);
			}
		} else if (strcmp(t.lx, ".") == 0){
			GetNextToken(); // consume the token
			t = GetNextToken(); // get the next token
			if (t.tp != ID){
				error(idExpected, t);
				return;
			}
			t = PeekNextToken();
			if (strcmp(t.lx, "(") == 0){
				GetNextToken(); // consume the token
				expressionList();
				t = GetNextToken();
				if (strcmp(t.lx, ")") != 0){
					error(closeParenExpected, t);
				}
			}
		}
	} else {
		error(idExpected, t);
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
	printf("returnStatemnt current token %s\n", t.lx);
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
				error(semicolonExpected, t);
			}
			printf("token: %s", t.lx); // consume the token
			// Process return statement with expression
		}
	} else {
		error(syntaxError, t);
	}
	return;
}

void expression(){
	relationalExpression();
	while (1){
		Token t = PeekNextToken();
		printf("expression current token %s\n", t.lx);
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
		printf("relationalExpression current token %s\n", t.lx);
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
	printf("factor current token %s\n", t.lx);
	if (strcpy(t.lx, "-") == 0 || strcmp(t.lx, "~") == 0){
		GetNextToken(); // consume the token
		// Process unary operator
		Operand();
	} else if (t.tp == INT || t.tp == ID || t.tp == STRING || t.tp == SYMBOL){
		// Process operand
		Operand();
	} else {
		error(syntaxError, t);
	}
	return;
}

void Operand(){
	Token t = PeekNextToken();
	printf("Operand current token %s\n", t.lx);
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
					error(closeBracketExpected, t);
					return;
				}
			// Check for function call
			} else if (strcmp(t.lx, "(") == 0){
				GetNextToken(); // consume the token
				expressionList();
				t = GetNextToken();
				if (strcmp(t.lx, ")") != 0){
					error(closeBracketExpected, t);
					return;
				}
			// Check for dot operator (method call)
			} else if (strcmp(t.lx, ".") == 0){
				GetNextToken(); // consume the token
				t = GetNextToken(); // get the next token
				if (t.tp != ID){
					error(idExpected, t);
					return;
				}
				t = PeekNextToken();
				if (strcmp(t.lx, "(") == 0){
					GetNextToken(); // consume the token
					expressionList();
					t = GetNextToken();
					if (strcmp(t.lx, ")") != 0){
						error(closeBracketExpected, t);
						return;
					}
				}
				else if (strcmp(t.lx, "[") == 0){
					GetNextToken(); // consume the token
					expression();
					t = GetNextToken();
					if (strcmp(t.lx, "]") != 0){
						error(closeBracketExpected, t);
						return;
					}
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
				error(closeParenExpected, t);
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
		error(syntaxError, t);
	}
}


int InitParser (char* file_name)
{
	Token temp; temp.tp = ERR;
	InitLexer(file_name);
	pi.er = none;
	pi.tk = temp;
	return 1;
}

ParserInfo Parse ()
{
	// implement the function
	// parse the input file (the one passed to InitParser)
	// and return the ParserInfo struct
	Token t = PeekNextToken();
	while (1){
		t = PeekNextToken();
		printf("%s\n", t.lx);
		if (t.tp == EOFile){
			break;
		}
		if (t.tp == ERR){
			error(lexerErr, t);
			break;
		}
		if (strcmp(t.lx, "class") == 0){
			classDeclar();
		}
		else if (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0 || strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0){
			memberDeclar();
		}
		else{
			statement();
			printf("Here\n");
		}
		printf("Parse current token %s\nError: %d", t.lx, pi.er);
		if (pi.er != none){
			break;
		}
	}
	printf("Parse returns\n Current token %s\n", t.lx);
	return pi;
}


int StopParser ()
{
	StopLexer();
	pi.er = none;
	pi.tk.tp = ERR;
	return 1;
}

#ifndef TEST_PARSER
int main ()
{
	printf("Parser module\n");
	InitParser("Main.jack");
	ParserInfo pi = Parse();
	StopParser();
	return 0;
}
#endif
