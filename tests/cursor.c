#include "../src/include/cursor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void
test_mv_cursor_up(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__down();
    Cursor__down();
    Cursor__up();

    // assert
    assert(cursor.y == 1);
}

static void
test_mv_cursor_right(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__right();

    // assert
    assert(cursor.x == 1);
}

static void
test_mv_cursor_down(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__down();

    // assert
    assert(cursor.y == 1);
}

static void
test_mv_cursor_left(void) {
    // arrange
    Cursor cursor = Cursor__new();

    // act
    Cursor__right();
    Cursor__right();
    Cursor__left();

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
