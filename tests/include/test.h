#ifndef QUICKWARE_TEST_H
#define QUICKWARE_TEST_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define __TEST__(fn)       \
    do {                   \
        printf("%s", #fn); \
        fn();              \
        printf("...OK\n"); \
    } while (0)

#define UNIMPLEMENTED                       \
    do {                                    \
        assert(false && "NOT IMPLEMENTED"); \
    } while (0)

#endif
