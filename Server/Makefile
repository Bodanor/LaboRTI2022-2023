NAME        := server

LIBS        := ovesp mysqlclient
LIBS_TARGET := ../libovesp/libovesp.a

INCS        := include /usr/include/mysql ../libsockets/include ../libserver/include ../libovesp/include /usr/include/mysql

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

all: $(NAME)

$(NAME): $(OBJS) $(LIBS_TARGET)
	$(info Building the final target : $@)
	$(CC) $(OBJS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $(NAME)


$(LIBS_TARGET):
	-$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: %.c
	$(info Compiling object file : $@)
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c -o $@ $<

-include $(DEPS)
	
clean:
	###for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

.PHONY: clean

