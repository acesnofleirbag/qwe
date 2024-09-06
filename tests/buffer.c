#include "../src/include/buffer.h"
#include <assert.h>

static void
test_buffer_new() {
    // arrange & act
    buffer_t buffer = buffer__new(0);

    // assert
    assert(buffer.id == 0);
}

void
T__buffer() {
    test_buffer_new();
}
