# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a simple C program that displays a binary clock using moon emojis (🌚 for 0, 🌝 for 1). The clock shows the current time in binary format with different representations for hours, minutes, and seconds.

## Project Structure

The project is organized into the following directories:

```
binaryclock/
├── src/                    # Source code files
│   ├── binary_clock.c     # Main application
│   └── binary_clock_lib.c # Library implementation
├── include/               # Header files
│   └── binary_clock_lib.h # Library header
├── tests/                 # Test files
│   ├── test_binary_clock.c
│   └── test_signal_handling.c
├── build/                 # Build artifacts (created during compilation)
├── .github/
│   └── workflows/
│       └── ci.yml         # CI/CD pipeline with path-based triggers
├── Makefile              # Cross-platform build system
└── CLAUDE.md             # Project documentation
```

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
gcc -I include -o binary_clock src/binary_clock.c
```

## Code Architecture

- **Main application**: `src/binary_clock.c` contains the clock display logic
- **Library code**: `src/binary_clock_lib.c` and `include/binary_clock_lib.h` contain reusable functions
- **Test suite**: `tests/test_binary_clock.c` and `tests/test_signal_handling.c` provide comprehensive testing
- **Build system**: `Makefile` handles compilation, testing, and cross-platform builds
- **CI/CD**: `.github/workflows/ci.yml` runs tests on multiple platforms with path-based triggers

### Core Functions
- `to_binary()`: Thread-safe conversion of integers to binary strings
- `display_binary()`: Renders binary strings as moon emojis
- `signal_handler()`: Handles graceful exit on Ctrl+C

## Key Implementation Details

- Cross-platform compatibility (Windows, macOS, Linux)
- Uses appropriate screen clearing commands per platform (`cls` on Windows, `clear` on Unix)
- Cross-platform sleep functions (Windows `Sleep()` vs Unix `sleep()`)
- Time components split into tens and units for proper binary clock display
- Thread-safe `to_binary()` function using caller-provided buffers
- Comprehensive test coverage with 41+ test cases
- Signal handling for graceful exit with Ctrl+C
- Organized directory structure with separate source, include, and test directories
- CI/CD pipeline with path-based triggers to optimize build performance

## Testing

The project includes a comprehensive test suite covering:

- Unit tests for all core functions
- Edge case testing (boundary values, buffer safety)
- Performance testing (100,000+ operations)
- Signal handling validation
- Integration tests with realistic time scenarios

Run tests with: `make test`
