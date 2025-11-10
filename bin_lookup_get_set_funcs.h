#ifndef BIN_LOOKUP_SET_GET_FUNCS
#define BIN_LOOKUP_SET_GET_FUNCS

//GET

struct Node* GetLeft(struct Node* node);

struct Node* GetRight(struct Node* node);

int GetSize(struct Tree* tree);

struct Node* GetRoot(struct Tree* tree);

//SET

void SetLeft(struct Node* node, struct Node* new_left);

void SetRight(struct Node* node, struct Node* new_right);

void SetRoot(struct Tree* tree, struct Node* new_root);

void SetSize(struct Tree* tree, int new_size);

#endif

