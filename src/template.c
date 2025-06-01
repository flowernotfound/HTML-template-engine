#include "template.h"

#define TEMPLATE_DEBUG 1

#if TEMPLATE_DEBUG
#define TEMPLATE_LOG(fmt, ...) printf("[TEMPLATE] " fmt "\n", ##__VA_ARGS__)
#else
#define TEMPLATE_LOG(fmt, ...)
#endif

char *replace_placeholders(char *result, const char **keys, const char **values,
                           int num_pairs) {
  TEMPLATE_LOG("Starting placeholder replacement with %d pairs", num_pairs);

  for (int i = 0; i < num_pairs; i++) {
    char placeholder[64];
    snprintf(placeholder, sizeof(placeholder), "{{%s}}", keys[i]);

    TEMPLATE_LOG("Processing placeholder: %s -> %s", placeholder, values[i]);

    char *pos;
    while ((pos = strstr(result, placeholder)) != NULL) {
      TEMPLATE_LOG("Found placeholder at position: %ld", pos - result);
      size_t before_len = pos - result;
      size_t placeholder_len = strlen(placeholder);
      size_t value_len = strlen(values[i]);
      size_t after_len = strlen(pos + placeholder_len);
      char *new_result = malloc(before_len + value_len + after_len + 1);
      if (!new_result) {
        TEMPLATE_LOG("ERROR: Memory allocation failed");
        free(result);
        return NULL;
      }
      strncpy(new_result, result, before_len);
      new_result[before_len] = '\0';

      strcpy(new_result + before_len, values[i]);

      strcpy(new_result + before_len + value_len, pos + placeholder_len);

      free(result);
      result = new_result;

      TEMPLATE_LOG("Replacement completed. New length: %zu", strlen(result));
    }
  }

  TEMPLATE_LOG("Placeholder replacement completed");
  return result;
}

char *process_if_else(char *result, const char **keys, const char **values,
                      int num_pairs) {
  TEMPLATE_LOG("process_if_else called (not implemented yet)");
  (void)keys;
  (void)values;
  (void)num_pairs;
  return result;
}

char *process_loops(char *result, const char *loop_key,
                    const char **loop_values, int loop_count) {
  TEMPLATE_LOG("process_loops called (not implemented yet)");

  (void)loop_key;
  (void)loop_values;
  (void)loop_count;
  return result;
}

char *process_template(const char *template, const char **keys,
                       const char **values, int num_pairs, const char *loop_key,
                       const char **loop_values, int loop_count) {
  TEMPLATE_LOG("=== Template Processing Started ===");
  TEMPLATE_LOG("Template length: %zu", strlen(template));


  char *result = strdup(template);
  if (!result) {
    TEMPLATE_LOG("ERROR: Failed to duplicate template");
    return NULL;
  }


  result = replace_placeholders(result, keys, values, num_pairs);

  // TODO
  // result = process_if_else(result, keys, values, num_pairs);
  // result = process_loops(result, loop_key, loop_values, loop_count);

  (void)loop_key;
  (void)loop_values;
  (void)loop_count;

  TEMPLATE_LOG("=== Template Processing Completed ===");
  return result;
}
