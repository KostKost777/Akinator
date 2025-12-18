#include<TXLib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "read_from_file_funcs.h"
#include "dump_funcs.h"

int GetDataFromFile(Buffer* buffer,
                       const char* input_filename)
{
    assert(input_filename != NULL);

    int file_descriptor = open(input_filename, O_RDONLY);

    if (file_descriptor == -1)
    {
        fprintf(stdout, "<h3>Error opening file |%s|</h3>", input_filename);
        close(file_descriptor);

        return 1;
    }

    int size = GetSizeOfFile(input_filename);

    assert(size != -1);

    BufferCtor(buffer, (size_t)size);

    size = read(file_descriptor, buffer->data, (unsigned int)buffer->size);

    if (size == -1)
    {
        fprintf(log_file, "<h3>Error reading file |%s|</h3>", input_filename);
        close(file_descriptor);

        return 1;
    }

    buffer->size = size_t(size);

    buffer->data[buffer->size] = '\0';

    close(file_descriptor);

    return 0;
}

int GetSizeOfFile(const char* filename)
{
    assert(filename != NULL);

    struct stat file_info = {};

    if (stat(filename, &file_info) == -1)
    {
        fprintf(stdout, "Не смог открыть файл |%s|", filename);

        return -1;
    }

    return (int)file_info.st_size;
}

void BufferDtor(Buffer* buffer)
{
    assert(buffer);

    free(buffer->data);

    buffer->line = 0;
    buffer->column = 0;
    buffer->size = 0;
}

void BufferCtor(Buffer* buffer, size_t size)
{
    assert(buffer);

    buffer->size = size;

    buffer->data = (char* )calloc(buffer->size + 1, sizeof(char));
    assert(buffer->data != NULL);

    buffer->column = 1;
    buffer->line = 1;
}
