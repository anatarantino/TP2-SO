#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

void exceptionDispatcher(int exception, uint64_t * registers); 

void initExceptionHandler(uint64_t rip,uint64_t rsp);

#endif