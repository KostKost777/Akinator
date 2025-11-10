#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

int main() {

    atexit(CloseLogFile);
    OpenLogFile("bin_lookup_log_file.html");

    struct Tree tree = {};

    TreeCtor(&tree);

    char data[MAX_LEN] = "Zhivotnoe";

    tree.size = 3;

    strncpy(tree.root->data, data, MAX_LEN);

    strncpy(data, "Poltorashka", MAX_LEN);
    tree.root->left = NodeCtor(data, tree.root);

    strncpy(data, "Divari", MAX_LEN);
    tree.root->right = NodeCtor(data, tree.root);

    TreeDump(&tree);

    StartGuessing(&tree);

    TreeDump(&tree);

    return 0;
}
