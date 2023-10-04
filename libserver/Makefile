NAME        := libserver.a

LIBS_TARGET := ../libsockets/libsockets.a

INCS        := include ../libsockets/include /usr/include/mysql ../libsqlrequests/include

SRC_DIR     := src
SRCS        := $(wildcard $(SRC_DIR)/*.c) #Tu prends tout les fichiers SRC_DIR#

BUILD_DIR   := .build
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

CC          := gcc
CFLAGS      := -Wall -Wextra -std=gnu89 -g -DDEBUG -MMD -MP
AR          := ar
ARFLAGS     := -r -c -s -v

CPPFLAGS    := $(addprefix -I,$(INCS))


RM          := rm -f
DIR_DUP     = mkdir -p $(@D)

all: $(LIBS_TARGET) $(NAME)

$(NAME): $(OBJS)
	$(info Making static library : $@)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS)

$(LIBS_TARGET):
	-$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(info Compiling object file : $@)
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

-include $(DEPS)

	
clean:
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

.PHONY: clean $(LIBS_TARGET)

