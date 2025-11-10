#ifndef BIN_LOOKUP_FUNCS
#define BIN_LOOKUP_FUNCS

const int MAX_LEN = 50;

enum Ans
{
    YES = 1,
    NO = 0
};

enum Status
{
    success = 0,
    error = 1
};

struct Node
{
    struct Node* parent;
    char data[MAX_LEN];
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

struct Node* NodeCtor(char value[MAX_LEN], struct Node* parent);

enum Status CreateNewNode(struct Node* node, enum Ans ans);

void StartGuessing(struct Tree* tree);

void CloseLogFile();

void OpenLogFile(const char* log_file_name);

#endif
