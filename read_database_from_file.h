#ifndef READ_DATABASE_FUNCS
#define READ_DATABASE_FUNCS

#include "akinator_funcs.h"

struct Buffer
{
    char* data;
    int size;
};

void SkipSpaces(char** cur_pos);

void BufferDtor( Buffer* buffer);

int GetSizeOfFile(const char* filename);

Status GetDataBaseFromFile(Buffer* buffer,
                                const char* input_filename);

Node* FillNodeDataFromBuffer(char** cur_pos, int* size, Node* parent);

bool IsNil(char* cur_pos);

char* RaedName(char* cur_pos);

#endif
