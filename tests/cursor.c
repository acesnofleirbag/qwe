#include "../src/include/cursor.h"
#include "../src/include/buffer.h"
#include "../src/include/editor.h"
#include <assert.h>

static void
test_mv_cursor_up() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

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
test_mv_up_offset() {
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

static void
test_mv_right_offset() {
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
    (void) (test_mv_cursor_right);
    (void) (test_mv_cursor_down);
    (void) (test_mv_cursor_left);
    (void) (test_mv_down_offset);
    (void) (test_mv_up_offset);
    (void) (test_mv_right_offset);
    (void) (test_mv_left_offset);
}
