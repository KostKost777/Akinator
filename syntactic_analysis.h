#ifndef SYNTACTIC_ANALYSIS
#define SYNTACTIC_ANALYSIS

#include "tree_funcs.h"
#include "lexical_analysis.h"

Status MakeSyntacticAnalysis(Tree* tree, TokenArray* tokens);

Node* GetIdentifier(TokenArray* tokens, size_t* pos, Tree* tree);

Node* GetWord(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetNumber(TokenArray* tokens, size_t* pos, Tree* tree);

Node* GetMostPreority(TokenArray* tokens, size_t* pos,
                      Tree* tree, Node* node);

Node* GetPowOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetMulDivModOp(TokenArray* tokens, size_t* pos,
                     Tree* tree, Node* node);

Node* GetAddSubOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetGrammer(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetOperator(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetWhileOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetAssignedOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetIfOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Token GetSeparateToken(Type type);

Node* GetFuncArgs(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetFunc(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetInitOfFunc(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetArgsOfInitFunc(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetInOutOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetReturnOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetFirstCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetSecondCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetThirdCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetFourthCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetSqrt(TokenArray* tokens, size_t* pos, Tree* tree, Node* node);

Node* GetElseOp(TokenArray* tokens, size_t* pos, Tree* tree,
                Node* node, Node* node_action);

#endif
