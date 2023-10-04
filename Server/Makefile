NAME        := server

LIBS        := ovesp sockets server sqlrequests mysqlclient
LIBS_TARGET := ../libovesp/libovesp.a ../libsockets/libsockets.a ../libsqlrequests/libsqlrequests.a ../libserver/libserver.a

INCS        := 	/usr/include/mysql\
				../libovesp/include\
				../libserver/include\
				../libsockets/include\
				../libsqlrequests/include

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

all: $(LIBS_TARGET) $(NAME)

$(NAME): $(OBJS)
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
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

.PHONY: clean $(LIBS_TARGET)

