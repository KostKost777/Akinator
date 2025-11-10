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

struct Node* NodeCtor(char value[MAX_LEN], struct Node* parent);

enum Status CreateNewNode(struct Tree* tree, struct Node* node, enum Ans ans);

enum Status CreateFirstObject(struct Tree* tree);

void StartGuessing(struct Tree* tree);

void CloseLogFile();

void OpenLogFile(const char* log_file_name);

#endif
