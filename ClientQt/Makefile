NAME        := Client

LIBS        := pthread Qt5Widgets Qt5Gui Qt5Core GL mysqlclient sockets
LIBS_TARGET := ../libsockets/libsockets.a . /usr/lib64/qt5/mkspecs/linux-g++ /usr/lib64 /usr/include/mysql

INCS        := ../libsockets/ . ../Server/include /usr/include/mysql /usr/include/qt5 /usr/include/qt5/QtWidgets /usr/include/qt5/QtGui /usr/include/qt5/QtCore

SRCS        := $(wildcard *.cpp)
BUILD_DIR   := .build
OBJS        := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

CC = g++
CFLAGS =  -MMD -MP -Wno-unused-parameter -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
CPPFLAGS    := $(addprefix -I,$(INCS))
LDFLAGS     := $(addprefix -L,$(dir $(LIBS_TARGET)))
LDLIBS      := $(addprefix -l,$(LIBS))

RM          := rm -f
DIR_DUP     = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJS) $(LIBS_TARGET)
	$(CC) $(OBJS) ../Server/.build/server.o ../Server/.build/OVESP.o  ../Server/.build/sql_requests.o $(CPPFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	$(info CREATED $(NAME))

$(LIBS_TARGET):
	$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: %.cpp
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $<
	$(info CREATED $@)

-include $(DEPS)

clean:
	$(RM) $(OBJS) $(DEPS) $(NAME)
