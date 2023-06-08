.PHONY: test

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TEST_FLAGS = `pkg-config --cflags --libs check` -lm -lcheck

SRC_FILES = $(wildcard backend/*.c)

all:
	$(MAKE) clean
	$(MAKE) uninstall
	$(MAKE) install

install:
	cmake -S ./front -B ./install
	make -C ./install

uninstall:
	rm -rf ./install

dvi:
	open manual.md

dist:
	cd .. && tar -cf src.tar src && mv src.tar src

test:
	$(MAKE) clean
	cd backend && make viewer.a && mv viewer.a ../
	$(CC) $(CFLAGS) test.c viewer.a $(TEST_FLAGS) -o test
	./test
	rm -rf viewer.a

gcov_report:
	$(MAKE) clean
	$(CC) $(CFLAGS) test.c -coverage $(TEST_FLAGS) $(SRC_FILES) -o gcovreport
	./gcovreport
	lcov -t "gcovreport" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	@rm -rf *.a *.info *.gcda *.gcno gcovreport

style:
	clang-format -style=Google -n backend/*.c backend/*.h front/*.cpp front/*.h test.c GIF/*.c GIF/*.cpp GIF/*.h

clean:
	@rm -rf *.o *.a report *.gcno *.gcda  *.tar test gcovreport