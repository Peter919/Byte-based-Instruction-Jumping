#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "longcharprocessing.h"

char * file_ext(char * filename)
{
        logger(INFO, "Finding the extension of \"%s\" ...\n", filename);

        // finds the last occurance of '.' in the file
        char * dot = strrchr(filename, '.');

        if (!dot)
        {
                logger(ERROR, "Found no \'.\' in \"%s\".\n", filename);
        }
        else if (dot == filename)
        {
                logger(ERROR, "The only occurance of \'.\' is at the very beginning of the filename (\"%s\").\n", filename);
        }
        else if (dot[1] == '\0')
        {
                logger(ERROR, "\"%s\" ends with a \'.\', so there is no extension.\n", filename);
        }
        else
        {
                logger(INFO, "The file extension of \"%s\" is \"%s\".\n", filename, dot);
                return dot;
        }

        return 0;
}

static unsigned int occurances_of_char(char ch, char * str)
{
        unsigned int occurances = 0;

        for (char * chptr = str; *chptr; chptr += char_size(*chptr))
        {
                if (*chptr == ch)
                {
                        ++occurances;
                }
        }

        return occurances;
}

// creates a buffer filled with the contents of a file
char * file_to_string(char * fname)
{
        char * fbuffer;

        logger(INFO, "Opening \"%s\" ...\n", fname);
        FILE * fp = fopen(fname, "r");

        if (!fp)
        {
                logger(ERROR, "Couldn't open \"%s\". Are you sure it exists?\n", fname);
                return 0;
        }

        logger(SUCCESS, "Done opening \"%s\".\n", fname);

        fseek(fp, 0, SEEK_END);
        unsigned int flength = ftell(fp);
        rewind(fp);

        logger(INFO, "Allocating %ld bytes of memory for \"%s\" ...\n", flength + 1 * sizeof(char), fname);
        fbuffer = malloc(flength + 1 * sizeof(char));

        if (!fbuffer)
        {
                logger(ERROR, "Couldn't create space for \"%s\".\n", fname);
                return 0;
        }

        logger(SUCCESS, "Done allocating space for \"%s\".\n", fname);
        fread(fbuffer, 1, flength, fp);

        fclose(fp);

        fbuffer[flength] = 0;

        // newlines takes 2 bytes of space in files, but only 1 in the program
        // therefore, there will be an amount of bytes equal to the amount of newlines that are not used
        // these are removed with an early null termination here
        fbuffer[flength - occurances_of_char('\n', fbuffer)] = 0;

        return fbuffer;
}

void open_file_window(const char * programpath, char * fname)
{
        logger(INFO, "Opening \"%s\" in a separate window ...\n", fname);

        // it can't be a literal because it needs enough space for thsse filename at the end
        char syscmd[256];
        sprintf(syscmd, "start \"%s\" \"%s\"", programpath, fname);
        system(syscmd);

        logger(SUCCESS, "Done opening \"%s\" in a saperate window.\n", programpath, fname);
}

void close_program(const char * programpath)
{
        logger(INFO, "Closing \"%s\" ...\n", programpath);

        // opens a file before closing so it won't throw an error about no file being opened
        char syscmd[256];
        sprintf(syscmd, "start \"%s\" \"Ascii Table.txt\"", programpath);
        system(syscmd);

        sprintf(syscmd, "taskkill /IM \"%s\" /F", programpath);
        system(syscmd);

        logger(SUCCESS, "Done closing \"%s\".\n", programpath);
}

// replaces the contents of the translation files with nothing to free memory
char close_and_clear_file(char * editor, char * fname)
{

        close_program(editor);

        logger(INFO, "Resetting \"%s\" ...\n", fname);
        FILE * fp = fopen(fname, "w");

        if (!fp)
        {
                logger(ERROR, "Failed to open \"%s\".\n", fname);
                return 0;
        }

        fwrite("", 0, 1, fp);
        fclose(fp);

        logger(SUCCESS, "Done resetting \"%s\".\n", fname);
        return 1;
}
