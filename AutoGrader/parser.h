#define TEST_PARSER  // uncomment to run selfgrader for the parser

#include "lexer.h"

typedef enum {
	none,					// no errors
	lexerErr,				// lexer error
	classExpected,			// keyword class expected
	idExpected,				// identifier expected
	openBraceExpected,		// { expected
	closeBraceExpected,		// } expected
	memberDeclarErr,		// class member declaration must begin with static, field, constructor , function , or method
	classVarErr,			// class variables must begin with field or static
	illegalType,			// a type must be int, char, boolean, or identifier
	semicolonExpected,		// ; expected
	subroutineDeclarErr,	// subrouting declaration must begin with constructor, function, or method
	openParenExpected,		// ( expected
	closeParenExpected,		// ) expected
	closeBracketExpected,	// ] expected
	equalExpected,			// = expected
	syntaxError				// any other kind of syntax error
} SyntaxErrors;


// every parsing function should return this struct
// the struct contains an error type field (er)
// and a token field (tk) that is set to the token at or near which the error was encountered
// if no errors are encountered er should be set to none
typedef struct
{
	SyntaxErrors er;
	Token tk;
} ParserInfo;

int InitParser (char* file_name); // initialise the parser to parse source code in file_name
ParserInfo Parse (); // parse the input file (the one passed to InitParser)
int StopParser (); // stop the parser and do any necessary clean up
char* ErrorString (SyntaxErrors e);
void error (SyntaxErrors err, Token t); // print an error message and exit the program
void classDeclar(); // parse a class declaration
void memberDeclar(); // parse a class member declaration
void classVarDeclar(); // parse a class variable declaration
void type(); // parse a type (int, char, boolean, or identifier)
void subroutineDeclar(); // parse a subroutine declaration (constructor, function, or method)
void paramList(); // parse a parameter list (a list of types and identifiers)
void subroutineBody(); // parse a subroutine body (a list of statements)
void statement(); // parse a statement (let, if, while, do, return)
void varDeclarStatement(); // parse a variable declaration statement (var)
void letStatement(); // parse a let statement (let)
void ifStatement(); // parse an if statement (if)
void whileStatement(); // parse a while statement (while)
void doStatement(); // parse a do statement (do)
void subroutineCall(); // parse a subroutine call (a method or function call)
void expressionList(); // parse an expression list (a list of expressions)
void returnStatemnt(); // parse a return statement (return)
void expression(); // parse an expression (a term or a series of terms)
void relationalExpression(); // parse a relational expression (a term or a series of terms)
void ArithmeticExpression(); // parse an arithmetic expression (a term or a series of terms)
void term(); // parse a term (a variable, a constant, or an expression in parentheses)
void factor(); // parse a factor (a variable, a constant, or a string literal)
void Operand(); // parse an operand (a variable, a constant, or a string literal)