#pragma once

struct Instr
{
        char * off;
        char * on;
};

struct Instr instr_list[8];

void init_instr_list(void);
