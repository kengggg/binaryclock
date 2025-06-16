/**
 * @file binary_clock_display.h
 * @brief Binary Clock Display Utilities - Visualization layer
 * @version 1.0.0
 * 
 * This module provides visualization utilities for binary clock data.
 * It uses the core binary clock API to get data and renders it in
 * various formats (emoji, ASCII, JSON, etc.).
 * 
 * This demonstrates the separation between core logic (in binary_clock_api.h)
 * and visualization. The core API provides only raw binary data, while
 * this module interprets that data for human-readable display.
 * 
 * Key Features:
 * - Multiple display formats (emoji, ASCII, JSON)
 * - Optional display callback system
 * - Example implementations for reference
 * - Uses core API for all data access
 */

#ifndef BINARY_CLOCK_DISPLAY_H
#define BINARY_CLOCK_DISPLAY_H

#include <binary_clock_api.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/* DISPLAY CALLBACK SYSTEM (OPTIONAL)                                        */
/* ========================================================================== */

/**
 * @brief Display callback function signature
 * 
 * Display functions receive the current binary clock state and optional
 * context data for rendering. Functions should not modify the state.
 * 
 * @param state Current binary clock state (never NULL)
 * @param context User-provided context data (may be NULL)
 */
typedef void (*binary_clock_display_fn_t)(const binary_clock_state_t* state, void* context);

/**
 * @brief Register a display callback
 * 
 * Registers a display function to be called when displays are updated.
 * Multiple displays can be registered (maximum 16). The context parameter
 * will be passed to the display function on each call.
 * 
 * @param display_fn Display function to register (must not be NULL)
 * @param context Context data to pass to display function (may be NULL)
 * @return Registration ID for later removal, -1 on failure
 * @note This is an optional convenience feature
 */
int binary_clock_display_register(binary_clock_display_fn_t display_fn, void* context);

/**
 * @brief Remove a previously registered display callback
 * 
 * Unregisters a display callback using the ID returned by register function.
 * Invalid IDs are silently ignored.
 * 
 * @param registration_id Registration ID returned by register function (>= 0)
 * @return BINARY_CLOCK_SUCCESS or error code
 */
binary_clock_error_t binary_clock_display_unregister(int registration_id);

/**
 * @brief Update all registered displays with current state
 * 
 * Retrieves current system time using core API and calls all registered 
 * display functions with the current binary clock state.
 */
void binary_clock_display_update_all(void);

/**
 * @brief Update all registered displays with specific state
 * 
 * Calls all registered display functions with the provided state.
 * NULL pointer is silently ignored.
 * 
 * @param state State to display (may be NULL)
 */
void binary_clock_display_update_all_with_state(const binary_clock_state_t* state);

/* ========================================================================== */
/* BUILT-IN DISPLAY FUNCTIONS                                                 */
/* ========================================================================== */

/**
 * @brief Console emoji display (🌚🌝 format)
 * 
 * Displays binary clock state using moon emojis to stdout.
 * Format: 🌚 for 0 bits, 🌝 for 1 bits
 * Not thread-safe due to stdout usage.
 * 
 * Example usage:
 * @code
 * binary_clock_state_t state = binary_clock_get_current_state();
 * binary_clock_display_console_emoji(&state, NULL);
 * @endcode
 * 
 * @param state Binary clock state to display (must not be NULL)
 * @param context Unused (pass NULL)
 */
void binary_clock_display_console_emoji(const binary_clock_state_t* state, void* context);

/**
 * @brief Console ASCII display (0/1 format)
 * 
 * Displays binary clock state using ASCII characters to stdout.
 * Format: 0 for off bits, 1 for on bits
 * Not thread-safe due to stdout usage.
 * 
 * Example usage:
 * @code
 * binary_clock_state_t state = binary_clock_get_current_state();
 * binary_clock_display_console_ascii(&state, NULL);
 * @endcode
 * 
 * @param state Binary clock state to display (must not be NULL)
 * @param context Unused (pass NULL)
 */
void binary_clock_display_console_ascii(const binary_clock_state_t* state, void* context);

/**
 * @brief JSON format display
 * 
 * Outputs binary clock state as JSON to the specified context or stdout.
 * The JSON includes timestamp, readable time, and bit arrays.
 * Not thread-safe due to output stream usage.
 * 
 * Example usage:
 * @code
 * binary_clock_state_t state = binary_clock_get_current_state();
 * binary_clock_display_json(&state, NULL);  // Output to stdout
 * 
 * FILE* file = fopen("time.json", "w");
 * binary_clock_display_json(&state, file);  // Output to file
 * fclose(file);
 * @endcode
 * 
 * @param state Binary clock state to display (must not be NULL)
 * @param context FILE* or buffer to write to (stdout if NULL)
 */
void binary_clock_display_json(const binary_clock_state_t* state, void* context);

/**
 * @brief Compact one-line display for debugging
 * 
 * Displays binary clock state in a compact format suitable for logs.
 * Format: "HH:MM:SS [001 0010 : 011 0100 : 101 0110]"
 * 
 * Example usage:
 * @code
 * binary_clock_state_t state = binary_clock_get_current_state();
 * binary_clock_display_compact(&state, NULL);
 * @endcode
 * 
 * @param state Binary clock state to display (must not be NULL)
 * @param context Unused (pass NULL)
 */
void binary_clock_display_compact(const binary_clock_state_t* state, void* context);

/* ========================================================================== */
/* HELPER UTILITIES                                                           */
/* ========================================================================== */

/**
 * @brief Get readable time string from binary clock state
 * 
 * Converts binary clock state back to readable time format.
 * This is a utility function that demonstrates how to extract
 * readable time from the binary representation.
 * 
 * @param state Binary clock state (must not be NULL)
 * @param buffer Buffer to write time string (minimum 9 bytes: "HH:MM:SS\0")
 * @param buffer_size Size of the buffer
 * @return true if successful, false if buffer too small or state invalid
 */
bool binary_clock_display_get_time_string(const binary_clock_state_t* state, char* buffer, size_t buffer_size);

/**
 * @brief Convert binary_value_t to string representation
 * 
 * Converts a binary value to string format for display purposes.
 * This demonstrates how visualization layers can interpret raw binary data.
 * 
 * @param binary_val Binary value to convert (must not be NULL)
 * @param buffer Buffer to write string (minimum bit_count + 1 bytes)
 * @param buffer_size Size of the buffer
 * @param format '0'/'1' for binary, 'e' for emoji, 'd' for decimal
 * @return true if successful, false if buffer too small or invalid format
 */
bool binary_clock_display_binary_to_string(const binary_value_t* binary_val, char* buffer, size_t buffer_size, char format);

#ifdef __cplusplus
}
#endif

#endif /* BINARY_CLOCK_DISPLAY_H */
