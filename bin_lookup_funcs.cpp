#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

FILE* log_file = NULL;

enum Status TreeCtor(struct Tree* tree)
{
    assert(tree);

    tree->root = (Node*)calloc(1, sizeof(Node));

    if (tree->root == NULL)
        return error;

    return success;
}

struct Node* NodeCtor(char value[MAX_LEN])
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    assert(new_node);

    strncpy(new_node->data, value, MAX_LEN);

    return  new_node;
}

void CloseLogFile()
{
    fclose(log_file);
    printf("Logfile close");
}

void OpenLogFile(const char* log_file_name)
{
    log_file = fopen(log_file_name, "w");

    if (log_file != NULL)
        printf("Logfile open\n");

    else
        printf("Logfile open ERROR\n");

    fprintf(log_file, "<pre>\n");
}

