#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "instructionstruct.h"
#include "ascii_convertion.h"
#include "filehandling.h"
#include "translation_to_bij.h"
#define WORD_LEN 3
#define BITS_IN_CHAR 8
#define INS_LEN ((WORD_LEN + 1) * BITS_IN_CHAR)

// instructions are separated by spaces, newlines or nulls
// so a strcmp version that takes this into account is needed
static char whitespace_strequal(char * str1, char * str2)
{
        char * ch1 = str1;
        char * ch2 = str2;

        while (*ch1 && *ch1 != ' ' && *ch1 != '\n' && *ch2 && *ch2 != ' ' && *ch2 != '\n')
        {
                if (*ch1 != *ch2)
                {
                        return 0;
                }
                ++ch1;
                ++ch2;
        }

        return 1;
}

static short instruction_to_asciichar(char * instruction)
{
        char returnval = 0;
        for (int i = 0; i < BITS_IN_CHAR; i++)
        {
                // checks if the current instruction word is equal to
                // one of the corresponding words in instr_list
                if (whitespace_strequal(instruction, instr_list[i].on))
                {
                        returnval += 1 << (BITS_IN_CHAR - 1 - i);
                }
                else if (!whitespace_strequal(instruction, instr_list[i].off))
                {
                        return 300;
                }

                if (i < BITS_IN_CHAR - 1 && instruction[WORD_LEN] != ' ')
                {
                        return 300;
                }

                instruction += WORD_LEN + 1;
        }

        return returnval;
}

struct LengthTerminatedString ins_to_bij(char * insfname)
{
        struct LengthTerminatedString asciistr;

        char * fbuffer = file_to_string(insfname);

        logger(DEBUG, "\"%s\": %s", insfname, fbuffer);
        newlines(DEBUG, 4);

        unsigned int fbufferlen = strlen(fbuffer);

        // you must add 1 to fbufferlen in case the instruction file doesn't have a newline at the end
        asciistr.length = (fbufferlen + 1) / INS_LEN;
        asciistr.start = malloc(asciistr.length);

        char * curr_instr = fbuffer;
        for (int i = 0; i < asciistr.length; i++)
        {
                short asciich = instruction_to_asciichar(curr_instr);

                // 300 is an error code
                if (asciich == 300)
                {
                        logger(NONE, "Something is wrong about \"%s\" around here:\n", insfname);
                        log_string_area(NONE, fbuffer, curr_instr);
                        return (struct LengthTerminatedString) {0, 0};
                }

                // simply checks if the character after the instruction is a newline or a null termination
                switch (curr_instr[INS_LEN - 1])
                {
                case '\n':
                case 0:
                        break;
                default:
                        logger(NONE, "Unexpected character \'%c\' in \"%s\".\n", curr_instr[INS_LEN - 1], insfname);
                        log_string_area(NONE, fbuffer, curr_instr);
                        return (struct LengthTerminatedString) {0, 0};
                }

                asciistr.start[i] = (char) asciich;

                // goes to the next instruction
                curr_instr += INS_LEN;
        }

        free(fbuffer);

        return asciistr;
}
