#ifndef AKINATOR_DUMP_FUNCS
#define AKINATOR_DUMP_FUNCS

enum ErrCodes
{
    BAD_SIZE = 1,
    BAD_ROOT = 2,
    BAD_PARENT = 4,
    BAD_DATA = 8
};

void PrintTree(struct Node* node, FILE* graphiz_file);

void TreeDump(struct Tree* tree);

void PrintGraphizNode(FILE* graphiz_file, struct Node* node);

void PrintGraphizEdge(FILE* graphiz_file, struct Node* node);

void PrintBazeNode(FILE* graphiz_file, struct Tree* tree);

void PrintBazeEdge(FILE* graphiz_file, struct Tree* tree);

int TreeVerifier(struct Tree* tree);

enum Status CheckParents(struct Node* node);

enum Status CheckDataPtr(struct Node* node);

char* GetPrettyPtr(void* ptr);

void PrintNameOfErrors(int code_err);

void FillLogFile(char* image_file_name, struct Tree* tree, int file_counter);

static char* GetNewDotCmd(int file_counter);

static char* GetNewImageFileName(int file_counter);

#endif

