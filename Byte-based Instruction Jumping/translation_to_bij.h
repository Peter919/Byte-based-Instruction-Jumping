#pragma once
#include "translation_from_bij.h"
#include "ascii_convertion.h"

char log_string_area(int lvl, char * str, char * index);

char asciistr_to_longchar_bijfile(struct LengthTerminatedString * str, struct Files * files);
