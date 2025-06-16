#ifndef BINARY_CLOCK_LIB_H
#define BINARY_CLOCK_LIB_H

// Function declarations
void to_binary(int value, int bits, char* buffer);
void display_binary(const char* bin);
void signal_handler(int sig);

#endif // BINARY_CLOCK_LIB_H