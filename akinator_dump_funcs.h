#ifndef AKINATOR_DUMP_FUNCS
#define AKINATOR_DUMP_FUNCS

enum ErrCodes
{
    BAD_SIZE = 1,
    BAD_ROOT = 2,
    BAD_PARENT = 4,
    BAD_DATA = 8
};

void PrintTree( Node* node, FILE* graphiz_file);

void TreeDump( Tree* tree);

void PrintGraphizNode(FILE* graphiz_file,  Node* node);

void PrintGraphizEdge(FILE* graphiz_file,  Node* node);

void PrintBazeNode(FILE* graphiz_file,  Tree* tree);

void PrintBazeEdge(FILE* graphiz_file,  Tree* tree);

int TreeVerifier( Tree* tree);

 Status CheckParents( Node* node);

 Status CheckDataPtr( Node* node);

char* GetPrettyPtr(void* ptr);

void PrintNameOfErrors(int code_err);

void FillLogFile(char* image_file_name,  Tree* tree, int file_counter);

static char* GetNewDotCmd(int file_counter);

static char* GetNewImageFileName(int file_counter);

char* ConvertEncoding(char* win1251);

#endif

