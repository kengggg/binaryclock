import Foundation
import SwiftUI
import WidgetKit

/**
 * Binary Clock Swift Integration Example
 * 
 * This file demonstrates how to integrate the Binary Clock API
 * into iOS/macOS applications and widgets using Swift.
 */

// MARK: - Data Models

/// Swift representation of binary clock data
struct BinaryClockData: Codable {
    let timestamp: TimeInterval
    let time: String
    let binary: BinaryComponents
    
    struct BinaryComponents: Codable {
        let hours: TimeComponent
        let minutes: TimeComponent
        let seconds: TimeComponent
    }
    
    struct TimeComponent: Codable {
        let tens: [Int]
        let units: [Int]
    }
}

// MARK: - Binary Clock Service

class BinaryClockService: ObservableObject {
    @Published var currentState: BinaryClockData?
    @Published var isConnected: Bool = false
    
    private var timer: Timer?
    
    /// Get current binary clock state by calling the CLI tool
    func getCurrentState() async -> BinaryClockData? {
        return await withCheckedContinuation { continuation in
            getBinaryClockData { result in
                continuation.resume(returning: result)
            }
        }
    }
    
    /// Start continuous updates
    func startUpdates() {
        timer?.invalidate()
        timer = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { _ in
            Task {
                if let newState = await self.getCurrentState() {
                    DispatchQueue.main.async {
                        self.currentState = newState
                        self.isConnected = true
                    }
                } else {
                    DispatchQueue.main.async {
                        self.isConnected = false
                    }
                }
            }
        }
        timer?.fire()
    }
    
    /// Stop continuous updates
    func stopUpdates() {
        timer?.invalidate()
        timer = nil
        isConnected = false
    }
    
    // MARK: - Private Methods
    
    private func getBinaryClockData(completion: @escaping (BinaryClockData?) -> Void) {
        let process = Process()
        let pipe = Pipe()
        
        // Configure process to run binary_clock CLI
        process.standardOutput = pipe
        process.standardError = pipe
        process.arguments = ["--display=json"]
        
        // Try to find the binary_clock executable
        if let executablePath = findBinaryClockExecutable() {
            process.executableURL = URL(fileURLWithPath: executablePath)
        } else {
            print("Binary clock executable not found")
            completion(nil)
            return
        }
        
        do {
            try process.run()
            process.waitUntilExit()
            
            let data = pipe.fileHandleForReading.readDataToEndOfFile()
            
            if process.terminationStatus == 0 {
                // Parse JSON output
                let decoder = JSONDecoder()
                if let binaryData = try? decoder.decode(BinaryClockData.self, from: data) {
                    completion(binaryData)
                } else {
                    print("Failed to decode binary clock JSON")
                    completion(nil)
                }
            } else {
                print("Binary clock process failed with status: \(process.terminationStatus)")
                if let errorString = String(data: data, encoding: .utf8) {
                    print("Error output: \(errorString)")
                }
                completion(nil)
            }
        } catch {
            print("Failed to run binary clock process: \(error)")
            completion(nil)
        }
    }
    
    private func findBinaryClockExecutable() -> String? {
        let possiblePaths = [
            Bundle.main.path(forResource: "binary_clock", ofType: nil),
            Bundle.main.path(forResource: "binary_clock", ofType: "exe"),
            "/usr/local/bin/binary_clock",
            "./binary_clock",
            "../binary_clock"
        ]
        
        for path in possiblePaths {
            if let path = path, FileManager.default.fileExists(atPath: path) {
                return path
            }
        }
        
        return nil
    }
}

// MARK: - SwiftUI Views

/// Main binary clock display view
struct BinaryClockView: View {
    @StateObject private var clockService = BinaryClockService()
    
    var body: some View {
        VStack(spacing: 20) {
            Text("Binary Clock")
                .font(.largeTitle)
                .fontWeight(.bold)
            
            if let state = clockService.currentState {
                VStack(spacing: 10) {
                    // Display readable time
                    Text(state.time)
                        .font(.title)
                        .monospacedDigit()
                    
                    // Display binary representation
                    BinaryDisplayView(binaryData: state.binary)
                }
            } else {
                if clockService.isConnected {
                    ProgressView("Loading...")
                } else {
                    VStack {
                        Image(systemName: "exclamationmark.triangle")
                            .foregroundColor(.orange)
                            .font(.title)
                        Text("Cannot connect to binary clock")
                            .foregroundColor(.secondary)
                    }
                }
            }
            
            // Connection status
            HStack {
                Circle()
                    .fill(clockService.isConnected ? Color.green : Color.red)
                    .frame(width: 8, height: 8)
                Text(clockService.isConnected ? "Connected" : "Disconnected")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
        .onAppear {
            clockService.startUpdates()
        }
        .onDisappear {
            clockService.stopUpdates()
        }
    }
}

/// Binary digit display component
struct BinaryDisplayView: View {
    let binaryData: BinaryClockData.BinaryComponents
    
    var body: some View {
        VStack(spacing: 8) {
            // Hours
            HStack(spacing: 4) {
                Text("H:")
                    .font(.caption)
                    .foregroundColor(.secondary)
                BinaryDigitView(digits: binaryData.hours.tens, color: .blue)
                BinaryDigitView(digits: binaryData.hours.units, color: .blue)
            }
            
            // Minutes
            HStack(spacing: 4) {
                Text("M:")
                    .font(.caption)
                    .foregroundColor(.secondary)
                BinaryDigitView(digits: binaryData.minutes.tens, color: .green)
                BinaryDigitView(digits: binaryData.minutes.units, color: .green)
            }
            
            // Seconds
            HStack(spacing: 4) {
                Text("S:")
                    .font(.caption)
                    .foregroundColor(.secondary)
                BinaryDigitView(digits: binaryData.seconds.tens, color: .orange)
                BinaryDigitView(digits: binaryData.seconds.units, color: .orange)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

/// Individual binary digit display
struct BinaryDigitView: View {
    let digits: [Int]
    let color: Color
    
    var body: some View {
        VStack(spacing: 2) {
            ForEach(0..<digits.count, id: \.self) { index in
                Circle()
                    .fill(digits[index] == 1 ? color : Color.gray.opacity(0.3))
                    .frame(width: 12, height: 12)
                    .overlay(
                        Circle()
                            .stroke(color.opacity(0.5), lineWidth: 1)
                    )
            }
        }
        .padding(.horizontal, 4)
    }
}

// MARK: - Widget Implementation

struct BinaryClockWidget: Widget {
    let kind: String = "BinaryClockWidget"
    
    var body: some WidgetConfiguration {
        StaticConfiguration(kind: kind, provider: BinaryClockProvider()) { entry in
            BinaryClockWidgetEntryView(entry: entry)
        }
        .configurationDisplayName("Binary Clock")
        .description("Shows the current time in binary format")
        .supportedFamilies([.systemSmall, .systemMedium])
    }
}

struct BinaryClockEntry: TimelineEntry {
    let date: Date
    let binaryData: BinaryClockData?
}

struct BinaryClockProvider: TimelineProvider {
    func placeholder(in context: Context) -> BinaryClockEntry {
        BinaryClockEntry(
            date: Date(),
            binaryData: createPlaceholderData()
        )
    }
    
    func getSnapshot(in context: Context, completion: @escaping (BinaryClockEntry) -> ()) {
        Task {
            let service = BinaryClockService()
            let data = await service.getCurrentState()
            let entry = BinaryClockEntry(date: Date(), binaryData: data)
            completion(entry)
        }
    }
    
    func getTimeline(in context: Context, completion: @escaping (Timeline<BinaryClockEntry>) -> ()) {
        let currentDate = Date()
        var entries: [BinaryClockEntry] = []
        
        // Create timeline entries for the next hour (updates every minute)
        Task {
            let service = BinaryClockService()
            
            for offset in 0..<60 {
                let entryDate = Calendar.current.date(byAdding: .minute, value: offset, to: currentDate)!
                let data = await service.getCurrentState()
                let entry = BinaryClockEntry(date: entryDate, binaryData: data)
                entries.append(entry)
            }
            
            let timeline = Timeline(entries: entries, policy: .atEnd)
            completion(timeline)
        }
    }
    
    private func createPlaceholderData() -> BinaryClockData {
        return BinaryClockData(
            timestamp: Date().timeIntervalSince1970,
            time: "12:34:56",
            binary: BinaryClockData.BinaryComponents(
                hours: BinaryClockData.TimeComponent(tens: [0,0,1], units: [0,0,1,0]),
                minutes: BinaryClockData.TimeComponent(tens: [0,1,1], units: [0,1,0,0]),
                seconds: BinaryClockData.TimeComponent(tens: [0,1,0], units: [0,1,1,0])
            )
        )
    }
}

struct BinaryClockWidgetEntryView: View {
    var entry: BinaryClockEntry
    
    var body: some View {
        VStack {
            if let data = entry.binaryData {
                VStack(spacing: 4) {
                    Text(data.time)
                        .font(.headline)
                        .monospacedDigit()
                    
                    CompactBinaryDisplayView(binaryData: data.binary)
                }
            } else {
                VStack {
                    Image(systemName: "clock.badge.exclamationmark")
                        .font(.title2)
                        .foregroundColor(.secondary)
                    Text("Unavailable")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
        }
        .containerBackground(.regularMaterial, for: .widget)
    }
}

/// Compact binary display for widgets
struct CompactBinaryDisplayView: View {
    let binaryData: BinaryClockData.BinaryComponents
    
    var body: some View {
        VStack(spacing: 2) {
            // Hours
            HStack(spacing: 1) {
                CompactBinaryDigitView(digits: binaryData.hours.tens)
                CompactBinaryDigitView(digits: binaryData.hours.units)
            }
            
            // Minutes
            HStack(spacing: 1) {
                CompactBinaryDigitView(digits: binaryData.minutes.tens)
                CompactBinaryDigitView(digits: binaryData.minutes.units)
            }
            
            // Seconds
            HStack(spacing: 1) {
                CompactBinaryDigitView(digits: binaryData.seconds.tens)
                CompactBinaryDigitView(digits: binaryData.seconds.units)
            }
        }
    }
}

struct CompactBinaryDigitView: View {
    let digits: [Int]
    
    var body: some View {
        VStack(spacing: 1) {
            ForEach(0..<digits.count, id: \.self) { index in
                Circle()
                    .fill(digits[index] == 1 ? Color.primary : Color.secondary.opacity(0.3))
                    .frame(width: 6, height: 6)
            }
        }
        .padding(.horizontal, 1)
    }
}

// MARK: - App Integration Example

@main
struct BinaryClockApp: App {
    var body: some Scene {
        WindowGroup {
            BinaryClockView()
        }
    }
}

// MARK: - Usage Examples

/*
 Integration Examples:
 
 1. Basic usage in SwiftUI view:
    ```swift
    struct ContentView: View {
        var body: some View {
            BinaryClockView()
        }
    }
    ```
 
 2. Get single reading:
    ```swift
    let service = BinaryClockService()
    if let data = await service.getCurrentState() {
        print("Current time: \(data.time)")
        print("Binary representation: \(data.binary)")
    }
    ```
 
 3. Continuous monitoring:
    ```swift
    let service = BinaryClockService()
    service.startUpdates()
    
    // Service will publish updates to @Published currentState
    ```
 
 4. Widget implementation:
    - Add BinaryClockWidget to your widget extension
    - Configure timeline provider for desired update frequency
    - Use CompactBinaryDisplayView for space-efficient display
 
 Build Requirements:
 - Include binary_clock executable in app bundle
 - Add binary_clock to app's Resources or Bundle
 - Ensure executable permissions are set
 - Add appropriate entitlements for process execution
 
 Deployment Notes:
 - For iOS: Include binary_clock compiled for ARM64
 - For macOS: Include universal binary (Intel + Apple Silicon)
 - For simulator: Include x86_64 version
 - Consider code signing requirements for embedded executables
*/
