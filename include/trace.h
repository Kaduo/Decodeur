#ifndef TRACE
#define TRACE

#ifdef DEBUG
#define trace(fmt, ...) do { fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
#else
#define trace(fmt, ...)  do {} while (0)
#endif

#endif
