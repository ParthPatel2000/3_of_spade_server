# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 

# Source files
SRCS := $(wildcard *.cpp)

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
TARGET := server

# Libraries
LIBS := -lmysqlmysqlcppconn8

# Include directories
CPPFLAGS := 

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	DEL $(OBJS) $(TARGET) *.exe