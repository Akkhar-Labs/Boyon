/* 
 * main.c - 'বয়ন' অ্যাসেম্বলারের মূল এন্ট্রি পয়েন্ট
 * আউটপুট ফাইল ফরম্যাট: .ব (সার্বভৌম বাংলা মেশিন কোড)
 */

#include "boyon.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    printf("--- বয়ন (Boyon) অ্যাসেম্বলার v১.০ ---\n");
    printf("স্থাপত্য: নির্দেশিকা-১৬ | এনকোডিং: অক্ষর-লিপি ৮\n\n");

    // টেস্ট সোর্স কোড
    const char *source = 
        "আদান সাময়িক-১, ০ষএ৪\n"      
        "আদান সাময়িক-২, ৫\n"         
        "সংযোজন সাময়িক-১, সাময়িক-২\n" 
        "প্রদর্শন সাময়িক-১\n"        
        "থামো";                    

    printf("অ্যাসেম্বলিং শুরু হচ্ছে...\n");

    Lexer lexer;
    init_lexer(&lexer, source);

    Parser parser;
    init_parser(&parser, &lexer);

    // চূড়ান্ত মেশিন কোড ফাইল তৈরি (এক্সটেনশন: .ব)
    const char *output_file = "নির্দেশিকা.ব";
    parse_program(&parser, output_file);

    printf("সফলভাবে সম্পন্ন হয়েছে! আউটপুট ফাইল: %s\n", output_file);
    printf("--------------------------------------------\n");
    printf("অভিনন্দন রাহাত, আপনার সার্বভৌম '.ব' বাইনারি তৈরি।\n");

    return 0;
}