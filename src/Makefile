LIBS        :=  sockets mysqlclient
LIBS_TARGET := ../lib/libsockets/libsockets.a

INCS        := 	/usr/include/mysql\
				../lib/libsockets/include\
				../include\
				.

SRCS        := $(wildcard *.c) #Tu prends tout les fichiers SRC_DIR#

BUILD_DIR   := .build
OBJS        := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

CC          := gcc
CFLAGS      := -Wall -Wextra -std=gnu89 -g -DDEBUG -MMD -MP
CPPFLAGS    := $(addprefix -I,$(INCS))
LDFLAGS     := $(addprefix -L,$(dir $(LIBS_TARGET)))
LDLIBS      := $(addprefix -l,$(LIBS))


RM          := rm -f
DIR_DUP     = mkdir -p $(@D)

all: $(LIBS_TARGET) $(OBJS)

$(LIBS_TARGET):
	-$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: %.c
	$(info Compiling object file : $@)
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c -o $@ $<

-include $(DEPS)
	
clean:
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

.PHONY: clean $(LIBS_TARGET)

