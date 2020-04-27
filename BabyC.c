#include "BabyC.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SymbolNode* symRoot = (SymbolNode*)0;	//couldnt define in .h, got link errors
SymbolNode* symTail = (SymbolNode*)0;
int symListSize = 0;
int virtReg = 1;
int labelCnt = 1;

FILE* fout;

ASTNode* CreateNumNode(int num)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = ASTNODE_NUM; 
    node->num = num;
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
    return node;
}


ASTNode* CreateIdentNode(char* name, int lhident)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	
	if (lhident == 0)
		node->type = ASTNODE_IDENT;
	else if (lhident == 1)
		node->type = ASTNODE_LHIDENT;
	
	node->name = name;
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	LinkIdentNodes(node);
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateStatementListNode(ASTNode* st, ASTNode* stList)
{
	if (stList)	//check if NULL
	{
		st->next = stList;
		return st;
	}
}


void AddDeclaration(char* name)
{
	if (symRoot == (SymbolNode*)0)	//bit of a hack, couldnt define as NULL outside function
		symRoot = symTail = NULL;
	
	if(CheckSymbolList(name))
	{
		char error[] = "Multiple declarations of ' '";
		error[26] = *name;		//also bit of a hack, error only works with 1 character variable names
		yyerror(error);
	}
	
	SymbolNode* new = (SymbolNode*)malloc(sizeof(SymbolNode));
	new->name = name;
	new->offset = symListSize * 4;
	new->next = NULL;
	
	if (symRoot)	//add to list
	{
		symTail->next = new;
		symTail = symTail->next;
	}
	else if (!symRoot)	//first node to be added
		symRoot = symTail = new;
	
	symListSize++;
}


ASTNode* CreateAddNode(ASTNode* left, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = ADD_OP;
	node->left = left;
	node->right = right;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateSubNode(ASTNode* left, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = SUB_OP;
	node->left = left;
	node->right = right;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateMultNode(ASTNode* left, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = MULT_OP;
	node->left = left;
	node->right = right;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateDivNode(ASTNode* left, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ARITH_OP;
	node->op = DIV_OP;
	node->left = left;
	node->right = right;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateAssignNode(ASTNode* left, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_ASSIGN;
	node->left = left;
	node->right = right;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateIfNode(ASTNode* cond, ASTNode* stList, ASTNode* elseList)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_IF;
	node->cond = cond;
	node->thenSt = stList;
	node->elseSt = elseList;	//is null for if only node		
	node->next = NULL;
	node->sym = NULL;
	node->left = NULL;
	node->right = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateCompareNode(ASTNode* left, ASTOp op, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_COMPARE;
	node->op = op;
	node->left = left;
	node->right = right;	
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}

ASTNode* CreateWhileNode(ASTNode* cond, ASTNode* body)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_WHILE;
	node->cond = cond;
	node->body = body;
	node->left = NULL;
	node->right = NULL;	
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->reg = NOT_DEFINED;
	return node;
}



ASTNode* CreateORNode(ASTNode* left, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_LOGIC_OP;
	node->op = OR_OP;
	node->left = left;
	node->right = right;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


ASTNode* CreateANDNode(ASTNode* left, ASTNode* right)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTNODE_LOGIC_OP;
	node->op = AND_OP;
	node->left = left;
	node->right = right;
	node->next = NULL;
	node->sym = NULL;
	node->elseSt = NULL;
	node->thenSt = NULL;
	node->cond = NULL;
	node->body = NULL;
	node->reg = NOT_DEFINED;
	return node;
}


void LinkIdentNodes(ASTNode* node)
{
	SymbolNode* sym;
	
	if (node->type == ASTNODE_IDENT || node->type == ASTNODE_LHIDENT)
	{
		sym = CheckSymbolList(node->name);
		
		if (sym)
			node->sym = sym;
		else
		{
			yyerror("Ident not declared");
		}
	}
}


SymbolNode* CheckSymbolList(char* name)
{
	SymbolNode* cur = symRoot;
	
	while (cur)	//check if symbol is in list
	{
		if (strcmp(cur->name, name) == 0)
		{
			return cur;
		}			
		cur = cur->next;
	}
	
	return NULL;
}


//Traverses using DFS and prints ILOC to file
void GenerateILOC(ASTNode* node)
{
	if(!node)
		return;
	
	GenerateILOC(node->left);
	GenerateILOC(node->right);
	
	if (node->type == ASTNODE_WHILE)
	{
		int labelCnt = GetNextLabel();
		fprintf(fout, "L%d_C:\n", labelCnt);
		GenerateILOC(node->cond);
		fprintf(fout, "	cbr r%d -> L%d_B, L%d_O\n", node->cond->reg, labelCnt, labelCnt);
		fprintf(fout, "L%d_B:\n", labelCnt);
		GenerateILOC(node->body);
		fprintf(fout, "	jumpi -> L%d_C\n", labelCnt);
		fprintf(fout, "L%d_O:\n", labelCnt);
	}
	
	if (node->type == ASTNODE_IF)
	{
		int labelCnt = GetNextLabel();
		GenerateILOC(node->cond);
		
		if (!node->elseSt)
			fprintf(fout, "	cbr r%d -> L%d_T, L%d_M\n", node->cond->reg, labelCnt, labelCnt);
		else
			fprintf(fout, "	cbr r%d -> L%d_T, L%d_E\n", node->cond->reg, labelCnt, labelCnt);
		
		
		fprintf(fout, "L%d_T:\n", labelCnt);
		GenerateILOC(node->thenSt);
		
		if (node->elseSt)
		{
			fprintf(fout, "	jumpi -> L%d_M\n", labelCnt);
			fprintf(fout, "L%d_E:\n", labelCnt);
			GenerateILOC(node->elseSt);
		}
		
		fprintf(fout, "L%d_M:\n", labelCnt);
	}

	switch(node->type)
	{
		case ASTNODE_LHIDENT:	//these are nodes that dont need to do anything in this case,
			break;				//I am just using them to keep track of nodes completed via the default print.
			
		case ASTNODE_WHILE:
			break;
			
		case ASTNODE_IF:
			break;
			
		case ASTNODE_IDENT:
			node->reg = GetNextReg();
			fprintf(fout, "	loadAI rarp, %d -> r%d\n", node->sym->offset, node->reg);
			break;
			
		case ASTNODE_NUM:
			node->reg = GetNextReg();
			fprintf(fout, "	loadi %d -> r%d\n", node->num, node->reg);
			break;
			
		case ASTNODE_ASSIGN:
			fprintf(fout, "	storeAI r%d -> rarp, %d\n", node->right->reg, node->left->sym->offset);
			break;
			
		case ASTNODE_ARITH_OP:
			node->reg = GetNextReg();
		
			switch(node->op)
			{
				case ADD_OP:
					fprintf(fout, "	add r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case SUB_OP:
					fprintf(fout, "	sub r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case MULT_OP:
					fprintf(fout, "	mult r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case DIV_OP:
					fprintf(fout, "	div r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				default:
					fprintf(fout, "Missed node case %d\n", node->type);
			}
			break;
		
		case ASTNODE_LOGIC_OP:
			node->reg = GetNextReg();
			
			switch(node->op)
			{
				case AND_OP:
					fprintf(fout, "	and r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case OR_OP:
					fprintf(fout, "	or r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
				
				default:
					fprintf(fout, "Missed node case %d\n", node->type);
			}
			break;
			
		case ASTNODE_COMPARE:
			node->reg = GetNextReg();
			
			switch(node->op)
			{
				case GT_OP:
					fprintf(fout, "	cmp_GT r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case GE_OP:
					fprintf(fout, "	cmp_GE r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case LT_OP:
					fprintf(fout, "	cmp_LT r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case LE_OP:
					fprintf(fout, "	cmp_LE r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case EQ_OP:
					fprintf(fout, "	cmp_EQ r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
					
				case NE_OP:
					fprintf(fout, "	cmp_NE r%d, r%d -> r%d\n", node->left->reg, node->right->reg, node->reg);
					break;
			}
			break;
			
		default:	//to make sure i dont miss any cases
			fprintf(fout, "Missed node case %d\n", node->type);
	}
	
	GenerateILOC(node->next);
	
}


int GetNextReg(){return virtReg++;}


int GetNextLabel(){return labelCnt++;}

