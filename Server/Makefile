NAME        := server

BIN_DIR := ../bin
LIBS        := sockets mysqlclient
LIBS_TARGET := ../lib/libsockets/libsockets.a

INCS        := 	/usr/include/mysql\
				../lib/libsockets/include\
				../include\
				../src\
				.

LOCAL_SRCS        := $(wildcard *.c) #Tu prends tout les fichiers SRC_DIR#
BUILD_DIR   := .build
# Object files from current directory
LOCAL_OBJS  := $(LOCAL_SRCS:%.c=$(BUILD_DIR)/%.o)

EXTERNAL_SRC_DIR := ../src
EXTERNAL_SRCS := $(wildcard $(EXTERNAL_SRC_DIR)/*.c)
# Object files from ../src/.build directory
EXTERNAL_OBJS := $(EXTERNAL_SRCS:$(EXTERNAL_SRC_DIR)/%.c=$(EXTERNAL_SRC_DIR)/.build/%.o)

# All object files
OBJS        := $(LOCAL_OBJS) $(EXTERNAL_OBJS)

DEPS        := $(OBJS:.o=.d) $(EXTERNAL_OBJS:.o=.d)

CC          := gcc
CFLAGS      := -Wall -Wextra -std=gnu89 -g -DDEBUG -MMD -MP
CPPFLAGS    := $(addprefix -I,$(INCS))
LDFLAGS     := $(addprefix -L,$(dir $(LIBS_TARGET)))
LDLIBS      := $(addprefix -l,$(LIBS))


RM          := rm -f
DIR_DUP     = mkdir -p $(@D)

all: $(LIBS_TARGET) $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJS)
	$(info Building the final target : $@)
	$(DIR_DUP)
	$(CC) $(OBJS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@


$(LIBS_TARGET):
	-$(MAKE) -C $(@D)


$(EXTERNAL_SRC_DIR)/$(BUILD_DIR)/%.o: $(EXTERNAL_SRC_DIR)/%.c
	-$(MAKE) -C $(EXTERNAL_SRC_DIR)
	
$(BUILD_DIR)/%.o: %.c
	$(info Compiling object file : $@)
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c -o $@ $<


-include $(DEPS)

clean:
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(BIN_DIR)/$(NAME)

.PHONY: clean $(LIBS_TARGET)

