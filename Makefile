# Cross-platform configuration
ifeq ($(OS),Windows_NT)
    CC = gcc
    TARGET = binary_clock.exe
    TEST_TARGET = test_binary_clock.exe
    SIGNAL_TEST = test_signal_handling.exe
    # MSYS2 provides Unix-like commands
    RM = rm -f
    MKDIR = mkdir -p
    PATHSEP = /
else
    CC = gcc
    TARGET = binary_clock
    TEST_TARGET = test_binary_clock
    SIGNAL_TEST = test_signal_handling
    RM = rm -f
    MKDIR = mkdir -p
    PATHSEP = /
endif

# Directory structure
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build

CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -I$(INCLUDE_DIR)
LIB_OBJ = $(BUILD_DIR)/binary_clock_lib.o
API_OBJ = $(BUILD_DIR)/binary_clock_api.o
DISPLAY_OBJ = $(BUILD_DIR)/binary_clock_display.o
API_TEST_TARGET = test_binary_clock_api

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

# Build the main binary clock application
$(TARGET): $(SRC_DIR)/binary_clock.c $(API_OBJ) $(DISPLAY_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/binary_clock.c $(API_OBJ) $(DISPLAY_OBJ)

# Build the library object file
$(LIB_OBJ): $(SRC_DIR)/binary_clock_lib.c $(INCLUDE_DIR)/binary_clock_lib.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/binary_clock_lib.c -o $(LIB_OBJ)

# Build the API object file
$(API_OBJ): $(SRC_DIR)/binary_clock_api.c $(INCLUDE_DIR)/binary_clock_api.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/binary_clock_api.c -o $(API_OBJ)

# Build the display utilities object file
$(DISPLAY_OBJ): $(SRC_DIR)/binary_clock_display.c $(INCLUDE_DIR)/binary_clock_display.h $(INCLUDE_DIR)/binary_clock_api.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/binary_clock_display.c -o $(DISPLAY_OBJ)

# Build and run tests
test: $(TEST_TARGET) $(SIGNAL_TEST) $(API_TEST_TARGET)
ifeq ($(OS),Windows_NT)
	./$(TEST_TARGET)
	./$(SIGNAL_TEST)
	./$(API_TEST_TARGET)
else
	./$(TEST_TARGET)
	./$(SIGNAL_TEST)
	./$(API_TEST_TARGET)
endif

# Build the test executable
$(TEST_TARGET): $(TEST_DIR)/test_binary_clock.c $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_DIR)/test_binary_clock.c $(LIB_OBJ)

# Build the signal handling test
$(SIGNAL_TEST): $(TEST_DIR)/test_signal_handling.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(SIGNAL_TEST) $(TEST_DIR)/test_signal_handling.c

# Build the API test executable
$(API_TEST_TARGET): $(TEST_DIR)/test_binary_clock_api.c $(API_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(API_TEST_TARGET) $(TEST_DIR)/test_binary_clock_api.c $(API_OBJ)

# Clean build artifacts
clean:
	$(RM) $(TARGET) $(TEST_TARGET) $(SIGNAL_TEST) $(API_TEST_TARGET) $(LIB_OBJ) $(API_OBJ) $(DISPLAY_OBJ)
	$(RM) -r $(BUILD_DIR)

# Run the binary clock
run: $(TARGET)
ifeq ($(OS),Windows_NT)
	./$(TARGET)
else
	./$(TARGET)
endif

# Install (simple copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Check for memory leaks with valgrind (if available, Unix only)
memcheck: $(TEST_TARGET)
ifeq ($(OS),Windows_NT)
	@echo "Valgrind not available on Windows, skipping memory check"
else
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_TARGET); \
	else \
		echo "Valgrind not available, skipping memory check"; \
	fi
endif

# Static analysis with cppcheck (if available)
analyze:
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --std=c99 $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h $(TEST_DIR)/*.c; \
	else \
		echo "cppcheck not available, skipping static analysis"; \
	fi

# Format code (if clang-format is available)
format:
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h $(TEST_DIR)/*.c; \
		echo "Code formatted"; \
	else \
		echo "clang-format not available, skipping formatting"; \
	fi

# ============================================================================
# DISTRIBUTION AND PACKAGING
# ============================================================================

# Version management
VERSION ?= $(shell git describe --tags --abbrev=0 2>/dev/null | sed 's/^v//' || echo "0.1.0")
GIT_TAG ?= $(shell git describe --tags --abbrev=0 2>/dev/null || echo "v0.1.0")
PLATFORM := $(shell uname -s | tr '[:upper:]' '[:lower:]')
ARCH := $(shell uname -m)

# Distribution directories
DIST_DIR = dist
RELEASE_DIR = release

# Distribution targets
dist-api-only: $(API_OBJ)
	@echo "📦 Creating API-only distribution package..."
	$(MKDIR) $(DIST_DIR)/api-only/{include,src,docs,examples}
	
	# Copy core API files
	cp include/binary_clock_api.h $(DIST_DIR)/api-only/include/
	cp src/binary_clock_api.c $(DIST_DIR)/api-only/src/
	
	# Copy API documentation
	cp docs/API_REFERENCE.md $(DIST_DIR)/api-only/docs/
	cp docs/SWIFT_INTEGRATION_EXAMPLE.swift $(DIST_DIR)/api-only/examples/
	
	# Create API-specific README
	@echo "# Binary Clock API v$(VERSION)" > $(DIST_DIR)/api-only/README.md
	@echo "" >> $(DIST_DIR)/api-only/README.md
	@echo "Lightweight binary clock API for widget and application integration." >> $(DIST_DIR)/api-only/README.md
	@echo "" >> $(DIST_DIR)/api-only/README.md
	@echo "## Quick Start" >> $(DIST_DIR)/api-only/README.md
	@echo "" >> $(DIST_DIR)/api-only/README.md
	@echo "\`\`\`c" >> $(DIST_DIR)/api-only/README.md
	@echo "#include <binary_clock_api.h>" >> $(DIST_DIR)/api-only/README.md
	@echo "" >> $(DIST_DIR)/api-only/README.md
	@echo "binary_clock_state_t state = binary_clock_get_current_state();" >> $(DIST_DIR)/api-only/README.md
	@echo "// Use state.hours_tens.bits[], state.hours_units.bits[], etc." >> $(DIST_DIR)/api-only/README.md
	@echo "\`\`\`" >> $(DIST_DIR)/api-only/README.md
	@echo "" >> $(DIST_DIR)/api-only/README.md
	@echo "## Compilation" >> $(DIST_DIR)/api-only/README.md
	@echo "" >> $(DIST_DIR)/api-only/README.md
	@echo "\`\`\`bash" >> $(DIST_DIR)/api-only/README.md
	@echo "gcc -Iinclude -c src/binary_clock_api.c -o binary_clock_api.o" >> $(DIST_DIR)/api-only/README.md
	@echo "gcc -Iinclude -o myapp myapp.c binary_clock_api.o" >> $(DIST_DIR)/api-only/README.md
	@echo "\`\`\`" >> $(DIST_DIR)/api-only/README.md
	@echo "" >> $(DIST_DIR)/api-only/README.md
	@echo "See docs/API_REFERENCE.md for complete documentation." >> $(DIST_DIR)/api-only/README.md
	
	# Create minimal Makefile for API users
	@echo "# Binary Clock API - Minimal Makefile" > $(DIST_DIR)/api-only/Makefile
	@echo "CC = gcc" >> $(DIST_DIR)/api-only/Makefile
	@echo "CFLAGS = -Wall -Wextra -std=c99 -pedantic -g" >> $(DIST_DIR)/api-only/Makefile
	@echo "" >> $(DIST_DIR)/api-only/Makefile
	@echo "binary_clock_api.o: src/binary_clock_api.c include/binary_clock_api.h" >> $(DIST_DIR)/api-only/Makefile
	@echo "	\$$(CC) \$$(CFLAGS) -Iinclude -c src/binary_clock_api.c -o \$$@" >> $(DIST_DIR)/api-only/Makefile
	@echo "" >> $(DIST_DIR)/api-only/Makefile
	@echo "clean:" >> $(DIST_DIR)/api-only/Makefile
	@echo "	rm -f *.o" >> $(DIST_DIR)/api-only/Makefile
	@echo "" >> $(DIST_DIR)/api-only/Makefile
	@echo ".PHONY: clean" >> $(DIST_DIR)/api-only/Makefile
	
	@echo "✅ API-only package ready in $(DIST_DIR)/api-only"

dist-cli: $(TARGET)
	@echo "📦 Creating CLI distribution package..."
	$(MKDIR) $(DIST_DIR)/cli
	
	# Copy binary
	cp $(TARGET) $(DIST_DIR)/cli/
	
	# Create CLI-specific README
	@echo "# Binary Clock CLI v$(VERSION)" > $(DIST_DIR)/cli/README.md
	@echo "" >> $(DIST_DIR)/cli/README.md
	@echo "Binary clock with multiple display modes and command-line interface." >> $(DIST_DIR)/cli/README.md
	@echo "" >> $(DIST_DIR)/cli/README.md
	@echo "## Usage" >> $(DIST_DIR)/cli/README.md
	@echo "" >> $(DIST_DIR)/cli/README.md
	@echo "\`\`\`bash" >> $(DIST_DIR)/cli/README.md
	@echo "# Single output modes" >> $(DIST_DIR)/cli/README.md
	@echo "./$(TARGET) --display=emoji   # Moon emojis (default)" >> $(DIST_DIR)/cli/README.md
	@echo "./$(TARGET) --display=binary  # 0s and 1s" >> $(DIST_DIR)/cli/README.md
	@echo "./$(TARGET) --display=json    # JSON format" >> $(DIST_DIR)/cli/README.md
	@echo "./$(TARGET) --display=raw     # Raw API data" >> $(DIST_DIR)/cli/README.md
	@echo "" >> $(DIST_DIR)/cli/README.md
	@echo "# Continuous modes" >> $(DIST_DIR)/cli/README.md
	@echo "./$(TARGET) --loop            # Continuous emoji display" >> $(DIST_DIR)/cli/README.md
	@echo "./$(TARGET) --display=json --loop  # Continuous JSON output" >> $(DIST_DIR)/cli/README.md
	@echo "" >> $(DIST_DIR)/cli/README.md
	@echo "# Help" >> $(DIST_DIR)/cli/README.md
	@echo "./$(TARGET) --help" >> $(DIST_DIR)/cli/README.md
	@echo "\`\`\`" >> $(DIST_DIR)/cli/README.md
	
	# Copy license
	@if [ -f LICENSE ]; then cp LICENSE $(DIST_DIR)/cli/; fi
	
	@echo "✅ CLI package ready in $(DIST_DIR)/cli"

dist-library: $(API_OBJ) $(DISPLAY_OBJ)
	@echo "📦 Creating library distribution package..."
	$(MKDIR) $(DIST_DIR)/library/{include,src,docs,examples}
	
	# Copy headers
	cp include/binary_clock_api.h $(DIST_DIR)/library/include/
	cp include/binary_clock_display.h $(DIST_DIR)/library/include/
	
	# Copy source files
	cp src/binary_clock_api.c $(DIST_DIR)/library/src/
	cp src/binary_clock_display.c $(DIST_DIR)/library/src/
	
	# Copy documentation
	cp docs/API_REFERENCE.md $(DIST_DIR)/library/docs/
	cp docs/SWIFT_INTEGRATION_EXAMPLE.swift $(DIST_DIR)/library/examples/
	
	# Create library-specific README
	@echo "# Binary Clock Library v$(VERSION)" > $(DIST_DIR)/library/README.md
	@echo "" >> $(DIST_DIR)/library/README.md
	@echo "Complete binary clock library with core API and display utilities." >> $(DIST_DIR)/library/README.md
	@echo "" >> $(DIST_DIR)/library/README.md
	@echo "## Components" >> $(DIST_DIR)/library/README.md
	@echo "" >> $(DIST_DIR)/library/README.md
	@echo "- **binary_clock_api.h/.c**: Core API (data only, no visualization)" >> $(DIST_DIR)/library/README.md
	@echo "- **binary_clock_display.h/.c**: Display utilities (emoji, ASCII, JSON, raw)" >> $(DIST_DIR)/library/README.md
	@echo "" >> $(DIST_DIR)/library/README.md
	@echo "## Quick Start" >> $(DIST_DIR)/library/README.md
	@echo "" >> $(DIST_DIR)/library/README.md
	@echo "\`\`\`c" >> $(DIST_DIR)/library/README.md
	@echo "#include <binary_clock_api.h>     // Core API" >> $(DIST_DIR)/library/README.md
	@echo "#include <binary_clock_display.h> // Display utilities" >> $(DIST_DIR)/library/README.md
	@echo "" >> $(DIST_DIR)/library/README.md
	@echo "binary_clock_state_t state = binary_clock_get_current_state();" >> $(DIST_DIR)/library/README.md
	@echo "binary_clock_display_console_emoji(&state, NULL);" >> $(DIST_DIR)/library/README.md
	@echo "\`\`\`" >> $(DIST_DIR)/library/README.md
	@echo "" >> $(DIST_DIR)/library/README.md
	@echo "See docs/API_REFERENCE.md for complete documentation." >> $(DIST_DIR)/library/README.md
	
	# Create library Makefile
	@echo "# Binary Clock Library - Makefile" > $(DIST_DIR)/library/Makefile
	@echo "CC = gcc" >> $(DIST_DIR)/library/Makefile
	@echo "CFLAGS = -Wall -Wextra -std=c99 -pedantic -g" >> $(DIST_DIR)/library/Makefile
	@echo "" >> $(DIST_DIR)/library/Makefile
	@echo "API_OBJ = binary_clock_api.o" >> $(DIST_DIR)/library/Makefile
	@echo "DISPLAY_OBJ = binary_clock_display.o" >> $(DIST_DIR)/library/Makefile
	@echo "" >> $(DIST_DIR)/library/Makefile
	@echo "all: \$$(API_OBJ) \$$(DISPLAY_OBJ)" >> $(DIST_DIR)/library/Makefile
	@echo "" >> $(DIST_DIR)/library/Makefile
	@echo "\$$(API_OBJ): src/binary_clock_api.c include/binary_clock_api.h" >> $(DIST_DIR)/library/Makefile
	@echo "	\$$(CC) \$$(CFLAGS) -Iinclude -c src/binary_clock_api.c -o \$$@" >> $(DIST_DIR)/library/Makefile
	@echo "" >> $(DIST_DIR)/library/Makefile
	@echo "\$$(DISPLAY_OBJ): src/binary_clock_display.c include/binary_clock_display.h include/binary_clock_api.h" >> $(DIST_DIR)/library/Makefile
	@echo "	\$$(CC) \$$(CFLAGS) -Iinclude -c src/binary_clock_display.c -o \$$@" >> $(DIST_DIR)/library/Makefile
	@echo "" >> $(DIST_DIR)/library/Makefile
	@echo "clean:" >> $(DIST_DIR)/library/Makefile
	@echo "	rm -f *.o" >> $(DIST_DIR)/library/Makefile
	@echo "" >> $(DIST_DIR)/library/Makefile
	@echo ".PHONY: all clean" >> $(DIST_DIR)/library/Makefile
	
	@echo "✅ Library package ready in $(DIST_DIR)/library"

# Build all distribution packages
dist-all: dist-api-only dist-cli dist-library
	@echo "🎉 All distribution packages created in $(DIST_DIR)/"

# Create release packages (archives)
package-api: dist-api-only
	@echo "📦 Creating API package archive..."
	$(MKDIR) $(RELEASE_DIR)
	cd $(DIST_DIR) && tar -czf ../$(RELEASE_DIR)/binary-clock-api-$(GIT_TAG).tar.gz api-only/
	cd $(DIST_DIR) && zip -r ../$(RELEASE_DIR)/binary-clock-api-$(GIT_TAG).zip api-only/
	@echo "✅ API package: $(RELEASE_DIR)/binary-clock-api-$(GIT_TAG).tar.gz"

package-cli: dist-cli
	@echo "📦 Creating CLI package archive..."
	$(MKDIR) $(RELEASE_DIR)
ifeq ($(OS),Windows_NT)
	cd $(DIST_DIR) && zip -r ../$(RELEASE_DIR)/binary-clock-cli-$(GIT_TAG)-windows-$(ARCH).zip cli/
	@echo "✅ CLI package: $(RELEASE_DIR)/binary-clock-cli-$(GIT_TAG)-windows-$(ARCH).zip"
else
	cd $(DIST_DIR) && tar -czf ../$(RELEASE_DIR)/binary-clock-cli-$(GIT_TAG)-$(PLATFORM)-$(ARCH).tar.gz cli/
	@echo "✅ CLI package: $(RELEASE_DIR)/binary-clock-cli-$(GIT_TAG)-$(PLATFORM)-$(ARCH).tar.gz"
endif

package-library: dist-library
	@echo "📦 Creating library package archive..."
	$(MKDIR) $(RELEASE_DIR)
	cd $(DIST_DIR) && tar -czf ../$(RELEASE_DIR)/binary-clock-library-$(GIT_TAG).tar.gz library/
	cd $(DIST_DIR) && zip -r ../$(RELEASE_DIR)/binary-clock-library-$(GIT_TAG).zip library/
	@echo "✅ Library package: $(RELEASE_DIR)/binary-clock-library-$(GIT_TAG).tar.gz"

package-source:
	@echo "📦 Creating source package archive..."
	$(MKDIR) $(RELEASE_DIR)
	git archive --format=tar.gz --prefix=binary-clock-$(VERSION)/ HEAD > $(RELEASE_DIR)/binary-clock-source-$(GIT_TAG).tar.gz
	git archive --format=zip --prefix=binary-clock-$(VERSION)/ HEAD > $(RELEASE_DIR)/binary-clock-source-$(GIT_TAG).zip
	@echo "✅ Source package: $(RELEASE_DIR)/binary-clock-source-$(GIT_TAG).tar.gz"

# Create all packages
package-all: package-api package-cli package-library package-source
	@echo "🎉 All release packages created in $(RELEASE_DIR)/"

# Generate checksums for release packages
generate-checksums:
	@echo "🔐 Generating checksums..."
	$(MKDIR) $(RELEASE_DIR)
ifeq ($(OS),Windows_NT)
	cd $(RELEASE_DIR) && for %%f in (*) do certutil -hashfile "%%f" SHA256 >> checksums.txt
else
	cd $(RELEASE_DIR) && sha256sum * > checksums.txt 2>/dev/null || shasum -a 256 * > checksums.txt
endif
	@echo "✅ Checksums generated: $(RELEASE_DIR)/checksums.txt"

# Complete release preparation
prepare-release: clean all test package-all generate-checksums
	@echo "🚀 Release $(GIT_TAG) prepared!"
	@echo ""
	@echo "📦 Release packages:"
	@ls -la $(RELEASE_DIR)/
	@echo ""
	@echo "To create the release:"
	@echo "  git tag $(GIT_TAG)"
	@echo "  git push origin $(GIT_TAG)"

# Clean distribution artifacts
clean-dist:
	$(RM) -r $(DIST_DIR) $(RELEASE_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo ""
	@echo "Build & Test:"
	@echo "  all       - Build the binary clock application"
	@echo "  test      - Build and run tests"
	@echo "  run       - Build and run the binary clock"
	@echo "  clean     - Remove build artifacts"
	@echo ""
	@echo "Distribution & Packaging:"
	@echo "  dist-api-only    - Create API-only distribution"
	@echo "  dist-cli         - Create CLI distribution"  
	@echo "  dist-library     - Create library distribution"
	@echo "  dist-all         - Create all distributions"
	@echo "  package-api      - Create API package archives"
	@echo "  package-cli      - Create CLI package archives"
	@echo "  package-library  - Create library package archives"
	@echo "  package-source   - Create source package archives"
	@echo "  package-all      - Create all package archives"
	@echo "  prepare-release  - Complete release preparation"
	@echo "  clean-dist       - Remove distribution artifacts"
	@echo ""
	@echo "Installation:"
	@echo "  install   - Install binary to /usr/local/bin"
	@echo "  uninstall - Remove binary from /usr/local/bin"
	@echo ""
	@echo "Development:"
	@echo "  memcheck  - Run tests with valgrind (if available)"
	@echo "  analyze   - Run static analysis with cppcheck (if available)"
	@echo "  format    - Format code with clang-format (if available)"
	@echo "  help      - Show this help message"

.PHONY: all test run clean install uninstall memcheck analyze format help
.PHONY: dist-api-only dist-cli dist-library dist-all
.PHONY: package-api package-cli package-library package-source package-all
.PHONY: generate-checksums prepare-release clean-dist