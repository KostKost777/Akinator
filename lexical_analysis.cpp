#include <TXLib.h>

#include "lexical_analysis.h"
#include "orig_lexicon_of_lang.h"
#include "hash_funcs.h"

void TokenArrayCtor(TokenArray* tokens)
{
    assert(tokens);
    tokens->arr = (Token*)calloc(DEFAULT_TOKEN_ARR_SIZE, sizeof(Token));
    tokens->size = 0;
}

void TokenArrayDtor(TokenArray* tokens)
{
    assert(tokens);

    for (size_t i = 0; i < tokens->size; ++i)
    {
        if (tokens->arr[i].type != NUM)
        {
           free(tokens->arr[i].lexeme.str.name);
           tokens->arr[i].lexeme.str.name = NULL;
           tokens->arr[i].lexeme.str.hash = 0;
        }
    }

    free(tokens->arr);
    tokens->arr = NULL;

    tokens->size = 0;
}

Status MakeLexicalAnalysis(Buffer* buffer, size_t* pos, TokenArray* tokens)
{
    assert(buffer);
    assert(pos);
    assert(buffer->data);
    assert(tokens);

    while(buffer->data[*pos] != '\0')
    {
        size_t old_size = buffer->size;

        // printf("---------------------------------\n");
        // printf("BUFFER: %s\n\n", buffer->data);
        // printf("POS: %llu\n", *pos);
        // printf("---------------------------------\n\n");
        ;

        if (ParseNumber(buffer, pos, tokens) == success)
            continue;

        if (ParseKeyWord(buffer, pos, tokens) == success)
            continue;

        if (ParseOperator(buffer, pos, tokens)== success)
            continue;

        if(ParseIdentifier(buffer, pos, tokens) == success)
            continue;

        if (old_size == buffer->size && buffer->data[*pos] != '\0')
        {
            printf("ERROR on line: %llu on column: %llu\n",
                                              buffer->line,
                                              buffer->column);
            return error;
        }
    }

    AddStringToken(tokens, buffer, END, buffer->data + *pos);

    return success;
}

Status ParseNumber(Buffer* buffer, size_t* pos, TokenArray* tokens)
{
    assert(buffer);
    assert(buffer->data);
    assert(pos);
    assert(tokens);

    int sign = GetSignOfNumber(buffer, pos);
    if (sign == 0) return error;

    int num = 0;

    while(isdigit(buffer->data[*pos]))
    {
        num *= 10;
        num += (buffer->data[*pos] - '0');
        MoveBufferPointer(buffer, pos, 1);
    }

    //printf("NUM: %d\n", num);
    AddNumToken(tokens, buffer, NUM, num);
    SkipSpaces(buffer, pos);

    return success;
}

Status ParseKeyWord(Buffer* buffer, size_t* pos, TokenArray* tokens)
{
    assert(buffer);
    assert(buffer->data);
    assert(pos);
    assert(tokens);

    char keyword_name[MAX_LEN_OF_WORD] = {};
    int len = 0;

    sscanf(buffer->data + *pos, "%s%n", keyword_name, &len);

    if (len < 0) assert(false);

    size_t keyword_hash = GetHash(keyword_name);

    for (int i = 0; i < NUM_OF_KEYWORDS; ++i)
    {
        // printf("KEY_WORD: |%s|   MY_KEY_WORD: |%s|\n", keywords_arr[i].name,
        //                                               keyword_name);
        if (keywords_arr[i].hash == keyword_hash)
        {
            AddStringToken(tokens, buffer, keywords_arr[i].type, keyword_name);

            MoveBufferPointer(buffer, pos, (size_t)len);

            SkipSpaces(buffer, pos);
            return success;
        }
    }

    return error;
}

Status ParseOperator(Buffer* buffer, size_t* pos, TokenArray* tokens)
{
    assert(buffer);
    assert(buffer->data);
    assert(pos);
    assert(tokens);

    char operator_name[MAX_LEN_OF_WORD] = {};
    int len = 0;

    sscanf(buffer->data + *pos, "%s%n", operator_name, &len);

    if (len < 0) assert(false);

    size_t operator_hash = GetHash(operator_name);

    for (int i = 0; i < NUM_OF_OPERATORS; ++i)
    {
        //printf("ORIG: |%s|   MY: |%s|\n", operator_arr[i].name, operator_name);
        if (operator_arr[i].hash == operator_hash)
        {
            AddStringToken(tokens, buffer,
                           operator_arr[i].type, operator_name);

            MoveBufferPointer(buffer, pos, (size_t)len);

            SkipSpaces(buffer, pos);
            return success;
        }
    }
    return error;
}

Status ParseIdentifier(Buffer* buffer, size_t* pos, TokenArray* tokens)
{
    assert(buffer);
    assert(buffer->data);
    assert(pos);
    assert(tokens);

    size_t len = 0;
    char identifier_name[MAX_LEN_OF_WORD] = {};

    while(true)
    {
        if (   !((len != 0 && isdigit(buffer->data[*pos]))
            || IsSymInIdentifierName(buffer->data[*pos]))
            || isspace(buffer->data[*pos]))
            break;

        identifier_name[len] = buffer->data[*pos];

        len++;
        MoveBufferPointer(buffer, pos, 1);
        printf("ABOBA: %s\n", identifier_name);
    }

    AddStringToken(tokens, buffer, IDENT, identifier_name);

    SkipSpaces(buffer, pos);
    return success;
}

int GetSignOfNumber(Buffer* buffer, size_t* pos)
{
    assert(buffer);
    assert(pos);
    assert(buffer->data);

    if (buffer->data[*pos] == '-' && isdigit (*(buffer->data + 1)))
    {
        MoveBufferPointer(buffer, pos, 1);
        return -1;
    }
    else if (isdigit (buffer->data[*pos]) )
        return 1;

    return 0;
}

void AddNumToken(TokenArray* tokens, Buffer* buffer,
                 Type type, int num)
{
    assert(tokens);
    assert(buffer);

    tokens->arr[tokens->size].type = type;
    tokens->arr[tokens->size].lexeme.num = num;
    tokens->arr[tokens->size].line = buffer->line;
    tokens->arr[tokens->size].column = buffer->column;

    tokens->size++;
}

void AddStringToken(TokenArray* tokens, Buffer* buffer,
                    Type type, char* name)
{
    assert(tokens);
    assert(buffer);
    assert(name);

    tokens->arr[tokens->size].type = type;
    tokens->arr[tokens->size].lexeme.str.name = strdup(name);
    tokens->arr[tokens->size].lexeme.str.hash = GetHash(name);
    tokens->arr[tokens->size].line = buffer->line;
    tokens->arr[tokens->size].column = buffer->column;

    tokens->size++;
}

void MoveBufferPointer(Buffer* buffer, size_t* pos,  size_t offset)
{
    assert(buffer);
    assert(buffer->data);
    assert(pos);

    *pos += offset;
    buffer->column += offset;
}

void SkipSpaces(Buffer* buffer, size_t* pos)
{
    assert(buffer);
    assert(buffer->data);
    assert(pos);

    while(isspace(buffer->data[*pos]))
    {
        switch(buffer->data[*pos])
        {
            case ' ': buffer->column++;
                      break;

            case '\t': buffer->column += 4;
                       break;

            case '\n': buffer->column = 1;
                       buffer->line++;
                       break;

            default: break;
        }

        *pos += 1;
    }
}

bool IsSymInIdentifierName(char sym)
{
    unsigned char c = (unsigned char)sym;
    return isalpha(c) || c == '_'
           || c >= 192 || c == 168 || c == 184;
}

// bool IsRussianAlpha(char sym)
// {
//     unsigned char c = (unsigned char) sym;
//     return c >= 192 || c == 168 || c == 184;
// }
