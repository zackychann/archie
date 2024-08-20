CC = cc
CFLAGS = -Wall -Wextra -O2 -lreadline -lncurses -march=native -mtune=native
TARGET = archie
SRC = archie.c
INSTALL_DIR = /usr/bin
BUILD_DIR = build
OBJ = $(BUILD_DIR)/$(TARGET).o

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

install: $(BUILD_DIR)/$(TARGET)
	install -m 755 $(BUILD_DIR)/$(TARGET) $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all install uninstall clean $(BUILD_DIR)
