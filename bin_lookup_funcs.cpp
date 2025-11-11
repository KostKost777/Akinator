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

    tree->size = 1;

    tree->root->data = strdup("Nuchto");
    tree->root->parent = tree->root;

    return success;
}

struct Node* NodeCtor(char* value, struct Node* parent)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    assert(new_node);

    new_node->data = strdup(value);
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

    if (tree->size == 1 && ans == NO)
        CreateFirstObject(tree);

    else if (ans == NO)
        CreateNewNode(tree, current->parent, prev_ans);

    else
        printf("Вы загадали %s\n", current->data);

}

enum Ans GetAnswer(void)
{
    char* ans = NULL;
    size_t len = MAX_LEN;

    getline(&ans, &len, stdin);

    while (strcmp("y", ans) != 0 && strcmp("n", ans) != 0)
    {
        printf("Введите y или n: ");
        getline(&ans, &len, stdin);
    }

    if (strcmp("y", ans) == 0) {
        free(ans);
        return YES;
    }

    free(ans);
    return NO;
}

enum Status CreateFirstObject(struct Tree* tree)
{
    assert(tree);

    size_t len = 50;
    char* new_object = NULL;
    char* difference = NULL;
    char* old_object = NULL;

    printf("Добавьте первый объект\n");
    printf("Введите его: ");

    getline(&new_object, &len, stdin);

    old_object = strdup(tree->root->data);

    printf("\nЧем %s отличается от %s, он ... отличается: ", new_object,
                                                             old_object);
    getline(&difference, &len, stdin);

    free(tree->root);

    tree->root = NodeCtor(difference, tree->root);
    tree->root->left = NodeCtor(new_object, tree->root);
    tree->root->right = NodeCtor(old_object, tree->root);

    tree->size+=2;

    FILE* database_file = fopen("database.txt", "w");
    UpdateDataBase(GetRoot(tree), database_file);
    fclose(database_file);

    return success;
}

enum Status CreateNewNode(struct Tree* tree, struct Node* node, enum Ans ans)
{
    assert(node);

    size_t len = 50;
    char* new_object = NULL;
    char* difference = NULL;
    char* old_object = NULL;

    printf("В нашем справочнике нет такого объекта\n");
    printf("Введите его: ");

    getline(&new_object, &len, stdin);

    if (ans == NO) {
        old_object = strdup(node->right->data);

        printf("\nЧем %s отличается от %s, он ... отличается: ", new_object,
                                                                 old_object);
        getline(&difference, &len, stdin);

        free(node->right);

        node->right = NodeCtor(difference, node);
        node->right->left = NodeCtor(new_object, node->right);
        node->right->right = NodeCtor(old_object, node->right);
    }

    else {
        old_object = strdup(node->left->data);

        printf("\nЧем %s отличается от %s, он ... отличается: ", new_object,
                                                                 old_object);
        getline(&difference, &len, stdin);

        free(node->left);

        node->left = NodeCtor(difference, node);
        node->left->left = NodeCtor(new_object, node->left);
        node->left->right = NodeCtor(old_object, node->left);

    }

    free(difference);
    free(new_object);
    free(old_object);

    tree->size+=2;

    FILE* database_file = fopen("database.txt", "w");
    UpdateDataBase(GetRoot(tree), database_file);
    fclose(database_file);

    return success;
}

void UpdateDataBase(struct Node* node, FILE* database_file)
{

    assert(node);
    assert(database_file);

    fprintf(database_file,"(");
    fprintf(database_file, "\"%s\"", node->data);

    if (GetLeft(node))
        UpdateDataBase(GetLeft(node), database_file);
    else
        fprintf(database_file, "$");

    if (GetRight(node))
        UpdateDataBase(GetRight(node), database_file);
    else
        fprintf(database_file, "$");

    fprintf(database_file, ")");

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

void NodeDtor(struct Node* node)
{
    assert(node != NULL);

    free(node->left);
    free(node->right);
    free(node->data);
    free(node);
}

ssize_t getline(char** dest, size_t* n, FILE* file){
    assert(dest != NULL);
    assert(file != NULL);

    const int INCREASE_BUFFER = 2;
    const int DEFAULT_BUFFER = 64;

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

