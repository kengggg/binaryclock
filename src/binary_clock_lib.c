#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <stddef.h>

// Cross-platform compatibility
#ifdef _WIN32
    #include <windows.h>  // For Sleep on Windows
#else
    #include <unistd.h>   // For sleep on Unix-like systems
#endif

// Function to convert an integer to binary string
// Thread-safe version that uses provided buffer
// Returns 0 on success, -1 on failure (e.g., buffer too small)
int to_binary(int value, int bits, char* buffer, size_t buffer_len) {
    if (buffer == NULL || buffer_len < (size_t)(bits + 1) || bits < 0) {
        return -1;
    }

    for (int i = bits - 1; i >= 0; i--) {
        buffer[bits - 1 - i] = (value & (1 << i)) ? '1' : '0';
    }
    buffer[bits] = '\0'; // Null-terminate the string
    return 0;
}

// Function to display binary string as emojis
void display_binary(const char* bin) {
    for (int i = 0; bin[i] != '\0'; i++) {
        if (bin[i] == '0') printf("🌚");
        else printf("🌝");
    }
    printf("\n");
}

// Signal handler for graceful exit
void signal_handler(int sig) {
    (void)sig; // Suppress unused parameter warning
    printf("\n\nBinary clock stopped.\n");
    exit(0);
}