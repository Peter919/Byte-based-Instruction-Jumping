#pragma once

struct LengthTerminatedString
{
        char * start;
        unsigned int length;
};

short longchar_to_ascii(char * longchar);

// replaces a string with an identical string, except this new string uses bij's ascii table instead for the unicode one
struct LengthTerminatedString convert_to_ascii(char * str);

void log_ascii_string_char(int lvl, struct LengthTerminatedString str);

void log_ascii_string_hex(int lvl, struct LengthTerminatedString str);

char ascii_to_longchar(char * dest, char source);
