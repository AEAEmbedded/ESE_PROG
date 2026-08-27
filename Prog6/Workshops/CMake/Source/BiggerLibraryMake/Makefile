# Makefile for Sensor Application
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2025

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include
LDFLAGS =

# Target executable
TARGET = sensor_app

# Source files
SOURCES = main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp

# Object files (replace .cpp with .o)
OBJECTS = main.o sht45.o i2c.o logger.o config.o filter.o

# Header files (for dependency tracking)
HEADERS = include/sht45.hpp include/i2c.hpp include/logger.hpp include/config.hpp include/filter.hpp

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Compile main.cpp
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile sht45.cpp
sht45.o: sht45.cpp include/sht45.hpp
	$(CXX) $(CXXFLAGS) -c sht45.cpp

# Compile i2c.cpp
i2c.o: i2c.cpp include/i2c.hpp
	$(CXX) $(CXXFLAGS) -c i2c.cpp

# Compile logger.cpp
logger.o: logger.cpp include/logger.hpp
	$(CXX) $(CXXFLAGS) -c logger.cpp

# Compile config.cpp
config.o: config.cpp include/config.hpp
	$(CXX) $(CXXFLAGS) -c config.cpp

# Compile filter.cpp
filter.o: filter.cpp include/filter.hpp
	$(CXX) $(CXXFLAGS) -c filter.cpp

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Rebuild everything from scratch
rebuild: clean all

# Run the application
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all     - Build the application (default)"
	@echo "  clean   - Remove all build artifacts"
	@echo "  rebuild - Clean and build from scratch"
	@echo "  run     - Build and run the application"
	@echo "  help    - Show this help message"

.PHONY: all clean rebuild run help
