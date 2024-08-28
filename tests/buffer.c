#include "../src/include/buffer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
test_buffer_new() {
    // arrange & act
    Buffer buffer = Buffer__new();

    // assert
    assert(buffer.id == 1);
}

void
T__buffer(void) {
    test_buffer_new();
}
