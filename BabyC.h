#ifndef BABYC_H
#define BABYC_H
#define NOT_DEFINED -1

typedef enum{ASTNODE_ARITH_OP, ASTNODE_LOGIC_OP, ASTNODE_COMPARE, ASTNODE_ASSIGN, ASTNODE_LHIDENT, ASTNODE_IDENT, ASTNODE_NUM, ASTNODE_IF, ASTNODE_WHILE} ASTNodeType;

// Define all operation types
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
    ASTOp op; 			// The actual operation (add, mult, etc)
	int num;  			//Need to store the actual value for number nodes
	char *name; 		//Need to store the actual variable name for ident nodes
	ASTNode* left; 		// Left child
	ASTNode* right; 	// Right child
	ASTNode* next; 		// a pointer used to link statement nodes together in a statement list
	SymbolNode* sym;	//pointer to symbol table node
	ASTNode* elseSt;
	ASTNode* thenSt;
	ASTNode* cond;
	ASTNode* body;
	int reg;
};

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
void AddDeclaration(char* name);
void GenerateILOC(ASTNode* node);

#endif
