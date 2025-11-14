#include<TXLib.h>
#include <sys/stat.h>

#include "akinator_funcs.h"
#include "akinator_dump_funcs.h"
#include "akinator_get_set_funcs.h"
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

void ConvertUnicodes(unsigned char* source, unsigned char* dest)
{
    assert(source);
    assert(dest);

    size_t source_index = 0;
    size_t dest_index = 0;

    while(source[source_index] != '\0')
    {
        unsigned char sym = source[source_index];
        unsigned char next_sym = source[source_index + 1];

        if ((sym & 0xE0) == 0xC0 && (next_sym & 0xC0) == 0x80)
        {
            printf("Зашел сюда %d %d\n", sym, next_sym);
            int unicode = ((sym & 0x1F) << 6) | (next_sym & 0x3F);

            if (unicode >= 0x0410 && unicode <= 0x042F)
                dest[dest_index++] = (char)(0xC0 + (unicode - 0x0410));

            source_index += 2;
        }

        else
        {
            printf("БЛЯ Зашел сюда %d %d\n", sym, next_sym);
            dest[dest_index++] = source[source_index++];
        }
    }

    dest[dest_index] = '\0';
}

struct Node* FillNodeDataFromBuffer(char** cur_pos, int* size, struct Node* parent)
{
    assert(cur_pos);

    const int NIL_LEN = 3;
    char* node_ptr = NULL;

    fprintf(log_file, "<h2>=======Новый вызов функции создания узла=======</h2>\n\n");

    fprintf(log_file, "<strong>BUFFER:</strong> \n|%s|\n\n", *cur_pos);
    fprintf(log_file, "<strong>SIZE:</strong> %d \n\n", *size);

    SkipSpaces(cur_pos);

    if (**cur_pos == '(')
    {
        fprintf(log_file, "<strong>Обнаружил скобку \"(\"</strong>\n\n");

        *cur_pos += 1;
        fprintf(log_file, "<strong>Пропустил скобку:</strong> \n|%s|\n\n", *cur_pos);

        SkipSpaces(cur_pos);

        struct Node* node = (Node*)calloc(1, sizeof(Node));
        node->data = RaedName(*cur_pos);
        node->parent = parent;

        node_ptr = GetPrettyPtr(node);

        fprintf(log_file, "<strong>Создал новую вершину</strong>\n"
                          "<strong>NODE_PTR:</strong> %s \n"
                          "<strong>NAME:</strong> |%s| \n"
                          "<strong>NAME_PTR:</strong> %p \n"
                          "<strong>BUFFER_PTR:</strong> %p \n\n",
                          node_ptr, node->data, node->data, *cur_pos);
        free(node_ptr);

        *size += 1;

        fprintf(log_file, "<strong>Увеличил размер SIZE:</strong> %d \n\n", *size);

        fprintf(log_file, "<strong>До пропуска слова:</strong> \n|%s| \n\n", *cur_pos);
        *cur_pos += strlen(node->data) + 2;
        fprintf(log_file, "<strong>Пропустил слово:</strong> \n|%s| \n\n", *cur_pos);

        node->left = FillNodeDataFromBuffer(cur_pos, size, node);

        node_ptr = GetPrettyPtr(node->left);
        fprintf(log_file, "<strong>Завершил левый узел:</strong> %s \n"
                          "<strong>У этого узла:</strong> %p \n"
                          "<strong>Имя главного:</strong> |%s|\n\n",
                                                   node_ptr, node, node->data);
        free(node_ptr);

        node->right = FillNodeDataFromBuffer(cur_pos, size, node);

        node_ptr = GetPrettyPtr(node->right);
        fprintf(log_file, "<strong>Завершил правый узел:</strong> %s "
                          "<strong>У этого узла:</strong> %p "
                          "<strong>Имя главного:</strong> |%s|\n\n",
                                                 node_ptr, node, node->data);
        free(node_ptr);

        SkipSpaces(cur_pos);

        *cur_pos += 1;

        fprintf(log_file, "Пропустил \")\": \n|%s|\n\n", *cur_pos);

        node_ptr = GetPrettyPtr(node);
        fprintf(log_file, "<strong>Сейчас верну этот указатель %s</strong> \n\n",
                                                                       node_ptr);
        free(node_ptr);

        return node;
    }

    else if (IsNil(*cur_pos))
    {
        *cur_pos += NIL_LEN;

        fprintf(log_file, "<strong>Пропустил \"nil\":</strong> \n|%s|\n\n", *cur_pos);

        SkipSpaces(cur_pos);

        return NULL;
    }

    return NULL;
}

char* RaedName(char* cur_pos)
{
    assert(cur_pos);

    int len = 0;

    cur_pos++;
    fprintf(log_file, "<strong>Пропустил \":</strong> \n|%s|\n\n", cur_pos);

    //len = strchr(cur_pos, '"') - cur_pos;

    sscanf(cur_pos, "%*[^\"]%n", &len);
    fprintf(log_file, "<strong>Посчитал длину нового имени LEN:</strong> %d\n\n", len);

    *(cur_pos + len) = '\0';
    fprintf(log_file, "<strong>Тут же получил имя NAME:</strong> |%s|\n\n", cur_pos);

    return cur_pos;
}

bool IsNil(char* cur_pos)
{
    char checker[4] = {};

    sscanf(cur_pos, "%3s", checker);
    fprintf(log_file, "<strong>Прочитал потенциальный NIL получил:</strong> |%s|\n\n",
                                                                             checker);
    if (strcmp(checker, "nil") == 0)
        return true;

    return false;
}

void SkipSpaces(char** cur_pos)
{
    assert(cur_pos);

    fprintf(log_file, "<strong>Пропускаю все пробельные символы</strong>\n\n");
    fprintf(log_file, "<strong>До пропуска BUFFER:</strong> \n|%s|\n\n", *cur_pos);

    while (isspace(**cur_pos))
        *cur_pos += 1;

    fprintf(log_file, "<strong>После пропуска BUFFER:</strong> \n|%s|\n\n", *cur_pos);
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
