#ifndef READ_DATABASE_FUNCS
#define READ_DATABASE_FUNCS

struct Buffer
{
    char* data;
    int size;
};

void SkipSpaces(char** cur_pos);

void ConvertUnicodes(unsigned char* source, unsigned char* dest);

int GetSizeOfFile(const char* filename);

enum Status GetDataBaseFromFile(struct Buffer* buffer,
                                const char* input_filename);

struct Node* FillNodeDataFromBuffer(char** cur_pos, int* size, struct Node* parent);

bool IsNil(char* cur_pos);

char* RaedName(char* cur_pos);

#endif
