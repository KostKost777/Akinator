#include <TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

//GET

struct Node* GetYes(const struct Node* node)
{
    assert(node);

    return node->yes;
}

struct Node* GetNo(const struct Node* node)
{
    assert(node);

    return node->no;
}

struct Node* GetRoot(const struct Tree* tree)
{
    assert(tree);
    return tree->root;
}

int GetSize(const struct Tree* tree)
{
    assert(tree);

    return tree->size;
}

//SET

void SetYes(struct Node* node, struct Node* new_left)
{
    assert(node);

    node->yes = new_left;
}

void SetNo(struct Node* node, struct Node* new_right)
{
    assert(node);

    node->no = new_right;
}

void SetRoot(struct Tree* tree, struct Node* new_root)
{
    assert(tree);

    tree->root = new_root;
}

void SetSize(struct Tree* tree, int new_size)
{
    assert(tree);

    tree->size = new_size;
}

