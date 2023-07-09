#include <u.h>
#include <libc.h>

void
fatal(const char *fmt, ...) {
    va_list arg;

    va_start(arg, fmt);
    fprint(2, fmt, arg);
    va_end(arg);

    fprint(2, "\n");
    exit(1);
}
