#include <TXLib.h>

#include "tree_funcs.h"
#include "lexical_analysis.h"
#include "syntactic_analysis.h"
#include "dump_funcs.h"
#include "tree_funcs.h"


Status MakeSyntacticAnalysis(Tree* tree, TokenArray* tokens)
{
    assert(tree);
    assert(tokens);

    size_t pos = 0;
    tree->root = GetGrammer(tokens, &pos, tree, tree->root);

    return success;
}

Node* GetGrammer(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetGeneral</strong>\n");
    fflush(log_file);

    node = GetInitOfFunc(tokens, pos, tree, node);

    if (tokens->arr[*pos].type != END)
    {
        fprintf(log_file, "<strong>Ошибочный токен: </strong>\n");
        PrintTokenArray(tokens, *pos);
    }

    return node;
}

Node* GetInitOfFunc(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetInitOfFunc</strong>\n");
    fflush(log_file);

    if (tokens->arr[*pos].type != KEY_INT)
        return GetOperator(tokens, pos, tree, node);
    *pos += 1;

    Node* node_left = GetIdentifier(tokens, pos, tree);

    if (node_left == NULL || tokens->arr[*pos + 1].type != KEY_LPAREN)
    {
        *pos -= 1;
        return GetOperator(tokens, pos, tree, node);
    }

    Token ident_token = tokens->arr[*pos];
    *pos += 2;

    Node* node_args = GetArgsOfInitFunc(tokens, pos, tree, node);

    if (tokens->arr[*pos].type != KEY_RPAREN)
        return NULL;
    *pos += 1;

    if (tokens->arr[*pos].type != KEY_LBRACE)
        return NULL;
    *pos += 1;

    Node* node_right = GetOperator(tokens, pos, tree, node);
    if (node_right == NULL)
        return NULL;

    if (tokens->arr[*pos].type != KEY_RBRACE)
        return NULL;
    *pos += 1;

    printf("H3\n");
    return  NewNode(GetSeparateToken(KEY_RBRACE),
                    NewNode(ident_token,
                            node_args,
                            node_right,
                            tree),
                    GetInitOfFunc(tokens, pos, tree, node),
                    tree);
}

Node* GetArgsOfInitFunc(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetArgsOfInitFunc</strong>\n");

    if (tokens->arr[*pos].type != KEY_INT) return NULL;
    Token init_token = tokens->arr[*pos];
    *pos += 1;

    Node* node_right = GetIdentifier(tokens, pos, tree);

    if (node_right == NULL)  return NULL;
    *pos += 1;

    if (   tokens->arr[*pos].type != KEY_RPAREN
        && tokens->arr[*pos].type != KEY_COMMA)
        return NULL;

    if (tokens->arr[*pos].type == KEY_COMMA)
        *pos += 1;

    return NewNode(init_token,
                   GetArgsOfInitFunc(tokens, pos, tree, node),
                   node_right,
                   tree);
}

Node* GetOperator(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetOperator</strong>\n");
    fflush(log_file);

    node = GetReturnOp(tokens, pos, tree, node);
    if (node != NULL)
        return node;

    node = GetAssignedOp(tokens, pos, tree, node);
    if (node != NULL)
        return node;

    node = GetIfOp(tokens, pos, tree, node);
    if (node != NULL)
        return node;

    node = GetWhileOp(tokens, pos, tree, node);
    if (node != NULL)
        return node;

    node = GetInOutOp(tokens, pos, tree, node);
    if (node != NULL)
        return node;

    return node;
}

Node* GetWhileOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetWhileOp</strong>\n");
    fflush(log_file);

    if (tokens->arr[*pos].type != KEY_WHILE) return NULL;
    Token while_token = tokens->arr[*pos];
    *pos += 1;

    if (tokens->arr[*pos].type != KEY_LPAREN) return NULL;
    *pos += 1;

    Node* node_left = GetFirstCompareOp(tokens, pos, tree, node);
    if (node_left == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_RPAREN) return NULL;
    *pos += 1;

    if (tokens->arr[*pos].type != KEY_LBRACE) return NULL;
    *pos += 1;

    Node* node_right = GetOperator(tokens, pos, tree, node);
    if (node_right == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_RBRACE) return NULL;
    *pos += 1;

    return NewNode(GetSeparateToken(KEY_SEMICOLON),
                   NewNode(while_token,
                        node_left,
                        node_right,
                        tree),
                    GetOperator(tokens, pos, tree, node),
                    tree);
}

Node* GetIfOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetIfOp</strong>\n");
    fflush(log_file);

    if (tokens->arr[*pos].type != KEY_IF) return NULL;
    Token if_token = tokens->arr[*pos];
    *pos += 1;

    if (tokens->arr[*pos].type != KEY_LPAREN) return NULL;
    *pos += 1;

    Node* node_condition = GetFirstCompareOp(tokens, pos, tree, node);
    if (node_condition == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_RPAREN) return NULL;
    *pos += 1;

    if (tokens->arr[*pos].type != KEY_LBRACE) return NULL;
    *pos += 1;

    Node* node_action = GetOperator(tokens, pos, tree, node);
    if (node_action == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_RBRACE) return NULL;
    *pos += 1;

    Node* node_else = GetElseOp(tokens, pos, tree, node, node_action);

    if (node_else == NULL)
        node_else = node_action;

    return NewNode(GetSeparateToken(KEY_SEMICOLON),
                   NewNode(if_token,
                            node_condition,
                            node_else,
                            tree),
                    GetOperator(tokens, pos, tree, node),
                    tree);
}

Node* GetElseOp(TokenArray* tokens, size_t* pos, Tree* tree,
                Node* node, Node* node_action)
{
    assert(tree);
    assert(tokens);
    assert(pos);
    assert(node_action);

    fprintf(log_file, "<strong>Вызов GetElseOp</strong>\n");
    fflush(log_file);

    if (tokens->arr[*pos].type != KEY_ELSE) return NULL;
    Token else_token = tokens->arr[*pos];
    *pos += 1;

    if (tokens->arr[*pos].type != KEY_LBRACE) return NULL;
    *pos += 1;

    Node* node_left = GetOperator(tokens, pos, tree, node);
    if (node_left == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_RBRACE) return NULL;
    *pos += 1;

    return NewNode(else_token,
                   node_action,
                   node_left,
                   tree);
}

Node* GetAssignedOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetAssignedOp</strong>\n");
    fflush(log_file);

    Node* node_left = GetWord(tokens, pos, tree, node);
    if (node_left == NULL) return NULL;

    if (tokens->arr[*pos].type != OP_ASSIGNED) return NULL;
    Token assigned_token = tokens->arr[*pos];
    *pos += 1;

    Node* node_right = GetFirstCompareOp(tokens, pos, tree, node);
    if (node_right == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_SEMICOLON) return NULL;
    *pos += 1;

    return  NewNode(GetSeparateToken(KEY_SEMICOLON),
                    NewNode(assigned_token, node_left, node_right, tree),
                    GetOperator(tokens, pos, tree, node),
                    tree);
}

Node* GetReturnOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetReturnOp</strong>\n");
    fflush(log_file);

    if (tokens->arr[*pos].type != KEY_RETURN) return NULL;
    Token return_token = tokens->arr[*pos];
    *pos += 1;

    Node* node_left = GetFirstCompareOp(tokens, pos, tree, node);
    if (node_left == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_SEMICOLON) return NULL;
    *pos += 1;

    return  NewNode(GetSeparateToken(KEY_SEMICOLON),
                    NewNode(return_token, node_left, NULL, tree),
                    NULL,
                    tree);
}

Node* GetInOutOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetReturnOp</strong>\n");
    fflush(log_file);

    if (   tokens->arr[*pos].type != KEY_IN
        && tokens->arr[*pos].type != KEY_OUT)
        return NULL;

    Token token = tokens->arr[*pos];
    *pos += 1;

    Node* node_left = NULL;

    if (token.type == KEY_IN)
    {
        node_left = GetIdentifier(tokens, pos, tree);
        *pos += 1;
    }
    else
        node_left = GetFirstCompareOp(tokens, pos, tree, node);

    if (node_left == NULL) return NULL;

    if (tokens->arr[*pos].type != KEY_SEMICOLON) return NULL;
    *pos += 1;

    return  NewNode(GetSeparateToken(KEY_SEMICOLON),
                    NewNode(token, node_left, NULL, tree),
                    GetOperator(tokens, pos, tree, node),
                    tree);
}

Node* GetFunc(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetFuncOp</strong>\n");
    fflush(log_file);

    Node* node_left = GetIdentifier(tokens, pos, tree);

    if (node_left == NULL || tokens->arr[*pos + 1].type != KEY_LPAREN)
        return NULL;
    Token ident_token = tokens->arr[*pos];
    *pos += 2;

    Node* node_args = GetFuncArgs(tokens, pos, tree, node);

    if (tokens->arr[*pos].type != KEY_RPAREN) return NULL;
    *pos += 1;

    return NewNode(ident_token,
                   node_args,
                   NULL,
                   tree);
}

Node* GetFuncArgs(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetFuncArgs</strong>\n");

    Node* node_right = GetFirstCompareOp(tokens, pos, tree, node);

    if (node_right == NULL)  return NULL;


    if (   tokens->arr[*pos].type != KEY_RPAREN
        && tokens->arr[*pos].type != KEY_COMMA)
        return NULL;


    if (tokens->arr[*pos].type == KEY_COMMA)
        *pos += 1;

    return NewNode(GetSeparateToken(PARAM),
                   GetFuncArgs(tokens, pos, tree, node),
                   node_right,
                   tree);
}

Node* GetFirstCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetFirstCompareOp</strong>\n");
    fflush(log_file);

    node = GetSecondCompareOp(tokens, pos, tree, node);

    while (  tokens->arr[*pos].type == OP_OR)
    {
        Token or_token = tokens->arr[*pos];

        *pos += 1;

        node = NewNode(or_token,
                       node,
                       GetSecondCompareOp(tokens, pos, tree, node),
                       tree);
    }

    return node;
}

Node* GetSecondCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetSecondCompareOp</strong>\n");
    fflush(log_file);

    node = GetThirdCompareOp(tokens, pos, tree, node);

    while (  tokens->arr[*pos].type == OP_AND)
    {
        Token and_token = tokens->arr[*pos];

        *pos += 1;

        node = NewNode(and_token,
                       node,
                       GetThirdCompareOp(tokens, pos, tree, node),
                       tree);
    }

    return node;
}

Node* GetThirdCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetThirdCompareOp</strong>\n");
    fflush(log_file);

    node = GetFourthCompareOp(tokens, pos, tree, node);

    while (   tokens->arr[*pos].type == OP_EQUAL
           || tokens->arr[*pos].type == OP_NOT_EQUAL)
    {
        Token token = tokens->arr[*pos];

        *pos += 1;

        node = NewNode(token,
                       node,
                       GetFourthCompareOp(tokens, pos, tree, node),
                       tree);
    }

    return node;
}

Node* GetFourthCompareOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetFourthCompareOp</strong>\n");
    fflush(log_file);

    node = GetAddSubOp(tokens, pos, tree, node);

    while (   tokens->arr[*pos].type == OP_LESS
           || tokens->arr[*pos].type == OP_BIGGER
           || tokens->arr[*pos].type == OP_LESS_OR_EQUAL
           || tokens->arr[*pos].type == OP_BIGGER_OR_EQUAL)
    {
        Token token = tokens->arr[*pos];

        *pos += 1;

        node = NewNode(token,
                       node,
                       GetAddSubOp(tokens, pos, tree, node),
                       tree);
    }

    return node;
}

Node* GetAddSubOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetAddSubOp</strong>\n");
    fflush(log_file);

    node = GetMulDivModOp(tokens, pos, tree, node);

    while (  tokens->arr[*pos].type == OP_ADD
          || tokens->arr[*pos].type == OP_SUB)
    {
        Token add_sub_token = tokens->arr[*pos];

        *pos += 1;

        node = NewNode(add_sub_token,
                       node,
                       GetMulDivModOp(tokens, pos, tree, node),
                       tree);
    }

    return node;
}

Node* GetMulDivModOp(TokenArray* tokens, size_t* pos,
                     Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetMulDivModOp</strong>\n");
    fflush(log_file);

    node = GetPowOp(tokens, pos, tree, node);

    while (   tokens->arr[*pos].type == OP_MUL
           || tokens->arr[*pos].type == OP_DIV
           || tokens->arr[*pos].type == OP_MOD)
    {
        Token mul_div_mod_token = tokens->arr[*pos];

        *pos += 1;

        node = NewNode(mul_div_mod_token,
                       node,
                       GetPowOp(tokens, pos, tree, node),
                       tree);
    }

    return node;
}

Node* GetPowOp(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetPowOp</strong>\n");
    fflush(log_file);

    node = GetMostPreority(tokens, pos, tree, node);

    while (tokens->arr[*pos].type == OP_POW)
    {
        Token pow_token = tokens->arr[*pos];

        *pos += 1;

        node = NewNode(pow_token,
                       node,
                       GetMostPreority(tokens, pos, tree, node),
                       tree);
    }

    return node;
}

Node* GetSqrt(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    if (tokens->arr[*pos].type != OP_SQRT) return NULL;
    Token sqrt_token = tokens->arr[*pos];
    *pos += 1;

    if (tokens->arr[*pos].type != KEY_LPAREN) return NULL;
    *pos += 1;

    Node* node_left = GetFirstCompareOp(tokens, pos, tree, node);

    if (tokens->arr[*pos].type != KEY_RPAREN) return NULL;
    *pos += 1;

    return NewNode(sqrt_token,
                   node_left,
                   NULL,
                   tree);
}

Node* GetMostPreority(TokenArray* tokens, size_t* pos,
                      Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetMostPreority</strong>\n");
    fflush(log_file);

    node = GetFunc(tokens, pos, tree, node);
    if (node != NULL)
    {
        fprintf(log_file, "<strong>Нашел вызов функции\n\n");
        return node;
    }

    node = GetSqrt(tokens, pos, tree, node);
    if (node != NULL)
    {
        fprintf(log_file, "<strong>Нашел вызов функции\n\n");
        return node;
    }

    if (tokens->arr[*pos].type == KEY_LPAREN)
    {
        fprintf(log_file, "<strong>Обнаружил\"(\": </strong>\n\n");
        fflush(log_file);
        *pos += 1;

        node = GetAddSubOp(tokens, pos, tree, node);

        if (tokens->arr[*pos].type == KEY_RPAREN)
        {
            *pos += 1;
            fprintf(log_file, "<strong>Обнаружил \")\"</strong>\n\n");
            return node;
        }

        fprintf(log_file, "<strong>Не обнаружил \")\"</strong>\n\n");
    }

    node = GetNumber(tokens, pos, tree);
    if (node != NULL)
    {
        *pos += 1;
        fprintf(log_file, "<strong>Нашел числа\n\n");
        return node;
    }

    node = GetWord(tokens, pos, tree, node);
    if (node != NULL)
    {
        fprintf(log_file, "<strong>Нашел слова</strong>\n\n");
        return node;
    }

    return NULL;
}

Node* GetNumber(TokenArray* tokens, size_t* pos, Tree* tree)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetNumber</strong>\n");

    if (tokens->arr[*pos].type == NUM)
    {
        fprintf(log_file, "<strong>Обнаружил число</strong>\n\n");
        return NewNode(tokens->arr[*pos], NULL, NULL, tree);
    }

    return NULL;
}

Node* GetWord(TokenArray* tokens, size_t* pos, Tree* tree, Node* node)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetWord</strong>\n");

//     node = GetFunction(tokens, pos, tree, node);
//
//     if (node != NULL)
//     {
//         fprintf(log_file, "<strong>Нашел функцию</strong>\n\n");
//         return node;
//     }

    node = GetIdentifier(tokens, pos, tree);

    if (node != NULL)
    {
        fprintf(log_file, "<strong>Нашел индификатор</strong>\n\n");
        *pos += 1;
        PrintTokenArray(tokens, *pos);
        return node;
    }

    return NULL;
}

Node* GetIdentifier(TokenArray* tokens, size_t* pos, Tree* tree)
{
    assert(tree);
    assert(tokens);
    assert(pos);

    fprintf(log_file, "<strong>Вызов GetIdentifier</strong>\n");

    if (tokens->arr[*pos].type == KEY_INT && tokens->arr[*pos + 1].type == IDENT)
    {
        fprintf(log_file, "<strong>Обнаружил инициализацию переменной</strong>\n\n");
        *pos += 1;
        return NewNode(tokens->arr[*pos - 1],
                       NewNode(tokens->arr[*pos],
                               NULL,
                               NULL,
                               tree),
                       NULL,
                       tree);
    }

    if (tokens->arr[*pos].type == IDENT)
    {
        fprintf(log_file, "<strong>Нашел индификатор</strong>\n\n");
        return NewNode(tokens->arr[*pos], NULL, NULL, tree);
    }

    return NULL;
}

Token GetSeparateToken(Type type)
{
    Token separate_token = {type, 0, 0, 0};
    return separate_token;
}
