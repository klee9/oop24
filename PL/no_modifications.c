/**
 * File Name: You should name your source file as your student ID (e.g., 20204870.c)
 * Author: Ix Lab TA Dohwa Kim
 * Year: 2024
 *
 * Description:
 * This file is the skeleton code for the PL parser project.
 * Basic code is provided in the main function and some other functions.
 * Students should not modify the main function or the provided functions.
 * Students should add additional functions and variables to complete the parser.
 * It is IMPORTANT to use the provided functions when you need to print information.
 * Detailed descriptions are given below, so please read carefully.
 * Our automated scoring system may give you 0 points if you modify the provided code, and no exceptions will be accepted.
 *
 * Instructions:
 * - Complete the parser project according to the instructions provided in the word file from the class.
 * - You should NOT modify the pre-made code.
 * - You may add standard library headers like string.h or stdlib.h, but do not use custom headers from the internet.
 *   (Standard libraries should be sufficient for this project.)
 * - You have to submit cmake file too, so you have to modify it little bit like in instruction file. Please just follow instruction
 *
 * Note:
 * If you have any questions about this project, please email me at: kimdohwa2@cau.ac.kr
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * @brief Represents an identifier with a name and value.
 *
 * This structure is used to store information about identifiers
 * in the PL parser project. Each identifier has a name and an associated value.
 * The value can be a string or a numeric value depending on the use case.
 * You can add more fields if you want but DO NOT MODIFY predefined fields
 */
typedef struct {
    char name[50];
    char value[100];
} Ident;

typedef enum {
    ASSIGN_OP, ADD_OP, MULT_OP, SEMICOLON, IDENT, CONSTANT, END_OF_FILE, LPAREN, RPAREN, UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char token_string[50];
} Token;

char line[100], error_ident[50];
FILE *file;
Ident idArray[50];
Token tokenArray[200];

void printResultByLine(char *line, int ID, int CON, int OP);
void printIdent(int num_ident);
void parse(char *line);
void parse_V(char *line);

void printOK(void);
void printToken(char *token);
void printOPWarning(int code);
void printIDError(char *name);

int isInt(const char *str);
void statements(char *line);
void statement(char *line);
char *expression(char *line, char *result);
char *term_tail(char *line, char *inherited_value, char *result);
char *term(char *line, char *result);
char *factor(char *line, char *result);
char *factor_tail(char *line, char *inherited_value, char *result);

void getChar(char *line);
void addChar(char *line);
void getNonBlank(char *line);
void lexical(char *line);
char *lookupIdentValue(char *name);
void setIdentValue(char *name, char *value);

int add_pos;
int get_pos;
int push_pos;

int id_cnt;
int const_cnt;
int op_cnt;

int id_tot = 0;
int id_error;
int id_warning;

char current_char;
char token_string[100];

Token next_token;

/**
 *
 * @brief Main function for processing a file.
 *
 * This program expects a file path as a command-line argument.
 * You should provide the absolute path to the file as the first argument
 * when running this program. An absolute path is the complete path from the
 * root directory of your file system to the file.
 *
 * Examples:
 * - On Windows:
 *   student_program.exe C:/Users/YourName/Desktop/test_input.txt
 *   or
 *   student_program.exe C:\\Users\\YourName\\Desktop\\test_input.txt
 *
 * - On Linux/MacOS:
 *   ./student_program /home/yourname/Desktop/test_input.txt
 *
 * Make sure that the file exists at the specified path, and that you have the
 * correct permissions to read the file.
 *
 *
 *
 */
 
 /*
 int main(int argc, char **argv) {
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

    // Gets input line for the first time
    fgets(line, sizeof(line), file);

    // Depending on the verbose flag, call the appropriate function
    if (verbose) {
        parse_V();
    } else {
        parse();
    }

    fclose(file);
    return 0;
}
 
 */
 
int main(int argc, char **argv) {
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
    
    // Gets input line for the first time
    fgets(line, sizeof(line), file);

    // Depending on the verbose flag, call the appropriate function
    if (verbose) {
        parse_V(file);
    } else {
        parse(file);
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief This function is the starting point of this project with no option -v (print option a).
 *
 * You can freely modify this code or add more functions.
 * However, you SHOULD use the print functions below when you need to print some lines on screen,
 * or you might risk receiving 0 points even if the program works perfectly.
 */
void parse(char *line) {
    while (fgets(line, sizeof(line), file)) {
        int i, s;
        char buf[100];
        for (i = 0, s = 0; line[i] >= 32; i++, s++) {
            if ((buf[s] = line[i]) == ';') {
                buf[s + 1] = '\0';
                s = -1;
            }
        }
        if (s > 1) {
            buf[s] = '\0';
        }
        
        id_cnt = const_cnt = op_cnt = 0;
        get_pos = push_pos = 0;
        id_error = id_warning = 0;
    
        getChar(buf);
        while(current_char != '\0') {
            lexical(buf);
            statements(buf);
        }
    
        // Print line
        char fline[100] = "";
        for (int i = 0; i < push_pos; i++) {
            const char *current = tokenArray[i].token_string;
            strcat(fline, current);
            if (i < push_pos - 1) {
                const char *next = tokenArray[i + 1].token_string;
                if (!strcmp(current, "(") || !strcmp(next, ")") || !strcmp(next, ";")) continue;
                strcat(fline, " ");
            }
        }
    
        // Print results
        printResultByLine(fline, id_cnt, const_cnt, op_cnt);
        if (!id_error && !id_warning) printOK();
        else if (id_warning > 0 && !id_error) printOPWarning(id_warning);
        else printIDError(error_ident);
    
        // Print the final result
        printIdent(id_tot);
    }
}

/**
 * @brief This function is the starting point of this project with option -v (print option b).
 *
 * You can freely modify this code or add more functions.
 * However, you SHOULD use the print functions below when you need to print some lines on screen,
 * or you might risk receiving 0 points even if the program works perfectly.
 */
void parse_V(char *line) {
    while (fgets(line, sizeof(line), file)) {
        id_cnt = const_cnt = op_cnt = 0;
        get_pos = add_pos = push_pos = 0;
        id_error = id_warning = 0;
    
        getChar(line);
        while(current_char != '\0') {
            add_pos = 0;
            lexical(line);
            if (next_token.type != UNKNOWN) {
                switch (next_token.type) {
                    case ASSIGN_OP:
                        printToken("assignment_op"); break;
                    case ADD_OP:
                        printToken("add_operator"); break;
                    case MULT_OP:
                        printToken("mult_operator"); break;
                    case SEMICOLON:
                        printToken("semi_colon"); break;
                    case IDENT:
                        printToken("ident"); break;
                    case CONSTANT:
                        printToken("const"); break;
                    case LPAREN:
                        printToken("left_paren"); break;
                    case RPAREN:
                        printToken("right_paren"); break;
                }
            }
        }    
    }
}

/**
 * @brief Function that prints the line and the number of IDs, constants, and operands.
 *
 * YOU SHOULD NOT MODIFY THIS CODE,
 * or you might risk receiving 0 points.
 *
 * @param line The line that the program has read.
 * @param ID The number of IDs.
 * @param CON The number of constants.
 * @param OP The number of operands.
 * @param 0 to print (OK)
 */
void printResultByLine(char *line, int ID, int CON, int OP) {
    printf("%s\n", line);
    printf("ID: %d; CONST: %d; OP: %d;\n", ID, CON, OP);
}

/**
 * @brief Function that prints the warning message about Operands
 *
 * YOU SHOULD NOT MODIFY THIS CODE,
 * or you might risk receiving 0 points.
 *
 * @param code The warning code of operand
 * 1 : Multiple (+) Operation | 2 : Multiple (-) Operation
 * 3 : Multiple (*) Operation | 4 : Multiple (/) Operation
 * 5 : Wrong Assignment Operation (:=)
 */
void printOPWarning(int code){
    switch(code){
    
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
            printf("(Warning) \"Substituting assignment operator (:=)\"\n");
            break;
        }
}
/**
 * @brief Function that prints OK sign
 *
 */
void printOK(){
    printf("(OK)\n");
}

/**
 * @brief Function that prints the line and the number of IDs, constants, and operands.
 *
 * YOU SHOULD NOT MODIFY THIS CODE,
 * or you might risk receiving 0 points.
 *
 * @param name The name of Identifier that didn't referenced before
 */
void printIDError(char *name){
    printf("(Error) \"referring to undefined identifiers(%s)\"\n",name);
}

/**
 * @brief Function that prints the result of identifiers.
 *
 * YOU SHOULD NOT MODIFY THIS CODE,
 * or you might risk receiving 0 points.
 *
 * Save identifiers in predefined array
 * @param num_ident The number of identifiers.
 *
 * Result ==> operand1: 2; total: 2;
 */
void printIdent(int num_ident) {
    int i;
    printf("Result ==>");
    for (i = 0; i < num_ident; i++) {
        printf(" %s: %s;", idArray[i].name, idArray[i].value);
    }
}
/**
 * @brief Funtion that prints token by line
 *
 * YOU SHOULD NOT MODIFY THIS CODE,
 * or you might risk receiving 0 points
 *
 * Print token by line
 * This function is used for verbose mode(-v)
 * @param token Token that you want to print
 *
 */
void printToken(char *token){
    printf("%s\n", token);
}

// Subprograms
// <statements> → <statement> | <statement><semi_colon><statements>
void statements(char *line) {
    statement(line);
    if (next_token.type == SEMICOLON) {
        lexical(line);
        statements(line);
    }
}

// <statement> → <ident><assignment_op><expression>
void statement(char *line) {
    char ident_name[50];

    if (next_token.type == IDENT) {
        strcpy(ident_name, next_token.token_string);

        lexical(line);

        if (next_token.type == ASSIGN_OP) {
            char expr_value[50] = "";
            lexical(line);
            expression(line, expr_value);
            setIdentValue(ident_name, expr_value);
        }
    }
}

// <expression> → <term><term_tail>
char *expression(char *line, char *result) {
    char term_result[50] = "";
    term(line, term_result);
    term_tail(line, term_result, result);
    return result;
}

// <term_tail> → <add_op><term><term_tail> | ε
char *term_tail(char *line, char *inherited_value, char *result) {
    if (next_token.type == ADD_OP) {
        char term_result[50] = "";
        char operator = next_token.token_string[0];
        lexical(line);

        term(line, term_result);
        if (strcmp(inherited_value, "Unknown") && strcmp(term_result, "Unknown")) {
            int answer = (operator == '+') ? atoi(inherited_value) + atoi(term_result)
                                           : atoi(inherited_value) - atoi(term_result);
            sprintf(result, "%d", answer);
        } else {
            strcpy(result, "Unknown");
        }
        return term_tail(line, result, result);  // Continue parsing additional terms
    }
    if (result != inherited_value)
        strcpy(result, inherited_value);
    return result;
}

// <term> → <factor> <factor_tail>
char *term(char *line, char *result) {
    char factor_result[50] = "";
    factor(line, factor_result);  // Parse the first factor
    factor_tail(line, factor_result, result);  // Handle multiplication/division
    return result;
}

// <factor_tail> → <mult_op><factor><factor_tail> | ε
char *factor_tail(char *line, char *inherited_value, char *result) {
    if (next_token.type == MULT_OP) {
        char factor_result[50] = "";
        char operator = next_token.token_string[0];
        lexical(line);

        factor(line, factor_result);  // Parse the next factor
        if (strcmp(inherited_value, "Unknown") && strcmp(factor_result, "Unknown")) {
            int answer = (operator == '*') ? atoi(inherited_value) * atoi(factor_result)
                                           : atoi(inherited_value) / atoi(factor_result);
            sprintf(result, "%d", answer);
        } else {
            strcpy(result, "Unknown");
        }
        return factor_tail(line, result, result);  // Continue parsing additional factors
    }
    if (result != inherited_value)
        strcpy(result, inherited_value);
    return result;
}

// <factor> → <left_paren><expression><right_paren> | <ident> | <const>
char *factor(char *line, char *result) {
    if (next_token.type == LPAREN) {
        lexical(line);  // Consume '('
        expression(line, result);  // Parse the expression inside parentheses
        if (next_token.type == RPAREN) {
            lexical(line);  // Consume ')'
        }
    } else if (next_token.type == IDENT) {
        char *found_val = lookupIdentValue(next_token.token_string);
        if (!found_val) {
            strcpy(result, "Unknown");
        } else {
            strcpy(result, found_val);
        }
        lexical(line);
    } else if (next_token.type == CONSTANT) {
        strcpy(result, next_token.token_string);
        lexical(line);
    }
    return result;
}


char *lookupIdentValue(char *name) {
    for (int i = 0; i < id_tot; i++) {
        if (!strcmp(idArray[i].name, name)) {
            return idArray[i].value;
        }
    }
    setIdentValue(name, "Unknown");
    id_error = 1;
    strcpy(error_ident, name);
    return NULL;
}

void setIdentValue(char *name, char *value) {
    for (int i = 0; i < id_tot; i++) {
        if (!strcmp(idArray[i].name, name)) {
            strcpy(idArray[i].value, value);
            return;
        }
    }
    strcpy(idArray[id_tot].name, name);
    strcpy(idArray[id_tot++].value, value);
}

// LEXER
// Set current_char to the next non blank char
void getChar(char *line) {
    if (line[get_pos] != '\0') {
        current_char = line[get_pos++];
    } else {
        current_char = '\0';
    }
}

// Concatenate current_char to token_string
void addChar(char *line) {
    if (add_pos < 99) {
        token_string[add_pos++] = current_char;
        token_string[add_pos] = '\0';
    }
}

// Returns the next non blank character
void getNonBlank(char *line) {
    while (((int)current_char < 33 && current_char) || isspace(current_char)) {
        getChar(line);
    }
}

// Creates a new token
Token new_token(TokenType type, char *token_string) {
    Token token;
    token.type = type;
    strncpy(token.token_string, token_string, sizeof(token.token_string) - 1);
    token.token_string[sizeof(token.token_string) - 1] = '\0';
    return token;
}

// Lexical analyzer
void lexical(char *line) {
    add_pos = 0;
    getNonBlank(line);
    
    // CONSTANT
    if (isdigit(current_char)) {
        while (isdigit(current_char)) {
            addChar(line);
            getChar(line);
        }
        const_cnt++;
        next_token = new_token(CONSTANT, token_string);
        tokenArray[push_pos++] = next_token;
    }
    
    // IDENT
    else if (isalpha(current_char)) {
        while (isalnum(current_char)) {
            addChar(line);
            getChar(line);
        }
        id_cnt++;
        next_token = new_token(IDENT, token_string);
        tokenArray[push_pos++] = next_token;
    }
    
    // LPAREN
    else if (current_char == '(') {
        getChar(line);
        next_token = new_token(LPAREN, "(");
        tokenArray[push_pos++] = next_token;
    }
    
    // RPAREN
    else if (current_char == ')') {
        getChar(line);
        next_token = new_token(RPAREN, ")");
        tokenArray[push_pos++] = next_token;
    }
    
    // SEMICOLON
    else if (current_char == ';') {
        getChar(line);
        next_token = new_token(SEMICOLON, ";");
        tokenArray[push_pos++] = next_token;
    }
    
    // ADD_OP
    else if (current_char == '+') {
        getChar(line);
        getNonBlank(line);
        if (current_char == '+') {
            id_warning = 1;
            getChar(line);
        }
        op_cnt++;
        next_token = new_token(ADD_OP, "+");
        tokenArray[push_pos++] = next_token;
    }
    
    else if (current_char == '-') {
        getChar(line);
        getNonBlank(line);
        if (current_char == '-') {
            id_warning = 2;
            getChar(line);
        }
        op_cnt++;
        next_token = new_token(ADD_OP, "-");
        tokenArray[push_pos++] = next_token;
    }
    
    // MULT_OP
    else if (current_char == '*') {
        getChar(line);
        getNonBlank(line);
        if (current_char == '*') {
            id_warning = 3;
            getChar(line);
        }
        op_cnt++;
        next_token = new_token(MULT_OP, "*");
        tokenArray[push_pos++] = next_token;
    }
    
    else if (current_char == '/') {
        getChar(line);
        getNonBlank(line);
        if (current_char == '/') {
            id_warning = 4;
            getChar(line);
        }
        op_cnt++;
        next_token = new_token(ADD_OP, "/");
        tokenArray[push_pos++] = next_token;
    }
    
    // ASSIGN_OP
    else if (current_char == '=' || current_char == ':') {
        if (current_char == '=') {
            getChar(line);
            id_warning = 5;
            next_token = new_token(ASSIGN_OP, ":=");
            tokenArray[push_pos++] = next_token;
        }
        else if (current_char == ':') {
            getChar(line);
            if (current_char == '=') {
                getChar(line);
                next_token = new_token(ASSIGN_OP, ":=");
                tokenArray[push_pos++] = next_token;
            }
        }
    }
    
    // UNKNOWN
    else {
        while (current_char > 32 && current_char != ' ') {
            addChar(line);
            getChar(line);
        }
        next_token = new_token(UNKNOWN, token_string);
    }
}
