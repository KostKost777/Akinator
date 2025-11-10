#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

int main() {

    atexit(CloseLogFile);
    OpenLogFile("bin_lookup_log_file.html");

    struct Tree tree = {};

    TreeCtor(&tree);

    TreeDump(&tree);

    StartGuessing(&tree);
    TreeDump(&tree);
    StartGuessing(&tree);

    TreeDump(&tree);

    NodeDtor(tree.root->left);
    NodeDtor(tree.root->right);
    NodeDtor(tree.root);

    return 0;
}
