#pragma once

#include <cstddef> 

int complexMain(int argc, char* argv[]);

class Complex 
  {
  public:
    Complex (double a, double b): r (a), c (b) {}
    inline void* operator new (size_t);
    inline void   operator delete (void*);
  private:
    double r; // Real Part
    double c; // Complex Part
  };