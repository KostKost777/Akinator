#ifndef BIN_LOOKUP_SET_GET_FUNCS
#define BIN_LOOKUP_SET_GET_FUNCS

//GET

struct Node* GetYes(const struct Node* node);

struct Node* GetNo(const struct Node* node);

struct Node* GetRoot(const struct Tree* tree);

int GetSize(const struct Tree* tree);

//SET

void SetYes(struct Node* node, struct Node* new_left);

void SetNo(struct Node* node, struct Node* new_right);

void SetRoot(struct Tree* tree, struct Node* new_root);

void SetSize(struct Tree* tree, int new_size);

#endif

