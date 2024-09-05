#include "../src/include/editor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
test_editor_new() {
    // arrange & act
    EDITOR = Editor__new(Buffer__new(0));

    // assert
    assert(EDITOR.mode == MODE__NORMAL);

    // release
    Editor__release();
}

static void
test_editor_newline() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Editor__newline();
    Editor__newline();

    // assert
    assert(EDITOR.buffer.lines == 2);

    // release
    Editor__release();
}

static void
test_editor_add_char() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Editor__add_char('a');
    Editor__add_char('b');
    Editor__newline();
    Cursor__down();
    Editor__add_char('c');

    // assert
    assert(EDITOR.buffer.lines == 2);
    assert(strcmp(EDITOR.buffer.data[0].data, "ab") == 0);
    assert(strcmp(EDITOR.buffer.data[1].data, "c") == 0);

    // release
    Editor__release();
}

void
T__editor() {
    test_editor_new();
    test_editor_newline();
    (void) (test_editor_add_char);  // FIXME: depending of cursor fixes
}
