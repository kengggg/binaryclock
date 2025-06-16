/**
 * @file binary_clock_display.c
 * @brief Binary Clock Display Utilities Implementation
 * 
 * Implementation of visualization functions for binary clock data.
 * This module demonstrates the separation of concerns: it uses the
 * core API for all data access and focuses purely on presentation.
 */

#include <binary_clock_display.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* ========================================================================== */
/* DISPLAY CALLBACK SYSTEM (OPTIONAL)                                        */
/* ========================================================================== */

#define MAX_REGISTERED_DISPLAYS 16

/**
 * @brief Internal display registration entry
 */
typedef struct {
    binary_clock_display_fn_t display_fn;
    void* context;
    bool active;
    int id;
} display_entry_t;

/**
 * @brief Global display registry
 */
static display_entry_t display_registry[MAX_REGISTERED_DISPLAYS];
static int next_registration_id = 0;

int binary_clock_display_register(binary_clock_display_fn_t display_fn, void* context) {
    if (display_fn == NULL) {
        return -1;
    }
    
    // Find empty slot
    for (int i = 0; i < MAX_REGISTERED_DISPLAYS; i++) {
        if (!display_registry[i].active) {
            display_registry[i].display_fn = display_fn;
            display_registry[i].context = context;
            display_registry[i].active = true;
            display_registry[i].id = next_registration_id;
            return next_registration_id++;
        }
    }
    
    return -1; // No slots available
}

binary_clock_error_t binary_clock_display_unregister(int registration_id) {
    if (registration_id < 0) {
        return BINARY_CLOCK_ERROR_INVALID_TIME; // Invalid ID parameter
    }

    // Find entry matching the registration ID
    for (int i = 0; i < MAX_REGISTERED_DISPLAYS; i++) {
        if (display_registry[i].active && display_registry[i].id == registration_id) {
            display_registry[i].active = false;
            display_registry[i].display_fn = NULL;
            display_registry[i].context = NULL;
            display_registry[i].id = -1;
            return BINARY_CLOCK_SUCCESS;
        }
    }

    return BINARY_CLOCK_ERROR_INVALID_TIME; // ID not found
}

void binary_clock_display_update_all(void) {
    // Use core API to get current state
    binary_clock_state_t state = binary_clock_get_current_state();
    binary_clock_display_update_all_with_state(&state);
}

void binary_clock_display_update_all_with_state(const binary_clock_state_t* state) {
    if (state == NULL) {
        return; // Silently ignore null pointer
    }
    
    // Call all active display functions
    for (int i = 0; i < MAX_REGISTERED_DISPLAYS; i++) {
        if (display_registry[i].active && display_registry[i].display_fn != NULL) {
            display_registry[i].display_fn(state, display_registry[i].context);
        }
    }
}

/* ========================================================================== */
/* BUILT-IN DISPLAY FUNCTIONS                                                 */
/* ========================================================================== */

void binary_clock_display_console_emoji(const binary_clock_state_t* state, void* context) {
    (void)context; // Unused parameter
    
    if (state == NULL) {
        return;
    }
    
    // Calculate readable time from binary data
    int hours = state->hours_tens.decimal_value * 10 + state->hours_units.decimal_value;
    int minutes = state->minutes_tens.decimal_value * 10 + state->minutes_units.decimal_value;
    int seconds = state->seconds_tens.decimal_value * 10 + state->seconds_units.decimal_value;
    
    // Display header
    printf("🌝 Binary Clock 🌚\n");
    printf("Time: %02d:%02d:%02d\n", hours, minutes, seconds);
    printf("\n");
    
    // Hours
    printf("Hours   : ");
    for (int i = 0; i < state->hours_tens.bit_count; i++) {
        printf("%s", state->hours_tens.bits[i] ? "🌝" : "🌚");
    }
    printf(" ");
    for (int i = 0; i < state->hours_units.bit_count; i++) {
        printf("%s", state->hours_units.bits[i] ? "🌝" : "🌚");
    }
    printf("\n");
    
    // Minutes
    printf("Minutes : ");
    for (int i = 0; i < state->minutes_tens.bit_count; i++) {
        printf("%s", state->minutes_tens.bits[i] ? "🌝" : "🌚");
    }
    printf(" ");
    for (int i = 0; i < state->minutes_units.bit_count; i++) {
        printf("%s", state->minutes_units.bits[i] ? "🌝" : "🌚");
    }
    printf("\n");
    
    // Seconds
    printf("Seconds : ");
    for (int i = 0; i < state->seconds_tens.bit_count; i++) {
        printf("%s", state->seconds_tens.bits[i] ? "🌝" : "🌚");
    }
    printf(" ");
    for (int i = 0; i < state->seconds_units.bit_count; i++) {
        printf("%s", state->seconds_units.bits[i] ? "🌝" : "🌚");
    }
    printf("\n");
}

void binary_clock_display_console_ascii(const binary_clock_state_t* state, void* context) {
    (void)context; // Unused parameter
    
    if (state == NULL) {
        return;
    }
    
    // Calculate readable time from binary data
    int hours = state->hours_tens.decimal_value * 10 + state->hours_units.decimal_value;
    int minutes = state->minutes_tens.decimal_value * 10 + state->minutes_units.decimal_value;
    int seconds = state->seconds_tens.decimal_value * 10 + state->seconds_units.decimal_value;
    
    // Display header
    printf("Binary Clock (ASCII)\n");
    printf("Time: %02d:%02d:%02d\n", hours, minutes, seconds);
    printf("\n");
    
    // Hours
    printf("Hours   : ");
    for (int i = 0; i < state->hours_tens.bit_count; i++) {
        printf("%d", state->hours_tens.bits[i] ? 1 : 0);
    }
    printf(" ");
    for (int i = 0; i < state->hours_units.bit_count; i++) {
        printf("%d", state->hours_units.bits[i] ? 1 : 0);
    }
    printf("\n");
    
    // Minutes
    printf("Minutes : ");
    for (int i = 0; i < state->minutes_tens.bit_count; i++) {
        printf("%d", state->minutes_tens.bits[i] ? 1 : 0);
    }
    printf(" ");
    for (int i = 0; i < state->minutes_units.bit_count; i++) {
        printf("%d", state->minutes_units.bits[i] ? 1 : 0);
    }
    printf("\n");
    
    // Seconds
    printf("Seconds : ");
    for (int i = 0; i < state->seconds_tens.bit_count; i++) {
        printf("%d", state->seconds_tens.bits[i] ? 1 : 0);
    }
    printf(" ");
    for (int i = 0; i < state->seconds_units.bit_count; i++) {
        printf("%d", state->seconds_units.bits[i] ? 1 : 0);
    }
    printf("\n");
}

void binary_clock_display_json(const binary_clock_state_t* state, void* context) {
    FILE* output = (FILE*)context;
    if (output == NULL) {
        output = stdout;
    }
    
    if (state == NULL) {
        return;
    }
    
    // Calculate readable time from binary data
    int hours = state->hours_tens.decimal_value * 10 + state->hours_units.decimal_value;
    int minutes = state->minutes_tens.decimal_value * 10 + state->minutes_units.decimal_value;
    int seconds = state->seconds_tens.decimal_value * 10 + state->seconds_units.decimal_value;
    
    fprintf(output, "{\n");
    fprintf(output, "  \"timestamp\": %ld,\n", (long)state->timestamp);
    fprintf(output, "  \"time\": \"%02d:%02d:%02d\",\n", hours, minutes, seconds);
    fprintf(output, "  \"binary\": {\n");
    
    // Hours
    fprintf(output, "    \"hours\": {\n");
    fprintf(output, "      \"tens\": [");
    for (int i = 0; i < state->hours_tens.bit_count; i++) {
        fprintf(output, "%s%d", (i > 0) ? "," : "", state->hours_tens.bits[i] ? 1 : 0);
    }
    fprintf(output, "],\n");
    fprintf(output, "      \"units\": [");
    for (int i = 0; i < state->hours_units.bit_count; i++) {
        fprintf(output, "%s%d", (i > 0) ? "," : "", state->hours_units.bits[i] ? 1 : 0);
    }
    fprintf(output, "]\n");
    fprintf(output, "    },\n");
    
    // Minutes
    fprintf(output, "    \"minutes\": {\n");
    fprintf(output, "      \"tens\": [");
    for (int i = 0; i < state->minutes_tens.bit_count; i++) {
        fprintf(output, "%s%d", (i > 0) ? "," : "", state->minutes_tens.bits[i] ? 1 : 0);
    }
    fprintf(output, "],\n");
    fprintf(output, "      \"units\": [");
    for (int i = 0; i < state->minutes_units.bit_count; i++) {
        fprintf(output, "%s%d", (i > 0) ? "," : "", state->minutes_units.bits[i] ? 1 : 0);
    }
    fprintf(output, "]\n");
    fprintf(output, "    },\n");
    
    // Seconds
    fprintf(output, "    \"seconds\": {\n");
    fprintf(output, "      \"tens\": [");
    for (int i = 0; i < state->seconds_tens.bit_count; i++) {
        fprintf(output, "%s%d", (i > 0) ? "," : "", state->seconds_tens.bits[i] ? 1 : 0);
    }
    fprintf(output, "],\n");
    fprintf(output, "      \"units\": [");
    for (int i = 0; i < state->seconds_units.bit_count; i++) {
        fprintf(output, "%s%d", (i > 0) ? "," : "", state->seconds_units.bits[i] ? 1 : 0);
    }
    fprintf(output, "]\n");
    fprintf(output, "    }\n");
    fprintf(output, "  }\n");
    fprintf(output, "}\n");
}

void binary_clock_display_compact(const binary_clock_state_t* state, void* context) {
    (void)context; // Unused parameter
    
    if (state == NULL) {
        return;
    }
    
    // Calculate readable time from binary data
    int hours = state->hours_tens.decimal_value * 10 + state->hours_units.decimal_value;
    int minutes = state->minutes_tens.decimal_value * 10 + state->minutes_units.decimal_value;
    int seconds = state->seconds_tens.decimal_value * 10 + state->seconds_units.decimal_value;
    
    printf("%02d:%02d:%02d [", hours, minutes, seconds);
    
    // Hours
    for (int i = 0; i < state->hours_tens.bit_count; i++) {
        printf("%d", state->hours_tens.bits[i] ? 1 : 0);
    }
    printf(" ");
    for (int i = 0; i < state->hours_units.bit_count; i++) {
        printf("%d", state->hours_units.bits[i] ? 1 : 0);
    }
    printf(" : ");
    
    // Minutes
    for (int i = 0; i < state->minutes_tens.bit_count; i++) {
        printf("%d", state->minutes_tens.bits[i] ? 1 : 0);
    }
    printf(" ");
    for (int i = 0; i < state->minutes_units.bit_count; i++) {
        printf("%d", state->minutes_units.bits[i] ? 1 : 0);
    }
    printf(" : ");
    
    // Seconds
    for (int i = 0; i < state->seconds_tens.bit_count; i++) {
        printf("%d", state->seconds_tens.bits[i] ? 1 : 0);
    }
    printf(" ");
    for (int i = 0; i < state->seconds_units.bit_count; i++) {
        printf("%d", state->seconds_units.bits[i] ? 1 : 0);
    }
    printf("]\n");
}

/* ========================================================================== */
/* HELPER UTILITIES                                                           */
/* ========================================================================== */

bool binary_clock_display_get_time_string(const binary_clock_state_t* state, char* buffer, size_t buffer_size) {
    if (state == NULL || buffer == NULL || buffer_size < 9) {
        return false;
    }
    
    // Calculate readable time from binary data
    int hours = state->hours_tens.decimal_value * 10 + state->hours_units.decimal_value;
    int minutes = state->minutes_tens.decimal_value * 10 + state->minutes_units.decimal_value;
    int seconds = state->seconds_tens.decimal_value * 10 + state->seconds_units.decimal_value;
    
    snprintf(buffer, buffer_size, "%02d:%02d:%02d", hours, minutes, seconds);
    return true;
}

bool binary_clock_display_binary_to_string(const binary_value_t* binary_val, char* buffer, size_t buffer_size, char format) {
    if (binary_val == NULL || buffer == NULL || binary_val->bit_count == 0) {
        return false;
    }
    
    switch (format) {
        case '0':
        case '1': // Binary format
            if (buffer_size < binary_val->bit_count + 1) {
                return false;
            }
            for (int i = 0; i < binary_val->bit_count; i++) {
                buffer[i] = binary_val->bits[i] ? '1' : '0';
            }
            buffer[binary_val->bit_count] = '\0';
            break;
            
        case 'e': // Emoji format
            if (buffer_size < (binary_val->bit_count * 4) + 1) { // 4 bytes per emoji
                return false;
            }
            buffer[0] = '\0';
            for (int i = 0; i < binary_val->bit_count; i++) {
                strcat(buffer, binary_val->bits[i] ? "🌝" : "🌚");
            }
            break;
            
        case 'd': // Decimal format
            if (buffer_size < 4) { // Max "255\0"
                return false;
            }
            snprintf(buffer, buffer_size, "%d", binary_val->decimal_value);
            break;
            
        default:
            return false;
    }
    
    return true;
}
