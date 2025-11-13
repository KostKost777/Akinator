#ifndef READ_DATABASE_FUNCS
#define READ_DATABASE_FUNCS

struct Buffer
{
    char* data;
    int size;
};

int GetSizeOfFile(const char* filename);

enum Status GetDataBaseFromFile(struct Buffer* buffer,
                                const char* input_filename);

struct Node* FillNodeDataFromBuffer(char** cur_pos,
                                    struct Node** node, int* size);

bool IsNil(char* cur_pos);

char* RaedName(char* cur_pos);

#endif
