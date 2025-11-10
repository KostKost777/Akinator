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

void PrintBazeNode(FILE* graphiz_file, const struct Tree* tree)
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

void PrintBazeEdge(FILE* graphiz_file, const struct Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n", tree,
                                                                 GetRoot(tree));
}

void PrintTree(const struct Node* node, FILE* graphiz_file)
{
    assert(node);
    assert(graphiz_file);

    //printf("(");

    PrintGraphizNode(graphiz_file, node);
    PrintGraphizEdge(graphiz_file, node);

    if (GetYes(node))
        PrintTree(GetYes(node), graphiz_file);

    //printf("%d", GetData(node));

    if (GetNo(node))
        PrintTree(GetNo(node), graphiz_file);

    //printf(")");

}

void PrintGraphizEdge(FILE* graphiz_file, const struct Node* node)
{
    assert(node);
    assert(graphiz_file);

    if (GetYes(node) != NULL && GetNo(node) != NULL)
        fprintf(graphiz_file, "node%p -> { node%p node%p } [dir = both]\n",
                                                               node,
                                                               GetYes(node),
                                                               GetNo(node));

    else if (GetYes(node) != NULL && GetNo(node) == NULL)
        fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n", node,
                                                                     GetYes(node));

    else if (GetYes(node) == NULL && GetNo(node) != NULL)
        fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n", node,
                                                                     GetNo(node));
}

void PrintGraphizNode(FILE* graphiz_file, const struct Node* node)
{
    assert(node);
    assert(graphiz_file);

    fprintf(graphiz_file, "node%p "
                          "[shape = Mrecord, "
                          "style = filled, "
                          "fillcolor = \"#8ABAD3\", "
                          "color = \"#00000\", "
                          "label = \" {PTR: %p | DATA: %s} \""
                          "]", node, node, node->data);
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

