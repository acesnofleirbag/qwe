#include "../src/include/timeline.h"
#include "../src/include/buffer.h"
#include "../src/include/editor.h"
#include <assert.h>

static void
test_timeline_push() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));

    // act
    Timeline__push('a');

    // assert
    assert(EDITOR.timeline.data[EDITOR.timeline.len - 1] == 'a');

    // release
    Editor__release();
}

static void
test_timeline_pop() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));
    Timeline__push('a');
    Timeline__push('b');
    Timeline__push('c');

    // act
    Timeline__pop();

    // assert
    assert(EDITOR.timeline.data[EDITOR.timeline.len - 1] == 'b');

    // release
    Editor__release();
}

void
T__timeline() {
    test_timeline_push();
    test_timeline_pop();
}
