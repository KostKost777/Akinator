#include<TXLib.h>

#include "akinator_funcs.h"
#include "read_database_from_file.h"
#include "akinator_dump_funcs.h"
#include "akinator_get_set_funcs.h"
#include "STACK\stack_functions.h"
#include "STACK\dump_functions.h"

FILE* log_file = NULL;

 Status TreeCtor(Tree* tree)
{
    assert(tree);

    tree->size = 1;

    return success;
}

Node* NodeCtor(char* value, Node* parent)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    assert(new_node);

    new_node->data = strdup(value);
    new_node->parent = parent;

    return  new_node;
}

void PrintAkinatorOptions(void)
{
    printf("\n\n\n------------Это Акинатор------------\n\n"
           "            Выбери опцию:\n\n"
           "            Угадать   - [у]\n"
           "            Описание  - [о]\n"
           "            Сравнение - [с]\n"
           "            Выход     - [в]\n\n\n");
}

 Status StartAkinator(Tree* tree)
{
    assert(tree);

    printf( "Привет это Акинатор"
            " Я умею угадывать, сравнивать"
            " и описывать предметы из моей базыданных");

    while (true)
    {
        PrintAkinatorOptions();

        printf("Введите опцию: ");

         Mode mode = GetMode();

        switch(mode)
        {
            case EXIT: return success;

            case GUESS: Guess(tree);
                       break;

            case DESCR: FindDescription(tree);
                        break;

            case COMP: Comparation(tree);
                       break;

            default: return error;
        }
    }

    return success;
}

 Status FindDescription(Tree* tree)
{
    assert(tree);

    size_t len = MAX_LEN;
    char* name = NULL;

    printf("Введите имя объекта: ");
    my_getline(&name, &len, stdin);

    INIT_STACK(stk_path);
    StackCtor(&stk_path, 2);

    if (GetDescription(GetRoot(tree), name, &stk_path) == error)
    {
        printf("Я не знаю кто такой |%s|\n\n", name);

        StackDtor(&stk_path);

        return error;
    }

    PrintDescription(GetRoot(tree), name, &stk_path);

    StackDtor(&stk_path);

    return success;
}

 Status Guess(Tree* tree)
{
    assert(tree);

    Node* current = GetRoot(tree);

     Ans ans = EMPTY;

    while(true)
    {
        printf("Оно %s? (д/н): ", current->data);

        ans = GetAnswer();

        if (current->left == NULL && current->right == NULL)
            break;

        switch(ans)
        {
            case YES: current = GetLeft(current);
                      break;

            case NO: current = GetRight(current);
                     break;

            case EMPTY:
            default: return error;

        }
    }

    switch(ans)
        {
            case YES: printf("Я знаю, ты загадал это: %s\n\n", current->data);
                      return success;

            case NO: CreateNewNode(tree, current);
                     return success;

            case EMPTY:
            default: return error;
        }

    return error;
}

 Status Comparation(Tree* tree)
{
    assert(tree);

    size_t len = MAX_LEN;
    char* obj_1 = NULL;
    char* obj_2 = NULL;

    printf("\nВведите два объекта, которые вы хотите сравнить\n");

    printf("\nВведите имя первого объекта: ");
    my_getline(&obj_1, &len, stdin);

    printf("\nВведите имя второго объекта: ");
    my_getline(&obj_2, &len, stdin);

    INIT_STACK(stk_1_path);
    StackCtor(&stk_1_path, 2);

    INIT_STACK(stk_2_path);
    StackCtor(&stk_2_path, 2);

    if(GetDescription(tree->root, obj_1, &stk_1_path))
    {
        printf("Я не знаю кто такой |%s|\n", obj_1);
        StackDtor(&stk_1_path);
        return error;
    }

    if(GetDescription(tree->root, obj_2, &stk_2_path))
    {
        printf("Я не знаю кто такой |%s|\n", obj_2);
        StackDtor(&stk_2_path);
        return error;
    }

    PrintComparison(tree->root, tree->root,
                    obj_1, obj_2,
                    &stk_1_path, &stk_2_path);

    StackDtor(&stk_1_path);
    StackDtor(&stk_2_path);

    return success;
}

 Status PrintComparison (Node* node_1, Node* node_2,
                             char* obj_1, char* obj_2,
                             Stack* stk_1_path, Stack* stk_2_path)
{
    assert(node_1);
    assert(node_2);
    assert(obj_1);
    assert(obj_2);

    StackValueType last_el_1 = EMPTY;
    StackValueType last_el_2 = EMPTY;

    while(stk_1_path->size != 0 || stk_2_path->size != 0)
    {
        if (stk_1_path->size != 0)
            StackPop(stk_1_path, &last_el_1);

        if (stk_2_path->size != 0)
            StackPop(stk_2_path, &last_el_2);

        if (last_el_1 == last_el_2 && node_1 == node_2)
        {
            PrintEqualDescription(&node_1, &node_2,
                                  obj_1, obj_2, last_el_1);
            continue;
        }

        if (stk_1_path->size != 0)
            PrintDiffDescription(&node_1,
                                 obj_1, obj_2, last_el_1);

        if (stk_2_path->size != 0)
            PrintDiffDescription(&node_2,
                                 obj_2, obj_1, last_el_2);
    }

    return success;
}

 Status PrintEqualDescription(Node** node_1, Node** node_2,
                                  char* obj_1, char* obj_2, StackValueType last_el)
{
    assert(node_1);
    assert(node_2);
    assert(obj_1);
    assert(obj_2);

    if (last_el == YES)
    {
        printf("%s и %s оба %s\n", obj_1, obj_2, (*node_1)->data);
        *node_1 = GetLeft(*node_1);
        *node_2 = GetLeft(*node_2);
    }

    if (last_el == NO)
    {
        printf("%s и %s оба не %s\n", obj_1, obj_2, (*node_1)->data);
        *node_1 = GetRight(*node_1);
        *node_2 = GetRight(*node_2);
    }

    return success;
}

 Status PrintDiffDescription(Node** node,
                                 char* obj_1, char* obj_2, StackValueType last_el)
{
    assert(node);
    assert(obj_1);
    assert(obj_2);

    if (last_el == YES)
    {
        printf("%s %s, но %s нет", obj_1, (*node)->data, obj_2);
        *node = GetLeft(*node);
    }

    if (last_el == NO)
    {
        *node = GetRight(*node);
    }

    return success;
}

 Mode GetMode(void)
{
    char* ans = NULL;
    size_t len = MAX_LEN;

    my_getline(&ans, &len, stdin);

    while (true)
    {
        if (strlen(ans) == 1 && (ans[0] == 'в' || ans[0] == 'о'
                              || ans[0] == 'у' || ans[0] == 'с'))
            break;

        else
        {
            printf("Неверный ввод, попробуйте еще раз: ");
            my_getline(&ans, &len, stdin);
        }
    }

    char mode = ans[0];

    free(ans);
    ans = NULL;

    switch(mode)
    {
        case 'в': return EXIT;
        case 'о': return DESCR;
        case 'у': return GUESS;
        case 'с': return COMP;
        default:  return EXIT;
    }
}

 Ans GetAnswer(void)
{
    char* ans = NULL;
    size_t len = MAX_LEN;

    my_getline(&ans, &len, stdin);

    while (true)
    {
        if (strlen(ans) == 1 && (ans[0] == 'д' || ans[0] == 'н'))
            break;

        else
        {
            printf("Неверный ввод, попробйте еще раз: ");
            my_getline(&ans, &len, stdin);
        }
    }

    char answer = ans[0];

    free(ans);
    ans = NULL;

    switch(answer)
    {
        case 'д': return YES;
        case 'н': return NO;
        default:  return EMPTY;
    }
}

 Status GetDescription(Node* node, char* name, Stack* path)
{
    assert(node);
    assert(name);
    assert(path);


    if (strcmp(node->data, name) == 0)
        return success;

    if (    GetLeft(node) != NULL
        && (GetDescription(GetLeft(node), name, path) == success))
    {
        StackPush(path, YES);
        StackDump(path);
        return success;
    }

    if (    GetRight(node) != NULL
        && (GetDescription(GetRight(node), name, path) == success))
    {
        StackPush(path, NO);
        StackDump(path);
        return success;
    }

    return error;
}

 Status PrintDescription(Node* node, char* name, Stack* path)
{
    assert(node);
    assert(name);
    assert(path);

    printf("Описание %s это: ", name);

    StackValueType last_el = EMPTY;

    while (path->size > 1)
    {
        StackDump(path);
        StackPop(path, &last_el);

        if (last_el == YES)
        {
            printf("%s и ", node->data);
            node = GetLeft(node);
        }

        if (last_el == NO)
        {
            printf("не %s и ", node->data);
            node = GetRight(node);
        }
    }

    printf("это все что касается %s\n", name);

    return success;
}

 Status CreateNewNode(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    size_t len = MAX_LEN;
    char* new_object = NULL;

    printf("Я не знаю такого объекта\n");
    printf("Введите его имя: ");

    my_getline(&new_object, &len, stdin);

    printf("Чем %s отличается от %s?: ", new_object,
                                         node->data);

    node->right = NodeCtor(node->data, node);
    node->left = NodeCtor(new_object, node);

    my_getline(&node->data, &len, stdin);

    free(new_object);
    new_object = NULL;

    tree->size += 2;

    TreeDump(tree);

    return success;
}

void UpdateDataBase(Node* node, FILE* database_file)
{
    assert(node);
    assert(database_file);

    fprintf(database_file,"( ");
    fprintf(database_file, "\"%s\" ", node->data);

    if (GetLeft(node) != NULL)
        UpdateDataBase(GetLeft(node), database_file);
    else
        fprintf(database_file, "nil ");

    if (GetRight(node) != NULL)
        UpdateDataBase(GetRight(node), database_file);
    else
        fprintf(database_file, "nil ");

    fprintf(database_file, ") ");
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

void DeleteNode(Tree* tree, Node* node, Buffer* buffer)
{
    assert(node);
    assert(tree);

    if (node->left != NULL)
        DeleteNode(tree, node->left, buffer);

    if (node->left != NULL)
        tree->size--;

    if (   node->left != NULL
        && IsDinamicMemory(node->left->data, buffer->data, buffer->size))
    {

        fprintf(log_file, "<strong>Этот объект удалится как динамический </strong>|%s|\n",
                                                    node->left->data);

        free(node->left->data);
        node->left->data = NULL;
    }

    free(node->left);
    node->left = NULL;

    if (node->right != NULL)
        DeleteNode(tree, node->right, buffer);

    if (node->right != NULL)
        tree->size--;

    if (   node->right != NULL
        && IsDinamicMemory(node->right->data, buffer->data, buffer->size))
    {
        fprintf(log_file, "<strong>Этот объект удалится как динамический </strong>|%s|\n",
                                                    node->right->data);
        free(node->right->data);
        node->right->data = NULL;
    }

    free(node->right);
    node->right = NULL;
}

 Status WriteDataBaseInFile(Tree* tree, const char* database_file_name)
{
    assert(database_file_name);

    FILE* database_file = fopen("database.txt", "w");

    if (database_file == NULL)
    {
        fprintf(log_file, "<h2>Error open |%s| file</h2>\n", database_file_name);
        return error;
    }

    UpdateDataBase(GetRoot(tree), database_file);
    fclose(database_file);

    return success;
}

void TreeDtor(Tree* tree, Buffer* buffer)
{
    DeleteNode(tree, tree->root, buffer);

    free(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

ssize_t my_getline(char** dest, size_t* n, FILE* file)
{
    assert(dest != NULL);
    assert(file != NULL);

    const int INCREASE_BUFFER = 2;
    const int DEFAULT_BUFFER = 64;

    if (*dest == NULL)
    {
        *n = DEFAULT_BUFFER;
        *dest = (char* )calloc(*n, sizeof(char));
    }

    char char_from_file = '\0';
    size_t counter = 0;

    while(true)
    {
        int inp_c = fgetc(file);

        if (inp_c == EOF)
            return -1;

        char_from_file = (char)inp_c;

        if (char_from_file == '\n')
            break;

        else if (counter > *n - 1)
        {
           (*n) *= INCREASE_BUFFER;

           char* check = (char* )realloc(*dest, *n);

           if (check == NULL)
           {
                free(*dest);
                *dest = NULL;

                return -1;
           }

            *dest = check;
           (*dest)[counter] = char_from_file;

        }

        else
            (*dest)[counter] = char_from_file;

        counter++;
    }

    (*dest)[counter] = '\0';

    return counter;
}

void BufferDtor(Buffer* buffer)
{
    assert(buffer);

    free(buffer->data);
    buffer->data = NULL;

    buffer->size = 0;
}

bool IsDinamicMemory(void* ptr, void* buffer_ptr, int size)
{
    assert(ptr);
    assert(buffer_ptr);

    if (   (uintptr_t)ptr >= (uintptr_t)buffer_ptr
        && (uintptr_t)ptr <= ((uintptr_t)buffer_ptr + size))
        return false;

    return true;
}
