#include "include/commander.h"
#include "include/editor.h"
#include "include/str.h"
#include <string.h>

Commander
Commander__new() {
    return (Commander) {
        .cmd = String__new(1),
    };
}

void
Commander__eval() {
    Str cmd = EDITOR.commander.cmd;

    if (strcmp("q", cmd.data) == 0) {
        EDITOR.exit = true;
    }

    Editor__set_mode(MODE__NORMAL);
}

void
Commander__release() {
    Commander *commander = &EDITOR.commander;

    String__release(&commander->cmd);
}
