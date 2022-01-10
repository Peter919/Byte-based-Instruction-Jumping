#include <stdio.h>
#include <stdarg.h>
#include "log.h"

// the only clean function i have
static void print_loglvl(int lvl)
{
        switch (lvl)
        {
        case DEBUG:
                printf("DEBUG: ");
                break;
        case INFO:
                printf("INFO: ");
                break;
        case SUCCESS:
                printf("SUCCESS: ");
                break;
        case ERROR:
                printf("ERROR: ");
                break;
        }
}

const int min_log_lvl = NONE;

// copied from
// https://stackoverflow.com/questions/68154231/how-do-i-define-a-function-that-accepts-a-formatted-input-string-in-c?rq=1
// from https://stackoverflow.com/users/3923896/steve-summit
// thanks, Steve!

char logger(int lvl, const char * text, ...)
{
        static int last_loglvl = -1;

        if(lvl < min_log_lvl)
        {
                return 0;
        }

        if (last_loglvl != lvl)
        {
                print_loglvl(lvl);
        }
        last_loglvl = lvl;

        va_list argp;
        va_start(argp, text);

        vprintf(text, argp);

        va_end(argp);

        return 1;
}

// prints newlines ... duh
char newlines(int lvl, unsigned int amount)
{
        if (lvl < min_log_lvl)
        {
                return 0;
        }

        for (int i = 0; i < amount; i++)
        {
                printf("\n");
        }

        return 1;
}
