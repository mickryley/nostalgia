#pragma once

#include <cstdlib>

class IMemoryManager 
  {
  public:
    virtual void* allocate(size_t) = 0;
    virtual void   free(void*) = 0;
  };