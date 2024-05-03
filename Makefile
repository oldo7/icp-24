# Project settings
PROJECT = simulator
CXX = g++
CXXFLAGS = -std=c++17 -Wall -fPIC $(shell pkg-config --cflags Qt5Widgets)
LIBS = $(shell pkg-config --libs Qt5Widgets)
OBJECTS = main.o addrobot.o simulation.o robot.o controllablerobot.o scanareat.o savestate.o

# Default target
all: $(PROJECT)

# Link the application
$(PROJECT): $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS)

# Compile the source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(PROJECT)

# Run the application
run: $(PROJECT)
	./$(PROJECT)

# Generate documentation with Doxygen
doxygen:
	doxygen Doxyfile

.PHONY: all clean run doxygen
