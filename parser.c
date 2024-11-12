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

// Character classes
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token codes
#define IDENTIFIER 11
#define CONSTANT 12
#define ASSIGN_OP 13
#define ADD_OP 14
#define MULT_OP 15
#define LPAREN 16
#define RPAREN 17
#define SEMICOLON 18

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
}Ident;

char line[100];
FILE *file;
Ident idArray[50];

int id_cnt = 0;
int const_cnt = 0;
int op_cnt = 0;

// Lexer variables
int char_class;
char lexeme[100];
char token_string[100];
char next_char = ' ';
int lex_len;
int token;
int next_token;

void printResultByLine(char *line, int ID, int CON, int OP);
void printIdent(int num_ident);
void parse(char *line);
void parse_V(char *line);
void printOPWarning(int code);
void printIDError(char *name);

// Lexer functions
void addChar();
void getChar();
int lookup(char *ch);
int lexical();

// Subprograms for recursive descent parsing
void program();
void statements();
void statement();
void expression();
void term_tail();
void term();
void factor_tail();
void factor();

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
    while(fgets(line, sizeof(line), file) != NULL) {
        // Depending on the verbose flag, call the appropriate function
        if (verbose) {
            parse_V(line);
        } else {
            parse(line);
        }
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
    id_cnt = 0;
    const_cnt = 0;
    op_cnt = 0;
    
    lexical();
    statements();
    printResultByLine(line, id_cnt, const_cnt, op_cnt);
}

/**
 * @brief This function is the starting point of this project with option -v (print option b).
 *
 * You can freely modify this code or add more functions.
 * However, you SHOULD use the print functions below when you need to print some lines on screen,
 * or you might risk receiving 0 points even if the program works perfectly.
 */
void parse_V(char *line) {
    
}

// Subprograms
// <statements> → <statement> | <statement><semi_colon><statements>
void statements() {
    printf("Enter statements()\n");
    statement();
    lexical();
    if (next_token == SEMICOLON) {
        lexical();
        statements();
    } else if (next_token != EOF) {
        //printIDError("Unexpected token-- expected semicolon");  // Error handling for missing semicolon
    }
}

// <statement> → <ident><assignment_op><expression>
void statement() {
    printf("Enter statement()\n");
    if (next_token == IDENTIFIER) {
        // Store identifier in symbol table
        strcpy(idArray[id_cnt].name, lexeme);
        
        lexical();
        if (next_token == ASSIGN_OP) {
            lexical();
            expression();
            strcpy(idArray[id_cnt++].value, lexeme);  // Store expression result as identifier value
        } else {
            //printOPWarning(5);
        }
    } else {
        //printIDError("Undefined identifier in statement");  // Error handling for missing identifier
    }
}

// <expression> → <term><term_tail>
void expression() {
    printf("Enter expression()\n");
    term();
    term_tail();
}

// <term_tail> → <add_op><term><term_tail> | ε
void term_tail() {
    printf("Enter term_tail()\n");
    lexical();
    if (next_token == ADD_OP) {
        lexical();
        term();
        term_tail();
    }
}

// <term> → <factor> <factor_tail>
void term() {
    printf("Enter term()\n");
    factor();
    factor_tail();
}

// <factor_tail> → <mult_op><factor><factor_tail> | ε
void factor_tail() {
    printf("Enter factor_tail()\n");
    lexical();
    if (next_token == MULT_OP) {
        lexical();
        factor();
        factor_tail();
    }
    // Do nothing if no mult_op(ε production)
}

// <factor> → <left_paren><expression><right_paren> | <ident> | <const>
void factor() {
    printf("Enter factor()\n");
    lexical();
    if (next_token == LPAREN) {
        expression();
        lexical();
        if (next_token != RPAREN) {
            // maybe handle this as warning
            //printIDError("Missing closing parenthesis");
        }
    } else if (next_token != IDENTIFIER && next_token != INT_LIT) {
        // If the token is neither an identifier nor a constant, handle it as an error
        //printIDError("Unexpected token in factor");
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

// Below functions are for lexer

/**
 * @brief Function to lookup operators and parentheses and return the token
 *
 * @param s String to be classified
 */
int lookup (char *s) {
    if(!strcmp(s, ":=")) {
        next_token = ASSIGN_OP;
    }
    else if(!strcmp(s, "(")) {
        next_token = LPAREN;
    }
    else if(!strcmp(s, "+") || !strcmp(s, "-")) {
        next_token = ADD_OP;
    }
    else if(!strcmp(s, "*") || !strcmp(s, "/")) {
        next_token = MULT_OP;
    }
    else if(!strcmp(s, ";")) {
        next_token = SEMICOLON;
    }
    else {
        next_token = EOF;
    }
    return next_token;
}

/**
 * @brief Function to add next_char to lexeme
 */
void addChar() {
    if (lex_len <= 98) {
        lexeme[lex_len++] = next_char;
        lexeme[lex_len] = '\0'
    }
}

/**
 * @brief Function to get the next character of input and determine its character class
 */
void getChar() {
    if ((next_char = getc(file)) != EOF) {
        if (isalpha(next_char))
            char_class = LETTER;
        else if (isdigit(next_char))
            char_class = DIGIT;
        else
            char_class = UNKNOWN;
    } else {
        char_class = EOF;
    }
}

/**
 * @brief Function for lexer for arithmetic expressions
 */
int lexical() {
    lex_len = 0;
    
    // Read until first non blank char
    while (isspace(next_char)) { getChar(); }

    switch (char_class) {
        // Parse identifiers
        // &@194& := 192
        case LETTER:
            addChar();
            getChar();
            while (char_class == LETTER || char_class == DIGIT) {
                addChar();
                getChar();
            }
            next_token = IDENTIFIER;
            id_cnt++;
            break;
    
        // Parse integer literals (constants)
        case DIGIT:
            addChar();
            getChar();
            while (char_class == DIGIT) {
                addChar();
                getChar();
            }
            next_token = CONSTANT;
            const_cnt++;
            break;
    
        // Parentheses and operators
        case UNKNOWN:
            next_token = lookup(lexeme);
            getChar();
            if (next_token == ADD_OP || next_token == MULT_OP) {
                op_cnt++;
            }
            break;
    
        // EOF
        case EOF:
            next_token = EOF;
            strcpy(lexeme, "EOF");
            break;
    }
    printf("Got: %s\n", lexeme);
    return next_token;
}
