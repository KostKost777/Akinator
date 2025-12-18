#include <TXLib.h>

#include "lexical_analysis.h"
#include "syntactic_analysis.h"
#include "read_from_file_funcs.h"
#include "tree_funcs.h"
#include "dump_funcs.h"
#include "write_tree_in_file_funcs.h"

const char* source_file_name = "source.txt";
const char* out_file_name = "outfile.txt";

int main(const int argv, const char** argc)
{
    atexit(CloseLogFile);
    OpenLogFile();

    if (argv > 1)  out_file_name = argc[1];

    Buffer buffer = {};
    GetDataFromFile(&buffer, source_file_name);

    printf("BUFFER: %s \n", buffer.data);
    TokenArray tokens = {};
    TokenArrayCtor(&tokens);

    size_t pos = 0;

    MakeLexicalAnalysis(&buffer, &pos, &tokens);
    BufferDtor(&buffer);
    PrintTokenArray(&tokens, 0);

    Tree tree = {};
    TreeCtor(&tree);

    MakeSyntacticAnalysis(&tree, &tokens);

    TreeDump(&tree);

    WriteTreeInFile(&tree, out_file_name);

    TokenArrayDtor(&tokens);
}
