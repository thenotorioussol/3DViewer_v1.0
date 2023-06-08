.PHONY: viewer.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c,%.o, ${SRC_FILES})

all:
	$(MAKE) viewer.a

viewer.a:
	$(MAKE) clean
	$(CC) $(CFLAGS) -c $(SRC_FILES)
	ar rc viewer.a $(OBJ_FILES)
	@rm -rf *.o

style:
	clang-format -style=Google -n *.c *.h

clean:
	@rm -rf *.o *.a


