#include "../src/include/editor.h"
#include <assert.h>
#include <string.h>

static void
test_editor_new() {
    // arrange & act
    EDITOR = editor__new(buffer__new(0));

    // assert
    assert(EDITOR.mode == MODE__NORMAL);

    // release
    editor__release();
}

static void
test_editor_newline() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    // act
    editor__newline();
    editor__newline();

    // assert
    assert(EDITOR.buffer.lines == 2);

    // release
    editor__release();
}

static void
test_editor_add_char() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 5, .y = 5};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    // act
    editor__add_char('a');
    editor__add_char('b');
    editor__newline();
    cursor__from(0, 1);  // ENTER press
    editor__add_char('c');

    // assert
    assert(EDITOR.buffer.lines == 2);
    assert(strcmp(EDITOR.buffer.data[0].data, "ab") == 0);
    assert(strcmp(EDITOR.buffer.data[1].data, "c") == 0);

    // release
    editor__release();
}

void
T__editor() {
    test_editor_new();
    test_editor_newline();
    test_editor_add_char();
}
