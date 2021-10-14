#pragma once

#include <stdlib.h>
#include <mymacro.h>

char *src_open(const char *filename);
size_t count_string_length(const char *s, char endch);

void line_print(char *s);
