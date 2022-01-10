#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "filehandling.h"
#include "ascii_convertion.h"
#include "translation_from_bij.h"
#include "longcharprocessing.h"

char log_string_area(int lvl, char * str, char * index)
{
        if (lvl < min_log_lvl)
        {
                return 0;
        }

        char * curr = index;

        // only == and != works on pointers. if < worked, i would have used that instead
        for (int i = 0; i < 40; i++)
        {
                if (--curr == str)
                {
                        break;
                }
        }

        while (*curr && curr != index + 40)
        {
                logger(lvl, "%c", *curr);
                ++curr;
        }

        newlines(lvl, 2);

        return 1;
}

char asciistr_to_longchar_bijfile(struct LengthTerminatedString * str, struct Files * files)
{
        close_and_clear_file(files->editor, files->bijfname);

        logger(INFO, "Opening \"%s\" for appending ...\n", files->bijfname);
        FILE * fp = fopen(files->bijfname, "a");

        if (!fp)
        {
                logger(ERROR, "Something went wrong opening \"%s\" for appending.\n", files->bijfname);
        }
        logger(SUCCESS, "Done opening \"%s\" for appending.\n", files->bijfname);

        char * longchar = alloca(5);
        for (unsigned int i = 0; i < str->length; i++)
        {
                if (!ascii_to_longchar(longchar, str->start[i]))
                {
                        fclose(fp);
                        return 0;
                }

                fwrite(longchar, char_size(*longchar), 1, fp);
        }

        fclose(fp);
        return 1;
}
