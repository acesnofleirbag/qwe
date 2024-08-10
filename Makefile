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

TARGETS = $(shell find src -type f -iname '*.h' -or -iname '*.c')

.PHONY: lint
lint: $(OBJ)
	@clang-format -style=file -i $(TARGETS)
	@echo "== REFORMATTED SUCCESSFULLY =="

.PHONY: leak-check
leak-check: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./build/qwe src/main.c
