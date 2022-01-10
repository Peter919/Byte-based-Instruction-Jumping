#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "ascii_convertion.h"
#include "filehandling.h"
#include "setup.h"
#include "instructionstruct.h"
#include "translation_from_bij.h"
#define BITS_IN_CHAR 8
#define TRANSLATED_CHAR_SIZE 100

static void char_to_hex(char * dest, char source, char include_space)
{
        for (int i = 0; i < TRANSLATED_CHAR_SIZE; i++)
        {
                dest[i] = 0;
        }

        sprintf(dest, "%02x", (unsigned char) source);

        if (include_space)
        {
                strcat(dest, " ");
        }
}

static void char_to_ins(char * dest, char source, char include_newline)
{
        for (int i = 0; i < TRANSLATED_CHAR_SIZE; i++)
        {
                dest[i] = 0;
        }

        // picks the right instruction for each bit in the source char
        for (int i = 0; i < 8; i++)
        {
                if ((source >> (7 - i)) & 1)
                {
                        strcat(dest, instr_list[i].on);
                }
                else
                {
                        strcat(dest, instr_list[i].off);
                }

                if (i < 7)
                {
                        strcat(dest, " ");
                }
        }
        if (include_newline)
        {
                strcat(dest, "\n");
        }
}

static char translate_to_file(char * fname, char is_hex, struct LengthTerminatedString * bijstr)
{
        logger(INFO, "Opening \"%s\" for appending ...\n", fname);
        FILE * fp = fopen(fname, "a");
        if (!fp)
        {
                logger(INFO, "ERROR: Could not open \"%s\".\n", fname);
                return 0;
        }
        logger(SUCCESS, "Done opening %s.\n", fname);

        logger(INFO, "Translating a bij file to \"%s\" ...\n", fname);

        char currchar_translated[TRANSLATED_CHAR_SIZE];

        void (*translate_char)(char *, char, char);

        if (is_hex)
        {
                translate_char = char_to_hex;
        }
        else
        {
                translate_char = char_to_ins;
        }

        for (int i = 0; i < bijstr->length; i++)
        {
                translate_char(currchar_translated, bijstr->start[i], i + 1 < bijstr->length);
                fwrite(currchar_translated, strlen(currchar_translated), 1, fp);
        }

        fclose(fp);

        logger(SUCCESS, "Done translating a bij file to \"%s\".\n", fname);

        return 1;
}

char bij_to_hex(struct LengthTerminatedString * bijstr, struct Files * files)
{
        close_and_clear_file(files->editor, files->hexfname);

        if (!translate_to_file(files->hexfname, 1, bijstr))
        {
                return 0;
        }

        return 1;
}

char bij_to_ins(struct LengthTerminatedString * bijstr, struct Files * files)
{
        close_and_clear_file(files->editor, files->insfname);

        if (!translate_to_file(files->insfname, 0, bijstr))
        {
                return 0;
        }

        return 1;
}
