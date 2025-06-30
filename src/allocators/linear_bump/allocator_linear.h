#pragma once

namespace nostalgia::linear {

    // No (Default) Implementation
    void linearAllocator();

    // Object Override Implementation (Static & Singleton Access)
    void linearAllocatorObjectOverrideStatic();
    void linearAllocatorObjectOverrideSingleton();

/*
    - Standard / None
- Global Override Static + Singleton
- Object Override Static + Singleton
    - Pointer Container Variant
    - STD Container Variant
    - Custom Container Variant
- Template Allocator
    - Pointer Container Variant
    - STD Container Variant
    - Custom Container Variant
- STL Conformed Template Allocator
    - Pointer Container Variant
    - STD Container Variant
- PMR Conformed Static Allocator
    - PMR Container
- PMR Confirmed Singleton Allocator
    - PMR Container

    */
}