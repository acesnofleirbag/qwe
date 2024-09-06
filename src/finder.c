#include "include/finder.h"
#include "include/editor.h"
#include "include/str.h"

finder_t
finder__new() {
    return (finder_t) {
        .mode = FIND_MODE__FORWARD,
        .query = string__new(1),
    };
}

void
finder__release() {
    finder_t *finder = &EDITOR.finder;

    string__release(&finder->query);
}
