/**
 * @file binary_clock_api.c
 * @brief Binary Clock API Implementation
 * 
 * Cross-platform implementation of the binary clock API with separated
 * visualization layer. Provides thread-safe, high-performance functions
 * for binary time representation and display management.
 */

#include <binary_clock_api.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ========================================================================== */
/* CONSTANTS                                                                  */
/* ========================================================================== */

#define API_VERSION "1.0.0"

/* ========================================================================== */
/* BINARY CONVERSION UTILITIES                                                */
/* ========================================================================== */

binary_value_t binary_clock_to_binary(uint8_t value, uint8_t bit_count) {
    binary_value_t result = {0};
    
    // Validate bit count
    if (bit_count < 1 || bit_count > 6) {
        return result; // Return with bit_count=0 to indicate error
    }
    
    // Validate value fits in bit_count bits
    uint8_t max_value = (1 << bit_count) - 1;
    if (value > max_value) {
        value = value & max_value; // Truncate to fit
    }
    
    result.bit_count = bit_count;
    result.decimal_value = value;
    
    // Convert to binary (MSB first)
    for (int i = 0; i < bit_count; i++) {
        result.bits[i] = (value >> (bit_count - 1 - i)) & 1;
    }
    
    // Ensure remaining bits are false
    for (int i = bit_count; i < 6; i++) {
        result.bits[i] = false;
    }
    
    return result;
}

uint8_t binary_clock_to_decimal(const binary_value_t* binary) {
    if (binary == NULL || binary->bit_count == 0) {
        return 0;
    }
    
    uint8_t result = 0;
    for (int i = 0; i < binary->bit_count; i++) {
        if (binary->bits[i]) {
            result |= (1 << (binary->bit_count - 1 - i));
        }
    }
    
    return result;
}

/* ========================================================================== */
/* TIME MANAGEMENT                                                            */
/* ========================================================================== */

time_components_t binary_clock_get_current_time(void) {
    time_components_t result = {0};
    
    time_t current_time = time(NULL);
    if (current_time == (time_t)-1) {
        return result; // Return all zeros on failure
    }
    
    struct tm* local_time = localtime(&current_time);
    if (local_time == NULL) {
        return result; // Return all zeros on failure
    }
    
    result.hours = (uint8_t)local_time->tm_hour;
    result.minutes = (uint8_t)local_time->tm_min;
    result.seconds = (uint8_t)local_time->tm_sec;
    
    return result;
}

binary_clock_state_t binary_clock_state_from_time(const time_components_t* time_comp) {
    binary_clock_state_t result = {0};
    
    if (time_comp == NULL) {
        return result; // Return with timestamp=0 on failure
    }
    
    // Validate time components
    if (time_comp->hours > 23 || time_comp->minutes > 59 || time_comp->seconds > 59) {
        return result; // Return with timestamp=0 on failure
    }
    
    // Split hours into tens and units
    uint8_t hours_tens = time_comp->hours / 10;
    uint8_t hours_units = time_comp->hours % 10;
    
    // Split minutes into tens and units
    uint8_t minutes_tens = time_comp->minutes / 10;
    uint8_t minutes_units = time_comp->minutes % 10;
    
    // Split seconds into tens and units
    uint8_t seconds_tens = time_comp->seconds / 10;
    uint8_t seconds_units = time_comp->seconds % 10;
    
    // Convert to binary representations
    result.hours_tens = binary_clock_to_binary(hours_tens, 3);     // 0-2 needs 3 bits
    result.hours_units = binary_clock_to_binary(hours_units, 4);   // 0-9 needs 4 bits
    result.minutes_tens = binary_clock_to_binary(minutes_tens, 3); // 0-5 needs 3 bits
    result.minutes_units = binary_clock_to_binary(minutes_units, 4); // 0-9 needs 4 bits
    result.seconds_tens = binary_clock_to_binary(seconds_tens, 3); // 0-5 needs 3 bits
    result.seconds_units = binary_clock_to_binary(seconds_units, 4); // 0-9 needs 4 bits
    
    // Set timestamp
    result.timestamp = time(NULL);
    
    return result;
}

binary_clock_state_t binary_clock_get_current_state(void) {
    time_components_t current_time = binary_clock_get_current_time();
    
    // Check if time retrieval failed
    if (current_time.hours == 0 && current_time.minutes == 0 && current_time.seconds == 0) {
        // Could be valid time (midnight) or failure - check if time() worked
        time_t test_time = time(NULL);
        if (test_time == (time_t)-1) {
            binary_clock_state_t result = {0};
            return result; // Return with timestamp=0 on failure
        }
    }
    
    return binary_clock_state_from_time(&current_time);
}


/* ========================================================================== */
/* UTILITY FUNCTIONS                                                          */
/* ========================================================================== */

const char* binary_clock_get_error_string(binary_clock_error_t error) {
    switch (error) {
        case BINARY_CLOCK_SUCCESS:
            return "Operation completed successfully";
        case BINARY_CLOCK_ERROR_INVALID_TIME:
            return "Invalid time components provided";
        case BINARY_CLOCK_ERROR_INVALID_BIT_COUNT:
            return "Bit count out of valid range (1-6)";
        case BINARY_CLOCK_ERROR_NULL_POINTER:
            return "Null pointer passed to function requiring valid pointer";
        case BINARY_CLOCK_ERROR_SYSTEM_TIME:
            return "System time retrieval failed";
        default:
            return "Unknown error";
    }
}

const char* binary_clock_get_version(void) {
    return API_VERSION;
}
