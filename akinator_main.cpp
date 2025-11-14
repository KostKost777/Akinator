#include<TXLib.h>

#include "akinator_funcs.h"
#include "akinator_dump_funcs.h"
#include "akinator_get_set_funcs.h"
#include "read_database_from_file.h"

int main() {

    atexit(CloseLogFile);
    OpenLogFile("akinator_log_file.html");

    const char* database_file_name = "database.txt";

    struct Buffer buffer = {};

    GetDataBaseFromFile(&buffer, database_file_name);

    //printf("BUFFER: %s\n SIZE: %d\n", buffer.data, buffer.size);

    struct Tree tree = {};

    TreeCtor(&tree);

    char* cur_pos = buffer.data;
    tree.root = FillNodeDataFromBuffer(&cur_pos, &tree.size, tree.root);

    //printf("BUFF: %s\n", buffer.data);
    //printf("CUR_POS: %s\n", cur_pos);

    TreeDump(&tree);

    StartAkinator(&tree);

    TreeDump(&tree);

    WriteDataBaseInFile(&tree, database_file_name);

    TreeDtor(&tree, &buffer);
    BufferDtor(&buffer);

    return 0;
}
