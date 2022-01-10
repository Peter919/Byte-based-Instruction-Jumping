#pragma once

// initializes the ascii tables and checks that it works like it should
char setup_ascii_table(void);

void input_bij_file(char * dest);

// converts a char array to indexes of the bij ascii table, returns the length since NULLs are used for other purposes than ending the string
struct LengthTerminatedString file_to_ascii(char * fname);

// comes from instructionstruct.c
void init_instr_list(void);
