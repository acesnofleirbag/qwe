#include "include/timeline.h"
#include <stdlib.h>

Timeline
Timeline__new() {
    return (Timeline) {
        .len = 0,
        .data = NULL,
    };
}

void
Timeline__release(Timeline *timeline) {
    free(timeline->data);
    timeline->data = NULL;
}
