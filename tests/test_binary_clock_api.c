/**
 * @file test_binary_clock_api.c
 * @brief Comprehensive test suite for the Binary Clock API
 * 
 * Tests all functions in the new separated API to ensure correct
 * functionality and adherence to specifications.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <binary_clock_api.h>

// Test counters
static int tests_run = 0;
static int tests_passed = 0;

// Test utility macros
#define ASSERT_EQ(actual, expected, message) \
    do { \
        tests_run++; \
        if ((actual) == (expected)) { \
            tests_passed++; \
            printf("✓ Test %d passed: %s\n", tests_run, message); \
        } else { \
            printf("✗ Test %d failed: %s (expected %d, got %d)\n", tests_run, message, (int)(expected), (int)(actual)); \
        } \
    } while(0)

#define ASSERT_TRUE(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            printf("✓ Test %d passed: %s\n", tests_run, message); \
        } else { \
            printf("✗ Test %d failed: %s\n", tests_run, message); \
        } \
    } while(0)

#define ASSERT_STR_EQ(actual, expected, message) \
    do { \
        tests_run++; \
        if (strcmp((actual), (expected)) == 0) { \
            tests_passed++; \
            printf("✓ Test %d passed: %s\n", tests_run, message); \
        } else { \
            printf("✗ Test %d failed: %s (expected '%s', got '%s')\n", tests_run, message, expected, actual); \
        } \
    } while(0)

// Test binary conversion functions
void test_binary_conversion(void) {
    printf("\n=== Testing Binary Conversion Functions ===\n");
    
    // Test binary_clock_to_binary with various values
    binary_value_t result = binary_clock_to_binary(7, 4);
    ASSERT_EQ(result.bit_count, 4, "binary_clock_to_binary bit_count");
    ASSERT_EQ(result.decimal_value, 7, "binary_clock_to_binary decimal_value");
    ASSERT_TRUE(result.bits[0] == false && result.bits[1] == true && 
                result.bits[2] == true && result.bits[3] == true, 
                "binary_clock_to_binary bits for 7 (0111)");
    
    // Test edge cases
    result = binary_clock_to_binary(0, 3);
    ASSERT_EQ(result.bit_count, 3, "binary_clock_to_binary zero value bit_count");
    ASSERT_TRUE(result.bits[0] == false && result.bits[1] == false && result.bits[2] == false, 
                "binary_clock_to_binary bits for 0 (000)");
    
    result = binary_clock_to_binary(63, 6);
    ASSERT_EQ(result.bit_count, 6, "binary_clock_to_binary max value bit_count");
    ASSERT_TRUE(result.bits[0] && result.bits[1] && result.bits[2] && 
                result.bits[3] && result.bits[4] && result.bits[5], 
                "binary_clock_to_binary bits for 63 (111111)");
    
    // Test invalid bit count
    result = binary_clock_to_binary(5, 0);
    ASSERT_EQ(result.bit_count, 0, "binary_clock_to_binary invalid bit_count");
    
    result = binary_clock_to_binary(5, 7);
    ASSERT_EQ(result.bit_count, 0, "binary_clock_to_binary bit_count too large");
    
    // Test binary_clock_to_decimal
    binary_value_t binary_val = {4, {false, true, true, true, false, false}, 7};
    uint8_t decimal = binary_clock_to_decimal(&binary_val);
    ASSERT_EQ(decimal, 7, "binary_clock_to_decimal conversion");
    
    binary_val = (binary_value_t){3, {false, false, false, false, false, false}, 0};
    decimal = binary_clock_to_decimal(&binary_val);
    ASSERT_EQ(decimal, 0, "binary_clock_to_decimal zero value");
    
    // Test null pointer
    decimal = binary_clock_to_decimal(NULL);
    ASSERT_EQ(decimal, 0, "binary_clock_to_decimal null pointer");
}

// Test time management functions
void test_time_management(void) {
    printf("\n=== Testing Time Management Functions ===\n");
    
    // Test binary_clock_state_from_time with valid time
    time_components_t test_time = {14, 30, 45}; // 14:30:45
    binary_clock_state_t state = binary_clock_state_from_time(&test_time);
    
    ASSERT_EQ(state.hours_tens.decimal_value, 1, "hours_tens for 14:30:45");
    ASSERT_EQ(state.hours_units.decimal_value, 4, "hours_units for 14:30:45");
    ASSERT_EQ(state.minutes_tens.decimal_value, 3, "minutes_tens for 14:30:45");
    ASSERT_EQ(state.minutes_units.decimal_value, 0, "minutes_units for 14:30:45");
    ASSERT_EQ(state.seconds_tens.decimal_value, 4, "seconds_tens for 14:30:45");
    ASSERT_EQ(state.seconds_units.decimal_value, 5, "seconds_units for 14:30:45");
    
    ASSERT_EQ(state.hours_tens.bit_count, 3, "hours_tens bit_count");
    ASSERT_EQ(state.hours_units.bit_count, 4, "hours_units bit_count");
    ASSERT_EQ(state.minutes_tens.bit_count, 3, "minutes_tens bit_count");
    ASSERT_EQ(state.minutes_units.bit_count, 4, "minutes_units bit_count");
    ASSERT_EQ(state.seconds_tens.bit_count, 3, "seconds_tens bit_count");
    ASSERT_EQ(state.seconds_units.bit_count, 4, "seconds_units bit_count");
    
    // Test edge cases
    test_time = (time_components_t){0, 0, 0}; // Midnight
    state = binary_clock_state_from_time(&test_time);
    ASSERT_EQ(state.hours_tens.decimal_value, 0, "midnight hours_tens");
    ASSERT_EQ(state.hours_units.decimal_value, 0, "midnight hours_units");
    
    test_time = (time_components_t){23, 59, 59}; // Just before midnight
    state = binary_clock_state_from_time(&test_time);
    ASSERT_EQ(state.hours_tens.decimal_value, 2, "23:59:59 hours_tens");
    ASSERT_EQ(state.hours_units.decimal_value, 3, "23:59:59 hours_units");
    ASSERT_EQ(state.minutes_tens.decimal_value, 5, "23:59:59 minutes_tens");
    ASSERT_EQ(state.minutes_units.decimal_value, 9, "23:59:59 minutes_units");
    ASSERT_EQ(state.seconds_tens.decimal_value, 5, "23:59:59 seconds_tens");
    ASSERT_EQ(state.seconds_units.decimal_value, 9, "23:59:59 seconds_units");
    
    // Test invalid time
    test_time = (time_components_t){25, 30, 45}; // Invalid hours
    state = binary_clock_state_from_time(&test_time);
    ASSERT_EQ(state.timestamp, 0, "invalid hours returns timestamp=0");
    
    test_time = (time_components_t){14, 60, 45}; // Invalid minutes
    state = binary_clock_state_from_time(&test_time);
    ASSERT_EQ(state.timestamp, 0, "invalid minutes returns timestamp=0");
    
    test_time = (time_components_t){14, 30, 60}; // Invalid seconds
    state = binary_clock_state_from_time(&test_time);
    ASSERT_EQ(state.timestamp, 0, "invalid seconds returns timestamp=0");
    
    // Test null pointer
    state = binary_clock_state_from_time(NULL);
    ASSERT_EQ(state.timestamp, 0, "null pointer returns timestamp=0");
    
    // Test binary_clock_get_current_time (basic test)
    time_components_t current_time = binary_clock_get_current_time();
    ASSERT_TRUE(current_time.hours <= 23, "current time hours valid");
    ASSERT_TRUE(current_time.minutes <= 59, "current time minutes valid");
    ASSERT_TRUE(current_time.seconds <= 59, "current time seconds valid");
    
    // Test binary_clock_get_current_state (basic test)
    binary_clock_state_t current_state = binary_clock_get_current_state();
    ASSERT_TRUE(current_state.timestamp > 0, "current state has valid timestamp");
    ASSERT_TRUE(current_state.hours_tens.decimal_value <= 2, "current state hours_tens valid");
    ASSERT_TRUE(current_state.hours_units.decimal_value <= 9, "current state hours_units valid");
}

// Test core API data integrity
void test_data_integrity(void) {
    printf("\n=== Testing Data Integrity ===\n");
    
    // Test that binary conversion is reversible
    for (int value = 0; value <= 63; value++) {
        for (int bits = 1; bits <= 6; bits++) {
            if (value < (1 << bits)) {
                binary_value_t binary = binary_clock_to_binary(value, bits);
                uint8_t recovered = binary_clock_to_decimal(&binary);
                ASSERT_EQ(recovered, value, "binary conversion is reversible");
            }
        }
    }
    
    // Test state consistency
    time_components_t test_time = {23, 59, 59};
    binary_clock_state_t state = binary_clock_state_from_time(&test_time);
    
    // Verify bit counts are correct
    ASSERT_EQ(state.hours_tens.bit_count, 3, "hours_tens has 3 bits");
    ASSERT_EQ(state.hours_units.bit_count, 4, "hours_units has 4 bits");
    ASSERT_EQ(state.minutes_tens.bit_count, 3, "minutes_tens has 3 bits");
    ASSERT_EQ(state.minutes_units.bit_count, 4, "minutes_units has 4 bits");
    ASSERT_EQ(state.seconds_tens.bit_count, 3, "seconds_tens has 3 bits");
    ASSERT_EQ(state.seconds_units.bit_count, 4, "seconds_units has 4 bits");
    
    // Verify decimal values match input
    ASSERT_EQ(state.hours_tens.decimal_value, 2, "hours_tens decimal matches");
    ASSERT_EQ(state.hours_units.decimal_value, 3, "hours_units decimal matches");
    ASSERT_EQ(state.minutes_tens.decimal_value, 5, "minutes_tens decimal matches");
    ASSERT_EQ(state.minutes_units.decimal_value, 9, "minutes_units decimal matches");
    ASSERT_EQ(state.seconds_tens.decimal_value, 5, "seconds_tens decimal matches");
    ASSERT_EQ(state.seconds_units.decimal_value, 9, "seconds_units decimal matches");
}

// Test utility functions
void test_utility_functions(void) {
    printf("\n=== Testing Utility Functions ===\n");
    
    // Test error string function
    const char* error_str = binary_clock_get_error_string(BINARY_CLOCK_SUCCESS);
    ASSERT_STR_EQ(error_str, "Operation completed successfully", "success error string");
    
    error_str = binary_clock_get_error_string(BINARY_CLOCK_ERROR_INVALID_TIME);
    ASSERT_STR_EQ(error_str, "Invalid time components provided", "invalid time error string");
    
    error_str = binary_clock_get_error_string(BINARY_CLOCK_ERROR_NULL_POINTER);
    ASSERT_STR_EQ(error_str, "Null pointer passed to function requiring valid pointer", "null pointer error string");
    
    // Test version function
    const char* version = binary_clock_get_version();
    ASSERT_STR_EQ(version, "1.0.0", "API version string");
}

// Test performance requirements
void test_performance(void) {
    printf("\n=== Testing Performance Requirements ===\n");
    
    clock_t start = clock();
    
    // Test 1000 state queries (should be well under 1ms each)
    for (int i = 0; i < 1000; i++) {
        binary_clock_state_t state = binary_clock_get_current_state();
        (void)state; // Suppress unused variable warning
    }
    
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    double avg_time_ms = (elapsed * 1000.0) / 1000.0;
    
    printf("1000 state queries took %.3f seconds (avg %.3f ms per query)\n", elapsed, avg_time_ms);
    ASSERT_TRUE(avg_time_ms < 1.0, "state queries meet performance requirement (< 1ms)");
    
    // Test binary conversion performance
    start = clock();
    for (int i = 0; i < 100000; i++) {
        binary_value_t result = binary_clock_to_binary(i % 64, (i % 6) + 1);
        uint8_t decimal = binary_clock_to_decimal(&result);
        (void)decimal; // Suppress unused variable warning
    }
    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("100,000 binary conversions took %.3f seconds\n", elapsed);
    ASSERT_TRUE(elapsed < 0.1, "binary conversions are fast");
}

int main(void) {
    printf("=== Binary Clock Core API Test Suite ===\n");
    printf("Testing core data functions (display utilities tested separately)\n\n");
    
    test_binary_conversion();
    test_time_management();
    test_data_integrity();
    test_utility_functions();
    test_performance();
    
    printf("\n=== Test Summary ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    printf("Success rate: %.1f%%\n", tests_run > 0 ? (100.0 * tests_passed / tests_run) : 0.0);
    
    if (tests_passed == tests_run) {
        printf("🎉 All API tests passed!\n");
        return 0;
    } else {
        printf("❌ Some tests failed\n");
        return 1;
    }
}
