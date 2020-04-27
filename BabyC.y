%{
	#include <stdio.h>
	#include "BabyC.h"

// The parser needs to call the scanner to get the next token 
	extern int yylex();

// The error function 
	extern int yyerror(const char *);

// The ASTNode root
	extern ASTNode* gASTRoot;

%}

//Put any initialization code here 
%initial-action 
{
	
}

%token LE "<="
%token GE ">="
%token EQ "=="
%token NE "!="
%token OR "||"
%token AND "&&"
%token MAIN "main"
%token INT "int"
%token IF "if"
%token ELSE "else"
%token WHILE "while"

//Define the types for the grammar attributes
%union 
{
	ASTNode* node; // Most $$ values will be ASTNodes 
	int num; // Integer numbers
	char* string; // Strings for identifiers 
}

//Specify the type for each token. Only needed for IDENT and NUM, because they are the only ones that have actual regexp rules
%token <string> IDENT
%token <num> NUM

//Specify the type for each non-terminal in the grammar.
%type <node> Statement
%type <node> Assignment
%type <node> If
%type <node> While
%type <node> Condition
%type <node> Cond
%type <node> Compare
%type <node> Expr
%type <node> Term
%type <node> Factor
%type <node> Goal
%type <node> StatementList
%type <node> LHS


%%

//The grammar for BabyC, and the embedded actions for each production.

Goal: "main" '(' ')' '{' DeclarationList StatementList '}'	{gASTRoot=$6;} // Store the AST root node in gASTRoot
;

DeclarationList:  | Declaration DeclarationList // Note that a DeclarationList may be empty
;

Declaration: "int" IDENT ';' {AddDeclaration($2); printf("Processing declaration of %s\n", $2);}
;

Expr: Term				{$$ = $1;}
	| Expr '+' Term   	{$$ = CreateAddNode($1,$3); printf("Creating Addition node\n");}
	| Expr '-' Term    	{$$ = CreateSubNode($1,$3); printf("Creating Subtraction node\n");}
;

Term: Factor			{$$ = $1;}
	| Term '*' Factor	{$$ = CreateMultNode($1,$3); printf("Creating Multiplication node\n");}
	| Term '/' Factor	{$$ = CreateDivNode($1,$3); printf("Creating Division node\n");}
;
	
Factor: IDENT 		{$$ = CreateIdentNode($1, 0); printf("Creating IDENT node for %s\n", $1);}
	| NUM 			{$$ = CreateNumNode($1); printf("Creating NUM node for %d\n", $1);}
	| '('Expr')'	{$$ = $2; printf("Creating Expression node in parentheses\n");}
;

StatementList: 					{$$ = NULL;} 
    | Statement StatementList	{$$ = CreateStatementListNode($1,$2); printf("Adding a Statement to a Statement list \n");}
;

Statement: Assignment			{$$ = $1;}
	| If						{$$ = $1;}
	| While						{$$ = $1;}
;

While: "while" '(' Condition ')' '{' StatementList '}'	{$$ = CreateWhileNode($3,$6); printf("Creating while loop node\n");}
;

Assignment: LHS '=' Expr ';'	{$$ = CreateAssignNode($1,$3); printf("Creating Assignment node\n");}
;

LHS: IDENT						{$$ = CreateIdentNode($1, 1); printf("Creating left-hand IDENT node for %s\n", $1);}
;

If: "if" '(' Condition ')' '{' StatementList '}'									{$$ = CreateIfNode($3,$6, NULL); printf("Creating if Statement node\n");}
	| "if" '(' Condition ')' '{' StatementList '}' "else" '{' StatementList '}'		{$$ = CreateIfNode($3,$6,$10); printf("Creating if-else Statement node\n");}
;

Condition: Cond				{$$ = $1;}
	| Condition "||" Cond	{$$ = CreateORNode($1,$3); printf("Creating OR node\n");}
;

Cond: Compare				{$$ = $1;}
	| Cond "&&" Compare		{$$ = CreateANDNode($1,$3); printf("Creating AND node\n");}
;

Compare: Expr "==" Expr	{$$ = CreateCompareNode($1,10,$3); printf("Creating Compare node\n");}
	| Expr "!=" Expr	{$$ = CreateCompareNode($1,11,$3);	printf("Creating Compare node\n");}
	| Expr '>' Expr		{$$ = CreateCompareNode($1,6,$3);	printf("Creating Compare node\n");}
	| Expr '<' Expr		{$$ = CreateCompareNode($1,8,$3);	printf("Creating Compare node\n");}
	| Expr "<=" Expr	{$$ = CreateCompareNode($1,9,$3);	printf("Creating Compare node\n");}
	| Expr ">=" Expr	{$$ = CreateCompareNode($1,7,$3);	printf("Creating Compare node\n");}
;

%%
