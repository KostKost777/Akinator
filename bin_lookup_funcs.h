#ifndef BIN_LOOKUP_FUNCS
#define BIN_LOOKUP_FUNCS

const int MAX_LEN = 50;

enum Status
{
    success = 0,
    error = 1
};

struct Node
{
    char data[MAX_LEN];
    struct Node* yes;
    struct Node* no;
};

struct Tree
{
    int size;
    int code_err;
    struct Node* root;
};

extern FILE* log_file;

enum Status TreeCtor(struct Tree* tree);

struct Node* NodeCtor(char value[MAX_LEN]);

void CloseLogFile();

void OpenLogFile(const char* log_file_name);

#endif
