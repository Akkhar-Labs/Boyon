/* 
 * lexer.c - সার্বভৌম 'বয়ন' অ্যাসেম্বলার (Standalone Bengali Number সাপোর্ট সহ)
 * Architect: Rahat Hasan | Akkhar Labs
 */

#include "lexer.h"
#include <string.h>
#include <ctype.h>

// বাংলা সংখ্যা থেকে ইংরেজি সংখ্যায় রূপান্তরের ফাংশন
static long bengali_str_to_int(const char *str, int base) {
    long result = 0;
    int i = 0;
    
    while (str[i] != '\0') {
        unsigned char c1 = str[i];
        unsigned char c2 = (unsigned char)str[i+1];
        unsigned char c3 = (unsigned char)str[i+2];

        // ইউটিএফ-৮ এ বাংলা সংখ্যার রেঞ্জ চেক (০-৯)
        if (c1 == 0xE0 && c2 == 0xA7 && (c3 >= 0xA6 && c3 <= 0xAF)) {
            result = result * base + (c3 - 0xA6);
            i += 3;
        } 
        // হেক্সাডেসিমাল 'এ' এবং 'ক' ম্যাপিং
        else if (c1 == 0xE0 && c2 == 0xA6 && c3 == 0x8F) { // 'এ'
            result = result * base + 14; 
            i += 3;
        }
        else if (c1 == 0xE0 && c2 == 0xA6 && c3 == 0x95) { // 'ক' (A)
            result = result * base + 10;
            i += 3;
        }
        // সাধারণ ইংরেজি সংখ্যার জন্য
        else if (isdigit(c1)) {
            result = result * base + (c1 - '0');
            i++;
        }
        else { i++; }
    }
    return result;
}

void init_lexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->cursor = 0;
    lexer->line = 1;
}

static void skip_whitespace(Lexer *lexer) {
    while (lexer->source[lexer->cursor] == ' ' || 
           lexer->source[lexer->cursor] == '\t' || 
           lexer->source[lexer->cursor] == '\r') {
        lexer->cursor++;
    }
}

static void read_word(Lexer *lexer, char *buffer) {
    int i = 0;
    while (lexer->source[lexer->cursor] != '\0' && 
           !isspace(lexer->source[lexer->cursor]) && 
           lexer->source[lexer->cursor] != ',') {
        buffer[i++] = lexer->source[lexer->cursor++];
    }
    buffer[i] = '\0';
}

Token get_next_token(Lexer *lexer) {
    skip_whitespace(lexer);

    Token token;
    token.line = lexer->line;
    token.type = TOKEN_ERROR;
    token.value = 0;
    strcpy(token.text, "");

    char current = lexer->source[lexer->cursor];

    if (current == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    if (current == '\n') {
        lexer->cursor++;
        lexer->line++;
        token.type = TOKEN_NEWLINE;
        strcpy(token.text, "\\n");
        return token;
    }

    if (current == ',') {
        lexer->cursor++;
        token.type = TOKEN_COMMA;
        strcpy(token.text, ",");
        return token;
    }

    char buffer[256];
    read_word(lexer, buffer);
    strcpy(token.text, buffer);

    /* --- ওকোড শনাক্তকরণ --- */
    if (strcmp(buffer, "শূণ্য") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_NOP; }
    else if (strcmp(buffer, "আদান") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_LOAD; }
    else if (strcmp(buffer, "প্রদান") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_STORE; }
    else if (strcmp(buffer, "স্থানান্তর") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_MOV; }
    else if (strcmp(buffer, "সংযোজন") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_ADD; }
    else if (strcmp(buffer, "বিয়োজন") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_SUB; }
    else if (strcmp(buffer, "গুনন") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_MUL; }
    else if (strcmp(buffer, "বিভাজন") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_DIV; }
    else if (strcmp(buffer, "উভয়") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_AND; }
    else if (strcmp(buffer, "অথবা") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_OR; }
    else if (strcmp(buffer, "বিচিত্র") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_XOR; }
    else if (strcmp(buffer, "বিপরীত") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_NOT; }
    else if (strcmp(buffer, "প্রস্থান") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_JMP; }
    else if (strcmp(buffer, "সাম্য-প্রস্থান") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_JZ; }
    else if (strcmp(buffer, "প্রদর্শন") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_OUT; }
    else if (strcmp(buffer, "থামো") == 0) { token.type = TOKEN_INSTRUCTION; token.value = OP_HALT; }

    /* --- সাময়িক শনাক্তকরণ --- */
    else if (strstr(buffer, "সাময়িক-") != NULL) {
        token.type = TOKEN_REGISTER;
        char *dash = strchr(buffer, '-');
        token.value = (int)bengali_str_to_int(dash + 1, 10); 
    }
    else if (strcmp(buffer, "ন-নি") == 0) { token.type = TOKEN_REGISTER; token.value = REG_PC; }
    else if (strcmp(buffer, "শ-নি") == 0) { token.type = TOKEN_REGISTER; token.value = REG_SP; }
    else if (strcmp(buffer, "অ-সা") == 0) { token.type = TOKEN_REGISTER; token.value = REG_FLAGS; }

    /* --- সংখ্যা শনাক্তকরণ --- */
    else if (strstr(buffer, "০ষ") != NULL) {
        token.type = TOKEN_NUMBER;
        char *prefix = strstr(buffer, "০ষ");
        token.value = (int)bengali_str_to_int(prefix + 6, 16);
    }
    else if (strstr(buffer, "০ব") != NULL) {
        token.type = TOKEN_NUMBER;
        char *prefix = strstr(buffer, "০ব");
        token.value = (int)bengali_str_to_int(prefix + 6, 2);
    }
    // নতুন লজিক: প্রিফিক্স ছাড়া সরাসরি বাংলা সংখ্যা শনাক্তকরণ
    else if ((unsigned char)buffer[0] == 0xE0 && (unsigned char)buffer[1] == 0xA7) {
        token.type = TOKEN_NUMBER;
        token.value = (int)bengali_str_to_int(buffer, 10);
    }
    // ইংরেজি সংখ্যার জন্য (যদি থাকে)
    else if (isdigit(buffer[0])) {
        token.type = TOKEN_NUMBER;
        token.value = (int)bengali_str_to_int(buffer, 10);
    }

    return token;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_INSTRUCTION: return "নির্দেশ";
        case TOKEN_REGISTER:    return "সাময়িক";
        case TOKEN_NUMBER:      return "সংখ্যা";
        case TOKEN_LABEL:       return "লেবেল";
        case TOKEN_COMMA:       return "কমা";
        case TOKEN_NEWLINE:     return "নতুন লাইন";
        case TOKEN_EOF:         return "সমাপ্ত";
        default:                return "ভুল";
    }
}