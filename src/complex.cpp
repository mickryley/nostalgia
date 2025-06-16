#include "memoryManager.h"

#include "complex.h"
#include <sys/types.h> 
#include <iostream>

#include "freeStore.h"

#define POOLSIZE 1000

void MemoryManager::expandPoolSize ()
  {
    //std::cout << "EXPANDING POOL\n";
  size_t size = (sizeof(Complex) > sizeof(FreeStore*)) ?
    sizeof(Complex) : sizeof(FreeStore*);

  std::cout << "size = " << size << '\n';
  // Allocate the first node
    freeStoreHead = reinterpret_cast<FreeStore*>(new char[size]);
    FreeStore* current = freeStoreHead;

    //std::cout << "First Node Allocated";
    //std::cout << "freeStoreHead = " << freeStoreHead << " & current = " << current;

    for (int i = 1; i < POOLSIZE; ++i) {
        current->next = reinterpret_cast<FreeStore*>(new char[size]);
        current = current->next;
    }

  current->next = 0;
  }

void MemoryManager::cleanUp()
  {
  FreeStore* nextPtr = freeStoreHead;
  for (; nextPtr; nextPtr = freeStoreHead) {
    freeStoreHead = freeStoreHead->next;
    delete [] nextPtr; // remember this was a char array
    }
  }

  inline void* MemoryManager::allocate(size_t size)
  {
    //std::cout << "Attempting to allocate\n";
    //std::cout << "ALLOCATE: freeStoreHead = " << freeStoreHead << "\n";
  if (0 == freeStoreHead)
    expandPoolSize ();

   // std::cout << "ALLOCATING";
  FreeStore* head = freeStoreHead;
  freeStoreHead = head->next;
  //std::cout << "Allocating from pool: " << head << "\n";
  return head;
  }

inline void MemoryManager::free(void* deleted)
  {
  FreeStore* head = static_cast <FreeStore*> (deleted);
  head->next = freeStoreHead;
  freeStoreHead = head;
  }

void* Complex::operator new (size_t size) 
  {
    //std::cout << "New Complex 1A called";
  return gMemoryManager.allocate(size);
  }

void Complex::operator delete (void* pointerToDelete)
  {
    //std::cout << "Delete Complex 1A called";
  gMemoryManager.free(pointerToDelete);
  }
  
int complexMain(int argc, char* argv[]) 
  {
  Complex* array[1000];
  for (int i = 0;i  <  5000; i++) {
    for (int j = 0; j  <  1000; j++) {
      array[j] = new Complex (i, j);
      }
    for (int j = 0; j  <  1000; j++) {
      delete array[j];
      }
    }
  return 0;
  }