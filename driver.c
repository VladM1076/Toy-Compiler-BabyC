#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BabyC.h"

extern FILE* yyin;
extern int yyparse();
FILE* fout;

// A global variable that holds a pointer to the AST root
ASTNode* gASTRoot;

int main(int argc, char**argv)
{
        if (argc != 2){
                 printf("ERROR: Invalid number of command-line arguments. Usage: bcc File_Name.bc\n");
                 exit(1);
        }
    	yyin = fopen(argv[1], "r" );
        if (yyin == NULL){
                 printf("ERROR: Failed to open the input file\n");
                 exit(1);
        }

        // Call the parser.
        yyparse();
        fclose(yyin);
		
		char* testNum = strstr(argv[1], ".bc") - sizeof(char);
		
		char outputFile[] = {'t', 'e', 's', 't', testNum[0], '.', 'i', 'l', 'o', 'c', '\0'};
		
		fout = fopen(outputFile, "w");
		
		if (!fout)
		{
			printf("ERROR: Failed to open the output file\n");
            exit(1);
		}
		
        // Now that the AST has been constructed, pass its root to the function that traverses it and generates the ILOC code.
        GenerateILOC(gASTRoot);
		
		fclose(fout);
}
