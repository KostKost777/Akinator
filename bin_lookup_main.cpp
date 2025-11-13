#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"
#include "read_database_from_file.h"

int main() {

    atexit(CloseLogFile);
    OpenLogFile("bin_lookup_log_file.html");

    struct Buffer buffer = {};

    GetDataBaseFromFile(&buffer, "database.txt");

    printf("BUFFER: %s\n SIZE: %d\n", buffer.data, buffer.size);

    struct Tree tree = {};

    TreeCtor(&tree);

    TreeDump(&tree);

    tree.size = 0;
    FillNodeDataFromBuffer(&buffer.data, &tree.root, &tree.size);

    printf("ROOT: %p", tree.root);

    TreeDump(&tree);

    StartAkinator(&tree);

    TreeDump(&tree);

    FILE* database_file = fopen("database.txt", "w");
    UpdateDataBase(GetRoot(&tree), database_file);
    fclose(database_file);

    TreeDtor(&tree);

    return 0;
}
