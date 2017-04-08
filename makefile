SRC_DIR= ./src
BUILD_DIR= ./build

SRCS= component.c game.c

OBJS= $(SRCS:%.c=$(BUILD_DIR)/%.o)

DEPS= $(OBJS:%.o=$(BUILD_DIR)/%.d)

CFLAGS = -Wall

LIB_NAME= retrodyn
LIB_BUILDNAME= $(LIB_NAME:%=lib%.a)

INSTALL_DIR?=/usr
LIB_PATH=$(INSTALL_DIR)/lib
INCLUDE_PATH=$(INSTALL_DIR)/include

$(LIB_BUILDNAME): $(OBJS)
	ar rcs $(BUILD_DIR)/$(LIB_BUILDNAME) $(OBJS)

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
