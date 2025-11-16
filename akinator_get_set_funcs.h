#ifndef AKINATOR_SET_GET_FUNCS
#define AKINATOR_SET_GET_FUNCS

//GET

Node* GetLeft(Node* node);

Node* GetRight(Node* node);

int GetSize(Tree* tree);

Node* GetRoot(Tree* tree);

//SET

void SetLeft(Node* node, Node* new_left);

void SetRight(Node* node, Node* new_right);

void SetRoot(Tree* tree, Node* new_root);

#endif

