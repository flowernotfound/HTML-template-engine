#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* process_template(const char* template,
                      const char** keys, const char** values, int num_pairs,
                      const char* loop_key, const char** loop_values, int loop_count);
char* replace_placeholders(char* result, const char** keys,
                          const char** values, int num_pairs);
char* process_if_else(char* result, const char** keys,
                     const char** values, int num_pairs);
char* process_loops(char* result, const char* loop_key,
                   const char** loop_values, int loop_count);

#endif
