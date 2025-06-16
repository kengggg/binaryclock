#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

// Cross-platform compatibility
#ifdef _WIN32
    #include <windows.h>  // For Sleep on Windows
#else
    #include <unistd.h>   // For sleep on Unix-like systems
#endif

// Function to convert an integer to binary string
// Thread-safe version that uses provided buffer
void to_binary(int value, int bits, char* buffer) {
    for (int i = bits - 1; i >= 0; i--) {
        buffer[bits - 1 - i] = (value & (1 << i)) ? '1' : '0';
    }
    buffer[bits] = '\0'; // Null-terminate the string
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
    printf("\n\nBinary clock stopped.\n");
    exit(0);
}