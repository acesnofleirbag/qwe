#include "include/finder.h"

Finder
Finder__new() {
    return (Finder) {
        .query = String__new(1),
    };
}
