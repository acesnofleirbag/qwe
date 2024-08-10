#include "include/editor.h"
#include "include/mem.h"
#include "include/str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void) {
    Editor editor = Editor__new();

    Editor__run(&editor);

    return 0;
}
