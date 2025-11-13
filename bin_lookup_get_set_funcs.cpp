#include <TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

//GET

struct Node* GetLeft(struct Node* node)
{
    assert(node);

    return node->left;
}

struct Node* GetRight(struct Node* node)
{
    assert(node);

    return node->right;
}

struct Node* GetRoot(struct Tree* tree)
{
    assert(tree);
    return tree->root;
}

int GetSize(struct Tree* tree)
{
    assert(tree);

    return tree->size;
}

//SET

void SetLeft(struct Node* node, struct Node* new_left)
{
    assert(node);

    node->left = new_left;
}

void SetRight(struct Node* node, struct Node* new_right)
{
    assert(node);

    node->right = new_right;
}

void SetRoot(struct Tree* tree, struct Node* new_root)
{
    assert(tree);

    tree->root = new_root;
}

