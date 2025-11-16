#define TX_USE_SPEAK
#include<TXLib.h>

#include "akinator_funcs.h"
#include "akinator_dump_funcs.h"
#include "akinator_get_set_funcs.h"
#include "read_database_from_file.h"

int main(int argc, char* argv[])
{
    atexit(CloseLogFile);

//     FILE* test = fopen("test.txt", "w");
//
//     char word[50] = "Привет как дела";
//     fprintf(test, "%s", ConvertEncoding(word));
//
//     fclose(test);

    //txSpeak("Привет как твои дела");

    const char* database_file_name = "database.txt";

    if (argc > 1)
        database_file_name = argv[1];

    OpenLogFile("akinator_log_file.html");

    Buffer buffer = {};

    GetDataBaseFromFile(&buffer, database_file_name);

    //printf("BUFFER: %s\n SIZE: %d\n", buffer.data, buffer.size);

    Tree tree = {};

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
