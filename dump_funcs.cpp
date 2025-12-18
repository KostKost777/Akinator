#include <TXLib.h>

#include "dump_funcs.h"
#include "lexical_analysis.h"
#include "tree_funcs.h"

FILE* log_file = NULL;
const char* log_file_name = "lang_log_file.html";

void CloseLogFile()
{
    fclose(log_file);
    printf("Logfile close");
}

void OpenLogFile()
{
    log_file = fopen(log_file_name, "w");

    if (log_file != NULL)
        printf("Logfile open \n");

    else
        perror("Logfile open ERROR \n");

    fprintf(log_file, "<pre>\n");
}

void TreeDump(Tree* tree)
{
    assert(tree);

    fprintf(log_file, "<h3>SIZE: %llu\n</h3>", tree->size);

    static int file_counter = 0;
    char* image_file_name = GetNewImageFileName(file_counter);

    FILE* graphiz_file = fopen(image_file_name, "w");
    assert(graphiz_file);

    fprintf(graphiz_file, "digraph {\n"
                          "graph [charset=\"utf-8\"]"
                          "rankdir = HR;\n"
                          "splines = true\n");

    PrintBazeNode(graphiz_file, tree);

    if (tree->size > 0) {
       printf("DUMP_TREE_ROOT: %p\n", tree->root);
       PrintBazeEdge(graphiz_file, tree);
       PrintTree(tree->root, graphiz_file);
    }

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);

    char* dot_cmd = GetNewDotCmd(file_counter);

    system(dot_cmd);
    free(dot_cmd);

    FillLogFile(image_file_name, tree, file_counter);

    free(image_file_name);
    file_counter++;
}

void PrintBazeNode(FILE* graphiz_file,  Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file,
            "node%p "
            "[shape = Mrecord, "
            "style = filled, "
            "fillcolor = \"#DD7538\", "
            "color = \"#00000\", "
            "label = \" {PTR: %p | SIZE: %llu | ROOT: %p} \"]\n",
            tree, tree, tree->size, tree->root);

}

void PrintBazeEdge(FILE* graphiz_file,  Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n",
                           tree, tree->root);
}

void PrintTree(Node* node, FILE* graphiz_file)
{
    assert(node);
    assert(graphiz_file);

    PrintGraphizNode(graphiz_file, node);
    PrintGraphizEdge(graphiz_file, node);

    if (node->left){
        //printf("PTR: %p   PTR_LEFT: %p\n  ",node,  node->left);
        PrintTree(node->left, graphiz_file);
    }

    if (node->right)
        PrintTree(node->right, graphiz_file);

}

void PrintGraphizEdge(FILE* graphiz_file,  Node* node)
{
    assert(node);
    assert(graphiz_file);

    if (node->left != NULL)
    {
        fprintf(graphiz_file,
                "node%p -> node%p "
                "[dir = both tailport=sw]\n",
                node, node->left);
    }

    if (node->right != NULL)
    {
        fprintf(graphiz_file,
                "node%p -> node%p "
                "[dir = both tailport=se]\n",
                node, node->right);
    }
}

void PrintGraphizNode(FILE* graphiz_file,  Node* node)
{
    assert(node);
    assert(graphiz_file);

    fprintf(graphiz_file,
            "node%p "
            "[shape = Mrecord, "
            "style = filled, "
            "fillcolor = \"%s\", "
            "color = \"#00000\", "
            "label = \" {"
            "TYPE: %s",
            node, GetNodeColor(node), GetNodeTypeName(node));

    if (node->type == NUM)
        fprintf(graphiz_file, "| VALUE: %d",
                              node->lexeme.num);

    else if (node->type == IDENT)
        fprintf(graphiz_file, "| VALUE: %s",
                              ConvertEncoding(node->lexeme.str.name));

    if (node->left != NULL && node->right != NULL)
        fprintf(graphiz_file,
                " | {LEFT | "
                " RIGHT }} \" ]\n");
    else
        fprintf(graphiz_file, "} \" ]\n");

}

void FillLogFile(char* image_file_name,  Tree* tree, int file_counter)
{
    assert(image_file_name != NULL);
    assert(tree != NULL);

    const int PICTURE_WIDTH = 2000;

    fprintf(log_file, "\n\n<img src=Images\\image%d.svg width=%dpx>\n\n",
                       file_counter, PICTURE_WIDTH);

    fprintf(log_file, "--------------------------------------------------------------------------------------------------------------------------------------------\n\n");

    fflush(log_file);
}

const char* GetNodeTypeName(Node* node)
{
    assert(node);

    switch(node->type)
    {
        case KEY_IF:                return "if";
        case KEY_ELSE:              return "else";
        case KEY_WHILE:             return "while";
        case KEY_RETURN:            return "retuen";
        case KEY_INT:               return "int";
        case KEY_LPAREN:            return "(";
        case KEY_RPAREN:            return ")";
        case KEY_SEMICOLON:         return ";";
        case KEY_LBRACE:            return "\\{";
        case KEY_RBRACE:            return "\\}";
        case OP_ASSIGNED:           return "=";
        case OP_EQUAL:              return "==";
        case OP_BIGGER:             return ">";
        case OP_LESS:               return "<";
        case OP_LESS_OR_EQUAL:      return "<=";
        case OP_BIGGER_OR_EQUAL:    return ">=L";
        case OP_NOT_EQUAL:          return "!=";
        case OP_MUL:                return "*";
        case OP_ADD:                return "+";
        case OP_SUB:                return "-";
        case OP_DIV:                return "/";
        case OP_POW:                return "^";
        case OP_MOD:                return "%";
        case OP_AND:                return "&&";
        case OP_OR:                 return "||";
        case IDENT:                 return "IDENT";
        case NUM:                   return "NUM";
        case END:                   return "END";
        case PARAM:                 return "PARAM";
        case KEY_COMMA:             return ",";
        case KEY_IN:                return "in";
        case KEY_OUT:               return "out";
        case OP_SQRT:               return "sqrt";

        default:                    return NULL;
    }
    return NULL;
}

const char* GetNodeColor(Node* node)
{
    assert(node);

    switch(node->type)
    {
        case KEY_IF:                return "#68c3a3ff";
        case KEY_ELSE:              return "#68c3a3ff";
        case KEY_WHILE:             return "#313ef4ff";
        case KEY_RETURN:            return "#ec4343ff";
        case KEY_INT:               return "#7dc368ff";
        case KEY_LPAREN:            return "#828282ff";
        case KEY_RPAREN:            return "#828282ff";
        case KEY_SEMICOLON:         return "#dd65c9ff";
        case KEY_LBRACE:            return "#828282ff";
        case KEY_RBRACE:            return "#c1b74dff";
        case OP_ASSIGNED:           return "#ffa551ff";
        case OP_EQUAL:              return "#ffa551ff";
        case OP_BIGGER:             return "#ffa551ff";
        case OP_LESS:               return "#ffa551ff";
        case OP_SQRT:               return "#ffa551ff";
        case OP_LESS_OR_EQUAL:      return "#ffa551ff";
        case OP_BIGGER_OR_EQUAL:    return "#ffa551ff";
        case OP_NOT_EQUAL:          return "#ffa551ff";
        case OP_MUL:                return "#ffa551ff";
        case OP_ADD:                return "#ffa551ff";
        case OP_SUB:                return "#ffa551ff";
        case OP_DIV:                return "#ffa551ff";
        case OP_POW:                return "#ffa551ff";
        case OP_MOD:                return "#ffa551ff";
        case OP_AND:                return "#ffa551ff";
        case OP_OR:                 return "#ffa551ff";
        case IDENT:                 return "#8791ffff";
        case NUM:                   return "#51ff51ff";
        case END:                   return "#838383ff";
        case PARAM:                 return "#dd65c9ff";
        case KEY_COMMA:             return "#828182ff";
        case KEY_IN:                return "#ec4343ff";
        case KEY_OUT:               return "#ec4343ff";

        default:                    return NULL;
    }
    return NULL;
}

static char* GetNewDotCmd(int file_counter)
{
    char str_command[100] = "";

    snprintf(str_command, sizeof(str_command),
            "dot -Tsvg Images\\image%d.txt -o Images\\image%d.svg",
             file_counter, file_counter);

    return strdup(str_command);
}

static char* GetNewImageFileName(int file_counter)
{
    char str_file_counter[100] = "";

    snprintf(str_file_counter, sizeof(str_file_counter),
             "Images\\image%d.txt", file_counter);

    return strdup(str_file_counter);
}

void PrintTokenArray(TokenArray* tokens, size_t begin_pos)
{
    assert(begin_pos < tokens->size);

    for (size_t i = begin_pos; i < tokens->size; ++i)
    {
        fprintf(log_file, "TOKEN[%llu]:\n", i);

        if (tokens->arr[i].type == NUM)
            fprintf(log_file, "NUM NAME: |%d| ", tokens->arr[i].lexeme.num);

        else
            fprintf(log_file, "(%d) NAME: |%s| ", tokens->arr[i].type, tokens->arr[i].lexeme.str.name);

        fprintf(log_file, " LINE: %llu COLUMN: %llu",
                                   tokens->arr[i].line,
                                   tokens->arr[i].column);

        fprintf(log_file, "\n----------------------------\n\n");
    }
}

char* ConvertEncoding(char* win1251)
{
    if (win1251 == NULL) return "";

    size_t win1251_size = strlen(win1251);
    size_t utf8_size = 0;

    //printf("SIZE: %d  STR: %s\n", win1251_size, win1251);

    for (size_t i = 0; i < win1251_size; i++)
    {
        unsigned char sym = win1251[i];

        if (sym < 128)
            utf8_size++;

        else if (sym >= 192 || sym == 168 || sym == 184)
            utf8_size += 2;
    }

    char* utf8 = (char*)calloc(utf8_size + 1, sizeof(char));

    assert(utf8);

    size_t utf8_i = 0;

    for (size_t win1251_i = 0; win1251_i < win1251_size; win1251_i++)
    {
        unsigned char sym = win1251[win1251_i];

        if (sym < 128)
            utf8[utf8_i++] = sym;

        if (sym >= 192 && sym <= 239)
        {
            utf8[utf8_i++] = (char)208;
            utf8[utf8_i++] = (char)(sym - 48);
        }

        else if (sym >= 240)
        {
            utf8[utf8_i++] = (char)209;
            utf8[utf8_i++] = (char)(sym - 112);
        }

        else if (sym == 168)
        {
            utf8[utf8_i++] = (char)208;
            utf8[utf8_i++] = (char)129;
        }

        else if (sym == 184)
        {
            utf8[utf8_i++] = (char)209;
            utf8[utf8_i++] = (char)145;
        }
    }

    utf8[utf8_i] = '\0';

    return utf8;
}


