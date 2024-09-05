#include "../src/include/commander.h"
#include "../src/include/buffer.h"
#include "../src/include/editor.h"
#include <assert.h>
#include <stdbool.h>

static void
test_commander_eval_exit_cmd1() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));
    String__append(&EDITOR.commander.cmd, EDITOR.commander.cmd.len, 'q');

    // act
    Commander__eval();

    // assert
    assert(EDITOR.exit == true);

    // release
    Editor__release();
}

static void
test_commander_eval_exit_cmd2() {
    // arrange
    EDITOR = Editor__new(Buffer__new(0));
    String__append(&EDITOR.commander.cmd, EDITOR.commander.cmd.len, 'Q');

    // act
    Commander__eval();

    // assert
    assert(EDITOR.exit == true);

    // release
    Editor__release();
}

void
T__commander() {
    test_commander_eval_exit_cmd1();
    test_commander_eval_exit_cmd2();
}
