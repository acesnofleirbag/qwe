#include "../src/include/cursor.h"
#include "../src/include/buffer.h"
#include "../src/include/editor.h"
#include <assert.h>

static void
test_mv_cursor_up() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 5, .y = 5};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    // act
    cursor__down();
    cursor__down();
    cursor__up();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->y == 1);

    // release
    editor__release();
}

static void
test_mv_cursor_right() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 5, .y = 5};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    editor__newline();
    str_t *line = &EDITOR.buffer.data[0];
    string__append(line, line->len, 'a');

    // act
    cursor__right();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->x == 1);

    // release
    editor__release();
}

static void
test_mv_cursor_down() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 5, .y = 5};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    // act
    cursor__down();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->y == 1);

    // release
    editor__release();
}

static void
test_mv_cursor_left() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 5, .y = 5};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    editor__newline();
    str_t *line = &EDITOR.buffer.data[0];
    string__append(line, line->len, 'a');
    string__append(line, line->len, 'b');

    // act
    cursor__right();
    cursor__right();
    cursor__left();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->x == 1);

    // release
    editor__release();
}

static void
test_mv_down_offset() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 3, .y = 3};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    // act
    cursor__down();
    cursor__down();
    cursor__down();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->offset.y == 1);

    // release
    editor__release();
}

static void
test_mv_up_offset() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 3, .y = 3};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    // act
    cursor__down();
    cursor__down();
    cursor__down();
    cursor__down();
    cursor__up();
    cursor__up();
    cursor__up();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->offset.y == 1);

    // release
    editor__release();
}

static void
test_mv_right_offset() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    viewport_t viewport = {.x = 3, .y = 3};
    display_t display = display__new(DISPLAY_TYPE__EDITOR, NULL, viewport);

    display_list__add(&EDITOR.display_list, display);

    // act
    cursor__right();
    cursor__right();
    cursor__left();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->x == 1);

    // release
    editor__release();
}

static void
test_mv_left_offset() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    // act
    cursor__right();
    cursor__right();
    cursor__left();

    // assert
    cursor_t *cursor = editor__get_cursor();

    assert(cursor->x == 1);

    // release
    editor__release();
}

void
T__cursor() {
    test_mv_cursor_up();
    test_mv_cursor_right();
    test_mv_cursor_down();
    test_mv_cursor_left();

    test_mv_down_offset();
    test_mv_up_offset();
    (void) test_mv_right_offset;
    (void) test_mv_left_offset;
}
