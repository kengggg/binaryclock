# Binary Clock API Reference

## Overview

The Binary Clock API provides a clean, lightweight interface for binary time representation with complete separation between data logic and visualization. This document covers integration examples for various platforms and programming languages.

## Table of Contents

- [Core API Functions](#core-api-functions)
- [Data Structures](#data-structures)
- [C Integration](#c-integration)
- [Swift Integration](#swift-integration)
- [CLI Usage](#cli-usage)
- [JSON API](#json-api)
- [Performance Guidelines](#performance-guidelines)
- [Error Handling](#error-handling)

---

## Core API Functions

### Time State Management

#### `binary_clock_get_current_state()`
```c
binary_clock_state_t binary_clock_get_current_state(void);
```
Returns the current system time as a binary clock state.

**Returns:** Complete binary representation of current time  
**Performance:** < 1ms typical execution  
**Thread Safety:** ✅ Thread-safe

**Example:**
```c
#include <binary_clock_api.h>

binary_clock_state_t state = binary_clock_get_current_state();
if (state.timestamp != 0) {
    // Successfully got current time
    printf("Current time in binary format obtained\n");
}
```

#### `binary_clock_state_from_time()`
```c
binary_clock_state_t binary_clock_state_from_time(const time_components_t* time_comp);
```
Converts specific time components to binary representation.

**Parameters:**
- `time_comp`: Pointer to time components (hours, minutes, seconds)

**Returns:** Binary clock state for specified time  
**Performance:** < 1ms typical execution  
**Thread Safety:** ✅ Thread-safe

**Example:**
```c
time_components_t time = {14, 30, 45}; // 2:30:45 PM
binary_clock_state_t state = binary_clock_state_from_time(&time);
```

#### `binary_clock_get_current_time()`
```c
time_components_t binary_clock_get_current_time(void);
```
Gets current time as standard time components (without binary conversion).

**Returns:** Current time as hours, minutes, seconds  
**Performance:** < 1ms typical execution  
**Thread Safety:** ✅ Thread-safe

### Binary Conversion

#### `binary_clock_to_binary()`
```c
binary_value_t binary_clock_to_binary(uint8_t value, uint8_t bit_count);
```
Converts decimal value to binary representation.

**Parameters:**
- `value`: Decimal value (0-63)
- `bit_count`: Number of bits (1-6)

**Returns:** Binary representation with specified bit count  
**Thread Safety:** ✅ Thread-safe

#### `binary_clock_to_decimal()`
```c
uint8_t binary_clock_to_decimal(const binary_value_t* binary);
```
Converts binary representation back to decimal.

**Parameters:**
- `binary`: Pointer to binary value structure

**Returns:** Decimal value  
**Thread Safety:** ✅ Thread-safe

---

## Data Structures

### `binary_value_t`
Represents a binary value with configurable bit count.

```c
typedef struct {
    uint8_t bit_count;      // Number of significant bits (1-6)
    bool bits[6];           // Bit array (MSB first)
    uint8_t decimal_value;  // Original decimal value (0-63)
} binary_value_t;
```

**Memory Layout:** 8 bytes total, 1-byte aligned

### `time_components_t`
Standard time representation.

```c
typedef struct {
    uint8_t hours;    // Hours in 24-hour format (0-23)
    uint8_t minutes;  // Minutes (0-59)
    uint8_t seconds;  // Seconds (0-59)
} time_components_t;
```

**Memory Layout:** 3 bytes total, 1-byte aligned

### `binary_clock_state_t`
Complete binary clock state representation.

```c
typedef struct {
    binary_value_t hours_tens;     // Hours tens digit (0-2, 3 bits)
    binary_value_t hours_units;    // Hours units digit (0-9, 4 bits)
    binary_value_t minutes_tens;   // Minutes tens digit (0-5, 3 bits)
    binary_value_t minutes_units;  // Minutes units digit (0-9, 4 bits)
    binary_value_t seconds_tens;   // Seconds tens digit (0-5, 3 bits)
    binary_value_t seconds_units;  // Seconds units digit (0-9, 4 bits)
    time_t timestamp;              // Unix timestamp when state was created
} binary_clock_state_t;
```

**Memory Layout:** 56 bytes total, 8-byte aligned

---

## C Integration

### Basic Usage

```c
#include <binary_clock_api.h>
#include <stdio.h>

int main() {
    // Get current binary time state
    binary_clock_state_t state = binary_clock_get_current_state();
    
    if (state.timestamp == 0) {
        fprintf(stderr, "Failed to get current time\n");
        return 1;
    }
    
    // Extract readable time
    int hours = state.hours_tens.decimal_value * 10 + state.hours_units.decimal_value;
    int minutes = state.minutes_tens.decimal_value * 10 + state.minutes_units.decimal_value;
    int seconds = state.seconds_tens.decimal_value * 10 + state.seconds_units.decimal_value;
    
    printf("Current time: %02d:%02d:%02d\n", hours, minutes, seconds);
    
    // Access binary representation
    printf("Hours tens binary: ");
    for (int i = 0; i < state.hours_tens.bit_count; i++) {
        printf("%d", state.hours_tens.bits[i] ? 1 : 0);
    }
    printf("\n");
    
    return 0;
}
```

### Widget/Real-time Usage

```c
#include <binary_clock_api.h>
#include <stdio.h>

// Callback function for time updates
void on_time_update(const binary_clock_state_t* state) {
    // Custom rendering logic here
    printf("Time updated: %ld\n", (long)state->timestamp);
}

int main() {
    while (1) {
        binary_clock_state_t state = binary_clock_get_current_state();
        on_time_update(&state);
        
        // Sleep for 1 second (platform-specific implementation)
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif
    }
    return 0;
}
```

### Custom Time Processing

```c
#include <binary_clock_api.h>

void process_specific_time(int h, int m, int s) {
    time_components_t time = {h, m, s};
    binary_clock_state_t state = binary_clock_state_from_time(&time);
    
    if (state.timestamp == 0) {
        printf("Invalid time: %02d:%02d:%02d\n", h, m, s);
        return;
    }
    
    // Process each time component's binary representation
    binary_value_t components[] = {
        state.hours_tens, state.hours_units,
        state.minutes_tens, state.minutes_units,
        state.seconds_tens, state.seconds_units
    };
    
    for (int i = 0; i < 6; i++) {
        printf("Component %d: decimal=%d, bits=", i, components[i].decimal_value);
        for (int j = 0; j < components[i].bit_count; j++) {
            printf("%d", components[i].bits[j] ? 1 : 0);
        }
        printf("\n");
    }
}

int main() {
    process_specific_time(13, 37, 42);  // Process 1:37:42 PM
    return 0;
}
```

---

## Swift Integration

### Basic Swift Wrapper

Create a Swift wrapper for iOS/macOS widget development:

```swift
import Foundation

// Swift wrapper for binary clock API
@objc public class BinaryClockAPI: NSObject {
    
    // MARK: - Data Structures
    
    @objc public class BinaryValue: NSObject {
        @objc public let bitCount: UInt8
        @objc public let bits: [Bool]
        @objc public let decimalValue: UInt8
        
        init(bitCount: UInt8, bits: [Bool], decimalValue: UInt8) {
            self.bitCount = bitCount
            self.bits = bits
            self.decimalValue = decimalValue
            super.init()
        }
    }
    
    @objc public class TimeComponents: NSObject {
        @objc public let hours: UInt8
        @objc public let minutes: UInt8
        @objc public let seconds: UInt8
        
        @objc public init(hours: UInt8, minutes: UInt8, seconds: UInt8) {
            self.hours = hours
            self.minutes = minutes
            self.seconds = seconds
            super.init()
        }
    }
    
    @objc public class BinaryClockState: NSObject {
        @objc public let hoursTens: BinaryValue
        @objc public let hoursUnits: BinaryValue
        @objc public let minutesTens: BinaryValue
        @objc public let minutesUnits: BinaryValue
        @objc public let secondsTens: BinaryValue
        @objc public let secondsUnits: BinaryValue
        @objc public let timestamp: TimeInterval
        
        init(hoursTens: BinaryValue, hoursUnits: BinaryValue,
             minutesTens: BinaryValue, minutesUnits: BinaryValue,
             secondsTens: BinaryValue, secondsUnits: BinaryValue,
             timestamp: TimeInterval) {
            self.hoursTens = hoursTens
            self.hoursUnits = hoursUnits
            self.minutesTens = minutesTens
            self.minutesUnits = minutesUnits
            self.secondsTens = secondsTens
            self.secondsUnits = secondsUnits
            self.timestamp = timestamp
            super.init()
        }
    }
    
    // MARK: - API Methods
    
    @objc public static func getCurrentState() -> BinaryClockState? {
        // Call C API through bridge
        // Implementation would use C interop
        return parseStateFromJSON()
    }
    
    @objc public static func getStateFrom(timeComponents: TimeComponents) -> BinaryClockState? {
        // Call C API through bridge
        return parseStateFromJSON()
    }
    
    // MARK: - Helper Methods
    
    private static func parseStateFromJSON() -> BinaryClockState? {
        // Use CLI tool to get JSON output
        let process = Process()
        process.executableURL = Bundle.main.url(forResource: "binary_clock", withExtension: "")
        process.arguments = ["--display=json"]
        
        let pipe = Pipe()
        process.standardOutput = pipe
        
        do {
            try process.run()
            process.waitUntilExit()
            
            let data = pipe.fileHandleForReading.readDataToEndOfFile()
            let json = try JSONSerialization.jsonObject(with: data) as? [String: Any]
            
            return parseState(from: json)
        } catch {
            print("Error executing binary clock: \(error)")
            return nil
        }
    }
    
    private static func parseState(from json: [String: Any]?) -> BinaryClockState? {
        guard let json = json,
              let timestamp = json["timestamp"] as? TimeInterval,
              let binary = json["binary"] as? [String: Any],
              let hours = binary["hours"] as? [String: Any],
              let minutes = binary["minutes"] as? [String: Any],
              let seconds = binary["seconds"] as? [String: Any] else {
            return nil
        }
        
        // Parse binary values from JSON
        let hoursTens = parseBinaryValue(from: hours["tens"] as? [Int])
        let hoursUnits = parseBinaryValue(from: hours["units"] as? [Int])
        let minutesTens = parseBinaryValue(from: minutes["tens"] as? [Int])
        let minutesUnits = parseBinaryValue(from: minutes["units"] as? [Int])
        let secondsTens = parseBinaryValue(from: seconds["tens"] as? [Int])
        let secondsUnits = parseBinaryValue(from: seconds["units"] as? [Int])
        
        return BinaryClockState(
            hoursTens: hoursTens,
            hoursUnits: hoursUnits,
            minutesTens: minutesTens,
            minutesUnits: minutesUnits,
            secondsTens: secondsTens,
            secondsUnits: secondsUnits,
            timestamp: timestamp
        )
    }
    
    private static func parseBinaryValue(from bits: [Int]?) -> BinaryValue {
        guard let bits = bits else {
            return BinaryValue(bitCount: 0, bits: [], decimalValue: 0)
        }
        
        let boolBits = bits.map { $0 != 0 }
        let decimal = bits.enumerated().reduce(0) { result, item in
            let (index, bit) = item
            return result + (bit * Int(pow(2.0, Double(bits.count - 1 - index))))
        }
        
        return BinaryValue(
            bitCount: UInt8(bits.count),
            bits: boolBits,
            decimalValue: UInt8(decimal)
        )
    }
}
```

### SwiftUI Widget Example

```swift
import SwiftUI
import WidgetKit

struct BinaryClockWidget: Widget {
    let kind: String = "BinaryClockWidget"
    
    var body: some WidgetConfiguration {
        StaticConfiguration(kind: kind, provider: BinaryClockProvider()) { entry in
            BinaryClockWidgetEntryView(entry: entry)
        }
        .configurationDisplayName("Binary Clock")
        .description("Displays current time in binary format using moon phases")
        .supportedFamilies([.systemSmall, .systemMedium])
    }
}

struct BinaryClockEntry: TimelineEntry {
    let date: Date
    let state: BinaryClockAPI.BinaryClockState?
}

struct BinaryClockProvider: TimelineProvider {
    func placeholder(in context: Context) -> BinaryClockEntry {
        BinaryClockEntry(date: Date(), state: nil)
    }
    
    func getSnapshot(in context: Context, completion: @escaping (BinaryClockEntry) -> ()) {
        let entry = BinaryClockEntry(
            date: Date(),
            state: BinaryClockAPI.getCurrentState()
        )
        completion(entry)
    }
    
    func getTimeline(in context: Context, completion: @escaping (Timeline<BinaryClockEntry>) -> ()) {
        let currentDate = Date()
        let entries = (0..<60).map { offset in
            BinaryClockEntry(
                date: Calendar.current.date(byAdding: .second, value: offset, to: currentDate)!,
                state: BinaryClockAPI.getCurrentState()
            )
        }
        
        let timeline = Timeline(entries: entries, policy: .atEnd)
        completion(timeline)
    }
}

struct BinaryClockWidgetEntryView: View {
    var entry: BinaryClockEntry
    
    var body: some View {
        VStack {
            if let state = entry.state {
                BinaryTimeView(state: state)
            } else {
                Text("Loading...")
                    .foregroundColor(.secondary)
            }
        }
        .padding()
    }
}

struct BinaryTimeView: View {
    let state: BinaryClockAPI.BinaryClockState
    
    var body: some View {
        VStack(spacing: 4) {
            // Hours
            HStack(spacing: 2) {
                BinaryDigitView(value: state.hoursTens)
                BinaryDigitView(value: state.hoursUnits)
            }
            
            // Minutes  
            HStack(spacing: 2) {
                BinaryDigitView(value: state.minutesTens)
                BinaryDigitView(value: state.minutesUnits)
            }
            
            // Seconds
            HStack(spacing: 2) {
                BinaryDigitView(value: state.secondsTens)
                BinaryDigitView(value: state.secondsUnits)
            }
        }
    }
}

struct BinaryDigitView: View {
    let value: BinaryClockAPI.BinaryValue
    
    var body: some View {
        VStack(spacing: 1) {
            ForEach(0..<Int(value.bitCount), id: \.self) { index in
                Circle()
                    .fill(value.bits[index] ? Color.yellow : Color.gray)
                    .frame(width: 8, height: 8)
            }
        }
    }
}
```

---

## CLI Usage

### Command Line Interface

The binary clock provides a comprehensive CLI for various integration scenarios:

#### Single Output Modes
```bash
# Default moon emoji display
./binary_clock

# Binary 0s and 1s
./binary_clock --display=binary

# JSON format (perfect for APIs)
./binary_clock --display=json

# Raw API data structures
./binary_clock --display=raw
```

#### Continuous Modes
```bash
# Continuous emoji display (classic behavior)
./binary_clock --loop

# Continuous JSON stream
./binary_clock --display=json --loop

# Continuous binary display
./binary_clock --display=binary --loop
```

#### Help and Options
```bash
# Show usage information
./binary_clock --help
./binary_clock -h
```

### Script Integration Examples

#### Bash Script Integration
```bash
#!/bin/bash

# Get current time as JSON
json_output=$(./binary_clock --display=json)
echo "Current binary time: $json_output"

# Extract timestamp using jq
timestamp=$(echo "$json_output" | jq '.timestamp')
echo "Unix timestamp: $timestamp"

# Monitor time changes
while true; do
    ./binary_clock --display=binary
    sleep 1
done
```

#### Python Script Integration
```python
#!/usr/bin/env python3
import subprocess
import json
import time

def get_binary_time():
    """Get current binary time as structured data."""
    try:
        result = subprocess.run(
            ['./binary_clock', '--display=json'],
            capture_output=True,
            text=True,
            check=True
        )
        return json.loads(result.stdout)
    except (subprocess.CalledProcessError, json.JSONDecodeError) as e:
        print(f"Error getting binary time: {e}")
        return None

def monitor_binary_time():
    """Monitor binary time changes."""
    while True:
        data = get_binary_time()
        if data:
            print(f"Time: {data['time']}")
            print(f"Hours binary: {data['binary']['hours']}")
        time.sleep(1)

if __name__ == "__main__":
    # Get single reading
    current_time = get_binary_time()
    print(json.dumps(current_time, indent=2))
    
    # Or monitor continuously
    # monitor_binary_time()
```

---

## JSON API

### JSON Output Format

The `--display=json` mode provides structured data perfect for API integration:

```json
{
  "timestamp": 1750047329,
  "time": "14:30:45",
  "binary": {
    "hours": {
      "tens": [0,0,1],
      "units": [0,1,0,0]
    },
    "minutes": {
      "tens": [0,1,1],
      "units": [0,0,0,0]
    },
    "seconds": {
      "tens": [0,1,0],
      "units": [0,1,0,1]
    }
  }
}
```

### Field Descriptions

- **timestamp**: Unix timestamp when the state was captured
- **time**: Human-readable time in HH:MM:SS format
- **binary**: Object containing binary representations
  - **hours/minutes/seconds**: Each split into tens and units digits
  - **tens/units**: Arrays of 0s and 1s representing binary bits (MSB first)

### API Integration Examples

#### REST API Endpoint (Node.js)
```javascript
const express = require('express');
const { exec } = require('child_process');

const app = express();

app.get('/api/binary-time', (req, res) => {
    exec('./binary_clock --display=json', (error, stdout, stderr) => {
        if (error) {
            res.status(500).json({ error: 'Failed to get binary time' });
            return;
        }
        
        try {
            const binaryTime = JSON.parse(stdout);
            res.json(binaryTime);
        } catch (parseError) {
            res.status(500).json({ error: 'Failed to parse binary time' });
        }
    });
});

app.listen(3000, () => {
    console.log('Binary Clock API running on port 3000');
});
```

#### Web Socket Stream
```javascript
const WebSocket = require('ws');
const { spawn } = require('child_process');

const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', (ws) => {
    console.log('Client connected to binary time stream');
    
    // Start continuous binary clock process
    const clockProcess = spawn('./binary_clock', ['--display=json', '--loop']);
    
    clockProcess.stdout.on('data', (data) => {
        try {
            const binaryTime = JSON.parse(data.toString());
            ws.send(JSON.stringify(binaryTime));
        } catch (error) {
            console.error('Failed to parse binary time:', error);
        }
    });
    
    ws.on('close', () => {
        clockProcess.kill();
        console.log('Client disconnected');
    });
});
```

---

## Performance Guidelines

### Core API Performance

- **State Queries**: < 1ms typical execution
- **Binary Conversions**: < 0.1ms typical execution
- **Memory Usage**: Zero heap allocation (stack only)
- **Thread Safety**: All functions are thread-safe

### Real-time Applications

For widget or real-time applications requiring 60Hz updates:

```c
#include <binary_clock_api.h>
#include <time.h>

// High-performance time monitoring
void high_frequency_update_loop() {
    binary_clock_state_t last_state = {0};
    
    while (1) {
        binary_clock_state_t current_state = binary_clock_get_current_state();
        
        // Only update if time actually changed
        if (current_state.timestamp != last_state.timestamp) {
            // Update your display here
            update_display(&current_state);
            last_state = current_state;
        }
        
        // Sleep for 16ms for 60Hz updates
        #ifdef _WIN32
            Sleep(16);
        #else
            usleep(16000);
        #endif
    }
}
```

### CLI Performance Tips

- Use single output mode for scripts: `./binary_clock --display=json`
- Batch multiple readings instead of calling repeatedly
- Cache JSON parsing results when possible
- Use appropriate update frequencies (1Hz for clocks, 60Hz for animations)

---

## Error Handling

### Error Codes

```c
typedef enum {
    BINARY_CLOCK_SUCCESS = 0,
    BINARY_CLOCK_ERROR_INVALID_TIME = 1,
    BINARY_CLOCK_ERROR_INVALID_BIT_COUNT = 2,
    BINARY_CLOCK_ERROR_NULL_POINTER = 3,
    BINARY_CLOCK_ERROR_SYSTEM_TIME = 4
} binary_clock_error_t;
```

### Error Detection

```c
#include <binary_clock_api.h>

void safe_time_processing() {
    // Check for system time errors
    binary_clock_state_t state = binary_clock_get_current_state();
    if (state.timestamp == 0) {
        fprintf(stderr, "Error: Failed to get system time\n");
        return;
    }
    
    // Check for invalid time input
    time_components_t invalid_time = {25, 70, 80}; // Invalid values
    binary_clock_state_t invalid_state = binary_clock_state_from_time(&invalid_time);
    if (invalid_state.timestamp == 0) {
        fprintf(stderr, "Error: Invalid time provided\n");
        return;
    }
    
    // Check for null pointer
    uint8_t result = binary_clock_to_decimal(NULL);
    if (result == 0) {
        // Note: 0 could be valid or error - check input
        fprintf(stderr, "Warning: Possible null pointer passed\n");
    }
}
```

### CLI Error Handling

```bash
#!/bin/bash

# Check if binary_clock exists
if ! command -v ./binary_clock &> /dev/null; then
    echo "Error: binary_clock not found"
    exit 1
fi

# Get binary time with error checking
if output=$(./binary_clock --display=json 2>/dev/null); then
    echo "Success: $output"
else
    echo "Error: Failed to get binary time (exit code: $?)"
    exit 1
fi
```

---

## Platform-Specific Notes

### Compilation and Include Setup

The API uses **portable system-style includes** for maximum compatibility:

```c
#include <binary_clock_api.h>     // ✅ Portable system-style include
#include <binary_clock_display.h> // ✅ Portable system-style include

// ❌ Avoid relative includes like "../include/binary_clock_api.h"
```

**Compilation with Include Paths:**
```bash
# Method 1: Specify include directory
gcc -I/path/to/binary_clock/include -o myapp myapp.c binary_clock_api.o

# Method 2: Use local project structure
gcc -Iinclude -o myapp myapp.c build/binary_clock_api.o build/binary_clock_display.o

# Method 3: System-wide installation
sudo cp include/*.h /usr/local/include/
gcc -o myapp myapp.c -lbinary_clock
```

**Distribution Options:**
1. **Include headers in your project**: Copy `binary_clock_api.h` to your include directory
2. **Package as library**: Create installable package with headers and compiled objects
3. **System installation**: Install headers system-wide for multiple projects

### Windows
- Use MinGW or MSYS2 for compilation
- Executable will be `binary_clock.exe`
- Sleep function uses milliseconds: `Sleep(1000)`
- Screen clear command: `cls`
- Include path: `-I C:\path\to\headers`

### macOS/Linux
- Use GCC or Clang for compilation
- Executable will be `binary_clock`
- Sleep function uses seconds: `sleep(1)`
- Screen clear command: `clear`
- Include path: `-I /path/to/headers`

### Build Requirements
- C99-compliant compiler
- Standard C library
- POSIX compliance for Unix platforms
- Windows API for Windows platforms
- Proper include path configuration (`-I` flag)

---

## Complete Examples

### Minimal Widget Data Fetcher
```c
#include <binary_clock_api.h>
#include <stdio.h>

// Minimal example for widget integration
int main() {
    binary_clock_state_t state = binary_clock_get_current_state();
    
    // Convert to widget-friendly format
    int time_digits[6] = {
        state.hours_tens.decimal_value,
        state.hours_units.decimal_value,
        state.minutes_tens.decimal_value,
        state.minutes_units.decimal_value,
        state.seconds_tens.decimal_value,
        state.seconds_units.decimal_value
    };
    
    // Output as simple format for widget consumption
    printf("%d %d %d %d %d %d\n", 
           time_digits[0], time_digits[1], time_digits[2],
           time_digits[3], time_digits[4], time_digits[5]);
    
    return 0;
}
```

This API reference provides comprehensive integration examples for the Binary Clock API across different platforms and use cases. The modular design ensures easy integration while maintaining high performance and cross-platform compatibility.
