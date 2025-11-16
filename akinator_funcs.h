#ifndef AKINATOR_FUNCS
#define AKINATOR_FUNCS

#include "STACK\stack_functions.h"

const int MAX_LEN = 50;

enum Ans
{
    EMPTY = 0,
    YES,
    NO,
};

enum Mode
{
    DESCR = 0,
    GUESS,
    EXIT,
    COMP
};

enum Status
{
    success = 0,
    error
};

struct Node
{
    char* data;
     Node* parent;
     Node* left;
     Node* right;
};

struct Tree
{
    int size;
    int code_err;
     Node* root;
};

#include "read_database_from_file.h"

extern FILE* log_file;

Status TreeCtor( Tree* tree);

Ans GetAnswer(void);

ssize_t my_getline(char** dest, size_t* n, FILE* file);

Node* NodeCtor(char* value,  Node* parent);

void PrintAkinatorOptions(void);

Status StartAkinator( Tree* tree);

Status CreateNewNode( Tree* tree,  Node* node);

void UpdateDataBase( Node* node, FILE* database_file);

Status GetDescription( Node* node, char* name,  Stack* path);

Status FindDescription( Tree* tree);

Status PrintDescription( Node* node, char* name,  Stack* path);

Status Guess( Tree* tree);

void TreeDtor( Tree* tree,  Buffer* buffer);

void DeleteNode( Tree* tree,  Node* node,  Buffer* buffer);

Status Comparation( Tree* tree);

Status WriteDataBaseInFile( Tree* tree, const char* database_file_name);

Status PrintComparison ( Node* node_1,  Node* node_2,
                             char* obj_1, char* obj_2,
                              Stack* stk_1_path,  Stack* stk_2_path);

Status PrintDiffDescription( Node** node,
                                 char* obj_1, char* obj_2, StackValueType last_el);

Status PrintEqualDescription( Node** node_1,  Node** node_2,
                                  char* obj_1, char* obj_2, StackValueType last_el);

Mode GetMode(void);

void CloseLogFile();

void OpenLogFile(const char* log_file_name);

bool IsDinamicMemory(void* ptr, void* buffer_ptr, int size);

#endif
