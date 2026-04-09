/* 
 * lexer.h - Lexical Analyzer for Boyon Assembler
 * Defines token types and lexer structures.
 * Architect: Rahat Hasan | Akkhar Labs
 */

#ifndef LEXER_H
#define LEXER_H

#include "boyon.h"
#include "arch/nirdeshika16.h"

/* --- Token Types (টোকেনের ধরন) --- */
typedef enum {
    TOKEN_INSTRUCTION, // ওকোড (e.g., আদান, প্রদান)
    TOKEN_REGISTER,    // সাময়িক (e.g., সাময়িক-১)
    TOKEN_NUMBER,      // সংখ্যা (Immediate/Address)
    TOKEN_LABEL,       // লেবেল (Jump targets)
    TOKEN_COMMA,       // কমা ( , )
    TOKEN_NEWLINE,     // নতুন লাইন (Newline)
    TOKEN_EOF,         // সমাপ্ত (End of File)
    TOKEN_ERROR        // ভুল (Invalid token)
} TokenType;

/* --- Token Structure (টোকেন কাঠামো) --- */
typedef struct {
    TokenType type;    // টোকেনের ধরন
    int value;         // টোকেনের গাণিতিক মান
    char text[256];    // মূল বাংলা টেক্সট (Original source text)
    int line;          // লাইন নম্বর
} Token;

/* --- Lexer State (লেক্সার স্টেট) --- */
typedef struct {
    const char *source; // সোর্স কোড স্ট্রিং
    int cursor;         // বর্তমান পজিশন
    int line;           // বর্তমান লাইন
} Lexer;

/* --- Lexer Prototypes --- */

// লেক্সার শুরু করা
void init_lexer(Lexer *lexer, const char *source);

// সোর্স কোড থেকে পরবর্তী টোকেন সংগ্রহ করা
Token get_next_token(Lexer *lexer);

// টোকেন এর নাম স্ট্রিং হিসেবে পাওয়ার জন্য
const char* token_type_to_string(TokenType type);

#endif // LEXER_H