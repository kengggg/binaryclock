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

CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LIB_OBJ = binary_clock_lib.o

# Default target
all: $(TARGET)

# Build the main binary clock application
$(TARGET): binary_clock.c
	$(CC) $(CFLAGS) -o $(TARGET) binary_clock.c

# Build the library object file
$(LIB_OBJ): binary_clock_lib.c binary_clock_lib.h
	$(CC) $(CFLAGS) -c binary_clock_lib.c

# Build and run tests
test: $(TEST_TARGET) $(SIGNAL_TEST)
ifeq ($(OS),Windows_NT)
	./$(TEST_TARGET)
	./$(SIGNAL_TEST)
else
	./$(TEST_TARGET)
	./$(SIGNAL_TEST)
endif

# Build the test executable
$(TEST_TARGET): test_binary_clock.c $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_binary_clock.c $(LIB_OBJ)

# Build the signal handling test
$(SIGNAL_TEST): test_signal_handling.c
	$(CC) $(CFLAGS) -o $(SIGNAL_TEST) test_signal_handling.c

# Clean build artifacts
clean:
	$(RM) $(TARGET) $(TEST_TARGET) $(SIGNAL_TEST) $(LIB_OBJ)

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
		cppcheck --enable=all --std=c99 *.c *.h; \
	else \
		echo "cppcheck not available, skipping static analysis"; \
	fi

# Format code (if clang-format is available)
format:
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i *.c *.h; \
		echo "Code formatted"; \
	else \
		echo "clang-format not available, skipping formatting"; \
	fi

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the binary clock application"
	@echo "  test      - Build and run tests"
	@echo "  run       - Build and run the binary clock"
	@echo "  clean     - Remove build artifacts"
	@echo "  install   - Install binary to /usr/local/bin"
	@echo "  uninstall - Remove binary from /usr/local/bin"
	@echo "  memcheck  - Run tests with valgrind (if available)"
	@echo "  analyze   - Run static analysis with cppcheck (if available)"
	@echo "  format    - Format code with clang-format (if available)"
	@echo "  help      - Show this help message"

.PHONY: all test run clean install uninstall memcheck analyze format help