/* 
 * parser.c - 'বয়ন' অ্যাসেম্বলার (ডিবাগিং লগ সুবিধা সহ)
 * এটি সরাসরি র-বাইনারি (.ব) এবং মানুষের পড়ার যোগ্য লগ ফাইল তৈরি করে।
 */

#include "parser.h"
#include "arch/nirdeshika16.h"
#include <string.h>

void init_parser(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
}

static void advance(Parser *parser) {
    parser->current_token = get_next_token(parser->lexer);
}

// নির্দেশ পার্স করা এবং একই সাথে বাইনারি ও লগে লেখা
void parse_instruction(Parser *parser, FILE *output_file, FILE *log_file) {
    if (parser->current_token.type == TOKEN_INSTRUCTION) {
        int opcode = parser->current_token.value;
        char opcode_name[64];
        strcpy(opcode_name, parser->current_token.text); // ওকোড এর নাম সেভ রাখা
        
        int reg = 0;
        int val = 0;

        advance(parser);

        if (parser->current_token.type == TOKEN_REGISTER) {
            reg = parser->current_token.value;
            advance(parser);
        }

        if (parser->current_token.type == TOKEN_COMMA) {
            advance(parser);
        }

        if (parser->current_token.type == TOKEN_NUMBER || parser->current_token.type == TOKEN_REGISTER) {
            val = parser->current_token.value;
            advance(parser);
        }

        unsigned short machine_code = (unsigned short)BUILD_INSTR(opcode, reg, val);

        // ১. র-বাইনারি ফাইলে রাইট করা
        fwrite(&machine_code, sizeof(unsigned short), 1, output_file);

        // ২. ডিবাগ লগে মানুষের পড়ার যোগ্য তথ্য রাইট করা
        if (log_file) {
            fprintf(log_file, "নির্দেশ: %-10s | ওকোড: 0x%X | সাময়িক: %-2d | মান: 0x%02X | মেশিন কোড: 0x%04X\n",
                    opcode_name, opcode, reg, val, machine_code);
        }
    } 
    else if (parser->current_token.type == TOKEN_NEWLINE) {
        advance(parser);
    }
    else {
        advance(parser);
    }
}

void parse_program(Parser *parser, const char *output_filename) {
    FILE *output_file = fopen(output_filename, "wb");
    FILE *log_file = fopen("ডিবাগ_লগ.txt", "w"); // লগ ফাইল ওপেন করা

    if (!output_file) {
        printf("আউটপুট ফাইল তৈরি করা যায়নি।\n");
        return;
    }

    if (log_file) {
        fprintf(log_file, "--- বয়ন (Boyon) অ্যাসেম্বলার ডিবাগ রিপোর্ট ---\n");
        fprintf(log_file, "আর্কিটেকচার: নির্দেশিকা-১৬\n\n");
    }

    while (parser->current_token.type != TOKEN_EOF) {
        parse_instruction(parser, output_file, log_file);
    }

    fclose(output_file);
    if (log_file) fclose(log_file);
}