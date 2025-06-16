# 💻 CLI User Quick Start

**Target**: End users who want a ready-to-run binary clock  
**Time to running**: 1 minute  
**Package needed**: `binary-clock-cli-*-{platform}.tar.gz` (Ready-to-run binary)

## ⚡ 30-Second Setup

### Linux/macOS
```bash
# Download and extract (replace with your platform)
curl -L https://github.com/kengggg/binaryclock/releases/latest/download/binary-clock-cli-v1.0.0-linux-x86_64.tar.gz | tar xz
cd cli

# Run immediately
./binary_clock
```

### Windows
```powershell
# Download and extract
curl -L https://github.com/kengggg/binaryclock/releases/latest/download/binary-clock-cli-v1.0.0-windows-x86_64.zip -o binary-clock.zip
# Extract with Windows Explorer or: tar -xf binary-clock.zip (Windows 10+)
cd cli

# Run immediately
binary_clock.exe
```

## 🎯 Usage Modes

### Single Output (Default)
```bash
./binary_clock                    # Moon emoji display 🌚🌝
./binary_clock --display=binary   # 0s and 1s
./binary_clock --display=json     # JSON format
./binary_clock --display=raw      # Raw API data
```

### Continuous Mode
```bash
./binary_clock --loop                      # Classic binary clock
./binary_clock --display=json --loop       # JSON stream
./binary_clock --display=binary --loop     # 0s and 1s stream
```

## 📋 Command Reference

### Display Modes
| Mode | Description | Best For |
|------|-------------|----------|
| `emoji` | Moon emojis 🌚🌝 | Visual/fun display |
| `binary` | 0s and 1s | Learning binary |
| `json` | Structured data | Scripts/automation |
| `raw` | API internals | Debugging |

### Options
| Option | Description | Example |
|--------|-------------|---------|
| `--display=MODE` | Set display mode | `--display=json` |
| `--loop` | Continuous updates | `--loop` |
| `--help`, `-h` | Show help | `--help` |

## 🔧 Integration with Scripts

### Bash Script
```bash
#!/bin/bash
# Get current time as JSON
TIME_JSON=$(./binary_clock --display=json)
echo "Current binary time: $TIME_JSON"

# Extract timestamp
TIMESTAMP=$(echo "$TIME_JSON" | jq '.timestamp')
echo "Unix timestamp: $TIMESTAMP"
```

### Python Script
```python
import subprocess
import json

# Get binary time
result = subprocess.run(['./binary_clock', '--display=json'], 
                       capture_output=True, text=True)
time_data = json.loads(result.stdout)

print(f"Time: {time_data['time']}")
print(f"Binary hours: {time_data['binary']['hours']}")
```

### PowerShell (Windows)
```powershell
# Get JSON output
$timeJson = .\binary_clock.exe --display=json | ConvertFrom-Json
Write-Host "Current time: $($timeJson.time)"
Write-Host "Binary representation available in object"
```

## 🎨 Display Examples

### Emoji Mode (Default)
```
🌝 Binary Clock 🌚
Time: 14:30:45

Hours   : 🌚🌚🌝 🌚🌝🌚🌚
Minutes : 🌚🌝🌝 🌚🌚🌚🌚  
Seconds : 🌚🌝🌚 🌚🌝🌚🌝
```

### Binary Mode
```
Binary Clock (ASCII)
Time: 14:30:45

Hours   : 001 0100
Minutes : 011 0000
Seconds : 010 0101
```

### JSON Mode
```json
{
  "timestamp": 1750047329,
  "time": "14:30:45",
  "binary": {
    "hours": {"tens": [0,0,1], "units": [0,1,0,0]},
    "minutes": {"tens": [0,1,1], "units": [0,0,0,0]},
    "seconds": {"tens": [0,1,0], "units": [0,1,0,1]}
  }
}
```

## 🚀 Installation (Optional)

### System-wide Installation
```bash
# Linux/macOS
sudo cp binary_clock /usr/local/bin/
binary_clock --help  # Now available globally

# Windows (Run as Administrator)
copy binary_clock.exe C:\Windows\System32\
binary_clock --help  # Now available in any directory
```

### Create Desktop Shortcut
```bash
# Linux (create .desktop file)
cat > ~/.local/share/applications/binary-clock.desktop << EOF
[Desktop Entry]
Name=Binary Clock
Exec=/path/to/binary_clock --loop
Icon=clock
Type=Application
Categories=Utility;
EOF

# macOS (create app bundle)
mkdir -p BinaryClock.app/Contents/MacOS
cp binary_clock BinaryClock.app/Contents/MacOS/
# Double-click to run
```

## 💡 Pro Tips

**Continuous JSON monitoring:**
```bash
./binary_clock --display=json --loop | jq '.time'
```

**Log binary time to file:**
```bash
./binary_clock --display=json >> binary_time.log
```

**Use as screensaver replacement:**
```bash
while true; do 
  clear
  ./binary_clock --display=emoji
  sleep 1
done
```

## 🆘 Troubleshooting

**Permission denied?**
```bash
chmod +x binary_clock  # Make executable
```

**Command not found?**
- Ensure you're in the correct directory (`cd cli`)
- Use `./binary_clock` (with `./`) on Unix systems

**Emoji not showing correctly?**
- Ensure your terminal supports Unicode
- Try `--display=binary` for ASCII-only output

---

**Enjoy your lunar binary clock!** 🌚🌝⏰
