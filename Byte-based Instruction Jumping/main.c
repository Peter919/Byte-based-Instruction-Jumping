#include <stdlib.h>
#include "log.h"
#include "setup.h"
#include "translation_from_bij.h"
#include "ascii_convertion.h"
#include "filehandling.h"
#include <string.h>
#include "hex_to_bij.h"
#include "stdio.h"
#include "translation_to_bij.h"
#include "ins_to_bij.h"
#include "running.h"

void open_all_file_windows(struct Files * files)
{
        open_file_window(files->editor, files->bijfname);
        open_file_window(files->editor, files->hexfname);
        open_file_window(files->editor, files->insfname);
}

void perform_command(char * command, struct Files * files, struct LengthTerminatedString * asciistr)
{
        scanf(" %s", command);

        if (strcmp(command, "run") == 0)
        {
                run(asciistr);
        }
        else if (strcmp(command, "bij") == 0)
        {
                struct LengthTerminatedString bijstr = file_to_ascii(files->bijfname);
                if (!bijstr.start)
                {
                        logger(NONE, "Couldn't translate from \"%s\".\n", files->bijfname);
                        return;
                }

                free(asciistr->start);
                *asciistr = file_to_ascii(files->bijfname);
                bij_to_hex(asciistr, files);
                bij_to_ins(asciistr, files);
                open_all_file_windows(files);
        }
        else if (strcmp(command, "hex") == 0)
        {
                struct LengthTerminatedString hexstr = hex_to_bij(files->hexfname);
                if (!hexstr.start)
                {
                        return;
                }

                free(asciistr->start);
                *asciistr = hexstr;
                asciistr_to_longchar_bijfile(asciistr, files);
                bij_to_ins(asciistr, files);
                open_all_file_windows(files);
        }
        else if(strcmp(command, "ins") == 0)
        {
                struct LengthTerminatedString insstr = ins_to_bij(files->insfname);
                if (!insstr.start)
                {
                        return;
                }

                free(asciistr->start);
                *asciistr = insstr;
                asciistr_to_longchar_bijfile(asciistr, files);
                bij_to_hex(asciistr, files);
                open_all_file_windows(files);
        }
        else if (strcmp(command, "exit") != 0)
        {
                logger(NONE, "Invalid command %s.\n");
                logger(NONE, "The allowed commands are \"run\", \"bij\", \"hex\", \"ins\" and \"exit\".\n");
        }
}

int main(void)
{
        setup_ascii_table();
        newlines(INFO, 4);

        init_instr_list();
        newlines(INFO, 4);


        struct Files files;
        files.editor = "notepad.exe";
        files.hexfname = "hexbij.txt";
        files.insfname = "insbij.txt";

        files.bijfname = alloca(50);
        input_bij_file(files.bijfname);


        struct LengthTerminatedString asciistr;
        asciistr = file_to_ascii(files.bijfname);

        bij_to_hex(&asciistr, &files);
        bij_to_ins(&asciistr, &files);

        open_all_file_windows(&files);

        char command[30];

        while (strcmp(command, "exit") != 0)
        {
                perform_command(command, &files, &asciistr);
        }

        close_and_clear_file(files.editor, files.hexfname);
        close_and_clear_file(files.editor, files.insfname);
        close_program(files.editor);

        logger(SUCCESS, "Done running the entire program!!!!\n\nIt worked!\n\nWeeeeeeee are the chaaaampions my frieeeeends dun dun dun\n\n");
        return 0;
}

/*

COMMANDS:
exit:
clears translation files and exits with error code 0

hex:
translates hex file to other files

ins:
translates ins file to other files

bij:
translates bij file to other files

run:
runs the program

*/




/*


4 * 4 cm chip

+-+---+
|3|   |
+-+ 1 |
|2|   |
+-+---+

1: DEEP MEMORY:
* stores commands
* stores values
* can also store pointers to other places within the deep memory

2: PROCESSOR:
* runs commands from deep memory

3: QUICK ACCESS MEMORY:
* stores pointers to the beginnings and ends of allocated deep memory
* stores pointers to labels (these are represented as indexes, not words)
* can be accessed directly

15 * 10 cm console

+--------------------+
| +----------------+ |
| |                | |
| |                | |
| |                | |
| |                | |
| |                | |
| |                | |
| +----------------+ |
|             _      |
|    |       |A|     |
|  --+--         _   |
|    |          |B|  |
|                    |
|                    |
+--------------------+






LAB NOT
    MOV CPT NOT
    LAB ARG
    MOV ALOC 8   ARG

*/
