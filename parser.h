// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct ASTNode {
    token_type type;
    union {
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
        } binary;
        int int_value;
        float float_value;
    } value;
} ASTNode;

typedef struct {
    token **tokens;
    int current;
} Parser;;

ASTNode* parse_expression(Parser *parser);
Parser* create_parser(token **tokens);
ASTNode* parse_term(Parser *parser);
ASTNode* parse_factor(Parser *parser);
token* current_token(Parser *parser);
token* next_token(Parser *parser);
ASTNode* create_binary_node(token_type type, ASTNode* left, ASTNode* right);
ASTNode* create_int_node(int value);
ASTNode* create_float_node(float value);
void free_ast(ASTNode *node);
void print_ast(ASTNode *node);
float evaluate_ast(ASTNode *node);

#endif // PARSER_H
