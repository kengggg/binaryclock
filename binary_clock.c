#include <stdio.h>    // For printf
#include <time.h>     // For time functions
#include <stdlib.h>   // For system
#include <signal.h>   // For signal handling

// Cross-platform compatibility
#ifdef _WIN32
    #include <windows.h>  // For Sleep on Windows
    #define CLEAR_COMMAND "cls"
    #define SLEEP_FUNC(x) Sleep((x) * 1000)  // Windows Sleep uses milliseconds
#else
    #include <unistd.h>   // For sleep on Unix-like systems
    #define CLEAR_COMMAND "clear"
    #define SLEEP_FUNC(x) sleep(x)  // Unix sleep uses seconds
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

int main() {
    // Set up signal handler for graceful exit (Ctrl+C)
    signal(SIGINT, signal_handler);
    
    printf("Binary Clock - Press Ctrl+C to exit\n\n");
    
    while (1) { // Infinite loop to keep clock running
        // Get current time
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        int hours = tm->tm_hour;   // 0-23
        int minutes = tm->tm_min;  // 0-59
        int seconds = tm->tm_sec;  // 0-59

        // Binary string buffers
        char hours_tens_bin[5], hours_units_bin[5];
        char minutes_col1[7], minutes_col2[5];
        char seconds_col1[7], seconds_col2[5];
        
        // Hours: split into tens and units
        int hours_tens = hours / 10;
        int hours_units = hours % 10;
        to_binary(hours_tens, 4, hours_tens_bin);
        to_binary(hours_units, 4, hours_units_bin);

        // Minutes: split into tens and units for proper binary clock display
        int minutes_tens = minutes / 10;
        int minutes_units = minutes % 10;
        to_binary(minutes_tens, 3, minutes_col1);
        to_binary(minutes_units, 4, minutes_col2);

        // Seconds: split into tens and units for proper binary clock display
        int seconds_tens = seconds / 10;
        int seconds_units = seconds % 10;
        to_binary(seconds_tens, 3, seconds_col1);
        to_binary(seconds_units, 4, seconds_col2);

        // Display the clock
        system(CLEAR_COMMAND); // Clear screen (cross-platform)
        printf("Hours:\n");
        printf("10: ");
        display_binary(hours_tens_bin);
        printf("1: ");
        display_binary(hours_units_bin);

        printf("\nMinutes:\n");
        printf("10: ");
        display_binary(minutes_col1);
        printf("1: ");
        display_binary(minutes_col2);

        printf("\nSeconds:\n");
        printf("10: ");
        display_binary(seconds_col1);
        printf("1: ");
        display_binary(seconds_col2);

        SLEEP_FUNC(1); // Wait 1 second (cross-platform)
    }
    return 0;
}