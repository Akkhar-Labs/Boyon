/* 
 * parser.h - 'বয়ন' অ্যাসেম্বলারের সিনট্যাক্স অ্যানালাইজার প্রোটোটাইপ
 * Architect: Rahat Hasan | Akkhar Labs
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdio.h>

/* --- পার্সার কাঠামো --- */
typedef struct {
    Lexer *lexer;
    Token current_token;
} Parser;

/* --- ফাংশন প্রোটোটাইপসমূহ --- */

// পার্সার শুরু করা
void init_parser(Parser *parser, Lexer *lexer);

// একটি পূর্ণ নির্দেশ পার্স করা (আউটপুট ফাইল এবং ডিবাগ লগের জন্য প্যারামিটার সহ)
void parse_instruction(Parser *parser, FILE *output_file, FILE *log_file);

// পুরো প্রোগ্রাম পার্স করে বাইনারি জেনারেট করা
void parse_program(Parser *parser, const char *output_filename);

#endif // PARSER_H