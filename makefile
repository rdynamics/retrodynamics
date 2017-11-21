# Ccompiler
CC= gcc

# Source and build paths
SRC_DIR= ./src
BUILD_DIR= ./build

# Source files
SRCS= main.c window.c loops.c event.c renderer.c glew.c images_diffuse.c vector.c entity.c

# Object files to build
OBJS= $(SRCS:%.c=%.o)

# Dependencies for each source
DEPS= $(OBJS:%.o=$(BUILD_DIR)/%.d)

# Flags for the compiler
CFLAGS= -Wall -lglfw3 -lopengl32 -lgdi32 -lpthread -lzgcl

# Default path for make install
INSTALL_PATH?=/usr/local

# Library name
LIB_NAME= retrodynamics
LIB_BUILDNAME= $(LIB_NAME:%=lib%.a)

INSTALL_DIR?=/usr
LIB_PATH=$(INSTALL_DIR)/lib
INCLUDE_PATH=$(INSTALL_DIR)/include

$(LIB_BUILDNAME): $(OBJS:%=$(BUILD_DIR)/%)
	ar rcs $(BUILD_DIR)/$(LIB_BUILDNAME) $(OBJS:%=$(BUILD_DIR)/%)

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -MMD -c $< -o $@ $(CFLAGS)

.PHONY: clean install
clean:
	rm -rf $(BUILD_DIR)

install: $(LIB_FULLNAME)
	cp $(BUILD_DIR)/$(LIB_BUILDNAME) $(LIB_PATH)
	chmod 0755 $(LIB_PATH)/$(LIB_BUILDNAME)
	mkdir -p $(INCLUDE_PATH)/$(LIB_NAME)
	cp $(SRC_DIR)/*.h $(INCLUDE_PATH)/$(LIB_NAME)
	ldconfig
