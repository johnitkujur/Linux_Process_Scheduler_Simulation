# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -pthread

# Target executable
TARGET := main

# Source and header files
SRCS := main.cpp interrupt.cpp process.cpp time_utility.cpp
HDRS := interrupt.hpp process.hpp time_utility.hpp

# Object files (replace .cpp with .o)
OBJS := $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Link the final binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp file into .o
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Convenience rule for rebuild
rebuild: clean all

.PHONY: all clean rebuild
