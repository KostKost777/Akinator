#include<TXLib.h>

#include "bin_lookup_funcs.h"
#include "bin_lookup_dump_funcs.h"
#include "bin_lookup_get_set_funcs.h"

FILE* log_file = NULL;

enum Status TreeCtor(struct Tree* tree)
{
    assert(tree);

    tree->root = (Node*)calloc(1, sizeof(Node));

    if (tree->root == NULL)
        return error;

    return success;
}

struct Node* NodeCtor(char value[MAX_LEN], struct Node* parent)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    assert(new_node);

    strncpy(new_node->data, value, MAX_LEN);
    new_node->parent = parent;

    return  new_node;
}

void StartGuessing(struct Tree* tree)
{
    assert(tree);

    Node* current = GetRoot(tree);

    enum Ans ans = NO;
    enum Ans prev_ans = NO;

    while(true)
    {
        printf("%s? (y/n): ", current->data);

        prev_ans = ans;
        ans = GetAnswer();

        if (current->left == NULL && current->right == NULL)
            break;

        if (ans == YES)
            current = current->left;

        else
           current = current->right;
    }

    if (ans == NO)
        CreateNewNode(current->parent, prev_ans);

    else
        printf("Вы загадали %s\n", current->data);

}

enum Ans GetAnswer(void)
{
    char ans[MAX_LEN] = {};
    scanf("%s", ans);

    while (strcmp("y", ans) != 0 && strcmp("n", ans) != 0)
    {
        printf("Введите y или n: ");
        scanf("%s", ans);
    }

    if (strcmp("y", ans) == 0)
        return YES;

    return NO;
}

enum Status CreateNewNode(struct Node* node, enum Ans ans)
{
    assert(node);

    char old_object[MAX_LEN] = {};
    char new_object[MAX_LEN] = {};
    char difference[MAX_LEN] = {};
    struct Node* now_node = NULL;

    if (ans == NO)
        now_node = node->right;

    else
        now_node = node->left;

    strncpy(old_object, now_node->data, MAX_LEN);

    printf("В нашем справочнике нет такого объекта\n");
    printf("Введите его: ");

    scanf("%49s", new_object);

    printf("\nЧем %s отличается от %s, он ... отличается: ", new_object,
                                                             now_node->data);
    scanf("%49s", difference);

    free(now_node);

    now_node = NodeCtor(difference, node);
    now_node->left = NodeCtor(new_object, now_node);
    now_node->right = NodeCtor(old_object, now_node);

    printf("\n");

    return success;
}

void CloseLogFile()
{
    fclose(log_file);
    printf("Logfile close");
}

void OpenLogFile(const char* log_file_name)
{
    log_file = fopen(log_file_name, "w");

    if (log_file != NULL)
        printf("Logfile open\n");

    else
        printf("Logfile open ERROR\n");

    fprintf(log_file, "<pre>\n");
}

ssize_t getline(char** dest, size_t* n, FILE* file){
    assert(dest != NULL);
    assert(file != NULL);

    const int INCREASE_BUFFER = 2;
    const int DEFAULT_BUFFER = 1024;

    if (*dest == NULL){
        *n = DEFAULT_BUFFER;
        *dest = (char* )calloc(*n, sizeof(char));
    }

    char char_from_file = '\0';
    size_t counter = 0;
    while(true){
        char_from_file = (char)fgetc(file);

        if (char_from_file == EOF)
            return -1;

        else if (char_from_file == '\n')
            break;

        else if (counter > *n - 1){
           (*n) *= INCREASE_BUFFER;
           char* check = (char* )realloc(*dest, *n);
           if (check == NULL){
                free(*dest);
                *dest = NULL;
                return -1;
           }
            *dest = check;
           (*dest)[counter] = char_from_file;
        }else

            (*dest)[counter] = char_from_file;

        counter++;

    }

    (*dest)[counter] = '\0';

    return counter;
}

