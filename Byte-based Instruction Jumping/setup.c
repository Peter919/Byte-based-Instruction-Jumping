#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
#include "log.h"
#include "ascii_table.h"
#include "ascii_convertion.h"
#include "filehandling.h"
#include "longcharprocessing.h"

// the shortest possible hello world program in c according to people on the internet (always trust them) is
// main(){puts("Hello World")}

// initializes the ascii tables and checks that it works like it should
char setup_ascii_table(void)
{
        if (!init_ascii_table())
        {
                return 0;
        }

        newlines(DEBUG, 1);

        // prints ascii table both indexed and unindexed to check that everything is working properly
        logger(DEBUG, "Ascii Table: ");
        logger(DEBUG, "%s", ascii_table);
        newlines(DEBUG, 2);

        logger(DEBUG, "Ascii Table: ");
        log_longcharstr(DEBUG, ascii_table);
        newlines(DEBUG, 4);

        return 1;
}

void input_bij_file(char * dest)
{
        logger(NONE, "What file would you like to open? ");
        fgets(dest, 49, stdin);
        dest[strlen(dest) - 1] = '\0'; // remove last newline from input

        char * fext = file_ext(dest);
        if (!fext || strcmp(fext, ".bij") != 0)
        {
                logger(NONE, "The file must have a \".bij\" extension.");
                newlines(NONE, 4);

                input_bij_file(dest);
        }
}

// converts a char array to indexes of the bij ascii table, returns the length since NULLs are used for other purposes than ending the string
struct LengthTerminatedString file_to_ascii(char * fname)
{
        char * destbuffer = file_to_string(fname);

        if (!destbuffer)
        {
                logger(ERROR, "Couldn't open \"%s\". Are you sure it exists?\n", fname);
                logger(NONE, "Invalid file.\n");
                free(destbuffer);
                newlines(NONE, 4);
                input_bij_file(fname);

                return file_to_ascii(fname);
        }

        // prints the file contents both indexed and unindexed to check that everything is working properly
        logger(DEBUG, "%s:\n", fname);
        logger(DEBUG, "%s", destbuffer);
        newlines(DEBUG, 2);

        logger(DEBUG, "%s:\n", fname);
        log_longcharstr(DEBUG, destbuffer);
        newlines(DEBUG, 2);

        struct LengthTerminatedString returnstr = convert_to_ascii(destbuffer);

        logger(DEBUG, "%s:\n", fname);
        log_ascii_string_char(DEBUG, returnstr);
        newlines(DEBUG, 2);

        logger(DEBUG, "%s:\n", fname);
        log_ascii_string_hex(DEBUG, returnstr);
        newlines(DEBUG, 4);

        return returnstr;
}
