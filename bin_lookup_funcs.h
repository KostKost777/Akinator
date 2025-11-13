#ifndef BIN_LOOKUP_FUNCS
#define BIN_LOOKUP_FUNCS

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
    struct Node* parent;
    struct Node* left;
    struct Node* right;
};

struct Tree
{
    int size;
    int code_err;
    struct Node* root;
};

extern FILE* log_file;

enum Status TreeCtor(struct Tree* tree);

enum Ans GetAnswer(void);

void NodeDtor(struct Node* node);

struct Node* NodeCtor(char* value, struct Node* parent);

void PrintAkinatorOptions(void);

enum Status StartAkinator(struct Tree* tree);

enum Status CreateNewNode(struct Tree* tree, struct Node* node);

void UpdateDataBase(struct Node* node, FILE* database_file);

enum Status GetDescription(struct Node* node, char* name, struct Stack* path);

enum Status FindDescription(struct Tree* tree);

enum Status PrintDescription(struct Node* node, char* name, struct Stack* path);

enum Status Guess(struct Tree* tree);

void TreeDtor(struct Tree* tree, struct Buffer* buffer);

void BufferDtor(struct Buffer* buffer);

void DeleteNode(struct Tree* tree, struct Node* node, struct Buffer* buffer);

enum Status Comparation(struct Tree* tree);

enum Status WriteDataBaseInFile(struct Tree* tree, const char* database_file_name);

enum Status PrintComparison (struct Node* node_1, struct Node* node_2,
                             char* obj_1, char* obj_2,
                             struct Stack* stk_1_path, struct Stack* stk_2_path);

enum Status PrintDiffDescription(struct Node** node,
                                 char* obj_1, char* obj_2, StackValueType last_el);

enum Status PrintEqualDescription(struct Node** node_1, struct Node** node_2,
                                  char* obj_1, char* obj_2, StackValueType last_el);

enum Mode GetMode(void);

void Speak(const char* text);

void CloseLogFile();

void OpenLogFile(const char* log_file_name);

bool IsDinamicMemory(void* ptr, void* buffer_ptr, int size);

#endif
