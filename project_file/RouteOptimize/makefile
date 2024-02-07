CC = g++
CFLAGS = -std=c++11 -Wall -Wextra

TARGET = main
SOURCE_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BIN_DIR = bin

# Kaynak dosyalarını bul
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)

# Nesne dosyalarının adlarını oluştur
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(LIB_DIR)/%.o, $(SOURCES))

# Hedef dosyayı oluşturma
$(BIN_DIR)/$(TARGET).out: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Nesne dosyalarını oluşturma
$(LIB_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Temizleme
clean:
	rm -f $(LIB_DIR)/*.o $(BIN_DIR)/$(TARGET).out

# Derleme ve sonra çalıştırma
run: $(BIN_DIR)/$(TARGET).out
	./$(BIN_DIR)/$(TARGET).out
