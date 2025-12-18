#ifndef DUMP_FUNCS
#define DUMP_FUNCS

extern FILE* log_file;
extern const char* log_file_name;

#include "lexical_analysis.h"
#include "tree_funcs.h"

void PrintTokenArray(TokenArray* tokens, size_t begin_pos);

void TreeDump(Tree* tree);

static char* GetNewImageFileName(int file_counter);

static char* GetNewDotCmd(int file_counter);

const char* GetNodeTypeName(Node* node);

void FillLogFile(char* image_file_name,  Tree* tree, int file_counter);

void PrintGraphizNode(FILE* graphiz_file,  Node* node);

void PrintGraphizEdge(FILE* graphiz_file,  Node* node);

void PrintBazeNode(FILE* graphiz_file,  Tree* tree);

void PrintBazeEdge(FILE* graphiz_file,  Tree* tree);

void PrintTree(Node* node, FILE* graphiz_file);

char* ConvertEncoding(char* win1251);

const char* GetNodeColor(Node* node);

void CloseLogFile();
void OpenLogFile();


#endif
