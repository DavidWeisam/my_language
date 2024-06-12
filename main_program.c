// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

#define BUFFER_SIZE 1024

int main() {
    char source_code[BUFFER_SIZE];

    while (1) {
        printf("dudu > ");
        if (fgets(source_code, BUFFER_SIZE, stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }

        size_t len = strlen(source_code);
        if (len > 0 && source_code[len - 1] == '\n') {
            source_code[len - 1] = '\0';
        }

        if (strcmp(source_code, "stop") == 0) {
            printf("Terminating program.\n");
            break;
        }

        token **tokens = lexer(source_code);
        if (tokens == NULL) {
            fprintf(stderr, "Error in tokenization\n");
            return 1;
        }

        printf("Tokens:\n");
        print_tokens(tokens);

        Parser *parser = create_parser(tokens);
        if (parser == NULL) {
            fprintf(stderr, "Error in parser initialization\n");
            return 1;
        }

        ASTNode *root = parse_expression(parser);
        if (root == NULL) {
            fprintf(stderr, "Error in parsing\n");
            return 1;
        }

        printf("AST: ");
        print_ast(root);
        printf("\n");

        float result = evaluate_ast(root);
        printf("Result: %f\n", result);

        free_ast(root);
        free_tokens(tokens);
        free(parser);
    }

    return 0;
}
