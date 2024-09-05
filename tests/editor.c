#include "../src/include/editor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
test_editor_new() {
    // arrange & act
    Editor editor = Editor__new(Buffer__new());

    // assert
    assert(editor.mode == MODE__NORMAL);

    // release
    // @@@
}

static void
test_editor_newline() {
    // arrange
    Editor editor = Editor__new(Buffer__new());

    // act
    Editor__newline();
    Editor__newline();

    // assert
    assert(editor.buffer.lines == 2);

    // release
    // @@@
}

static void
test_editor_addch() {
    // arrange
    Editor editor = Editor__new(Buffer__new());

    // act
    Editor__add_char('a');
    Editor__add_char('b');
    Editor__newline();
    Cursor__down();
    Editor__add_char('c');

    // assert
    assert(editor.buffer.lines == 2);
    assert(strcmp(editor.buffer.data[0].data, "ab") == 0);
    assert(strcmp(editor.buffer.data[1].data, "c") == 0);

    // release
    // @@@
}

void
T__editor(void) {
    (void) (test_editor_new);
    (void) (test_editor_newline);
    test_editor_addch();
}
