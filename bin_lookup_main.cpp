#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

int main() {

    atexit(CloseLogFile);
    OpenLogFile("bin_lookup_log_file.html");

    struct Tree tree = {};

    TreeCtor(&tree);

    char data[MAX_LEN] = "Zhivotnoe?";

    tree.size = 3;

    strncpy(tree.root->data, data, MAX_LEN);
    printf("PTR_ROOT: %p  DATA: %s\n", tree.root, tree.root->data);

    strncpy(data, "Poltorashka", MAX_LEN);


    tree.root->yes = NodeCtor(data);
    printf("PTR: %p  DATA: %s\n", tree.root->yes, tree.root->yes->data);


    strncpy(data, "Petrovich", MAX_LEN);

    tree.root->no = NodeCtor(data);
    printf("PTR: %p  DATA: %s\n", tree.root->no, tree.root->no->data);

    TreeDump(&tree);

    return 0;
}
