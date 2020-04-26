#ifndef BABYC_H
#define BABYC_H
#define NOT_DEFINED -1

typedef enum{ASTNODE_ARITH_OP, ASTNODE_LOGIC_OP, ASTNODE_COMPARE, ASTNODE_ASSIGN, ASTNODE_LHIDENT, ASTNODE_IDENT, ASTNODE_NUM, ASTNODE_IF, ASTNODE_WHILE} ASTNodeType;

// Define all operation types (add, mult, etc) here.
typedef enum{ADD_OP, MULT_OP, SUB_OP, DIV_OP, AND_OP, OR_OP, GT_OP, GE_OP, LT_OP, LE_OP, EQ_OP, NE_OP} ASTOp;


typedef struct SymbolNode SymbolNode;
struct SymbolNode{
	char* name;
	int offset;
	SymbolNode* next;
};

typedef struct ASTNode ASTNode;
struct ASTNode{
	ASTNodeType type; 
    ASTOp op; // The actual operation (add, mult, etc)
	int num;  //Need to store the actual value for number nodes
	char *name; //Need to store the actual variable name for ident nodes
	ASTNode* left; // Left child
	ASTNode* right; // Right child
	
	// The following pointer is just a suggestion; you don't have to use it if you have a better design.
	ASTNode* next; // a pointer used to link statement nodes together in a statement list
	
// Depending on your implementation, you may need to add other fields to this struct 
	SymbolNode* sym;
	ASTNode* elseSt;
	ASTNode* thenSt;
	ASTNode* cond;
	ASTNode* body;
	int reg;
};

//SymbolNode* symRoot = (SymbolNode*)0;	//creates multiple definition error if defined here, moved to .c

// Add functions to create the different kinds of ASTNodes
// These functions are called by the code embedded in the grammar.
// Here are some samples:
ASTNode* CreateNumNode(int num);
ASTNode* CreateIdentNode(char* name, int lhident);
ASTNode* CreateStatementListNode(ASTNode* st, ASTNode* stList);
ASTNode* CreateAddNode(ASTNode* left, ASTNode* right);
ASTNode* CreateSubNode(ASTNode* left, ASTNode* right);
ASTNode* CreateMultNode(ASTNode* left, ASTNode* right);
ASTNode* CreateDivNode(ASTNode* left, ASTNode* right);
ASTNode* CreateAssignNode(ASTNode* left, ASTNode* right);
ASTNode* CreateIfNode(ASTNode* cond, ASTNode* stList, ASTNode* elseList);
ASTNode* CreateCompareNode(ASTNode* left, ASTOp op, ASTNode* right);
ASTNode* CreateWhileNode(ASTNode* cond, ASTNode* body);
ASTNode* CreateORNode(ASTNode* left, ASTNode* right);
ASTNode* CreateANDNode(ASTNode* left, ASTNode* right);
SymbolNode* CheckSymbolList(char* name);
void LinkIdentNodes(ASTNode* node);
int GetNextReg();
int GetNextLabel();

// Need a function to add a declaration to your symbol table
void AddDeclaration(char* name);

// This is the function that generates ILOC code after the construction of the AST
void GenerateILOC(ASTNode* node);

#endif
