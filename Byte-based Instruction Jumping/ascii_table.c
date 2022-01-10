#include <stdlib.h>
#include "log.h"
#include "filehandling.h"

char * ascii_table;

char init_ascii_table(void)
{
        logger(INFO, "Initializing the ascii table buffer ...\n");
        ascii_table = file_to_string("Ascii Table.txt");

        if (!ascii_table)
        {
                logger(ERROR, "Something went wrong initializing ascii table ...\n");
                return 0;
        }

        logger(SUCCESS, "Done initializing ascii table.\n");
        return 1;
}
