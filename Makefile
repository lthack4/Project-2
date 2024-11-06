# Define compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Define the target executable
TARGET = project2_starter

# Rule to compile the project2_starter.cpp file
$(TARGET): project2_starter.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) project2_starter.cpp

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Rule to clean up compiled files
clean:
	rm -f $(TARGET)

# .PHONY targets (these are not files)
.PHONY: clean run
