#include "include/editor.h"
#include "include/mem.h"
#include <stdio.h>

int
main(void) {
    Editor editor = Editor__new();

    Editor__run(&editor);

    // Editor__clean
    Buffer *root = &editor.buffer;

    while (root->next) {
        Buffer *erase = root;
        root = root->next;

        MEM__release(erase);
    }

    MEM__release(root);

    return 0;
}
