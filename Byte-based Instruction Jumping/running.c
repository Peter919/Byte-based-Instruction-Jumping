#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "longcharprocessing.h"
#include "ascii_convertion.h"

#define RED 0b000
#define WRT 0b100
#define RED_CNS 0b010
#define WRT_CNS 0b110

#define RED_SPC 0b001
#define WRT_SPC 0b101
#define RED_CNS_SPC 0b011
#define WRT_CNS_SPC 0b111

static char run_instr(struct LengthTerminatedString * program, long * index, char * accumulator);

char run(struct LengthTerminatedString * program)
{
        logger(NONE, "\n\n-------- RUNNING --------\n\n");

        // index may be less than zero (in that case, the program returns 0)
        // therefore, it must be a long rather than an unsigned integer
        long index = 0;
        char accumulator = 0;
        char returnval = 2;

        while (returnval > 1)
        {
                returnval = run_instr(program, &index, &accumulator);
        }

        logger(NONE, "\n\n-------- PROGRAM EXITED WITH CODE %d --------\n\n", returnval);

        logger(DEBUG, "Final program string:\n");
        for (int i = 0; i < program->length; i++)
        {
                logger(DEBUG, "%c", program->start[i]);
        }
        newlines(DEBUG, 2);

        return returnval;
}

static char run_instr(struct LengthTerminatedString * program, long * index, char * accumulator)
{
        char instr = program->start[*index];

        // mvr/mvl
        if ((instr >> 7) & 1)
        {
                if (--(*index) < 0)
                {
                        return 0;
                }
        }
        else
        {
                if (++(*index) >= program->length)
                {
                        return 1;
                }
        }

        // .../jmr
        if ((instr >> 6) & 1)
        {
                char currch = program->start[*index];

                do
                {
                        if (++(*index) >= program->length)
                        {
                                return 1;
                        }
                }
                while (program->start[*index] != currch);
        }

        // .../jml
        if ((instr >> 5) & 1)
        {
                char currch = program->start[*index];

                do
                {
                        if (--(*index) < 0)
                        {
                                return 0;
                        }
                }
                while (program->start[*index] != currch);
        }

        char move_amount_in_last_cmd = 1;

        // red/wrt .../cns .../spc
        switch ((instr >> 2) & 0b111)
        {
        case RED:
                *accumulator = program->start[*index];
                break;
        case WRT:
                program->start[*index] = *accumulator;
                break;
        case RED_CNS:
                do
                {
                        scanf("%c", program->start + *index);
                } while (program->start[*index] == '\n');
                break;
        case WRT_CNS:
                /// IMPORTANT!!!
                // this doesn't take multi-byte characters into account
                // doing so would probably be a burdance to the performance
                // and doesn't look very nice on the console anyways, since
                // multi-byte characters can't be printed without extra libraries (i think)
                printf("%c", program->start[*index]);
                break;
        case RED_SPC:
                move_amount_in_last_cmd = 0;
                break;
        case WRT_SPC:
                program->start[*index] = ~(program->start[*index] & *accumulator);
                break;
        case RED_CNS_SPC:
                break;
        case WRT_CNS_SPC:
                if ((instr >> 0) & 1)
                {
                        program->start[*index] <<= 1;
                }
                else
                {
                        program->start[*index] >>= 1;
                }
                break;
        }
        
        // .../neq
        if ((instr >> 1) & 1 && *accumulator != program->start[*index])
        {
                ++move_amount_in_last_cmd;
        }
        
        // mvr/mvl
        // this could look better, but having an if in a for is probably slightly more
        // time-consuming, as it has to check the if statement several times
        // and i really want this programming language to be as fast as possible
        if ((instr >> 0) & 1)
        {
                for (char i = 0; i < move_amount_in_last_cmd; i++)
                {
                        if (--(*index) < 0)
                        {
                                return 0;
                        }
                }
        }
        else
        {
                for (char i = 0; i < move_amount_in_last_cmd; i++)
                {
                        if (++(*index) >= program->length)
                        {
                                return 0;
                        }
                }
        }

        return 2;
}
