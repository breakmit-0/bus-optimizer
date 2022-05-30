#ifndef INCLUDE_ENX_H
#define INCLUDE_EXN_H

#define FAIL(code, ...) printf(__VA_ARGS__); exit(code);

#define TRACE() printf("Trace at %s, %d\n", __FILE__, __LINE__)


#endif