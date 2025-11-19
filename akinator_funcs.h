#ifndef AKINATOR_FUNCS
#define AKINATOR_FUNCS

#include "STACK\stack_functions.h"

const int MAX_LEN = 50;
const int PUDGE_PHOTOS_NUM = 51;

const int X_CENTER = 0;
const int Y_CENTER = 400;

const int X_WINDOWS_SIZE = 610;
const int Y_WINDOWS_SIZE = 600;

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

Ans GetAnswer(HDC* video);

ssize_t my_getline(char** dest, size_t* n, FILE* file);

Node* NodeCtor(char* value,  Node* parent);

int  PrintAkinatorOptions(void);

Status StartAkinator(Tree* tree, HDC* video);

Status CreateNewNode( Tree* tree,  Node* node, HDC* video);

void UpdateDataBase( Node* node, FILE* database_file);

Status GetDescription( Node* node, char* name,  Stack* path);

Status FindDescription( Tree* tree, HDC* video);

Status PrintDescription(HDC* video, Node* node, char* name, Stack* path);

Status Guess( Tree* tree, HDC* video);

void TreeDtor( Tree* tree,  Buffer* buffer);

void DeleteNode( Tree* tree,  Node* node,  Buffer* buffer);

Status Comparation( Tree* tree, HDC* video);

Status WriteDataBaseInFile( Tree* tree, const char* database_file_name);

Status PrintComparison (Node* node_1,  Node* node_2, HDC* video,
                        char* obj_1, char* obj_2,
                        Stack* stk_1_path,  Stack* stk_2_path);

Status PrintDiffDescription(Node** node, int* y_now, HDC* video,
                            char* obj_1, char* obj_2, StackValueType last_el);

Status PrintEqualDescription(Node** node_1,  Node** node_2,
                             int* y_now, HDC* video,
                             char* obj_1, char* obj_2, StackValueType last_el);

Mode GetMode(HDC* video);

void CloseLogFile();

void OpenLogFile(const char* log_file_name);

bool IsDinamicMemory(void* ptr, void* buffer_ptr, int size);

void ProcessingPudgePhotos(HDC* pudge_photo);

int PrintPhrase(int x, int y, const char* message, ...);

int PrintAndSayPhrase(HDC* pudge_photo, int x, int y, const char* message, ...);

void PudgePhotosDtor(HDC* pudge_photo);

void CleanWindow(HDC* pudge_photo);

#endif
