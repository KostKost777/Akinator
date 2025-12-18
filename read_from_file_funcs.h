#ifndef READ_FROM_FILE
#define READ_FROM_FILE

struct Buffer
{
    char* data;
    size_t size;
    size_t line;
    size_t column;
};

int GetDataFromFile(Buffer* buffer,
                     const char* input_filename);

void BufferDtor(Buffer* buffer);

void BufferCtor(Buffer* buffer, size_t size);

int GetSizeOfFile(const char* filename);

#endif
