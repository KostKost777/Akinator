#include<TXLib.h>
#include <sys/stat.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"
#include "read_database_from_file.h"

enum Status GetDataBaseFromFile(struct Buffer* buffer,
                                   const char* input_filename)
{
    assert(input_filename != NULL);

    int file_descriptor = open(input_filename, O_RDONLY);

    if (file_descriptor == -1)
    {
        fprintf(log_file, "<h3>Error opening file |%s|</h3>", input_filename);
        close(file_descriptor);

        return error;
    }

    buffer->size = GetSizeOfFile(input_filename);

    if (buffer->size == -1)
        return error;

    buffer->data = (char* )calloc(buffer->size + 1, sizeof(char));
    assert(buffer->data != NULL);

    buffer->size = read(file_descriptor, buffer->data, buffer->size);

    if (buffer->size == -1)
    {
        fprintf(log_file, "<h3>Error reading file |%s|</h3>", input_filename);
        close(file_descriptor);

        return error;
    }

    buffer->data[buffer->size] = '\0';

    close(file_descriptor);

    return success;
}

struct Node* FillNodeDataFromBuffer(char* cur_pos, struct Node* node)
{
    assert(cur_pos);

    const int NILL_LEN = 3;
    char* node_ptr = GetPrettyPtr(node);

    fprintf(log_file, "=========Функция создания узла===========\n\n");

    fprintf(log_file, "BUFFER: %s\n", cur_pos);
    fprintf(log_file, "NODE_PTR: %p\n", node);

    if (*cur_pos == '(')
    {
        fprintf(log_file, "Обнаружил скобку \"(\"\n\n", cur_pos);
        cur_pos++;
        fprintf(log_file, "Пропустил скобку %s\n\n", cur_pos);

        char* name = RaedName(cur_pos);

        fprintf(log_file, "Прочитал имя новой вершины: %s \n\n", name);

        node = NodeCtor(name, node);

        cur_pos += strlen(name) + 2;

        node->left = FillNodeDataFromBuffer(cur_pos, node->left);
        node->right = FillNodeDataFromBuffer(cur_pos, node->right);

        cur_pos++;
    }

    if (IsNil(cur_pos))
    {
        cur_pos += NILL_LEN;

        return NULL;
    }

    return NULL;
}

char* RaedName(char* cur_pos)
{
    assert(cur_pos);

    cur_pos++;

    int len = strchr(cur_pos, '"') - cur_pos;
    printf("LEN: %d\n", len);

    *(cur_pos + len) = '\0';

    return cur_pos;
}

bool IsNil(char* cur_pos)
{
    char checker[4] = {};

    sscanf(cur_pos, "%3s", checker);

    if (!strcmp(checker, "nil"))
        return true;

    return false;
}

int GetSizeOfFile(const char* filename)
{
    assert(filename != NULL);

    struct stat file_info = {};

    if (stat(filename, &file_info) == -1)
    {
        fprintf(log_file, "Не открылся файл |%s|", filename);

        return -1;
    }

    return (int)file_info.st_size;
}
