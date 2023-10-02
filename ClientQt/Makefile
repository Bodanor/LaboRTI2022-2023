NAME        := Client

LIBS        := sockets
LIBS_TARGET := ../libsockets/libsockets.a

INCS        := . ../Server/include ../libsockets/ /usr/include/mysql

SRCS        := $(wildcard *.cpp)
BUILD_DIR   := .build
OBJS        := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

CC = g++
CFLAGS_OBJECTS =  -MMD -MP -Wno-unused-parameter -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ $(addprefix -I,$(INCS))
CFLAGS_EXEC = /usr/lib64/libQt5Widgets.so /usr/lib64/libQt5Gui.so /usr/lib64/libQt5Core.so /usr/lib64/libGL.so -lpthread
LDFLAGS     := $(addprefix -L,$(dir $(LIBS_TARGET)))
LDLIBS      := $(addprefix -l,$(LIBS))

RM          := rm -f
DIR_DUP     = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJS) $(LIBS_TARGET)
	$(CC) $(OBJS) $(CFLAGS_EXEC) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	$(info CREATED $(NAME))

$(LIBS_TARGET):
	$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: %.cpp
	$(DIR_DUP)
	$(CC) $(CFLAGS_OBJECTS) -o $@ $<
	$(info CREATED $@)

-include $(DEPS)

clean:
	$(RM) $(OBJS) $(DEPS) $(NAME)
