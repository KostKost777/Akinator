#include <TXLib.h>

#include "orig_lexicon_of_lang.h"
#include "lexical_analysis.h"
#include "hash_funcs.h"

BaseLexeme keywords_arr[NUM_OF_KEYWORDS] =
{
{KEY_IF,        "вилкойвглаз",  GetHash("вилкойвглаз")},    //if
{KEY_ELSE,      "иливжопураз",  GetHash("иливжопураз")},    //else
{KEY_WHILE,     "потрещим",     GetHash("потрещим")},           //while
{KEY_RETURN,    "шухер",        GetHash("шухер")},        //return
{KEY_INT,       "братуха",      GetHash("братуха")},        //int
{KEY_LPAREN,    "гыы",          GetHash("гыы")},          //(
{KEY_RPAREN,    "жыы",          GetHash("жыы")},       //)
{KEY_SEMICOLON, "наx",          GetHash("нах")},        //;
{KEY_LBRACE,    "гоп",          GetHash("гоп")},         // {
{KEY_RBRACE,    "стоп",         GetHash("стоп")},       // }
{KEY_COMMA,     ",",            GetHash(",")},           // ,
{KEY_OUT,       "малевать",     GetHash("малевать")},    // out
{KEY_IN,        "вечервхату",   GetHash("вечервхату")}  // in
};

BaseLexeme operator_arr[NUM_OF_OPERATORS] =
{
{OP_ASSIGNED,        "=",    GetHash("=")},
{OP_EQUAL,           "==",   GetHash("==")},
{OP_NOT_EQUAL,       "!=",   GetHash("!=")},
{OP_LESS_OR_EQUAL,   "<=",   GetHash("<=")},
{OP_BIGGER_OR_EQUAL, ">=",   GetHash(">=")},
{OP_LESS,            "<",    GetHash("<")},
{OP_BIGGER,          ">",    GetHash(">")},
{OP_BIGGER,          "||",   GetHash("||")},
{OP_BIGGER,          "&&",   GetHash("&&")},
{OP_ADD,             "+",    GetHash("+")},
{OP_SUB,             "-",    GetHash("-")},
{OP_MUL,             "*",    GetHash("*")},
{OP_DIV,             "/",    GetHash("/")},
{OP_POW,             "^",    GetHash("^")},
{OP_MOD,             "%",    GetHash("%")},
{OP_SQRT,            "sqrt", GetHash("sqrt")},
};
