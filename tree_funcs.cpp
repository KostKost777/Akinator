#include <TXLib.h>

#include "tree_funcs.h"

Status TreeCtor(Tree* tree)
{
    assert(tree);

    tree->size = 0;
    tree->root = NULL;

    return success;
}

Node* NodeCtor(Node* parent, Token* token)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    assert(new_node);

    new_node->parent = parent;
    new_node->type = token->type;
    new_node->lexeme = token->lexeme;


    return new_node;
}

void TreeDtor(Tree* tree)
{
    DeleteNode(tree, tree->root);

    free(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

void DeleteNode(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    if (node->left != NULL)
        DeleteNode(tree, node->left);

    if (node->left != NULL)
        tree->size--;

    free(node->left);
    node->left = NULL;

    if (node->right != NULL)
        DeleteNode(tree, node->right);

    if (node->right != NULL)
        tree->size--;

    free(node->right);
    node->right = NULL;

    free(node->lexeme.str.name);
    node->lexeme.str.hash = 0;
}

Node* NewNode(Token token, Node* left, Node* right, Tree* tree)
{
    assert(tree);

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    assert(new_node);

    new_node->left = left;
    new_node->right = right;

    if (left) left->parent = new_node;
    if (right) right->parent = new_node;

    // if (left == NULL || right == NULL)
    //     new_node->args = UNARY;
    // else
    //     new_node->args = BINARY;

    new_node->type = token.type;
    new_node->lexeme = token.lexeme;

    tree->size++;

    return new_node;
}

