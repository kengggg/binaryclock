# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a modular C project that provides binary clock functionality with complete separation between data logic and visualization. The console application now includes a comprehensive CLI interface with multiple display modes and operation options. The core API provides only raw binary time data, while separate display utilities handle visualization (including moon emojis 🌚🌝). This architecture enables reuse across platforms like widgets, web services, and embedded systems.

## Project Structure

The project is organized into the following directories:

```
binaryclock/
├── src/                           # Source code files
│   ├── binary_clock.c            # Console application (uses both modules)
│   ├── binary_clock_api.c        # Core API implementation (data only)
│   ├── binary_clock_display.c    # Display utilities (visualization)
├── include/                       # Header files
│   ├── binary_clock_api.h        # Core API header (data only)
│   ├── binary_clock_display.h    # Display utilities header
├── tests/                         # Test files
│   ├── test_binary_clock_api.c   # Core API tests (186+ tests)
│   └── test_signal_handling.c    # Signal handling tests
├── build/                         # Build artifacts (created during compilation)
├── docs/                          # Project documentation
│   ├── binary_clock_api_spec.json         # Machine-readable API specification
│   ├── API_SEPARATION_REQUIREMENTS.md    # Business requirements document
│   ├── API_REFERENCE.md                  # Complete API documentation
│   ├── QUICKSTART-CLI.md                 # CLI user quickstart guide
│   ├── QUICKSTART-WIDGET.md              # Widget developer quickstart guide
│   └── SWIFT_INTEGRATION_EXAMPLE.swift   # Swift integration example
├── .github/
│   └── workflows/
│       ├── ci.yml                # CI/CD pipeline with path-based triggers
│       └── release.yml           # Automated release workflow
├── scripts/
│   └── release/
│       └── package-api.sh        # API package generation script
├── Makefile                      # Cross-platform build system with packaging
└── CLAUDE.md                     # Project documentation
```

## Build Commands

```bash
# Build everything
make all

# Build and run tests
make test

# Build and run the binary clock (see CLI Usage below)
make run

# Clean build artifacts
make clean

# Manual compilation (if needed)
gcc -I include -o binary_clock src/binary_clock.c
```

## Distribution & Packaging

The project includes comprehensive packaging and automated release capabilities:

```bash
# Create distribution packages
make dist-all                 # Create all distribution packages
make dist-api-only            # API-only package for widget developers
make dist-cli                 # CLI binary package for end users
make dist-library             # Complete library package for developers

# Create release archives
make package-all              # Create all release archives
make package-api              # Create API package archives (.tar.gz, .zip)
make package-cli              # Create platform-specific CLI archives
make package-library          # Create library package archives
make package-source           # Create source distribution archives

# Complete release preparation
make prepare-release          # Full release pipeline: build, test, package, checksums
```

### Automated Releases

The project features automated GitHub Actions releases triggered by version tags:

1. **Tag-based releases**: Push a version tag (`v1.0.0`) to trigger automated release
2. **Cross-platform builds**: Automatically builds for Linux, macOS, and Windows
3. **Multiple package formats**: Generates persona-specific packages:
   - `binary-clock-api-*.tar.gz` - Core API for widget developers
   - `binary-clock-cli-*-{platform}.{tar.gz|zip}` - Platform-specific CLI binaries
   - `binary-clock-library-*.tar.gz` - Complete library for developers
   - `binary-clock-source-*.tar.gz` - Source distribution
4. **Security**: All packages include SHA256 checksums for verification
5. **Documentation**: Comprehensive release notes with usage examples

## Code Architecture

### Modular Design with Complete Separation

- **Core API** (`binary_clock_api.h/.c`): Pure data and conversion functions (NO visualization)
  - Data structures: `binary_value_t`, `time_components_t`, `binary_clock_state_t`
  - Functions: `binary_clock_get_current_state()`, `binary_clock_to_binary()`, etc.
  - Thread-safe, no memory allocation, C99 compatible

- **Display Utilities** (`binary_clock_display.h/.c`): All visualization functions
  - Functions: `binary_clock_display_console_emoji()`, `binary_clock_display_json()`, etc.
  - Callback system: `binary_clock_display_register()`, `binary_clock_display_update_all()`
  - Uses core API for all data access

- **Console Application** (`binary_clock.c`): Complete CLI binary clock app
  - Includes both core API and display utilities  
  - Command line interface with multiple display modes and options
  - Single output mode (default) and continuous loop mode
  - Backward compatibility maintained with --loop option

- **Test Suite**: Comprehensive coverage (225+ tests total)
- **Build System**: `Makefile` handles modular compilation
- **CI/CD**: `.github/workflows/ci.yml` runs tests on multiple platforms

### Separation Principle

**CRITICAL**: The core API contains ZERO visualization code. This enables:
- Widget development (macOS/iPadOS)
- Web services (JSON APIs)
- Embedded systems (LED displays)
- Any custom visualization using raw binary data

## CLI Usage

The binary clock now includes a comprehensive command-line interface:

```bash
# Single output modes (default - exits immediately)
./binary_clock                          # Moon emoji display
./binary_clock --display=emoji          # Moon emoji display (explicit)
./binary_clock --display=binary         # 0s and 1s display  
./binary_clock --display=json           # JSON format
./binary_clock --display=raw            # Raw API data structures

# Continuous modes (updates every second)
./binary_clock --loop                   # Continuous moon emoji display
./binary_clock --display=binary --loop  # Continuous 0s and 1s display
./binary_clock --display=json --loop    # Continuous JSON output
./binary_clock --display=raw --loop     # Continuous raw API output

# Help and information
./binary_clock --help                   # Show usage information
./binary_clock -h                       # Short help option
```

### Display Modes
- **emoji**: Moon emojis 🌚 (0) and 🌝 (1) - default and most visual
- **binary**: Clean 0s and 1s representation - practical for analysis
- **json**: Structured JSON data - perfect for API integration
- **raw**: Raw API data structures - useful for debugging

### Operation Modes
- **Single output**: Default behavior, outputs once and exits (ideal for scripts)
- **Loop mode**: Continuous display with --loop flag (classic binary clock behavior)

## Key Implementation Details

- **CLI Interface**: Full command-line argument parsing with error handling
- **Multiple Display Modes**: Four different output formats (emoji, binary, JSON, raw)
- **Single/Loop Modes**: Default single output or continuous loop with --loop
- **Cross-platform compatibility**: Windows, macOS, Linux support
- **Smart Screen Clearing**: Only clears screen in loop mode for non-JSON displays
- **Cross-platform sleep/clear**: Windows `Sleep()`/`cls` vs Unix `sleep()`/`clear`
- **Time components**: Split into tens and units for proper binary clock display
- **Thread-safe functions**: Core API uses caller-provided buffers
- **Comprehensive testing**: 228+ total test cases across all modules
- **Signal handling**: Graceful exit with Ctrl+C (only in loop mode)
- **Modular directory structure**: Separate source, include, and test directories
- **Enhanced CI/CD**: Tests all CLI modes across multiple platforms

## Testing

The project includes a comprehensive test suite covering:

- Unit tests for all core functions
- Edge case testing (boundary values, buffer safety)
- Performance testing (100,000+ operations)
- Signal handling validation
- Integration tests with realistic time scenarios

Run tests with: `make test`

## API Separation Initiative ✅ COMPLETED

**SUCCESS**: The API separation initiative has been successfully completed! The project now has complete separation between data logic and visualization, enabling reuse across multiple platforms.

### Implementation Complete
See `docs/API_SEPARATION_REQUIREMENTS.md` and `docs/binary_clock_api_spec.json` for comprehensive specifications.

### Design Principles ✅ Achieved
- **Separation of Concerns**: Core API contains ZERO visualization code ✅
- **Reusability**: API ready for widgets, web services, embedded systems ✅
- **Performance**: Sub-millisecond state queries, 60Hz refresh capability ✅
- **Cross-Platform**: C99 standard, no platform dependencies in core API ✅
- **Backward Compatibility**: Console application functionality preserved ✅

### Implementation Status
- Phase 0: Requirements documentation ✅ Complete
- Phase 1: Core API development ✅ Complete
- Phase 2: Integration and testing ✅ Complete (228+ tests passing)
- Phase 3: CLI interface development ✅ Complete (all display modes)
- Phase 4: Ready for widget implementation ✅ API ready

### Key Achievements
- **Pure Data API**: Core functions provide only binary time data
- **Modular Architecture**: Clean separation enables independent development
- **Comprehensive Testing**: 186 core API tests + 41 legacy tests + signal tests + CLI validation
- **Documentation**: Machine-readable JSON specification for easy integration
- **Performance**: All requirements met (< 1ms state queries, stack-only allocation)

### Next Steps for Widget Development
1. Include `binary_clock_api.h` for data access
2. Create custom display functions using raw binary data
3. Optionally use `binary_clock_display.h` for common visualizations

The separated architecture with CLI interface is now ready for:
- macOS/iPadOS widget development (using JSON output mode)
- Script integration (using single output modes)
- Real-time monitoring (using loop modes)
- API development (using raw or JSON modes)
