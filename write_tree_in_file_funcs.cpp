#include <TXLib.h>

#include "tree_funcs.h"
#include "dump_funcs.h"
#include "write_tree_in_file_funcs.h"

void WriteTreeInFile(Tree* tree, const char* out_file_name)
{
    assert(tree);
    assert(out_file_name);

    FILE* out_file = fopen(out_file_name, "w");

    WriteNode(tree->root, out_file);

    fclose(out_file);
}

void WriteNode(Node* node, FILE* out_file)
{
    assert(node);
    assert(out_file);

    fprintf(out_file,"( ");
    WriteNodeData(node, out_file);

    if (node->left != NULL)
        WriteNode(node->left, out_file);
    else
        fprintf(out_file, "nil ");

    if (node->right != NULL)
        WriteNode(node->right, out_file);
    else
        fprintf(out_file, "nil ");

    fprintf(out_file, ") ");
}

void WriteNodeData(Node* node, FILE* out_file)
{
    assert(node);
    assert(out_file);

    if (node->type == NUM)
        fprintf(out_file,"%d ", node->lexeme.num);

    else if (node->type == IDENT)
        fprintf(out_file,"%s ", node->lexeme.str.name);

    else if (node->type == KEY_LBRACE)
        fprintf(out_file,"{ ");

    else if (node->type == KEY_RBRACE)
        fprintf(out_file,"} ");

    else
        fprintf(out_file, "%s ", GetNodeTypeName(node));
}
