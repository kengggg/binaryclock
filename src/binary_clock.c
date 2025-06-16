#include <stdio.h>    // For printf
#include <stdlib.h>   // For exit
#include <signal.h>   // For signal handling
#include <string.h>   // For string comparison
#include <binary_clock_api.h>     // Core API (data only)
#include <binary_clock_display.h> // Display utilities

// Cross-platform compatibility
#ifdef _WIN32
    #include <windows.h>  // For Sleep and WinAPI console functions
    #define SLEEP_FUNC(x) Sleep((x) * 1000)  // Windows Sleep uses milliseconds
#else
    #include <unistd.h>   // For sleep on Unix-like systems
    #define SLEEP_FUNC(x) sleep(x)  // Unix sleep uses seconds
#endif

// Cross-platform console clear
static void clear_console(void) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD count;
    COORD homeCoords = {0, 0};

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
#else
    // ANSI escape codes to clear the screen and move the cursor to the top-left
    printf("\033[2J\033[H");
    fflush(stdout);
#endif
}

// Display mode enumeration
typedef enum {
    DISPLAY_EMOJI,   // Moon emojis (default)
    DISPLAY_BINARY,  // 0s and 1s
    DISPLAY_JSON,    // JSON format
    DISPLAY_RAW      // Raw API data structures
} display_mode_t;

// Operation mode enumeration
typedef enum {
    MODE_SINGLE,     // Output once and exit (default)
    MODE_LOOP        // Continuous loop
} operation_mode_t;

// Configuration structure
typedef struct {
    display_mode_t display_mode;
    operation_mode_t operation_mode;
} config_t;

// Signal handler for graceful exit
void signal_handler(int sig) {
    (void)sig; // Suppress unused parameter warning
    printf("\n\nBinary clock stopped.\n");
    exit(0);
}

// Raw API display function
void binary_clock_display_raw_api(const binary_clock_state_t* state, void* context) {
    (void)context; // Unused parameter
    
    if (state == NULL) {
        return;
    }
    
    printf("Binary Clock API Raw Data\n");
    printf("=========================\n");
    printf("Timestamp: %ld\n", (long)state->timestamp);
    printf("\n");
    
    printf("Hours Tens:   bit_count=%d, decimal_value=%d, bits=[", 
           state->hours_tens.bit_count, state->hours_tens.decimal_value);
    for (int i = 0; i < state->hours_tens.bit_count; i++) {
        printf("%s%d", (i > 0) ? "," : "", state->hours_tens.bits[i] ? 1 : 0);
    }
    printf("]\n");
    
    printf("Hours Units:  bit_count=%d, decimal_value=%d, bits=[", 
           state->hours_units.bit_count, state->hours_units.decimal_value);
    for (int i = 0; i < state->hours_units.bit_count; i++) {
        printf("%s%d", (i > 0) ? "," : "", state->hours_units.bits[i] ? 1 : 0);
    }
    printf("]\n");
    
    printf("Minutes Tens: bit_count=%d, decimal_value=%d, bits=[", 
           state->minutes_tens.bit_count, state->minutes_tens.decimal_value);
    for (int i = 0; i < state->minutes_tens.bit_count; i++) {
        printf("%s%d", (i > 0) ? "," : "", state->minutes_tens.bits[i] ? 1 : 0);
    }
    printf("]\n");
    
    printf("Minutes Units:bit_count=%d, decimal_value=%d, bits=[", 
           state->minutes_units.bit_count, state->minutes_units.decimal_value);
    for (int i = 0; i < state->minutes_units.bit_count; i++) {
        printf("%s%d", (i > 0) ? "," : "", state->minutes_units.bits[i] ? 1 : 0);
    }
    printf("]\n");
    
    printf("Seconds Tens: bit_count=%d, decimal_value=%d, bits=[", 
           state->seconds_tens.bit_count, state->seconds_tens.decimal_value);
    for (int i = 0; i < state->seconds_tens.bit_count; i++) {
        printf("%s%d", (i > 0) ? "," : "", state->seconds_tens.bits[i] ? 1 : 0);
    }
    printf("]\n");
    
    printf("Seconds Units:bit_count=%d, decimal_value=%d, bits=[", 
           state->seconds_units.bit_count, state->seconds_units.decimal_value);
    for (int i = 0; i < state->seconds_units.bit_count; i++) {
        printf("%s%d", (i > 0) ? "," : "", state->seconds_units.bits[i] ? 1 : 0);
    }
    printf("]\n");
}

// Display usage information
void print_usage(const char* program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("\n");
    printf("Options:\n");
    printf("  --display MODE    Set display mode (emoji, binary, json, raw)\n");
    printf("                    emoji:  Moon emojis 🌚🌝 (default)\n");
    printf("                    binary: 0s and 1s\n");
    printf("                    json:   JSON format\n");
    printf("                    raw:    Raw API data structures\n");
    printf("  --loop            Run continuously (default: single output)\n");
    printf("  --help, -h        Show this help message\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s                          # Single emoji output\n", program_name);
    printf("  %s --loop                   # Continuous emoji display\n", program_name);
    printf("  %s --display=binary         # Single binary output\n", program_name);
    printf("  %s --display=json --loop    # Continuous JSON output\n", program_name);
}

// Parse command line arguments
config_t parse_arguments(int argc, char* argv[]) {
    config_t config = {
        .display_mode = DISPLAY_EMOJI,  // Default to emoji
        .operation_mode = MODE_SINGLE   // Default to single output
    };
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            exit(0);
        }
        else if (strcmp(argv[i], "--loop") == 0) {
            config.operation_mode = MODE_LOOP;
        }
        else if (strncmp(argv[i], "--display=", 10) == 0) {
            const char* mode = argv[i] + 10;
            if (strcmp(mode, "emoji") == 0) {
                config.display_mode = DISPLAY_EMOJI;
            }
            else if (strcmp(mode, "binary") == 0) {
                config.display_mode = DISPLAY_BINARY;
            }
            else if (strcmp(mode, "json") == 0) {
                config.display_mode = DISPLAY_JSON;
            }
            else if (strcmp(mode, "raw") == 0) {
                config.display_mode = DISPLAY_RAW;
            }
            else {
                fprintf(stderr, "Error: Unknown display mode '%s'\n", mode);
                fprintf(stderr, "Valid modes: emoji, binary, json, raw\n");
                exit(1);
            }
        }
        else {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
            fprintf(stderr, "Use --help for usage information\n");
            exit(1);
        }
    }
    
    return config;
}

// Get display function based on mode
binary_clock_display_fn_t get_display_function(display_mode_t mode) {
    switch (mode) {
        case DISPLAY_EMOJI:
            return binary_clock_display_console_emoji;
        case DISPLAY_BINARY:
            return binary_clock_display_console_ascii;
        case DISPLAY_JSON:
            return binary_clock_display_json;
        case DISPLAY_RAW:
            return binary_clock_display_raw_api;
        default:
            return binary_clock_display_console_emoji;
    }
}

int main(int argc, char* argv[]) {
    // Parse command line arguments
    config_t config = parse_arguments(argc, argv);
    
    // Set up signal handler for graceful exit (Ctrl+C) - only needed for loop mode
    if (config.operation_mode == MODE_LOOP) {
        signal(SIGINT, signal_handler);
    }
    
    // Get the appropriate display function
    binary_clock_display_fn_t display_fn = get_display_function(config.display_mode);
    
    if (config.operation_mode == MODE_SINGLE) {
        // Single output mode: get current state and display once
        binary_clock_state_t state = binary_clock_get_current_state();
        if (state.timestamp == 0) {
            fprintf(stderr, "Error: Failed to get current time\n");
            return 1;
        }
        display_fn(&state, NULL);
    }
    else {
        // Loop mode: continuous display
        printf("🌚🌝 Binary Clock v%s 🌝🌚\n", binary_clock_get_version());
        printf("Press Ctrl+C to exit\n\n");
        
        // Register the selected display function
        int display_id = binary_clock_display_register(display_fn, NULL);
        if (display_id == -1) {
            printf("Error: Failed to register display function\n");
            return 1;
        }
        
        while (1) { // Infinite loop to keep clock running
            // Clear screen (cross-platform) - only for non-JSON mode to avoid cluttering
            if (config.display_mode != DISPLAY_JSON) {
                clear_console();
            }
            
            // Update all registered displays with current time
            binary_clock_display_update_all();
            
            SLEEP_FUNC(1); // Wait 1 second (cross-platform)
        }
    }
    
    return 0;
}
