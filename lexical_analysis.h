#ifndef LEXICAL_ANALYSIS
#define LEXICAL_ANALYSIS

#include "read_from_file_funcs.h"

const int DEFAULT_TOKEN_ARR_SIZE = 100;
const int MAX_LEN_OF_WORD = 100;

enum Status
{
    success,
    error
};

enum Type
{
    //Kewords
    KEY_IN = -4,
    KEY_OUT = -3,
    KEY_COMMA = -2,
    KEY_IF = -1,
    KEY_ELSE = 0,
    KEY_WHILE = 1,
    KEY_RETURN = 2,
    KEY_INT = 3,
    KEY_LPAREN = 4,
    KEY_RPAREN = 5,
    KEY_SEMICOLON = 6,
    KEY_LBRACE = 7,
    KEY_RBRACE = 8,

    //Operators

    OP_ASSIGNED = 9,
    OP_EQUAL = 10,
    OP_BIGGER = 11,
    OP_LESS = 12,
    OP_LESS_OR_EQUAL = 13,
    OP_BIGGER_OR_EQUAL = 14,
    OP_NOT_EQUAL = 15,
    OP_MUL = 16,
    OP_ADD = 17,
    OP_DIV = 18,
    OP_SUB = 19,
    OP_POW = 20,
    OP_MOD = 21,
    OP_AND = -5,
    OP_OR = -6,
    OP_SQRT = -7,

    //Identifier
    IDENT = 22,

    //Number
    NUM = 23,

    //Param
    PARAM = 24,

    //End
    END = 25
};

struct String
{
    char* name;
    size_t hash;
};

union Lexeme
{
    int num;
    String str;
};

struct Token
{
    Type type;
    Lexeme lexeme;
    size_t line;
    size_t column;
};

struct TokenArray
{
    Token* arr;
    size_t size;
};

void TokenArrayCtor(TokenArray* tokens);

void TokenArrayDtor(TokenArray* tokens);

Status MakeLexicalAnalysis(Buffer* buffer, size_t* pos, TokenArray* tokens);

void MoveBufferPointer(Buffer* buffer, size_t* pos, size_t offset);

Status ParseKeyWord(Buffer* buffer, size_t* pos, TokenArray* tokens);

Status ParseNumber(Buffer* buffer, size_t* pos, TokenArray* tokens);

Status ParseIdentifier(Buffer* buffer, size_t* pos, TokenArray* tokens);

Status ParseOperator(Buffer* buffer, size_t* pos, TokenArray* tokens);

int GetSignOfNumber(Buffer* buffer, size_t* pos);

void AddNumToken(TokenArray* tokens, Buffer* buffer,
                 Type type, int num);

void AddStringToken(TokenArray* tokens, Buffer* buffer,
                    Type type, char* str);

void SkipSpaces(Buffer* buffer, size_t* pos);

bool IsSymInIdentifierName(char sym);

void ReadWord(Buffer* buffer, size_t pos, char* word, size_t* len);

#endif
