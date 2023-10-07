NAME        := Client

BIN_DIR := ../bin

LIBS        := pthread Qt5Widgets Qt5Gui Qt5Core GL sockets mysqlclient
LIBS_TARGET := ../lib/libsockets/libsockets.a


INCS        := 	/usr/include/qt5/QtWidgets\
				/usr/include/qt5\
				/usr/include/qt5/QtGui\
				/usr/include/qt5/QtCore\
				/usr/include/mysql\
				../lib/libsockets/include\
				../include\
				../src\
				.

LOCAL_SRCS  := $(wildcard *.cpp) #Tu prends tout les fichiers SRC_DIR#
BUILD_DIR   := .build
# Object files from current directory
LOCAL_OBJS  := $(LOCAL_SRCS:%.cpp=$(BUILD_DIR)/%.o)

EXTERNAL_SRC_DIR := ../src
EXTERNAL_SRCS := $(wildcard $(EXTERNAL_SRC_DIR)/*.c)
# Object files from ../src/.build directory
EXTERNAL_OBJS := $(EXTERNAL_SRCS:$(EXTERNAL_SRC_DIR)/%.c=$(EXTERNAL_SRC_DIR)/.build/%.o)

$(info $$var is [${LOCAL_OBJS}])

# All object files
OBJS        := $(LOCAL_OBJS) $(EXTERNAL_OBJS)

DEPS        := $(OBJS:.o=.d) $(EXTERNAL_OBJS:.o=.d)

CC          := g++
CFLAGS      := -MMD -MP -Wno-unused-parameter -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
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
	
$(BUILD_DIR)/%.o: %.cpp
	$(info Compiling object file : $@)
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c -o $@ $<


-include $(DEPS)

clean:
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(BIN_DIR)/$(NAME)

.PHONY: clean $(LIBS_TARGET)

