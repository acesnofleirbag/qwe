CC = gcc
BIN = build/qwe
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
LIBS = ncurses
FILES = $(shell find src -type f -iname '*.c')

all: run

.PHONY: build
build:
	$(CC) $(FILES) $(CFLAGS) -l $(LIBS) -o $(BIN) 

.PHONY: run
run: build
	./build/qwe

# == TESTS ==

.PHONY: test-build
test-build:
	$(CC) $(FILES) tests/main.c -l $(LIBS) $(CFLAGS) -g -O0 -o build/_t -DTEST

.PHONY: test-run
test-run: test-build
	./build/_t

# == VALGRIND == 

.PHONY: leak-check
leak-check: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./build/qwe src/main.c

.PHONY: test-leak-check
test-leak-check: test-build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./build/_t tests/main.c

# == LINTING == 

TARGETS = $(shell find src -type f -iname '*.h' -or -iname '*.c')
TARGETS += $(shell find tests -type f -iname '*.h' -or -iname '*.c')

.PHONY: lint
lint: $(OBJ)
	@clang-format -style=file -i $(TARGETS)
	@echo "== REFORMATTED SUCCESSFULLY =="

