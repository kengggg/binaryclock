# 🌚🌝 Binary Clock 🌝🌚

[![CI](https://github.com/kengggg/binaryclock/actions/workflows/ci.yml/badge.svg)](https://github.com/kengggg/binaryclock/actions/workflows/ci.yml)
[![Release](https://github.com/kengggg/binaryclock/actions/workflows/release.yml/badge.svg)](https://github.com/kengggg/binaryclock/actions/workflows/release.yml)
[![Latest Release](https://img.shields.io/github/v/release/kengggg/binaryclock?include_prereleases&sort=semver)](https://github.com/kengggg/binaryclock/releases/latest)

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

## 🚀 Quick Start by Use Case

Choose your path based on what you want to do:

### 🍎 **Widget Developer** (2 minutes to integration)
```bash
# Download API-only package (replace v0.1.0 with latest version)
curl -L https://github.com/kengggg/binaryclock/releases/download/v0.1.0/binary-clock-api-v0.1.0.tar.gz | tar xz
cd api-only && make
```
**→ [Complete Widget Guide](docs/QUICKSTART-WIDGET.md)**

### 💻 **CLI User** (30 seconds to running)
```bash
# Download ready-to-run binary (Linux example - replace v0.1.0 with latest version)
curl -L https://github.com/kengggg/binaryclock/releases/download/v0.1.0/binary-clock-cli-v0.1.0-linux-x86_64.tar.gz | tar xz
cd cli && ./binary_clock --help
```
**→ [Complete CLI Guide](docs/QUICKSTART-CLI.md)**

### 📚 **Library Developer** (5 minutes to custom app)
```bash
# Download complete library (replace v0.1.0 with latest version)
curl -L https://github.com/kengggg/binaryclock/releases/download/v0.1.0/binary-clock-library-v0.1.0.tar.gz | tar xz
cd library && make
```
**→ [Complete Library Guide](docs/API_REFERENCE.md)**

### 👨‍💻 **Contributor** (Full development setup)
```bash
# Clone and build from source
git clone https://github.com/kengggg/binaryclock.git
cd binaryclock && make all test
```

## 📦 Distribution Packages

We provide multiple distribution formats to serve different use cases:

| Package | Description | Size | Best For |
|---------|-------------|------|----------|
| **API Only** | Core binary clock API | ~50KB | Widget developers, embedded systems |
| **CLI Binary** | Ready-to-run application | ~500KB | End users, scripting |
| **Library** | API + display utilities | ~100KB | Custom applications |
| **Source** | Complete development environment | ~200KB | Contributors, custom builds |

### Platform Support
- **Linux**: x86_64 (tested on Ubuntu, CentOS, Alpine)
- **macOS**: Universal binary (Intel + Apple Silicon)
- **Windows**: x86_64 (MSYS2/MinGW compatible)
- **Embedded**: C99 compatible systems

### 🔄 Automated Releases

This project features fully automated releases:

1. **Tag-triggered**: Push a version tag (`git tag v1.0.0 && git push origin v1.0.0`) to automatically trigger release builds
2. **Cross-platform**: Automatically builds and packages for Linux, macOS, and Windows
3. **Multiple formats**: Generates persona-specific packages (.tar.gz, .zip) with SHA256 checksums
4. **Comprehensive**: Includes API-only, CLI binaries, complete library, and source distributions
5. **Verified**: All packages include checksums for security verification

**Latest Release**: [View all releases →](https://github.com/kengggg/binaryclock/releases)

## Building & Testing

```bash
# Build everything
make all

# Run the comprehensive test suite (228 total tests!)
make test

# Clean up build artifacts
make clean
```

## Memory Management

This project avoids heap allocation entirely. All state lives on the stack or in
static buffers, which means there is no dynamic memory to free and memory leaks
are structurally prevented. Functions that return strings (like
`binary_clock_get_error_string` and `binary_clock_get_version`) provide pointers
to static data that must not be freed.

## How It Works

The binary clock uses a modular architecture with complete separation between core logic and visualization:

- **Core API**: Handles time conversion and binary representation (data only)
- **Display Layer**: Renders data in various formats (emoji, binary, JSON, raw)
- **CLI Interface**: Provides single-shot or continuous display modes

### Loop Mode
Run the program with `--loop` for continuous updates. Press Ctrl+C to exit.

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
