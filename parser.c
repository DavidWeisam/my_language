// parser.c

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>




ASTNode* parse_expression(Parser *parser) {
    ASTNode *node = parse_term(parser);
    while (current_token(parser)->type == PLUS || current_token(parser)->type == MINUS) {
        token_type op_type = current_token(parser)->type;
        next_token(parser);
        ASTNode *right = parse_term(parser);
        node = create_binary_node(op_type, node, right);
    }
    return node;
}

ASTNode* parse_term(Parser *parser) {
    ASTNode *node = parse_factor(parser);
    while (current_token(parser)->type == MUL || current_token(parser)->type == DIV) {
        token_type op_type = current_token(parser)->type;
        next_token(parser);
        ASTNode *right = parse_factor(parser);
        node = create_binary_node(op_type, node, right);
    }
    return node;
}

ASTNode* parse_factor(Parser *parser) {
    token *current = current_token(parser);
    if (current->type == INT) {
        next_token(parser);
        return create_int_node(current->value.int_value);
    } else if (current->type == FLOAT) {
        next_token(parser);
        return create_float_node(current->value.float_value);
    } else if (current->type == LPAREN) {
        next_token(parser);
        ASTNode *node = parse_expression(parser);
        if (current_token(parser)->type != RPAREN) {
            fprintf(stderr, "Expected closing parenthesis\n");
            exit(EXIT_FAILURE);
        }
        next_token(parser);
        return node;
    } else {
        fprintf(stderr, "Unexpected token type\n");
        exit(EXIT_FAILURE);
    }
}

token* current_token(Parser *parser) {
    return parser->tokens[parser->current];
}

token* next_token(Parser *parser) {
    parser->current++;
    return current_token(parser);
}

Parser* create_parser(token **tokens) {
    Parser *parser = malloc(sizeof(Parser));
    if (parser == NULL) {
        return NULL; // Memory allocation failed
    }
    parser->tokens = tokens;
    parser->current = 0;
    return parser;
}

ASTNode* create_binary_node(token_type type, ASTNode* left, ASTNode* right) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value.binary.left = left;
    node->value.binary.right = right;
    return node;
}

ASTNode* create_int_node(int value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = INT;
    node->value.int_value = value;
    return node;
}

ASTNode* create_float_node(float value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = FLOAT;
    node->value.float_value = value;
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->type == PLUS || node->type == MINUS || node->type == MUL || node->type == DIV) {
        free_ast(node->value.binary.left);
        free_ast(node->value.binary.right);
    }
    free(node);
}

void print_ast(ASTNode *node) {
    if (!node) return;
    if (node->type == INT) {
        printf("%d", node->value.int_value);
    } else if (node->type == FLOAT) {
        printf("%f", node->value.float_value);
    } else {
        printf("(");
        print_ast(node->value.binary.left);
        printf(" %s ", token_type_name(node->type));
        print_ast(node->value.binary.right);
        printf(")");
    }
}

float evaluate_ast(ASTNode *node) {
    if (node->type == INT) {
        return node->value.int_value;
    } else if (node->type == FLOAT) {
        return node->value.float_value;
    } else if (node->type == PLUS) {
        return evaluate_ast(node->value.binary.left) + evaluate_ast(node->value.binary.right);
    } else if (node->type == MINUS) {
        return evaluate_ast(node->value.binary.left) - evaluate_ast(node->value.binary.right);
    } else if (node->type == MUL) {
        return evaluate_ast(node->value.binary.left) * evaluate_ast(node->value.binary.right);
    } else if (node->type == DIV) {
        return evaluate_ast(node->value.binary.left) / evaluate_ast(node->value.binary.right);
    }
    return 0; // Default return value
}
