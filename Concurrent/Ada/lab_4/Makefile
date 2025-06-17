# Makefile for building Szymanski algorithm demo

# Compiler
GPRBUILD = gprbuild
GPRCLEAN = gprclean

# Target executable
TARGET = main

# Source files
SOURCES = main.adb szymanski_algorithm.ads szymanski_algorithm.adb

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SOURCES)
	$(GPRBUILD) -P build.gpr

# Clean target
clean:
	$(GPRCLEAN) -P build.gpr
	rm -rf obj

# Run target
run: $(TARGET)
	./$(TARGET)

# Manual build without project file (fallback)
manual: $(SOURCES)
	gnatmake main.adb -o $(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all    - Build the program using gprbuild (default)"
	@echo "  manual - Build using gnatmake without project file"
	@echo "  clean  - Clean build artifacts"
	@echo "  run    - Build and run the program"
	@echo "  help   - Show this help message"

.PHONY: all clean run manual help
