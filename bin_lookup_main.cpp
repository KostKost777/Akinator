#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"
#include "read_database_from_file.h"

int main() {

    atexit(CloseLogFile);
    OpenLogFile("bin_lookup_log_file.html");

    //speak("Здарова я жива нахуй, а ну быстро ебашь озвучку а то дед на кукан насадит");

    struct Buffer buffer = {};

    GetDataBaseFromFile(&buffer, "database.txt");

    printf("BUFFER: %s\n SIZE: %d\n", buffer.data, buffer.size);

    struct Tree tree = {};

    TreeCtor(&tree);

    char* cur_pos = buffer.data;
    tree.root = FillNodeDataFromBuffer(&cur_pos, &tree.size, tree.root);

    printf("BUFF: %s\n", buffer.data);
    printf("CUR_POS: %s\n", cur_pos);

    TreeDump(&tree);

    StartAkinator(&tree);

    TreeDump(&tree);

    WriteDataBaseInFile(&tree, "database.txt");

    TreeDtor(&tree, &buffer);
    BufferDtor(&buffer);

    return 0;
}
