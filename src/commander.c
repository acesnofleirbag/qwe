#include "include/commander.h"
#include "include/editor.h"
#include "include/str.h"
#include <string.h>

commander_t
commander__new() {
    return (commander_t) {
        .cmd = string__new(1),
    };
}

void
commander__eval() {
    str_t cmd = EDITOR.commander.cmd;

    if (strcmp("q", cmd.data) == 0) {
        EDITOR.exit = true;
    } else if (strcmp("Q", cmd.data) == 0) {
        EDITOR.exit = true;
    }

    editor__set_mode(MODE__NORMAL);
}

void
commander__release() {
    commander_t *commander = &EDITOR.commander;

    string__release(&commander->cmd);
}
