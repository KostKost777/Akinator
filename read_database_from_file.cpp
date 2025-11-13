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

struct Node* FillNodeDataFromBuffer(char** cur_pos,
                                    struct Node** node, int* size)
{
    assert(cur_pos);

    const int NIL_LEN = 3;
    char* node_ptr = NULL;

    fprintf(log_file, "=========Новый вызов функции создания узла===========\n\n");

    fprintf(log_file, "BUFFER: %s\n\n", *cur_pos);
    fprintf(log_file, "SIZE: %d \n\n", *size);
    fprintf(log_file, "NODE_PTR: %p\n\n", *node);

    if (**cur_pos == '(')
    {
        fprintf(log_file, "Обнаружил скобку \"(\"\n\n");
        (*cur_pos)++;
        fprintf(log_file, "Пропустил скобку %s\n\n", *cur_pos);

        //fprintf(log_file, "Прочитал имя новой вершины: %s \n\n", name);

        *node = NodeCtor(RaedName(*cur_pos), *node);

        node_ptr = GetPrettyPtr(*node);

        fprintf(log_file, "Создал новую вершину PTR: %s NAME: %s \n\n",
                                                        node_ptr, (*node)->data);
        free(node_ptr);

        *size += 1;

        fprintf(log_file, "Уеличил размер SIZE: %d \n\n", *size);

        *cur_pos += strlen((*node)->data) + 2;

        fprintf(log_file, "Пропустил слово: %s \n\n", *cur_pos);

        (*node)->left = FillNodeDataFromBuffer(cur_pos, &((*node)->left), size);

        node_ptr = GetPrettyPtr((*node)->left);
        fprintf(log_file, "Завершил левый узел: %s У этого узла: %p имя главного: %s\n\n",
                                                                node_ptr, (*node), (*node)->data);
        free(node_ptr);

        (*node)->right = FillNodeDataFromBuffer(cur_pos, &((*node)->right), size);

        node_ptr = GetPrettyPtr((*node)->right);
        fprintf(log_file, "Завершил правый узел: %s У этого узла: %p имя главного: %s\n\n",
                                                                node_ptr, (*node), (*node)->data);
        free(node_ptr);

        (*cur_pos)++;

        fprintf(log_file, "Пропустил \")\": %s\n\n", *cur_pos);

        node_ptr = GetPrettyPtr(*node);
        fprintf(log_file, "Сейчас верну этот указатель %s: \n\n", node_ptr);
        free(node_ptr);

        return (*node);
    }

    else if (IsNil(*cur_pos))
    {
        *cur_pos += NIL_LEN;

        fprintf(log_file, "Пропустил \"nil\": %s\n\n", *cur_pos);

        return NULL;
    }

    return NULL;
}

char* RaedName(char* cur_pos)
{
    assert(cur_pos);

    cur_pos++;

    fprintf(log_file, "Пропустил \" : %s\n\n", cur_pos);

    int len = strchr(cur_pos, '"') - cur_pos;

    fprintf(log_file, "Посчитал длину нового имени LEN: %d\n\n", len);

    *(cur_pos + len) = '\0';

    fprintf(log_file, "Тут же получил имя NAME: %s\n\n", cur_pos);

    return cur_pos;
}

bool IsNil(char* cur_pos)
{
    char checker[4] = {};

    sscanf(cur_pos, "%3s", checker);

    fprintf(log_file, "Прочитал потенциальный NIL получил: %s\n\n", checker);

    if (strcmp(checker, "nil") == 0)
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
