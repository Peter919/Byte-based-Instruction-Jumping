#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "filehandling.h"
#define MAX_CHAR_LENGTH 4

char char_size(char ch)
{
        if (!((ch >> 7) & 1))
        {
                return 1;
        }
        if (!((ch >> 5) & 1))
        {
                return 2;
        }
        if (!((ch >> 4) & 1))
        {
                return 3;
        }
        if (!((ch >> 3) & 1))
        {
                return 4;
        }

        logger(ERROR, "Invalid character \'%d\'.\n", ch);
        return 0;
}

char log_longchar(int lvl, char * longchar)
{
        // checks this at the beginning so it doesn't call logger a trillion times
        if (lvl < min_log_lvl)
        {
                return 0;
        }

        logger(lvl, "\0");

        unsigned char longcharsize = char_size(*longchar);

        for (int i = 0; i < longcharsize; i++)
        {
                printf("%c", longchar[i]);
        }

        for (int i = 0; i < MAX_CHAR_LENGTH - longcharsize; i++)
        {
                printf(" ");
        }

        return 1;
}

char log_longcharstr(int lvl, char * longcharstr)
{
        // checks this at the beginning so it doesn't call logger a trillion times
        if (lvl < min_log_lvl)
        {
                return 0;
        }

        unsigned int i = 0;
        for (char * ch = longcharstr; *ch; ch += char_size(*ch))
        {
                printf("%d: ", i);
                log_longchar(lvl, ch);
                ++i;
        }

        newlines(lvl, 1);

        return 1;
}

/// fat
//! fat
// thin
/** fat */
/*! fat */
/* thin */
