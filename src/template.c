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
  TEMPLATE_LOG("Starting if-else processing with %d keys", num_pairs);

  for (int i = 0; i < num_pairs; i++) {

    char condition_start[64];
    snprintf(condition_start, sizeof(condition_start), "{%% if %s %%}",
             keys[i]);

    TEMPLATE_LOG("Looking for condition: %s (value: '%s')", condition_start,
                 values[i]);

    char *if_pos;
    while ((if_pos = strstr(result, condition_start)) != NULL) {
      TEMPLATE_LOG("Found if condition at position: %ld", if_pos - result);

      char *else_pos = strstr(if_pos, "{% else %}");
      char *endif_pos = strstr(if_pos, "{% endif %}");

      if (!endif_pos) {
        TEMPLATE_LOG("ERROR: Missing {%% endif %%} for condition");
        break;
      }

      int condition_is_true = (strlen(values[i]) > 0);
      TEMPLATE_LOG("Condition evaluation: %s",
                   condition_is_true ? "TRUE" : "FALSE");

      char *selected_content = NULL;
      size_t selected_len = 0;

      if (else_pos && else_pos < endif_pos) {
        if (condition_is_true) {
          size_t true_start = strlen(condition_start);
          selected_len = else_pos - (if_pos + true_start);
          selected_content = malloc(selected_len + 1);
          if (!selected_content) {
            TEMPLATE_LOG("ERROR: Memory allocation failed");
            return result;
          }
          strncpy(selected_content, if_pos + true_start, selected_len);
          selected_content[selected_len] = '\0';
          TEMPLATE_LOG("Selected TRUE content: %.50s...", selected_content);
        } else {
          size_t false_start = strlen("{% else %}");
          selected_len = endif_pos - (else_pos + false_start);
          selected_content = malloc(selected_len + 1);
          if (!selected_content) {
            TEMPLATE_LOG("ERROR: Memory allocation failed");
            return result;
          }
          strncpy(selected_content, else_pos + false_start, selected_len);
          selected_content[selected_len] = '\0';
          TEMPLATE_LOG("Selected FALSE content: %.50s...", selected_content);
        }
      } else {
        if (condition_is_true) {
          size_t true_start = strlen(condition_start);
          selected_len = endif_pos - (if_pos + true_start);
          selected_content = malloc(selected_len + 1);
          if (!selected_content) {
            TEMPLATE_LOG("ERROR: Memory allocation failed");
            return result;
          }
          strncpy(selected_content, if_pos + true_start, selected_len);
          selected_content[selected_len] = '\0';
          TEMPLATE_LOG("Selected TRUE content (no else): %.50s...",
                       selected_content);
        } else {
          selected_content = strdup("");
          selected_len = 0;
          TEMPLATE_LOG("Selected empty content (condition false, no else)");
        }
      }

      size_t before_len = if_pos - result;
      size_t after_len = strlen(endif_pos + strlen("{% endif %}"));

      char *new_result = malloc(before_len + selected_len + after_len + 1);
      if (!new_result) {
        TEMPLATE_LOG("ERROR: Memory allocation failed for new result");
        free(selected_content);
        return result;
      }

      strncpy(new_result, result, before_len);
      new_result[before_len] = '\0';
      strcpy(new_result + before_len, selected_content);
      strcpy(new_result + before_len + selected_len,
             endif_pos + strlen("{% endif %}"));

      free(result);
      free(selected_content);
      result = new_result;

      TEMPLATE_LOG("If-else replacement completed. New length: %zu",
                   strlen(result));
    }
  }

  TEMPLATE_LOG("If-else processing completed");
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

  TEMPLATE_LOG("--- Starting placeholder replacement ---");
  result = replace_placeholders(result, keys, values, num_pairs);
  if (!result) {
    TEMPLATE_LOG("ERROR: Placeholder replacement failed");
    return NULL;
  }

  TEMPLATE_LOG("--- Starting if-else processing ---");
  result = process_if_else(result, keys, values, num_pairs);
  if (!result) {
    TEMPLATE_LOG("ERROR: If-else processing failed");
    return NULL;
  }

  (void)loop_key;
  (void)loop_values;
  (void)loop_count;

  TEMPLATE_LOG("=== Template Processing Completed ===");
  return result;
}
