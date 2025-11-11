#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

void TreeDump(struct Tree* tree)
{
    assert(tree);

    //fprintf(log_file, "<h3>TREE { %s %s:%d }</h3>", func, file, line);

    fprintf(log_file, "<h3>SIZE: %d\n</h3>", GetSize(tree));

    static int file_counter = 0;
    char* image_file_name = GetNewImageFileName(file_counter);
    FILE* graphiz_file = fopen(image_file_name, "w");

    fprintf(graphiz_file, "digraph {\n"
                          "rankdir = HR;\n"
                          "splines = true\n");

    PrintBazeNode(graphiz_file, tree);

    if (GetSize(tree) > 0) {

        PrintBazeEdge(graphiz_file, tree);
        PrintTree(GetRoot(tree), graphiz_file);
    }

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);
    free(image_file_name);

    char* command = GetNewDotCmd(file_counter);

    system(command);
    free(command);

    FillLogFile(image_file_name, tree, file_counter);

    file_counter++;


}

void PrintBazeNode(FILE* graphiz_file, struct Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file, "node%p "
                          "[shape = Mrecord, "
                          "style = filled, "
                          "fillcolor = \"#DD7538\", "
                          "color = \"#00000\", "
                          "label = \" {PTR: %p | SIZE: %d | ROOT: %p} \""
                          "]", tree, tree, GetSize(tree), GetRoot(tree));


}

void PrintBazeEdge(FILE* graphiz_file, struct Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n", tree,
                                                                 GetRoot(tree));
}

void PrintTree(struct Node* node, FILE* graphiz_file)
{
    assert(node);
    assert(graphiz_file);

    //printf("(");

    PrintGraphizNode(graphiz_file, node);
    PrintGraphizEdge(graphiz_file, node);

    if (GetLeft(node)){
        //printf("PTR: %p   PTR_LEFT: %p\n  ",node,  GetLeft(node));
        PrintTree(GetLeft(node), graphiz_file);
    }

    //printf("%d", GetData(node));

    if (GetRight(node))
        PrintTree(GetRight(node), graphiz_file);

    //printf(")");

}

void PrintGraphizEdge(FILE* graphiz_file, struct Node* node)
{
    assert(node);
    assert(graphiz_file);

    if (GetLeft(node) != NULL && GetRight(node) != NULL)
        fprintf(graphiz_file, "node%p -> { node%p node%p } [dir = both]\n",
                                                               node,
                                                               GetLeft(node),
                                                               GetRight(node));

    else if (GetLeft(node) != NULL && GetRight(node) == NULL)
        fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n", node,
                                                                     GetLeft(node));

    else if (GetLeft(node) == NULL && GetRight(node) != NULL)
        fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n", node,
                                                                     GetRight(node));
}

void PrintGraphizNode(FILE* graphiz_file, struct Node* node)
{
    assert(node);
    assert(graphiz_file);

    char* left_ptr = GetPrettyPtr(node->left);
    char* right_ptr = GetPrettyPtr(node->right);
    char* parent_ptr = GetPrettyPtr(node->parent);
    char* node_ptr = GetPrettyPtr(node);

    fprintf(graphiz_file, "node%p "
                          "[shape = Mrecord, "
                          "style = filled, "
                          "fillcolor = \"#8ABAD3\", "
                          "color = \"#00000\", "
                          "label = \" {PARANT_PTR: %s| PTR: %s | DATA: %s",
                          node, parent_ptr, node_ptr, node->data);

    if (node->left != NULL && node->right != NULL)
        fprintf(graphiz_file, " | {YES \\n PTR: %s | NO \\n PTR: %s}} \" ]\n",
                                                                     left_ptr,
                                                                     right_ptr);

    else
        fprintf(graphiz_file, "} \" ]\n");

    free(left_ptr);
    free(right_ptr);
    free(parent_ptr);
    free(node_ptr);
}

char* GetPrettyPtr(void* ptr)
{
    const int BUFFER_SIZE = 9;
    const int PRETTY_PTR_LEN = 11;

    char* pretty_ptr = (char*)calloc(PRETTY_PTR_LEN, sizeof(char));
    char buffer[BUFFER_SIZE] = {};

    snprintf(buffer, BUFFER_SIZE, "%p", ptr);

    int ptr_index = 0;

    for (int buffer_index = 0; buffer_index < BUFFER_SIZE; buffer_index++,
                                                           ptr_index++)
    {
        if (buffer_index == 2 || buffer_index == 5)
        {
            pretty_ptr[ptr_index] = '-';
            ptr_index++;
        }

        pretty_ptr[ptr_index] = toupper(buffer[buffer_index]);

    }

    return pretty_ptr;
}

void FillLogFile(char* image_file_name, struct Tree* tree, int file_counter)
{
    assert(image_file_name != NULL);
    assert(tree != NULL);

    fprintf(log_file, "\n\n<img src=image%d.png width=%dpx>\n\n",
                                                    file_counter, 500);

    fprintf(log_file, "--------------------------------------------------------------------------------------------------------------------------------------------\n\n");

    fflush(log_file);
}

static char* GetNewDotCmd(int file_counter)
{
    char str_command[100] = "";

    snprintf(str_command, sizeof(str_command),
            "dot -Tpng image%d.txt -o image%d.png",
             file_counter, file_counter);

    return strdup(str_command);
}

static char* GetNewImageFileName(int file_counter)
{
    char str_file_counter[100] = "";

    snprintf(str_file_counter, sizeof(str_file_counter),
             "image%d.txt", file_counter);

    return strdup(str_file_counter);
}

