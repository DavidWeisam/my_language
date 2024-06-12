// lexer.c

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

bool is_space(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}

token* create_op_token(char op, token_type type) {
    token *new_token = (token *)malloc(sizeof(token));
    if (!new_token) {
        perror("Failed to allocate token");
        exit(EXIT_FAILURE);
    }
    new_token->type = type;
    new_token->value.op = op;
    return new_token;
}

token* create_int_token(int int_value) {
    token *new_token = (token *)malloc(sizeof(token));
    if (!new_token) {
        perror("Failed to allocate token");
        exit(EXIT_FAILURE);
    }
    new_token->type = INT;
    new_token->value.int_value = int_value;
    return new_token;
}

token* create_float_token(float float_value) {
    token *new_token = (token *)malloc(sizeof(token));
    if (!new_token) {
        perror("Failed to allocate token");
        exit(EXIT_FAILURE);
    }
    new_token->type = FLOAT;
    new_token->value.float_value = float_value;
    return new_token;
}

const char* token_type_name(token_type type) {
    switch (type) {
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case INT: return "INT";
        case FLOAT: return "FLOAT";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

token** lexer(char *text) {
    int max_tokens = 10;
    token **tokens = malloc(sizeof(token*) * (max_tokens + 1));
    if (!tokens) {
        perror("Failed to allocate tokens array");
        exit(EXIT_FAILURE);
    }
    int text_index = 0;
    int tokens_index = 0;

    while (text[text_index] != '\0') {
        if (is_space(text[text_index])) {
            text_index++;
            continue;
        }

        if (tokens_index >= max_tokens) {
            max_tokens *= 2;
            token **temp_tokens = realloc(tokens, sizeof(token*) * (max_tokens + 1));
            if (!temp_tokens) {
                free(tokens);
                perror("Failed to reallocate tokens array: Not enough space");
                exit(EXIT_FAILURE);
            }
            tokens = temp_tokens;
        }

        if (text[text_index] == '+') {
            tokens[tokens_index++] = create_op_token('+', PLUS);
            text_index++;
        } else if (text[text_index] == '-') {
            tokens[tokens_index++] = create_op_token('-', MINUS);
            text_index++;
        } else if (text[text_index] == '*') {
            tokens[tokens_index++] = create_op_token('*', MUL);
            text_index++;
        } else if (text[text_index] == '/') {
            tokens[tokens_index++] = create_op_token('/', DIV);
            text_index++;
        } else if (text[text_index] == '(') {
            tokens[tokens_index++] = create_op_token('(', LPAREN);
            text_index++;
        } else if (text[text_index] == ')') {
            tokens[tokens_index++] = create_op_token(')', RPAREN);
            text_index++;
        } else if (isdigit(text[text_index])) {
            char buffer[256];
            int len = 0;
            int has_decimal = false;

            while (isdigit(text[text_index]) || (text[text_index] == '.' && !has_decimal)) {
                if (text[text_index] == '.') {
                    has_decimal = true;
                }
                buffer[len++] = text[text_index++];
            }
            buffer[len] = '\0';

            if (has_decimal) {
                tokens[tokens_index++] = create_float_token(strtof(buffer, NULL));
            } else {
                tokens[tokens_index++] = create_int_token(atoi(buffer));
            }
        } else {
            fprintf(stderr, "Unexpected character: %c\n", text[text_index]);
            text_index++;
        }
    }

    // End of tokens
    tokens[tokens_index] = create_op_token('\0', END);

    return tokens;
}

void free_tokens(token **tokens) {
    for (int i = 0; tokens[i]->type != END; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

void print_tokens(token **tokens) {
    for (int i = 0; tokens[i]->type != END; i++) {
        switch (tokens[i]->type) {
            case PLUS:
            case MINUS:
            case MUL:
            case DIV:
            case LPAREN:
            case RPAREN:
                printf("Operator (%s): %c\n", token_type_name(tokens[i]->type), tokens[i]->value.op);
                break;
            case INT:
                printf("Integer: %d\n", tokens[i]->value.int_value);
                break;
            case FLOAT:
                printf("Float: %f\n", tokens[i]->value.float_value);
                break;
            default:
                printf("Unknown token type\n");
                break;
        }
    }
}
