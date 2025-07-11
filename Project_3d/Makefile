# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Directories
SRC_DIR = geometry/src
INC_DIR = geometry/include
APP_DIR = application

# Source files
SRC_FILES = $(SRC_DIR)/geometry.cpp $(SRC_DIR)/cuboid.cpp $(SRC_DIR)/sphere.cpp $(SRC_DIR)/cylinder.cpp $(APP_DIR)/main.cpp

# Output executable
OUTPUT = shape_plotter

# Build rule
all: $(OUTPUT)

$(OUTPUT): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(SRC_FILES) -o $(OUTPUT)

# Clean rule
clean:
	rm -f $(OUTPUT)
