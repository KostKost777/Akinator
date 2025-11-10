#ifndef BIN_LOOKUP_DUMP_FUNCS
#define BIN_LOOKUP_DUMP_FUNCS

void PrintTree(struct Node* node, FILE* graphiz_file);

void TreeDump(struct Tree* tree);

void PrintGraphizNode(FILE* graphiz_file, struct Node* node);

void PrintGraphizEdge(FILE* graphiz_file, struct Node* node);

void PrintBazeNode(FILE* graphiz_file, struct Tree* tree);

void PrintBazeEdge(FILE* graphiz_file, struct Tree* tree);

void FillLogFile(char* image_file_name, struct Tree* tree, int file_counter);

static char* GetNewDotCmd(int file_counter);

static char* GetNewImageFileName(int file_counter);

#endif

