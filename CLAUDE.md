# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a simple C program that displays a binary clock using moon emojis (🌚 for 0, 🌝 for 1). The clock shows the current time in binary format with different representations for hours, minutes, and seconds.

## Build Commands

```bash
# Build everything
make all

# Build and run tests
make test

# Build and run the binary clock
make run

# Clean build artifacts
make clean

# Manual compilation (if needed)
gcc -o binary_clock binary_clock.c
```

## Code Architecture

- **Main application**: `binary_clock.c` contains the clock display logic
- **Library code**: `binary_clock_lib.c` and `binary_clock_lib.h` contain reusable functions
- **Test suite**: `test_binary_clock.c` and `test_signal_handling.c` provide comprehensive testing
- **Build system**: `Makefile` handles compilation, testing, and other tasks

### Core Functions
- `to_binary()`: Thread-safe conversion of integers to binary strings
- `display_binary()`: Renders binary strings as moon emojis
- `signal_handler()`: Handles graceful exit on Ctrl+C

## Key Implementation Details

- Uses `system("clear")` for screen clearing (Unix-specific)
- Time components split into tens and units for proper binary clock display
- Thread-safe `to_binary()` function using caller-provided buffers
- Comprehensive test coverage with 41+ test cases
- Signal handling for graceful exit with Ctrl+C

## Testing

The project includes a comprehensive test suite covering:

- Unit tests for all core functions
- Edge case testing (boundary values, buffer safety)
- Performance testing (100,000+ operations)
- Signal handling validation
- Integration tests with realistic time scenarios

Run tests with: `make test`
