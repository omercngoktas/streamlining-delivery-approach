CC = g++
CFLAGS = -std=c++14 -Wall -Wextra
# Include directory for MySQL headers
INCLUDES = -I/opt/homebrew/include
# Library directory for MySQL and additional libraries
LDFLAGS = -L/opt/homebrew/Cellar/mysql/8.3.0_1/lib
# Linking against the MySQL client library
LDLIBS = -lmysqlclient

TARGET = main
SOURCE_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BIN_DIR = bin

# Find source files
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)

# Generate names for object files
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(LIB_DIR)/%.o, $(SOURCES))

# Target file creation
$(BIN_DIR)/$(TARGET).out: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)

# Object file creation
$(LIB_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -f $(LIB_DIR)/*.o $(BIN_DIR)/$(TARGET).out

# Compile and then run
run: $(BIN_DIR)/$(TARGET).out
	clear && ./$(BIN_DIR)/$(TARGET).out
