# 🍎 Widget Developer Quick Start

**Target**: iOS/macOS widget developers who want binary clock functionality  
**Time to integration**: 5 minutes  
**Package needed**: `binary-clock-api-*.tar.gz` (API only)

## 🚀 2-Minute Integration

### Step 1: Download API Package
```bash
curl -L https://github.com/kengggg/binaryclock/releases/latest/download/binary-clock-api-v1.0.0.tar.gz | tar xz
cd api-only
```

### Step 2: Add to Xcode Project
```bash
# Copy files to your Xcode project
cp include/binary_clock_api.h /path/to/your/project/
cp src/binary_clock_api.c /path/to/your/project/

# Or drag & drop in Xcode
```

### Step 3: Use in Swift
```swift
import Foundation

// Bridge to C API
@_silgen_name("binary_clock_get_current_state")
func binary_clock_get_current_state() -> binary_clock_state_t

// Use in your widget
let state = binary_clock_get_current_state()
let hoursBinary = state.hours_units.bits
// hoursBinary now contains [Bool] array of binary bits
```

## 📱 Complete Widget Example

See `examples/SWIFT_INTEGRATION_EXAMPLE.swift` for a complete SwiftUI widget implementation.

## 🔧 Build Settings

### Xcode Configuration
1. **Add files** to your target
2. **Build Settings** → **Header Search Paths** → Add your include directory
3. **Build Phases** → **Compile Sources** → Add `binary_clock_api.c`

### CMake (Alternative)
```cmake
add_subdirectory(binary_clock_api)
target_link_libraries(your_target binary_clock_api)
```

## 📊 Performance Notes

- **API calls**: < 1ms typical execution
- **Memory usage**: 56 bytes per state (stack allocated)
- **Widget refresh**: Perfect for 60Hz updates
- **Thread safety**: Safe for concurrent widget updates

## 💡 Common Use Cases

### Real-time Widget
```swift
Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { _ in
    let state = binary_clock_get_current_state()
    updateWidget(with: state)
}
```

### Battery-Efficient Updates
```swift
// Update only when time actually changes
var lastTimestamp: TimeInterval = 0
let state = binary_clock_get_current_state()
if state.timestamp != lastTimestamp {
    updateWidget(with: state)
    lastTimestamp = state.timestamp
}
```

## 🆘 Troubleshooting

**Build errors?**
- Ensure `binary_clock_api.c` is added to compile sources
- Check header search paths include the API directory

**Runtime crashes?**
- Verify C bridge functions are correctly declared
- Check that the API is compiled with same architecture as your app

**Need help?**
- Check the complete API reference: `docs/API_REFERENCE.md`
- See working examples: `examples/SWIFT_INTEGRATION_EXAMPLE.swift`

---

**Next**: Build your widget with the binary clock data! 🎉