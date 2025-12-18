#ifndef WRITE_TREE_IN_FILE_FUNCS
#define WRITE_TREE_IN_FILE_FUNCS

#include "tree_funcs.h"

void WriteTreeInFile(Tree* tree, const char* out_file_name);

void WriteNode(Node* node, FILE* out_file);

void WriteNodeData(Node* node, FILE* out_file);

#endif
