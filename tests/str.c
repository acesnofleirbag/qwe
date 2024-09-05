#include "../src/include/str.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
test_str_new() {
    // arrange & act
    Str str = String__new(2);

    // assert
    assert(str.capacity == 2);

    // release
    String__release(&str);
}

static void
test_str_append() {
    // arrange
    Str str = String__new(3);

    // act
    String__append(&str, str.len, 'a');
    String__append(&str, str.len, 'b');

    // assert
    assert(strcmp(str.data, "ab") == 0);

    // release
    String__release(&str);
}

static void
test_str_append_on_middle() {
    // arrange
    Str str = String__new(5);
    String__append(&str, str.len, 'a');
    String__append(&str, str.len, 'b');
    String__append(&str, str.len, 'c');

    // act
    String__append(&str, 1, 'x');

    // assert
    assert(strcmp(str.data, "axbc") == 0);

    // release
    String__release(&str);
}

static void
test_str_remove() {
    // arrange
    Str str = String__new(3);
    String__append(&str, str.len, 'a');
    String__append(&str, str.len, 'b');
    String__append(&str, str.len, 'c');

    // act
    String__remove(&str, str.len);

    // assert
    assert(strcmp(str.data, "ab") == 0);

    // release
    String__release(&str);
}

static void
test_str_remove_on_middle() {
    // arrange
    Str str = String__new(3);
    String__append(&str, str.len, 'a');
    String__append(&str, str.len, 'b');
    String__append(&str, str.len, 'c');

    // act
    String__remove(&str, 2);

    // assert
    assert(strcmp(str.data, "ac") == 0);

    // release
    String__release(&str);
}

void
T__string() {
    test_str_new();
    test_str_append();
    test_str_append_on_middle();
    test_str_remove();
    test_str_remove_on_middle();
}
