#include "include/buffer.h"
#include "include/debug.h"
#include "include/editor.h"
#include "include/explorer.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void
usage() {
    printf("Usage:\n");
    printf("-h, --help: This help message\n");
    printf("-v, --version: Print the quickware editor version\n");
    printf("\nCopyright (C) The Quickware Editor\n");

    exit(0);
}

static void
eval_arguments(int argc, char **argv) {
    int arg;
    int index = 0;

    struct option flags[] = {{"help", no_argument, NULL, 'h'}, {"version", no_argument, NULL, 'v'}, {NULL, 0, NULL, 0}};

    while ((arg = getopt_long(argc, argv, "hv", flags, &index)) != -1) {
        switch (arg) {
            case 'h':
                usage();
                break;
            case 'v':
                printf("The Quickware Editor 0.1.0\n");
                exit(0);
                break;
        }
    }
}

Buffer
eval_entry_files(int argc, char **argv) {
    // eval non-option arguments
    for (int i = optind; i < argc; i++) {
        char *arg = argv[i];

        if (Explorer__is_file(arg)) {
            return Buffer__from_file(arg, i - optind);
        }
    }

    return Buffer__new(0);
}

Editor EDITOR;

#ifdef TEST
#define main yield_main
#endif

int
main(int argc, char **argv) {
    eval_arguments(argc, argv);

    Buffer buffer = eval_entry_files(argc, argv);

    // Global editor state
    EDITOR = Editor__new(buffer);

    Editor__run();

    return 0;
}
