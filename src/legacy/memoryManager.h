#pragma once

#include <cstdlib>

#include "freeStore.h"
#include "memoryInterface.h"

#include <iostream>


class MemoryManager: public IMemoryManager 
  { 

  void expandPoolSize ();
  void cleanUp ();
  FreeStore* freeStoreHead;
  
  public:
    MemoryManager () { 
      freeStoreHead = 0;
      expandPoolSize ();
      }
    virtual ~MemoryManager () { 
      cleanUp ();
      }
    virtual void* allocate(size_t);
    virtual void   free(void*);
  };

MemoryManager gMemoryManager;

/* For Nostalgia swap to singleton

class MemoryManager {
public:
    static MemoryManager& instance() {
        static MemoryManager mm;
        return mm;
    }

    void* allocate(std::size_t size) { ... }
    void free(void* ptr) { ... }

private:
    MemoryManager() = default;
};
void* operator new(std::size_t size) {
    return MemoryManager::instance().allocate(size);
}
*/
/*
void* operator new (size_t size) 
  {
    std::cout << "New 1A called";
  return gMemoryManager.allocate(size);
  }

void* operator new[ ] (size_t size)
  {
    std::cout << "New 1B Array called";
  return  gMemoryManager.allocate(size);
  }

void operator delete (void* pointerToDelete) noexcept
  {
    std::cout << "Delete 1A called";
  gMemoryManager.free(pointerToDelete);
  }

void operator delete[ ] (void* arrayToDelete) noexcept
  {
        std::cout << "Delete 1B Array called";
  gMemoryManager.free(arrayToDelete);
  }*/
  