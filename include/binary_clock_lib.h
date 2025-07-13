#ifndef BINARY_CLOCK_LIB_H
#define BINARY_CLOCK_LIB_H

#include <stddef.h>

// Function declarations
// Returns 0 on success, -1 on failure (buffer too small)
int to_binary(int value, int bits, char* buffer, size_t buffer_len);
void display_binary(const char* bin);
void signal_handler(int sig);

#endif // BINARY_CLOCK_LIB_H