#include "log.h"

struct Instr
{
        char * off;
        char * on;
};

// oh no it's global
struct Instr instr_list[8];

void init_instr_list(void)
{
        logger(INFO, "Initializing the instruction list ...\n");

        // beautiful
        instr_list[0] = (struct Instr) {"mvr", "mvl"};
        instr_list[1] = (struct Instr) {"...", "jmr"};
        instr_list[2] = (struct Instr) {"...", "jml"};
        instr_list[3] = (struct Instr) {"red", "wrt"};
        instr_list[4] = (struct Instr) {"...", "cns"};
        instr_list[5] = (struct Instr) {"...", "spc"};
        instr_list[6] = (struct Instr) {"...", "neq"};
        instr_list[7] = (struct Instr) {"mvr", "mvl"};

        logger(DEBUG, "\n");
        for (int i = 0; i < 8; i++)
        {
                logger(DEBUG, "%d: %s/%s\n", i, instr_list[i].off, instr_list[i].on);
        }

        logger(SUCCESS, "Done initializing the instruction list.\n");
}
