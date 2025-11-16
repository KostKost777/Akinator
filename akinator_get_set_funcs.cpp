#include <TXLib.h>

#include "akinator_funcs.h"
#include "akinator_dump_funcs.h"
#include "akinator_get_set_funcs.h"

//GET

 Node* GetLeft( Node* node)
{
    assert(node);

    return node->left;
}

 Node* GetRight( Node* node)
{
    assert(node);

    return node->right;
}

 Node* GetRoot( Tree* tree)
{
    assert(tree);
    return tree->root;
}

int GetSize( Tree* tree)
{
    assert(tree);

    return tree->size;
}

//SET

void SetLeft( Node* node,  Node* new_left)
{
    assert(node);

    node->left = new_left;
}

void SetRight( Node* node,  Node* new_right)
{
    assert(node);

    node->right = new_right;
}

void SetRoot( Tree* tree,  Node* new_root)
{
    assert(tree);

    tree->root = new_root;
}

