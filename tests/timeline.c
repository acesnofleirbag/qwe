#include "../src/include/timeline.h"
#include "../src/include/buffer.h"
#include "../src/include/editor.h"
#include <assert.h>

static void
test_timeline_push() {
    // arrange
    EDITOR = editor__new(buffer__new(0));

    // act
    timeline__push('a');

    // assert
    assert(EDITOR.timeline.data[EDITOR.timeline.len - 1] == 'a');

    // release
    editor__release();
}

static void
test_timeline_pop() {
    // arrange
    EDITOR = editor__new(buffer__new(0));
    timeline__push('a');
    timeline__push('b');
    timeline__push('c');

    // act
    timeline__pop();

    // assert
    assert(EDITOR.timeline.data[EDITOR.timeline.len - 1] == 'b');

    // release
    editor__release();
}

void
T__timeline() {
    test_timeline_push();
    test_timeline_pop();
}
