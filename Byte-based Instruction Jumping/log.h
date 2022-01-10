#pragma once

enum LOG_LEVELS
{
        DEBUG,
        INFO,
        SUCCESS,
        ERROR,
        NONE
};

extern const int min_log_lvl;

char logger(int lvl, const char * text, ...);

char newlines(int lvl, unsigned int amount);
