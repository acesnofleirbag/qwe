#include "include/finder.h"
#include "include/editor.h"
#include "include/str.h"

Finder
Finder__new() {
    return (Finder) {
        .mode = FIND_MODE__FORWARD,
        .query = String__new(1),
    };
}

void
Finder__release() {
    Finder *finder = &EDITOR.finder;

    String__release(&finder->query);
}
