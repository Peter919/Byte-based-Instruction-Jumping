#pragma once

struct Files
{
        char * bijfname;
        char * hexfname;
        char * insfname;
        char * editor;
};

char bij_to_hex(struct LengthTerminatedString * bijstr, struct Files * files);

char bij_to_ins(struct LengthTerminatedString * bijstr, struct Files * files);
