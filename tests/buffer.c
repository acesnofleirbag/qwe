#include "../src/include/buffer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
test_buffer_new() {
    // arrange & act
    Buffer buffer = Buffer__new(0);

    // assert
    assert(buffer.id == 0);
}

void
T__buffer() {
    test_buffer_new();
}
