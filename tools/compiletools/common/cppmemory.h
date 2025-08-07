#ifndef BLOCKALLOCATOR_H
#define BLOCKALLOCATOR_H

#include <cstring>

void* operator new(size_t size);
void operator delete(void* ptr) noexcept;

#endif // BLOCKALLOCATOR_H
