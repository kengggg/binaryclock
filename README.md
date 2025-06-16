# 🌚🌝 Binary Clock 🌝🌚

[![CI](https://github.com/USERNAME/binaryclock/actions/workflows/ci.yml/badge.svg)](https://github.com/USERNAME/binaryclock/actions/workflows/ci.yml)

A delightfully lunar way to tell time! This C program displays the current time in binary format using moon emojis - because who needs boring decimal digits when you can have 🌚 (0) and 🌝 (1)?

## What This Does

Ever wondered what time it is in binary? Wonder no more! This clock shows:
- Hours in binary (split into tens and units)
- Minutes in binary (split into tens and units) 
- Seconds in binary (split into tens and units)

All displayed with the cosmic beauty of moon phases! 🌙

## Quick Start

```bash
# Build and run the magic
make run

# Or if you want to do it manually
make all
./binary_clock
```

Press Ctrl+C to gracefully exit when you're done basking in binary time.

## Building & Testing

```bash
# Build everything
make all

# Run the comprehensive test suite (41+ tests!)
make test

# Clean up build artifacts
make clean
```

## How It Works

The clock updates every second, clearing the screen and showing the current time in glorious binary format. Each time component (hours, minutes, seconds) is split into tens and units, then converted to binary and displayed with moon emojis.

Example: If it's 13:37:42
- Hours: 1 (🌝🌚🌚🌝) and 3 (🌚🌚🌝🌝)
- Minutes: 3 (🌚🌚🌝🌝) and 7 (🌚🌝🌝🌝)
- Seconds: 4 (🌚🌝🌚🌚) and 2 (🌚🌚🌝🌚)

## Files

- `binary_clock.c` - Main application logic
- `binary_clock_lib.c/h` - Reusable library functions
- `test_*.c` - Comprehensive test suite
- `Makefile` - Build system that makes everything work

## Credits

This code was crafted as requested and represents collaborative human-AI programming. While the implementation is mine, the brilliant idea of a binary moon clock belongs to the human who dreamed it up! 🚀

No ownership claimed over the concept - just having fun with C, time, and celestial bodies! ✨