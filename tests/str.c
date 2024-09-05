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

void
T__string(void) {
    test_str_new();
    test_str_append();
}
