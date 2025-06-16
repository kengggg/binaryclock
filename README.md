# 🌚🌝 Binary Clock 🌝🌚

[![CI](https://github.com/kengggg/binaryclock/actions/workflows/ci.yml/badge.svg)](https://github.com/kengggg/binaryclock/actions/workflows/ci.yml)

A delightfully lunar way to tell time! This C program displays the current time in binary format with multiple output modes - from cosmic moon emojis 🌚🌝 to practical JSON data for your applications.

## What This Does

Ever wondered what time it is in binary? Wonder no more! This clock shows:
- Hours in binary (split into tens and units)
- Minutes in binary (split into tens and units) 
- Seconds in binary (split into tens and units)

**Multiple Display Modes:**
- 🌙 **Emoji Mode**: Cosmic moon phases (🌚 for 0, 🌝 for 1)
- 🔢 **Binary Mode**: Clean 0s and 1s display
- 📊 **JSON Mode**: Structured data perfect for APIs
- 🔍 **Raw Mode**: Core API data structures for debugging

## Quick Start

```bash
# Build the project
make all

# Single output (default - exits immediately)
./binary_clock                          # Moon emoji display
./binary_clock --display=binary         # 0s and 1s display
./binary_clock --display=json           # JSON format
./binary_clock --display=raw            # Raw API data

# Continuous mode (updates every second)
./binary_clock --loop                   # Continuous moon emoji display
./binary_clock --display=json --loop    # Continuous JSON output

# Get help
./binary_clock --help
```

Press Ctrl+C to gracefully exit continuous mode.

## Building & Testing

```bash
# Build everything
make all

# Run the comprehensive test suite (228 total tests!)
make test

# Clean up build artifacts
make clean
```

## How It Works

The binary clock uses a modular architecture with complete separation between core logic and visualization:

- **Core API**: Handles time conversion and binary representation (data only)
- **Display Layer**: Renders data in various formats (emoji, binary, JSON, raw)
- **CLI Interface**: Provides single-shot or continuous display modes

Each time component (hours, minutes, seconds) is split into tens and units, then converted to binary representation.

**Example Output for 13:37:42:**

*Emoji Mode:*
```
Hours   : 🌚🌚🌝 🌚🌚🌝🌝  (1 3)
Minutes : 🌚🌝🌝 🌚🌝🌝🌝  (3 7) 
Seconds : 🌚🌝🌚 🌚🌚🌝🌚  (4 2)
```

*JSON Mode:*
```json
{
  "timestamp": 1750047329,
  "time": "13:37:42",
  "binary": {
    "hours": {"tens": [0,0,1], "units": [0,0,1,1]},
    "minutes": {"tens": [0,1,1], "units": [0,1,1,1]},
    "seconds": {"tens": [0,1,0], "units": [0,0,1,0]}
  }
}
```

## Project Structure

```
binaryclock/
├── src/                    # Source code files
│   ├── binary_clock.c         # Main CLI application
│   ├── binary_clock_api.c     # Core API implementation (data only)
│   ├── binary_clock_display.c # Display utilities (visualization)
│   └── binary_clock_lib.c     # Legacy library (deprecated)
├── include/               # Header files
│   ├── binary_clock_api.h     # Core API header (pure data)
│   ├── binary_clock_display.h # Display utilities header
│   └── binary_clock_lib.h     # Legacy library header
├── tests/                 # Test files
│   ├── test_binary_clock.c      # Legacy library tests
│   ├── test_binary_clock_api.c  # Core API tests (186 tests)
│   └── test_signal_handling.c   # Signal handling tests
├── build/                 # Build artifacts (auto-created)
├── docs/                  # Project documentation
│   ├── API_REFERENCE.md             # Complete API reference & examples
│   ├── SWIFT_INTEGRATION_EXAMPLE.swift # Swift/iOS widget integration
│   ├── API_SEPARATION_REQUIREMENTS.md  # API design specifications
│   └── binary_clock_api_spec.json      # Machine-readable API spec
└── Makefile              # Cross-platform build system
```

## Development Roadmap

### ✅ API Separation Complete

The core binary clock logic has been successfully separated from visualization, enabling reuse across multiple platforms and applications!

**Current Architecture:**
- 🔧 **Core API** (`binary_clock_api.h/.c`): Pure data and conversion functions
- 🎨 **Display Layer** (`binary_clock_display.h/.c`): Multiple visualization modes
- 💻 **CLI Application** (`binary_clock.c`): Command-line interface with both modules

**Available Features:**
- 📱 **JSON Output**: Ready for widget integration (macOS/iPadOS)
- 🌐 **API Data**: Structured output for web applications
- 🔧 **Minimal Footprint**: Core API uses only stack allocation
- 🎨 **Multiple Displays**: Emoji, binary, JSON, and raw data modes

**Benefits Achieved:**
- ✅ Reusable core logic across platforms
- ✅ Optimized for real-time updates (< 1ms state queries)
- ✅ Maintained backward compatibility with console application
- ✅ Complete separation of concerns (ZERO visualization in core API)

## API Documentation

📚 **Complete API Reference**: See [`docs/API_REFERENCE.md`](docs/API_REFERENCE.md) for comprehensive integration examples including:
- C API functions and data structures
- Swift/iOS widget integration examples
- CLI usage and scripting
- JSON API format and examples
- Performance guidelines and error handling

🍎 **Swift Integration**: See [`docs/SWIFT_INTEGRATION_EXAMPLE.swift`](docs/SWIFT_INTEGRATION_EXAMPLE.swift) for complete iOS/macOS widget implementation

📋 **Technical Specifications**: See `docs/API_SEPARATION_REQUIREMENTS.md` and `docs/binary_clock_api_spec.json` for detailed specifications

## Credits

This code was crafted as requested and represents collaborative human-AI programming. While the implementation is mine, the brilliant idea of a binary moon clock belongs to the human who dreamed it up! 🚀

No ownership claimed over the concept - just having fun with C, time, and celestial bodies! ✨