#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"
#include "ascii_convertion.h"
#include "filehandling.h"
#include "translation_to_bij.h"

static short hex_to_dec(char hex)
{
        const static char hexdigits[] = "0123456789abcdef";

        for (int i = 0; i < 0x10; i++)
        {
                if (hex == hexdigits[i])
                {
                        return i;
                }
        }

        logger(ERROR, "Found no matching character for hexadecimal digit \'%c\'.\n", hex);
        return 300;
}

static short double_digit_hex_to_char(char * hexbufferstart, char * hexnum)
{
        char returnval = 0;
        short decnum;

        for (int i = 0; i < 2; i++)
        {
                decnum = hex_to_dec(hexnum[i]);
                // 300 is an error code
                if (decnum == 300)
                {
                        logger(NONE, "Unexpected character \'%c\'.\n", hexnum[i]);
                        log_string_area(NONE, hexnum, hexnum + i);
                        return 300;
                }

                returnval += (char) decnum;
                if (i == 0)
                {
                        returnval = returnval << 4;
                }
        }

        return returnval;
}

struct LengthTerminatedString hex_to_bij(char * hexfname)
{
        struct LengthTerminatedString asciistr;

        char * fbuffer = file_to_string(hexfname);

        logger(DEBUG, "\"%s\": %s", hexfname, fbuffer);
        newlines(DEBUG, 4);

        asciistr.length = (strlen(fbuffer) + 1) / 3;
        asciistr.start = malloc(asciistr.length);

        short ch;
        for (int i = 0; i < asciistr.length; i++)
        {
                ch = double_digit_hex_to_char(fbuffer, fbuffer + i * 3);

                if (ch == 300)
                {
                        return (struct LengthTerminatedString) {0, 0};
                }

                asciistr.start[i] = ch;

                if (i + 1 < asciistr.length && fbuffer[i * 3 + 2] != ' ')
                {
                        logger(NONE, "Unexpected character \'%c\'.\n", fbuffer[i * 3 + 2]);
                        log_string_area(NONE, fbuffer, fbuffer + i * 3 + 2);
                        return (struct LengthTerminatedString) {0, 0};
                }
        }

        free(fbuffer);

        return asciistr;
}
