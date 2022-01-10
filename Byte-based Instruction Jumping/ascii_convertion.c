#include <stdio.h>
#include "log.h"
#include "ascii_convertion.h"
#include "ascii_table.h"
#include "longcharprocessing.h"

static char longchar_equality(char * longchar_a, char * longchar_b)
{
        for (int j = 0; j < char_size(*longchar_a); j++)
        {
                if (longchar_a[j] != longchar_b[j])
                {
                        return 0;
                }
        }

        return 1;
}

// convert a single long char to the corresponding index in the ascii table
short longchar_to_ascii(char * longchar)
{
        char i = 0;

        for (char * asciich = ascii_table; *asciich; asciich += char_size(*asciich))
        {
                if (longchar_equality(longchar, asciich))
                {
                        return i;
                }
                ++i;
        }

        printf("\n");
        logger(ERROR, "Found no matching character for ");
        log_longchar(ERROR, longchar);
        logger(ERROR, ".\n");
        return 300;
}

static void help_with_invalid_char(char * invchar)
{
        logger(NONE, "Illegal character ");

        int unicode_index = 0;
        int multiplier = 1;
        for (int i = char_size(*invchar) - 1; i >= 0; i--)
        {
                unicode_index += ((unsigned char) invchar[i]) * multiplier;
                multiplier *= 256;

        }
        logger(NONE, "%d.\n");

        newlines(NONE, 2);
        logger(NONE, "Open https://onlineutf8tools.com/convert-hexadecimal-to-utf8 and write %x in the blue \"hexadecimal\" text box.\n", unicode_index, unicode_index);
        logger(NONE, "The character that appears in the \"utf8\" text box is the invalid character you included in your BIJ file.");
        newlines(NONE, 4);
}

// replaces a string with an identical string, except this new string uses bij's ascii table instead for the unicode one
struct LengthTerminatedString convert_to_ascii(char * str)
{
        logger(INFO, "Converting string to ascii ...\n");

        unsigned int len = 0;

        char * repl_source = str;
        char * repl_dest = str;
        short repl_source_ascii;

        while (*repl_source)
        {
                repl_source_ascii = longchar_to_ascii(repl_source);

                logger(DEBUG, "%d = ", (unsigned char) repl_source_ascii);
                log_longchar(DEBUG, repl_source);
                logger(DEBUG, " ");

                // 300 is an error code for when the character isn't in the ascii table
                if (repl_source_ascii == 300)
                {
                        help_with_invalid_char(repl_source);
                        return (struct LengthTerminatedString) {0, 0};
                }

                repl_source += char_size(*repl_source);

                // the changes of repl_dest has to be done AFTER all the calculations and changes done with repl_source
                // to avoid treating an overwritten string as an original one
                *repl_dest = (char) repl_source_ascii;
                ++repl_dest;

                ++len;
        }

        newlines(DEBUG, 1);
        logger(SUCCESS, "Done converting to ascii.\n");
        logger(DEBUG, "The ascii string has a length of %ld.\n", len);

        return (struct LengthTerminatedString) {str, len};
}

void log_ascii_string_char(int lvl, struct LengthTerminatedString str)
{
        for (unsigned int i = 0; i < str.length; i++)
        {
                logger(lvl, "%c", str.start[i]);
        }
}

void log_ascii_string_hex(int lvl, struct LengthTerminatedString str)
{
        for (unsigned int i = 0; i < str.length; i++)
        {
                logger(lvl, "%02x ", (unsigned char) str.start[i]);
        }
}

static void set_longchar(char * dest, char * source)
{
        int srclen = char_size(*source);

        for (int i = 0; i < srclen; i++)
        {
                dest[i] = source[i];
        }
}

char ascii_to_longchar(char * dest, char source)
{
        char i = 0;
        for (char * asciich = ascii_table; *asciich; asciich += char_size(*asciich))
        {
                if (i == source)
                {
                        set_longchar(dest, asciich);
                        return 1;
                }
                ++i;
        }

        logger(ERROR, "The ascii value \'%d\' is beyond the bounds of the ascii table.\n", source);
        return 0;
}
