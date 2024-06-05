# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra

# Include directories
INCLUDES := -I/usr/include/mysql

# Library directories
LIBDIRS := -L/usr/lib/mysql

# Libraries
LIBS := -lmysqlclient -lssl -lcrypto

#PHONY
.PHONY: all clean remake

# Source files
SRCS := $(wildcard *.cpp)

# Object directory
OBJDIR := objects

# Object files
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

# Executable name
TARGET := server

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBDIRS) -o $@ $^ $(LIBS)

# Rule to build object files
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create object directory if it does not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJDIR)


# Remake rule
remake: clean all