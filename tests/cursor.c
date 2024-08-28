#include "../src/include/cursor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void
test_mv_cursor_up(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__down(&cursor);
    Cursor__down(&cursor);
    Cursor__up(&cursor);

    // assert
    assert(cursor.y == 1);
}

static void
test_mv_cursor_right(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__right(&cursor);

    // assert
    assert(cursor.x == 1);
}

static void
test_mv_cursor_down(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__down(&cursor);

    // assert
    assert(cursor.y == 1);
}

static void
test_mv_cursor_left(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__right(&cursor);
    Cursor__right(&cursor);
    Cursor__left(&cursor);

    // assert
    assert(cursor.x == 1);
}

void
T__cursor(void) {
    test_mv_cursor_up();
    test_mv_cursor_right();
    test_mv_cursor_down();
    test_mv_cursor_left();
}
