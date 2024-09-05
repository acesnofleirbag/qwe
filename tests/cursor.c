#include "../src/include/cursor.h"
#include "../src/include/buffer.h"
#include "../src/include/editor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void
test_mv_cursor_up() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__down();
    Cursor__down();
    Cursor__up();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->y == 1);

    // release
    Editor__release();
}

static void
test_mv_cursor_right() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__right();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->x == 1);

    // release
    Editor__release();
}

static void
test_mv_cursor_down() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__down();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->y == 1);

    // release
    Editor__release();
}

static void
test_mv_cursor_left() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__right();
    Cursor__right();
    Cursor__left();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->x == 1);

    // release
    Editor__release();
}

static void
test_mv_down_offset() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__right();
    Cursor__right();
    Cursor__left();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->x == 1);

    // release
    Editor__release();
}

static void
test_mv_up_offset() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__right();
    Cursor__right();
    Cursor__left();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->x == 1);

    // release
    Editor__release();
}

static void
test_mv_right_offset() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__right();
    Cursor__right();
    Cursor__left();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->x == 1);

    // release
    Editor__release();
}

static void
test_mv_left_offset() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Cursor__right();
    Cursor__right();
    Cursor__left();

    // assert
    Cursor *cursor = Editor__get_cursor();

    assert(cursor->x == 1);

    // release
    Editor__release();
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
