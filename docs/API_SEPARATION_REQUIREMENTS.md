# Binary Clock API Separation - Business Requirements Document

## Document Information
- **Document Type**: Business Requirements Document (BRD)
- **Version**: 1.0
- **Date**: 2025-01-16
- **Status**: Draft
- **Owner**: Binary Clock Project Team

## Executive Summary

### Business Case
The current binary clock implementation tightly couples time logic, binary conversion, and visualization (emoji display). This monolithic approach limits reusability and prevents the core logic from being used in other applications, specifically planned macOS/iPadOS widget implementations.

### Objective
Create a clean API separation that decouples visualization from business logic, enabling the core binary clock functionality to be used across multiple platforms and user interfaces while maintaining the existing console application functionality.

### Expected Benefits
- **Reusability**: Core logic can be used in widgets, web applications, and other interfaces
- **Maintainability**: Clear separation of concerns reduces complexity
- **Testability**: Each layer can be tested independently
- **Performance**: Optimized for frequent updates (widget refresh cycles)
- **Extensibility**: Easy to add new visualization methods without touching core logic

## Current State Analysis

### Existing Implementation Issues
1. **Tight Coupling**: `display_binary()` function mixes binary conversion with emoji rendering
2. **Limited Reusability**: Core logic cannot be extracted for other applications
3. **String-Based Interface**: `to_binary()` returns formatted strings instead of raw data
4. **Platform Dependencies**: Visualization code embedded in business logic
5. **Testing Complexity**: Difficult to test binary logic without testing display output

### Current Architecture Problems
```c
// Current problematic approach
char* to_binary(int num) {
    // Returns string - not suitable for graphics rendering
}

void display_binary(const char* binary_str) {
    // Hardcoded emoji display - not reusable
}
```

## Business Requirements

### Primary Requirements

#### REQ-001: API Layer Separation
**Priority**: High  
**Description**: Separate core binary clock logic from visualization concerns
**Rationale**: Enable reuse across different platforms and applications
**Acceptance Criteria**:
- Core logic has zero UI dependencies
- Visualization layer is pluggable
- Existing console functionality preserved

#### REQ-002: Widget Compatibility
**Priority**: High  
**Description**: API must support real-time widget updates
**Rationale**: Primary use case for macOS/iPadOS widget development
**Acceptance Criteria**:
- Minimal memory allocation during updates
- Sub-second response time for state queries
- Support for both current time and arbitrary time injection

#### REQ-003: Cross-Platform Compatibility
**Priority**: Medium  
**Description**: API must work on macOS, iOS, Linux, Windows
**Rationale**: Support multiple deployment targets
**Acceptance Criteria**:
- No platform-specific dependencies in core API
- Standard C99 compatibility
- Optional platform-specific display implementations

#### REQ-004: Backward Compatibility
**Priority**: Medium  
**Description**: Existing console application continues to work
**Rationale**: Preserve current functionality during transition
**Acceptance Criteria**:
- Console app compiles and runs identically
- No breaking changes to build system
- Performance characteristics maintained or improved

### Secondary Requirements

#### REQ-005: Extensible Display System
**Priority**: Low  
**Description**: Easy addition of new visualization methods
**Rationale**: Future-proofing for new interfaces (web, LED displays, etc.)
**Acceptance Criteria**:
- Plugin-style display registration
- Multiple display formats supported
- No recompilation needed for new displays

#### REQ-006: Comprehensive Testing
**Priority**: Medium  
**Description**: Each layer must be independently testable
**Rationale**: Improve code quality and reliability
**Acceptance Criteria**:
- Unit tests for core logic
- Mock displays for testing
- Integration tests for complete workflows

## Technical Requirements

### Performance Requirements
- **Response Time**: State queries must complete within 1ms
- **Memory Usage**: Core API should use < 1KB of heap memory
- **CPU Usage**: Binary conversion should be O(1) complexity
- **Update Frequency**: Support 60Hz refresh rates for smooth animations

### Compatibility Requirements
- **C Standard**: C99 minimum, C11 preferred
- **Platforms**: macOS 10.15+, iOS 13+, Linux, Windows 10+
- **Dependencies**: Standard library only for core API
- **Thread Safety**: Core API must be thread-safe

### Data Format Requirements
- **Time Representation**: Integer hours/minutes/seconds (0-23, 0-59, 0-59)
- **Binary Format**: Boolean arrays, not strings
- **Bit Counts**: Configurable (4-bit for tens, 4-bit for units by default)
- **Endianness**: Consistent bit ordering across platforms

## Architecture Design

### Proposed Layer Architecture

```
┌─────────────────────────────────────────────────────────┐
│                 Application Layer                       │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────┐ │
│  │   Console App   │  │  macOS Widget   │  │   Web   │ │
│  └─────────────────┘  └─────────────────┘  └─────────┘ │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│                Visualization Layer                      │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────┐ │
│  │  Emoji Display  │  │  CoreGraphics   │  │  JSON   │ │
│  └─────────────────┘  └─────────────────┘  └─────────┘ │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│                    API Layer                            │
│           binary_clock_get_state()                      │
│           binary_clock_from_time()                      │
│           binary_clock_register_display()               │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│                  Core Logic Layer                       │
│     Time Management + Binary Conversion                 │
└─────────────────────────────────────────────────────────┘
```

### Core Data Structures

#### Binary Value Representation
```c
typedef struct {
    uint8_t bit_count;      // Number of significant bits (1-6)
    bool bits[6];           // Bit array (MSB first)
    uint8_t decimal_value;  // Original decimal value for reference
} binary_value_t;
```

#### Clock State Representation
```c
typedef struct {
    binary_value_t hours_tens;     // 0-2 (3 bits)
    binary_value_t hours_units;    // 0-9 (4 bits)
    binary_value_t minutes_tens;   // 0-5 (3 bits)
    binary_value_t minutes_units;  // 0-9 (4 bits)
    binary_value_t seconds_tens;   // 0-5 (3 bits)
    binary_value_t seconds_units;  // 0-9 (4 bits)
    time_t timestamp;              // Unix timestamp when state was created
} binary_clock_state_t;
```

#### Time Input Structure
```c
typedef struct {
    uint8_t hours;    // 0-23
    uint8_t minutes;  // 0-59
    uint8_t seconds;  // 0-59
} time_components_t;
```

## API Specifications

### Core API Functions

#### State Management
```c
/**
 * Get current binary clock state using system time
 * @return Binary clock state for current time
 * @note Thread-safe, no heap allocation
 */
binary_clock_state_t binary_clock_get_current_state(void);

/**
 * Create binary clock state from specific time
 * @param time Time components to convert
 * @return Binary clock state for specified time
 * @note Thread-safe, no heap allocation
 */
binary_clock_state_t binary_clock_state_from_time(const time_components_t* time);

/**
 * Create time components from current system time
 * @return Current time as components
 * @note Thread-safe
 */
time_components_t binary_clock_get_current_time(void);
```

#### Binary Conversion Utilities
```c
/**
 * Convert decimal number to binary representation
 * @param value Decimal value (0-63)
 * @param bit_count Number of bits to represent (1-6)
 * @return Binary representation
 * @note Validates input range, thread-safe
 */
binary_value_t binary_clock_to_binary(uint8_t value, uint8_t bit_count);

/**
 * Convert binary representation back to decimal
 * @param binary Binary representation
 * @return Decimal value
 * @note Thread-safe
 */
uint8_t binary_clock_to_decimal(const binary_value_t* binary);
```

### Display Interface

#### Display Callback System
```c
/**
 * Display callback function signature
 * @param state Current binary clock state
 * @param context User-provided context data
 */
typedef void (*binary_clock_display_fn_t)(const binary_clock_state_t* state, void* context);

/**
 * Register a display callback
 * @param display_fn Display function to register
 * @param context Context data to pass to display function
 * @return Registration ID for later removal
 */
int binary_clock_register_display(binary_clock_display_fn_t display_fn, void* context);

/**
 * Update all registered displays with current state
 * @note Calls all registered display functions
 */
void binary_clock_update_displays(void);

/**
 * Update all registered displays with specific state
 * @param state State to display
 */
void binary_clock_update_displays_with_state(const binary_clock_state_t* state);
```

#### Built-in Display Functions
```c
/**
 * Console emoji display (🌚🌝 format)
 * @param state Binary clock state to display
 * @param context Unused (pass NULL)
 */
void binary_clock_display_console_emoji(const binary_clock_state_t* state, void* context);

/**
 * Console ASCII display (0/1 format)
 * @param state Binary clock state to display  
 * @param context Unused (pass NULL)
 */
void binary_clock_display_console_ascii(const binary_clock_state_t* state, void* context);

/**
 * JSON format display
 * @param state Binary clock state to display
 * @param context FILE* or buffer to write to
 */
void binary_clock_display_json(const binary_clock_state_t* state, void* context);
```

### Error Handling
```c
typedef enum {
    BINARY_CLOCK_SUCCESS = 0,
    BINARY_CLOCK_ERROR_INVALID_TIME,
    BINARY_CLOCK_ERROR_INVALID_BIT_COUNT,
    BINARY_CLOCK_ERROR_NULL_POINTER,
    BINARY_CLOCK_ERROR_SYSTEM_TIME
} binary_clock_error_t;

/**
 * Get last error message
 * @return Human-readable error description
 */
const char* binary_clock_get_error_string(binary_clock_error_t error);
```

## Use Cases

### Use Case 1: macOS/iPadOS Widget

**Actor**: Widget framework  
**Goal**: Display real-time binary clock in system widget

**Primary Flow**:
1. Widget framework calls `binary_clock_get_current_state()`
2. Widget extracts bit arrays from returned state
3. Widget renders bits using CoreGraphics/SwiftUI
4. Widget schedules next update in 1 second
5. Repeat from step 1

**Widget Implementation Example**:
```c
// In widget update function
binary_clock_state_t state = binary_clock_get_current_state();

// Render hours tens (3 bits)
for (int i = 0; i < state.hours_tens.bit_count; i++) {
    draw_bit_led(x + i * LED_WIDTH, y, state.hours_tens.bits[i]);
}

// Render other components similarly...
```

### Use Case 2: Console Application

**Actor**: Command-line user  
**Goal**: View binary clock in terminal

**Primary Flow**:
1. Application registers emoji display callback
2. Application enters update loop
3. Loop calls `binary_clock_update_displays()` every second
4. Registered callback renders emoji output
5. Application clears screen and repeats

**Console Implementation Example**:
```c
int main() {
    binary_clock_register_display(binary_clock_display_console_emoji, NULL);
    
    while (running) {
        system("clear");
        binary_clock_update_displays();
        sleep(1);
    }
    
    return 0;
}
```

### Use Case 3: Web API Service

**Actor**: Web client  
**Goal**: Get binary clock data as JSON

**Primary Flow**:
1. HTTP request received for current time
2. Service calls `binary_clock_get_current_state()`
3. Service uses JSON display function to serialize state
4. JSON response returned to client

**Web Service Example**:
```c
void handle_api_request(char* response_buffer) {
    binary_clock_state_t state = binary_clock_get_current_state();
    binary_clock_display_json(&state, response_buffer);
}
```

### Use Case 4: Custom LED Display

**Actor**: Hardware interface  
**Goal**: Drive LED matrix with binary clock

**Primary Flow**:
1. Hardware driver registers custom display function
2. Display function extracts bit arrays from state
3. Function maps bits to LED matrix coordinates
4. LEDs updated via hardware interface

**LED Implementation Example**:
```c
void led_display_callback(const binary_clock_state_t* state, void* context) {
    led_matrix_t* matrix = (led_matrix_t*)context;
    
    // Map each bit to LED coordinates
    set_led_state(matrix, 0, 0, state->hours_tens.bits[0]);
    set_led_state(matrix, 0, 1, state->hours_tens.bits[1]);
    // ... etc
    
    update_led_matrix(matrix);
}
```

## Implementation Strategy

### Phase 1: Core API Development
1. **Week 1**: Implement core data structures and conversion functions
2. **Week 1**: Create state management functions
3. **Week 2**: Implement display callback system
4. **Week 2**: Create built-in display functions

### Phase 2: Integration and Testing  
1. **Week 3**: Refactor existing console app to use new API
2. **Week 3**: Comprehensive unit testing for all API functions
3. **Week 4**: Integration testing and performance validation
4. **Week 4**: Documentation and examples

### Phase 3: Platform Extensions
1. **Week 5**: macOS widget development using API
2. **Week 6**: Additional display formats (JSON, ASCII)
3. **Week 7**: Performance optimization and profiling
4. **Week 8**: Final testing and release preparation

### API Specification Format

The complete API specifications are maintained in machine-readable JSON format for easy parsing and code generation:

- **JSON API Definition**: `docs/binary_clock_api_spec.json`
- **Human-Readable Reference**: This document (API_SEPARATION_REQUIREMENTS.md)

The JSON specification includes:
- Complete data structure definitions with field types and constraints
- Function signatures with parameter validation rules
- Error codes and messages
- Usage examples and test cases
- Performance requirements and benchmarks

### Development Guidelines

#### Code Organization
```
include/
  binary_clock_api.h         # Public API declarations (generated from JSON)
  binary_clock_internal.h    # Internal implementation details
src/
  binary_clock_core.c        # Core logic implementation
  binary_clock_display.c     # Built-in display functions
  binary_clock_console.c     # Console application
tests/
  test_binary_clock_core.c   # Core logic tests (cases from JSON)
  test_binary_clock_api.c    # API integration tests
  test_display_functions.c   # Display function tests
examples/
  widget_example.c           # macOS widget example
  json_server_example.c      # Web service example
docs/
  binary_clock_api_spec.json # Machine-readable API specification
```

#### Coding Standards
- **Function Naming**: `binary_clock_` prefix for all public functions
- **Error Handling**: Return error codes, use last-error pattern
- **Documentation**: Doxygen-style comments for all public functions
- **Memory Management**: No dynamic allocation in core API
- **Thread Safety**: All public functions must be thread-safe

## Success Criteria

### Functional Success Criteria
- [ ] All existing console functionality preserved
- [ ] macOS widget successfully implements using API
- [ ] Core API has zero external dependencies
- [ ] Display system supports multiple concurrent outputs
- [ ] JSON serialization produces valid, parseable output

### Performance Success Criteria
- [ ] State query completes in < 1ms (measured on target platforms)
- [ ] Memory usage remains under 1KB for core API
- [ ] No memory leaks after 24-hour continuous operation
- [ ] Widget updates at 60fps without frame drops

### Quality Success Criteria
- [ ] Test coverage > 95% for core logic
- [ ] All public API functions have comprehensive documentation
- [ ] Zero compiler warnings on all target platforms
- [ ] Static analysis passes without issues
- [ ] Code review approval from senior developers

### Maintainability Success Criteria
- [ ] New display formats can be added without modifying core
- [ ] API changes require no modifications to existing displays
- [ ] Clear separation between layers (no circular dependencies)
- [ ] Documentation sufficient for external developers

## Future Considerations

### Planned Extensions
1. **Animation Support**: Smooth transitions between time updates
2. **Configuration System**: Customizable bit counts and formatting
3. **Multiple Time Zones**: Support for world clocks
4. **Accessibility**: Screen reader compatible display options
5. **Themes**: Customizable color schemes and symbols

### Potential Integrations
1. **Web Components**: JavaScript wrapper for web applications
2. **Arduino/Embedded**: Minimal footprint version for microcontrollers
3. **Smart Home**: Integration with home automation systems
4. **Screensavers**: Desktop screensaver implementations
5. **Mobile Apps**: Native iOS/Android applications

### Scalability Considerations
1. **Multi-threading**: Parallel display updates for performance
2. **Caching**: State caching for high-frequency updates
3. **Event System**: Notification callbacks for time changes
4. **Plugin Architecture**: Dynamic loading of display modules
5. **Network Protocol**: Distributed binary clock synchronization

## Risk Assessment

### Technical Risks
- **Performance Impact**: API overhead might slow existing console app
- **Complexity**: Over-engineering could make simple use cases difficult
- **Platform Issues**: Thread safety implementation varies by platform
- **Widget Limitations**: macOS widget constraints might require API changes

### Mitigation Strategies
- **Benchmarking**: Continuous performance monitoring during development
- **Incremental Development**: Start with minimal API, add features iteratively
- **Platform Testing**: Early testing on all target platforms
- **Widget Prototyping**: Early widget prototype to validate API design

## Conclusion

This API separation project will transform the binary clock from a single-purpose console application into a reusable, cross-platform library. The clean architecture will enable widget development, web services, and future applications while maintaining the simplicity and performance of the original implementation.

The success of this project will be measured by the successful implementation of the macOS widget and the preservation of all existing functionality. The modular design ensures long-term maintainability and extensibility for future enhancements.

---

**Document Status**: This document should be reviewed and approved before implementation begins. Updates to requirements should be tracked through version control and require stakeholder approval.
