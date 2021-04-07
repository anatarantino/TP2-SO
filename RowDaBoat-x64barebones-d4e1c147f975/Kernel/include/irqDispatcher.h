#ifndef IRQDISPATCHER_H
#define IRQDISPATCHER_H

#include <stdint.h>

static void int_20();
void int_21(uint64_t rsp);

#endif