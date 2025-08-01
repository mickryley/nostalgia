
// Templated
    // ~ Reverse Deallocation
run_templated_globalAccess_pointerBlock_rewindDealloc<PARAMETERISED_OBJECT_BASIC>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::Templated_GlobalAccess_PointerBlock_RewindDeallocation);

run_templated_globalAccess_pointerContainer_forwardDealloc<PARAMETERISED_OBJECT_BASIC>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::Templated_GlobalAccess_PointerContainer_ForwardDeallocation);
run_templated_globalAccess_pointerContainer_reverseDealloc<PARAMETERISED_OBJECT_BASIC>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::Templated_GlobalAccess_PointerContainer_ReverseDeallocation);
run_templated_globalAccess_pointerContainer_rewindDealloc<PARAMETERISED_OBJECT_BASIC>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::Templated_GlobalAccess_PointerContainer_RewindDeallocation);


run_templated_globalAccess_pointerVector_forwardDealloc<PARAMETERISED_OBJECT_BASIC>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::Templated_GlobalAccess_PointerVector_ForwardDeallocation);
run_templated_globalAccess_pointerVector_reverseDealloc<PARAMETERISED_OBJECT_BASIC>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::Templated_GlobalAccess_PointerVector_ReverseDeallocation);
run_templated_globalAccess_pointerVector_rewindDealloc<PARAMETERISED_OBJECT_BASIC>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::Templated_GlobalAccess_PointerVector_RewindDeallocation);


// Global access
    // ~ Forward Deallocation
run_objectOverride_pointerContainer_forwardDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_ForwardDeallocation);
    // ~ Reverse Deallocation
run_objectOverride_pointerContainer_reverseDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_ReverseDeallocation);
    // ~~ Rewind
run_objectOverride_globalAccess_pointerContainer_rewindDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_RewindDeallocation);

    // ~ Forward Deallocation
run_objectOverride_pointerVector_forwardDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerVector_ForwardDeallocation);
    // ~ Reverse Deallocation
run_objectOverride_pointerVector_reverseDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerVector_ReverseDeallocation);
    // ~~ Rewind
run_objectOverride_globalAccess_pointerVector_rewindDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerVector_RewindDeallocation);

// Singleton access
    // ~ Forward Deallocation
run_objectOverride_pointerContainer_forwardDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_ForwardDeallocation);
    // ~ Reverse Deallocation
run_objectOverride_pointerContainer_reverseDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_ReverseDeallocation);

    // ~ Forward Deallocation
run_objectOverride_pointerVector_forwardDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerVector_ForwardDeallocation);
    // ~ Reverse Deallocation
run_objectOverride_pointerVector_reverseDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerVector_ReverseDeallocation);


    // ~~ Rewind
run_objectOverride_singletonAccess_pointerContainer_rewindDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_RewindDeallocation);

run_objectOverride_cachedSingletonAccess_pointerContainer_rewindDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_CachedSingletonAccess_PointerContainer_RewindDeallocation);

run_objectOverride_singletonAccess_pointerVector_rewindDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerVector_RewindDeallocation);

run_objectOverride_cachedSingletonAccess_pointerVector_rewindDealloc<PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS>(
    allocator, iterations, passes, object_id_index,
    nostalgia::ImplementationID::ObjectOverride_CachedSingletonAccess_PointerVector_RewindDeallocation);

// Malloc
run_malloc_pointerContainer_forwardDealloc<PARAMETERISED_OBJECT_BASIC>(allocator, iterations, passes, object_id_index);
run_malloc_pointerContainer_reverseDealloc<PARAMETERISED_OBJECT_BASIC>(allocator, iterations, passes, object_id_index);
run_malloc_pointerVector_reverseDealloc<PARAMETERISED_OBJECT_BASIC>(allocator, iterations, passes, object_id_index);