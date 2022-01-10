#pragma once

char * file_ext(char * filename);

char * file_to_string(char * fname);

void open_file_window(const char * programpath, char * fname);

void close_program(const char * programpath);

char close_and_clear_file(char * editor, char * fname);
