#include "html_serve.h"

char* serve_html(const char* filename, size_t* out_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek failed");
        fclose(file);
        return NULL;
    }

    long size = ftell(file);
    if (size == -1) {
        perror("ftell failed");
        fclose(file);
        return NULL;
    }
    rewind(file);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        perror("malloc failed");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';

    fclose(file);

    if (out_size)
        *out_size = read_size;

    return buffer;
}
