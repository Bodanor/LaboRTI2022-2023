NAME        := server

LIBS        := sockets server ovesp mysqlclient
LIBS_TARGET := ../libsockets/libsockets.a ../libserver/libserver.a ../libovesp/libovesp.a

INCS        := include /usr/include/mysql ../libsockets/include ../libserver/include ../libovesp/include /usr/include/mysql

SRC_DIR     := src
SRCS        := $(wildcard $(SRC_DIR)/*.c) #Tu prends tout les fichiers SRC_DIR#

BUILD_DIR   := .build
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
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
	$(CC) main.c $(OBJS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	$(info CREATED $(NAME))


$(LIBS_TARGET):
	-$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c -o $@ $<
	$(info CREATED $@)

-include $(DEPS)

docs:
	doxygen
	$(MAKE) -C DOCS/latex

docs-rm:
	rm -rf DOCS
	
clean:
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

.PHONY: clean

