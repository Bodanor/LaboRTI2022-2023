NAME        := CreationBD

SRCS        := CreationBD.cpp

CC          := g++
CFLAGS      := -I/usr/include/mysql -m64 -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl


RM          := rm -f

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o $(NAME) 
	$(info CREATED $@)

clean:
	$(RM) $(NAME)


.PHONY: clean