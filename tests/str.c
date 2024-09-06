#include "../src/include/str.h"
#include <assert.h>
#include <string.h>

static void
test_str_new() {
    // arrange & act
    str_t str = string__new(2);

    // assert
    assert(str.capacity == 2);

    // release
    string__release(&str);
}

static void
test_str_append() {
    // arrange
    str_t str = string__new(3);

    // act
    string__append(&str, str.len, 'a');
    string__append(&str, str.len, 'b');

    // assert
    assert(strcmp(str.data, "ab") == 0);

    // release
    string__release(&str);
}

static void
test_str_append_on_middle() {
    // arrange
    str_t str = string__new(5);
    string__append(&str, str.len, 'a');
    string__append(&str, str.len, 'b');
    string__append(&str, str.len, 'c');

    // act
    string__append(&str, 1, 'x');

    // assert
    assert(strcmp(str.data, "axbc") == 0);

    // release
    string__release(&str);
}

static void
test_str_remove() {
    // arrange
    str_t str = string__new(3);
    string__append(&str, str.len, 'a');
    string__append(&str, str.len, 'b');
    string__append(&str, str.len, 'c');

    // act
    string__remove(&str, str.len);

    // assert
    assert(strcmp(str.data, "ab") == 0);

    // release
    string__release(&str);
}

static void
test_str_remove_on_middle() {
    // arrange
    str_t str = string__new(3);
    string__append(&str, str.len, 'a');
    string__append(&str, str.len, 'b');
    string__append(&str, str.len, 'c');

    // act
    string__remove(&str, 2);

    // assert
    assert(strcmp(str.data, "ac") == 0);

    // release
    string__release(&str);
}

void
T__string() {
    test_str_new();
    test_str_append();
    test_str_append_on_middle();
    test_str_remove();
    test_str_remove_on_middle();
}
