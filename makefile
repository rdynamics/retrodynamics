# Ccompiler
CC= gcc

# Executable name
BIN= rd-test

# Source and build paths
SRC_DIR= ./src
BUILD_DIR= ./build

# Source files
SRCS= main.c window.c loops.c event.c renderer.c glew.c images_diffuse.c vector.c entity.c rdtest.c

# Object files to build
OBJS= $(SRCS:%.c=%.o)

# Dependencies for each source
DEPS= $(OBJS:%.o=$(BUILD_DIR)/%.d)

# Flags for the compiler
CFLAGS= -Wall -lglfw3 -lopengl32 -lgdi32 -lpthread -lzgcl

# Default path for make install
INSTALL_PATH?=/usr/local

$(BIN) : $(OBJS:%=$(BUILD_DIR)/%)
	$(CC) $^ -o $@ $(CFLAGS)

-include $(DEPS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -MMD -c $< -o $@ $(CFLAGS)

.PHONY : clean install windows
clean :
	rm -rf $(BUILD_DIR)
	rm $(BIN) || rm $(BIN).exe

install : $(BIN)
	cp $(BIN) $(INSTALL_PATH)/bin
    
windows : $(OBJS:%=$(BUILD_DIR)/%)
	$(CC) $^ -o shader-test-windows.exe $(CFLAGS) -mwindows