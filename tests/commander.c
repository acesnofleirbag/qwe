#include "../src/include/commander.h"
#include "../src/include/buffer.h"
#include "../src/include/editor.h"
#include <assert.h>
#include <stdbool.h>

static void
test_commander_eval_exit_cmd1() {
    // arrange
    EDITOR = editor__new(buffer__new(0));
    string__append(&EDITOR.commander.cmd, EDITOR.commander.cmd.len, 'q');

    // act
    commander__eval();

    // assert
    assert(EDITOR.exit == true);

    // release
    editor__release();
}

static void
test_commander_eval_exit_cmd2() {
    // arrange
    EDITOR = editor__new(buffer__new(0));
    string__append(&EDITOR.commander.cmd, EDITOR.commander.cmd.len, 'Q');

    // act
    commander__eval();

    // assert
    assert(EDITOR.exit == true);

    // release
    editor__release();
}

void
T__commander() {
    test_commander_eval_exit_cmd1();
    test_commander_eval_exit_cmd2();
}
