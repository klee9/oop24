/*
 some fixing to do:
 1. keep track of all tokens with the lexer.
 2. use getNextToken to get the next token.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[50];
    int value;
} Ident;

typedef enum {
    ASSIGN, ADD, MULT, SEMICOLON, IDENT, INT_LIT, END_OF_FILE, LPAREN, RPAREN, UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
} Token;

char line[100];
FILE *file;
Ident idArray[50];
Token current_token;
int error = 0;
int ID_count = 0;
int CONST_count = 0;
int OP_count = 0;
int id_count = 0;
int pos = 0;
char current_char;

void printResultByLine(char *line, int ID, int CON, int OP);
void printIdent(int num_ident);
void parse(char *line);
void parse_V(char *line);
void printOK(void);
void printOPWarning(int code);
void printIDError(char *name);
void printToken(char *token);

void program(char *line);
void statements(char *line);
void statement(char *line);
int expression(char *line);
int term_tail(char *line, int val);
int term(char *line);
int factor(char *line);
int factor_tail(char *line, int val);

void advance(char *line);
int lookupIdentValue(char *name);
void setIdentValue(char *name, int value);
void lexical(char *line);
Token identifier(char *line);
Token integerLiteral(char *line);
Token createToken(TokenType type, const char *lexeme);

int main(int argc, char **argv) {
    /*
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-v] <filepath>\n", argv[0]);
        return 1;
    }

    int verbose = 0;
    char *filepath = NULL;

    if (strcmp("-v", argv[1]) == 0) {
        verbose = 1;
        if (argc < 3) {
            fprintf(stderr, "Error: No file specified.\n");
            return 1;
        }
        filepath = argv[2];
    } else {
        filepath = argv[1];
    }

    file = fopen(filepath, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filepath);
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (verbose) {
            parse_V(line);
        } else {
            parse(line);
        }
    }

    fclose(file);
    return 0;
    */
    
    char line[100] = "operator2 := operator1 + + 4;";
    parse(line);
}

void parse(char *line) {
    pos = 0;
    error = 0;
    ID_count = CONST_count = OP_count = 0;
    
    // Get the first token
    advance(line);
    lexical(line);
    
    // Check grammar
    program(line);
}

void parse_V(char *line) {
    pos = 0;
    error = 0;
    advance(line);
    lexical(line);

    while (current_token.type != END_OF_FILE) {
        printToken(current_token.lexeme);
        lexical(line);
    }
}

void printOK(void) {
    printf("(OK)\n");
}

void printOPWarning(int code) {
    switch (code) {
        case 1:
            printf("(Warning) \"Eliminating duplicate operator (+)\"\n");
            break;
        case 2:
            printf("(Warning) \"Eliminating duplicate operator (-)\"\n");
            break;
        case 3:
            printf("(Warning) \"Eliminating duplicate operator (*)\"\n");
            break;
        case 4:
            printf("(Warning) \"Eliminating duplicate operator (/)\"\n");
            break;
        case 5:
            printf("(Warning) \"Substituting assignment operator (: =)\"\n");
            break;
        default:
            printf("(Warning) \"Unknown issue\"\n");
    }
}

void printIDError(char *name) {
    printf("(Error) \"referring to undefined identifiers(%s)\"\n", name);
}

void printResultByLine(char *line, int ID, int CON, int OP) {
    printf("%s\n", line);
    printf("ID: %d; CONST: %d; OP: %d;\n", ID, CON, OP);
}

void printToken(char *token) {
    printf("%s\n", token);
}

void program(char *line) {
    statements(line);
}

void statements(char *line) {
    statement(line);
    if (current_token.type == SEMICOLON) {
        lexical(line);
        statements(line);
    }
    printResultByLine(line, ID_count, CONST_count, OP_count);
}

void statement(char *line) {
    char ident_name[50];

    if (current_token.type == IDENT) {
        strcpy(ident_name, current_token.lexeme);
        ID_count++;

        lexical(line);

        if (current_token.type == ASSIGN) {
            lexical(line);
            setIdentValue(ident_name, expression(line));
        } else {
            error = 1;
        }
    } else {
        printIDError(current_token.lexeme);
    }
}

int expression(char *line) {
    return term_tail(line, term(line));
}

int term_tail(char *line, int inherited_value) {
    if (current_token.type == ADD) {
        OP_count++;
        lexical(line);

        int term_value = term(line);
        int result = (current_token.type == ADD) ? inherited_value + term_value : inherited_value - term_value;

        return term_tail(line, result);
    }
    return inherited_value;
}

int term(char *line) {
    return factor_tail(line, factor(line));
}

int factor_tail(char *line, int inherited_value) {
    if (current_token.type == MULT) {
        OP_count++;
        lexical(line);

        int factor_value = factor(line);
        int result = (current_token.type == MULT) ? inherited_value * factor_value : inherited_value / factor_value;

        return factor_tail(line, result);
    }
    return inherited_value;
}

int factor(char *line) {
    int value = 0;

    if (current_token.type == LPAREN) {
        lexical(line);
        value = expression(line);
        if (current_token.type == RPAREN) {
            lexical(line);
        } else {
            printIDError(current_token.lexeme);
        }
    } else if (current_token.type == IDENT) {
        value = lookupIdentValue(current_token.lexeme);
        ID_count++;
        lexical(line);
    } else if (current_token.type == INT_LIT) {
        value = atoi(current_token.lexeme);
        CONST_count++;
        lexical(line);
    } else {
        printIDError(current_token.lexeme);
    }

    return value;
}

void getNonBlank(char *line) {
    while (isspace(current_char)) advance(line);  
}

void lexical(char *line) {
    
    getNonBlank(line);

    if (current_char == '\0') {
        current_token = createToken(END_OF_FILE, "EOF");
        return;
    }

    if (isalpha(current_char)) {
        current_token = identifier(line);
        return;
    }

    if (isdigit(current_char)) {
        current_token = integerLiteral(line);
        return;
    }

    if (current_char == ':') {
        getNonBlank(line);
        advance(line);
        if (current_char == '=') {
            advance(line);
            current_token = createToken(ASSIGN, ":=");
        } else {
            current_token = createToken(UNKNOWN, ":");
        }
        return;
    }

    if (current_char == '=') {
        printOPWarning(5);
        current_token = createToken(ASSIGN, ":=");
        advance(line);
        return;
    }
    
    char lexeme[2] = {current_char, '\0'};
    
    switch (current_char) {
        case '+':
            advance(line);
            if (current_char == '+') {
                printOPWarning(1);
                advance(line);
            }
            current_token = createToken(ADD, lexeme);
            break;
            
        case '-':
            advance(line);
            if (current_char == '-') {
                printOPWarning(2);
                advance(line);
            }
            current_token = createToken(ADD, lexeme);
            break;
            
        case '*':
            advance(line);
            if (current_char == '*') {
                printOPWarning(3);
                advance(line);
            }
            current_token = createToken(MULT, lexeme);
            break;
            
        case '/':
            advance(line);
            if (current_char == '/') {
                printOPWarning(4);
                advance(line);
            }
            current_token = createToken(MULT, lexeme);
            break;
            
        case ';':
            char lexeme[2] = {current_char, '\0'};
            current_token = createToken(SEMICOLON, lexeme);
            advance(line);
            break;
            
        case '(':
        case ')':
            current_token = createToken((current_char == '(') ? LPAREN : RPAREN, lexeme);
            advance(line);
            break;

        default:
            current_token = createToken(UNKNOWN, lexeme);
            advance(line);
            break;
    }
}

void advance(char *line) {
    if (line[pos] != '\0') {
        current_char = line[pos++];
    } else {
        current_char = '\0';
    }
}

Token createToken(TokenType type, const char *lexeme) {
    Token token;
    token.type = type;
    strncpy(token.lexeme, lexeme, sizeof(token.lexeme) - 1);
    token.lexeme[sizeof(token.lexeme) - 1] = '\0';
    return token;
}

Token identifier(char *line) {
    char lexeme[100];
    int length = 0;

    while (isalnum(current_char)) {
        if (length < sizeof(lexeme) - 1) {
            lexeme[length++] = current_char;
        }
        advance(line);
    }
    lexeme[length] = '\0';
    return createToken(IDENT, lexeme);
}

Token integerLiteral(char *line) {
    char lexeme[100];
    int length = 0;

    while (isdigit(current_char)) {
        if (length < sizeof(lexeme) - 1) {
            lexeme[length++] = current_char;
        }
        advance(line);
    }
    lexeme[length] = '\0';
    return createToken(INT_LIT, lexeme);
}

int lookupIdentValue(char *name) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(idArray[i].name, name) == 0) {
            return idArray[i].value;
        }
    }
    printIDError(name);
    return 0;
}

void setIdentValue(char *name, int value) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(idArray[i].name, name) == 0) {
            idArray[i].value = value;
            return;
        }
    }
    strcpy(idArray[id_count].name, name);
    idArray[id_count].value = value;
    id_count++;
}
