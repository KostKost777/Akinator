#define TX_USE_SPEAK
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

int  PrintAkinatorOptions(void)
{
    const int LINES = 5;

    return LINES * PrintPhrase(X_CENTER, Y_CENTER,
                        "Опции:\n"
                        "Угадать   - [у]\n"
                        "Описание  - [о]\n"
                        "Сравнение - [с]\n"
                        "Выход     - [в]\n");
}

Status StartAkinator(Tree* tree, HDC* video)
{
    assert(tree);

    PrintAndSayPhrase(video, X_CENTER, Y_CENTER,
                            "\aПривет это Акинатор\n"
                            " я умею угадывать, сравнивать\n"
                            " и описывать предметы из моей базыданных\n");

    while (true)
    {
        int y_now = Y_CENTER;

        CleanWindow(video);
        y_now += PrintAkinatorOptions();

        PrintAndSayPhrase(video, X_CENTER, y_now, "\aВведите опцию\n");

        Mode mode = GetMode(video);

        switch(mode)
        {
            case EXIT: return success;

            case GUESS: Guess(tree, video);
                       break;

            case DESCR: FindDescription(tree, video);
                        break;

            case COMP: Comparation(tree, video);
                       break;

            default: return error;
        }
    }

    return success;
}

 Status FindDescription(Tree* tree, HDC* video)
{
    assert(tree);

    size_t len = MAX_LEN;
    char* name = NULL;

    CleanWindow(video);
    PrintAndSayPhrase(video, X_CENTER, Y_CENTER, "\aВведите имя объекта");
    my_getline(&name, &len, stdin);

    INIT_STACK(stk_path);
    StackCtor(&stk_path, 2);

    if (GetDescription(GetRoot(tree), name, &stk_path) == error)
    {
        CleanWindow(video);
        PrintAndSayPhrase(video, X_CENTER, Y_CENTER, "\aЯ не знаю кто такой |%s|\n\n", name);

        StackDtor(&stk_path);

        return error;
    }

    PrintDescription(video, GetRoot(tree), name, &stk_path);

    StackDtor(&stk_path);

    return success;
}

 Status Guess(Tree* tree, HDC* video)
{
    assert(tree);

    Node* current = GetRoot(tree);

     Ans ans = EMPTY;

    while(true)
    {
        CleanWindow(video);

        PrintAndSayPhrase(video, X_CENTER, Y_CENTER,
                               "\aОно %s?\n", current->data);

        ans = GetAnswer(video);

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
            case YES: CleanWindow(video);

                      PrintAndSayPhrase(video, X_CENTER, Y_CENTER,
                               "\aЯ знаю, ты загадал: %s\n", current->data);

                      CleanWindow(video);

                      return success;

            case NO: CreateNewNode(tree, current, video);
                     return success;

            case EMPTY:
            default: return error;
        }

    return error;
}

 Status Comparation(Tree* tree, HDC* video)
{
    assert(tree);

    size_t len = MAX_LEN;
    char* obj_1 = NULL;
    char* obj_2 = NULL;

    CleanWindow(video);

    PrintAndSayPhrase(video, X_CENTER, Y_CENTER,
                               "\aВведите два объекта, которые вы хотите сравнить\n");
    CleanWindow(video);

    PrintAndSayPhrase(video, X_CENTER, Y_CENTER,
                               "\aВведите имя первого объекта\n");

    CleanWindow(video);

    my_getline(&obj_1, &len, stdin);

    PrintAndSayPhrase(video, X_CENTER,Y_CENTER,
                               "\aВведите имя второго объекта\n");

    CleanWindow(video);

    my_getline(&obj_2, &len, stdin);

    INIT_STACK(stk_1_path);
    StackCtor(&stk_1_path, 2);

    INIT_STACK(stk_2_path);
    StackCtor(&stk_2_path, 2);

    if(GetDescription(tree->root, obj_1, &stk_1_path))
    {
        CleanWindow(video);
        PrintAndSayPhrase(video, X_CENTER,Y_CENTER,
                               "\aЯ не знаю кто такой |%s|\n", obj_1);
        StackDtor(&stk_1_path);
        return error;
    }

    if(GetDescription(tree->root, obj_2, &stk_2_path))
    {
        CleanWindow(video);
        PrintAndSayPhrase(video, X_CENTER,Y_CENTER,
                               "\aЯ не знаю кто такой |%s|\n", obj_2);
        StackDtor(&stk_2_path);
        return error;
    }


    PrintComparison(tree->root, tree->root, video,
                    obj_1, obj_2,
                    &stk_1_path, &stk_2_path);

    StackDtor(&stk_1_path);
    StackDtor(&stk_2_path);

    return success;
}

 Status PrintComparison (Node* node_1, Node* node_2, HDC* video,
                         char* obj_1, char* obj_2,
                         Stack* stk_1_path, Stack* stk_2_path)
{
    assert(node_1);
    assert(node_2);
    assert(obj_1);
    assert(obj_2);

    int y_now = Y_CENTER;
    CleanWindow(video);

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
                                  &y_now, video,
                                  obj_1, obj_2, last_el_1);
            continue;
        }

        if (stk_1_path->size != 0)
            PrintDiffDescription(&node_1, &y_now, video,
                                 obj_1, obj_2, last_el_1);

        if (stk_2_path->size != 0)
            PrintDiffDescription(&node_2, &y_now, video,
                                 obj_2, obj_1, last_el_2);
    }

    return success;
}

 Status PrintEqualDescription(Node** node_1, Node** node_2,
                              int* y_now, HDC* video,
                              char* obj_1, char* obj_2, StackValueType last_el)
{
    assert(node_1);
    assert(node_2);
    assert(obj_1);
    assert(obj_2);

    if (last_el == YES)
    {
        *y_now += PrintAndSayPhrase(video, X_CENTER, *y_now,
                                    "\a%s и %s оба %s\n",
                                     obj_1, obj_2, (*node_1)->data);

        *node_1 = GetLeft(*node_1);
        *node_2 = GetLeft(*node_2);
    }

    if (last_el == NO)
    {
        *y_now += PrintAndSayPhrase(video, X_CENTER, *y_now,
                                    "\a%s и %s оба не %s\n",
                                     obj_1, obj_2, (*node_1)->data);

        *node_1 = GetRight(*node_1);
        *node_2 = GetRight(*node_2);
    }

    return success;
}

 Status PrintDiffDescription(Node** node, int* y_now, HDC* video,
                             char* obj_1, char* obj_2, StackValueType last_el)
{
    assert(node);
    assert(obj_1);
    assert(obj_2);

    if (last_el == YES)
    {
        *y_now += PrintAndSayPhrase(video, X_CENTER, *y_now,
                                    "\a%s %s, но %s нет\n",
                                    obj_1, (*node)->data, obj_2);
        *node = GetLeft(*node);
    }

    if (last_el == NO)
    {
        *node = GetRight(*node);
    }

    return success;
}

 Mode GetMode(HDC* video)
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
            CleanWindow(video);
            PrintAndSayPhrase(video, X_CENTER, Y_CENTER,
                              "\aНеверный ввод, введите корректную опцию\n");

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

 Ans GetAnswer(HDC* video)
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
            CleanWindow(video);
            PrintAndSayPhrase(video, X_CENTER, Y_CENTER,
                                     "\aНеверный ввод\n");

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

 Status PrintDescription(HDC* video, Node* node, char* name, Stack* path)
{
    assert(node);
    assert(name);
    assert(path);

    CleanWindow(video);
    int y_now = Y_CENTER;

    y_now += PrintAndSayPhrase(video, X_CENTER, y_now,
                               "\aОписание объекта %s это\n", name);

    StackValueType last_el = EMPTY;

    while (path->size > 1)
    {
        StackDump(path);
        StackPop(path, &last_el);

        if (last_el == YES)
        {
            y_now += PrintAndSayPhrase(video, X_CENTER, y_now,
                                       "\a%s\n", node->data);
            node = GetLeft(node);
        }

        if (last_el == NO)
        {
            y_now += PrintAndSayPhrase(video, X_CENTER, y_now,
                                       "\aне %s\n", node->data);
            node = GetRight(node);
        }
    }

    PrintAndSayPhrase(video, X_CENTER, y_now,
                      "\aЭто все что касается объекта %s\n", name);

    CleanWindow(video);
    return success;
}

 Status CreateNewNode(Tree* tree, Node* node, HDC* video)
{
    assert(node);
    assert(tree);

    size_t len = MAX_LEN;
    char* new_object = NULL;
    int y_now = Y_CENTER;

    CleanWindow(video);

    y_now += PrintAndSayPhrase(video, X_CENTER, y_now,
                       "\aЯ не знаю такого объекта\n");

    y_now += PrintAndSayPhrase(video, X_CENTER, y_now,
                       "\aВведите его имя\n");

    my_getline(&new_object, &len, stdin);

    CleanWindow(video);

    y_now += PrintAndSayPhrase(video, X_CENTER, y_now,
                               "\aЧем %s отличается от %s?: ", new_object,
                                                               node->data);

     CleanWindow(video);

    node->right = NodeCtor(node->data, node);
    node->left = NodeCtor(new_object, node);

    my_getline(&node->data, &len, stdin);

    y_now += PrintAndSayPhrase(video, X_CENTER, y_now,
                               "\aЯ добавил этот объект в базу данных");

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
    //printf("Logfile close");
}

void OpenLogFile(const char* log_file_name)
{
    log_file = fopen(log_file_name, "w");

//     if (log_file != NULL)
//         //printf("Logfile open\n");
//
//     else
//         printf("Logfile open ERROR\n");

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
    system("cls");

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

void ProcessingPudgePhotos(HDC* pudge_photo)
{
    assert(pudge_photo);

    txCreateWindow(X_WINDOWS_SIZE, Y_WINDOWS_SIZE);
    txSetFillColor(RGB (77, 58, 52));
    txSelectFont("Times New Roman", 22);
    txClear();

    const int NAME_LEN = 100;

    char pudge_photo_name[NAME_LEN] = {};

    for (int i = 0; i <= PUDGE_PHOTOS_NUM; ++i)
    {
        snprintf(pudge_photo_name, NAME_LEN, "PUDGE_PHOTO\\PUDGE_VIDEO_%d.bmp", i);

        pudge_photo[i] = txLoadImage(pudge_photo_name);
    }

}

void PudgePhotosDtor(HDC* pudge_photo)
{
    assert(pudge_photo);

    for (int i = 0; i <= PUDGE_PHOTOS_NUM; ++i)
    {
        txDeleteDC (pudge_photo[i]);
    }
}

int PrintAndSayPhrase(HDC* pudge_photo, int x, int y, const char* message, ...)
{
    assert(message);

    const int WORD_PER_MILISEC = 100;
    const int PHRASE_MAX_LEN = 100;
    char phrase[PHRASE_MAX_LEN] = {};

    va_list args = {};
    va_start(args, message);

    vsnprintf(phrase, PHRASE_MAX_LEN, message, args);

    txDrawText(x, y, X_WINDOWS_SIZE, Y_WINDOWS_SIZE, phrase + 1);

    int phrase_len = (int)strlen(phrase);

    phrase_len *= WORD_PER_MILISEC;

    txSpeak(phrase);

    int i = 0;

    while (phrase_len > 0)
    {
        txBitBlt (txDC(), 0, 0, X_WINDOWS_SIZE, Y_WINDOWS_SIZE, pudge_photo[i]);

        txSleep(50);
        phrase_len -= 60;

        i++;
        i %= (PUDGE_PHOTOS_NUM - 1);
    }

    txBitBlt (txDC(), 0, 0, X_WINDOWS_SIZE, Y_WINDOWS_SIZE, pudge_photo[PUDGE_PHOTOS_NUM]);

    return  txGetTextExtentY(phrase);
}

int PrintPhrase(int x, int y, const char* message, ...)
{
    assert(message);

    const int PHRASE_MAX_LEN = 100;
    char phrase[PHRASE_MAX_LEN] = {};

    va_list args = {};
    va_start(args, message);

    vsnprintf(phrase, PHRASE_MAX_LEN, message, args);

    txDrawText(x, y, X_WINDOWS_SIZE, Y_WINDOWS_SIZE, phrase);

    return txGetTextExtentY(phrase);
}

void CleanWindow(HDC* pudge_photo)
{
    assert(pudge_photo);
    txBitBlt (txDC(), 0, 0, X_WINDOWS_SIZE, Y_WINDOWS_SIZE,
              pudge_photo[PUDGE_PHOTOS_NUM - 1]);
}
