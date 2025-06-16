/**
 * @file binary_clock_api.h
 * @brief Binary Clock Core API - Pure data and conversion functions
 * @version 1.0.0
 * 
 * This API provides core binary clock functionality with complete separation
 * from visualization. It only handles time data conversion and binary 
 * representation, allowing any visualization layer to render the data
 * in any format (LEDs, emojis, graphics, etc.).
 * 
 * Key Features:
 * - Thread-safe operations
 * - No dynamic memory allocation
 * - C99 standard compatibility
 * - Pure data API (no visualization)
 * - High-performance state queries (< 1ms)
 */

#ifndef BINARY_CLOCK_API_H
#define BINARY_CLOCK_API_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/* ERROR HANDLING                                                             */
/* ========================================================================== */

/**
 * @brief Error codes returned by API functions
 */
typedef enum {
    BINARY_CLOCK_SUCCESS = 0,              /**< Operation completed successfully */
    BINARY_CLOCK_ERROR_INVALID_TIME = 1,   /**< Invalid time components provided */
    BINARY_CLOCK_ERROR_INVALID_BIT_COUNT = 2, /**< Bit count out of valid range (1-6) */
    BINARY_CLOCK_ERROR_NULL_POINTER = 3,   /**< Null pointer passed to function requiring valid pointer */
    BINARY_CLOCK_ERROR_SYSTEM_TIME = 4     /**< System time retrieval failed */
} binary_clock_error_t;

/* ========================================================================== */
/* CORE DATA STRUCTURES                                                       */
/* ========================================================================== */

/**
 * @brief Represents a binary value with configurable bit count
 * 
 * This structure stores a decimal value as both its original form
 * and as a binary representation with a specified number of bits.
 * Memory layout: 8 bytes total, 1-byte aligned
 */
typedef struct {
    uint8_t bit_count;      /**< Number of significant bits (1-6) */
    bool bits[6];           /**< Bit array (MSB first, remaining bits are false) */
    uint8_t decimal_value;  /**< Original decimal value for reference (0-63) */
} binary_value_t;

/**
 * @brief Standard time representation
 * 
 * Simple time components structure for input/output operations.
 * Memory layout: 3 bytes total, 1-byte aligned
 */
typedef struct {
    uint8_t hours;    /**< Hours in 24-hour format (0-23) */
    uint8_t minutes;  /**< Minutes (0-59) */
    uint8_t seconds;  /**< Seconds (0-59) */
} time_components_t;

/**
 * @brief Complete binary clock state representation
 * 
 * This structure contains the full state of a binary clock, with each
 * time component split into tens and units digits and converted to binary.
 * Memory layout: 56 bytes total, 8-byte aligned
 */
typedef struct {
    binary_value_t hours_tens;     /**< Hours tens digit (0-2, 3 bits) */
    binary_value_t hours_units;    /**< Hours units digit (0-9, 4 bits) */
    binary_value_t minutes_tens;   /**< Minutes tens digit (0-5, 3 bits) */
    binary_value_t minutes_units;  /**< Minutes units digit (0-9, 4 bits) */
    binary_value_t seconds_tens;   /**< Seconds tens digit (0-5, 3 bits) */
    binary_value_t seconds_units;  /**< Seconds units digit (0-9, 4 bits) */
    time_t timestamp;              /**< Unix timestamp when state was created */
} binary_clock_state_t;


/* ========================================================================== */
/* CORE API FUNCTIONS                                                         */
/* ========================================================================== */

/**
 * @brief Get current binary clock state using system time
 * 
 * Retrieves the current system time and converts it to binary clock format.
 * This function is thread-safe and performs no heap allocation.
 * 
 * @return Binary clock state for current time
 * @note On failure, returns state with timestamp=0
 * @performance Typical execution: 50μs, max: 1ms
 */
binary_clock_state_t binary_clock_get_current_state(void);

/**
 * @brief Create binary clock state from specific time
 * 
 * Converts the provided time components to binary clock format.
 * Input validation ensures hours (0-23), minutes (0-59), seconds (0-59).
 * 
 * @param time_comp Time components to convert (must not be NULL)
 * @return Binary clock state for specified time
 * @note On failure, returns state with timestamp=0
 * @performance Typical execution: 10μs, max: 1ms
 */
binary_clock_state_t binary_clock_state_from_time(const time_components_t* time_comp);

/**
 * @brief Create time components from current system time
 * 
 * Retrieves current system time and returns it as time components.
 * This is a convenience function for getting current time without binary conversion.
 * 
 * @return Current time as components
 * @note On failure, returns time with all fields = 0
 * @performance Typical execution: 20μs, max: 1ms
 */
time_components_t binary_clock_get_current_time(void);

/* ========================================================================== */
/* BINARY CONVERSION UTILITIES                                                */
/* ========================================================================== */

/**
 * @brief Convert decimal number to binary representation
 * 
 * Converts a decimal value to binary format with the specified number of bits.
 * Values that exceed the bit count will be truncated to fit.
 * 
 * @param value Decimal value to convert (0-63)
 * @param bit_count Number of bits to represent (1-6)
 * @return Binary representation
 * @note On failure, returns binary_value with bit_count=0
 * @performance Typical execution: 5μs, max: 1ms
 */
binary_value_t binary_clock_to_binary(uint8_t value, uint8_t bit_count);

/**
 * @brief Convert binary representation back to decimal
 * 
 * Extracts the decimal value from a binary representation.
 * Uses the bit_count field to determine significant bits.
 * 
 * @param binary Binary representation to convert (must not be NULL)
 * @return Decimal value
 * @note On failure (NULL pointer), returns 0
 * @performance Typical execution: 5μs, max: 1ms
 */
uint8_t binary_clock_to_decimal(const binary_value_t* binary);


/* ========================================================================== */
/* UTILITY FUNCTIONS                                                          */
/* ========================================================================== */

/**
 * @brief Get human-readable error message
 * 
 * Returns a descriptive string for the given error code.
 * The returned string is static and should not be freed.
 * 
 * @param error Error code to get message for
 * @return Human-readable error description (never NULL)
 */
const char* binary_clock_get_error_string(binary_clock_error_t error);

/**
 * @brief Get API version information
 * 
 * Returns the API version string in semantic versioning format.
 * The returned string is static and should not be freed.
 * 
 * @return Version string in semver format (never NULL)
 */
const char* binary_clock_get_version(void);

#ifdef __cplusplus
}
#endif

#endif /* BINARY_CLOCK_API_H */
