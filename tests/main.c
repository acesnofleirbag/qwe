#include "buffer.c"
#include "commander.c"
#include "cursor.c"
#include "editor.c"
#include "include/test.h"
#include "str.c"
#include "timeline.c"
#include <stdio.h>
#include <stdlib.h>

int
main() {
    __TEST__(T__cursor);
    __TEST__(T__string);
    __TEST__(T__buffer);
    __TEST__(T__editor);
    __TEST__(T__timeline);
    __TEST__(T__commander);

// skip test-cases area
#if 0
#endif
    return 0;
}
