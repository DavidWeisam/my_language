// lexer.h

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

typedef enum {
    INT,
    FLOAT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    END  // Indicates end of tokens
} token_type;

typedef struct {
    token_type type;
    union {
        char op;
        int int_value;
        float float_value;
    } value;
} token;

token* create_op_token(char op, token_type type);
token* create_int_token(int int_value);
token* create_float_token(float float_value);
const char* token_type_name(token_type type);
token** lexer(char *text);
void free_tokens(token **tokens);
void print_tokens(token **tokens);

#endif // LEXER_H
