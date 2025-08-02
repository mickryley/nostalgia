// === Templated Global Access ===
template <typename object_type>
void run_templated_globalAccess_pointerBlock_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_globalAccess_pointerBlock_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_globalAccess_pointerBlock_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_templated_globalAccess_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);

template <typename object_type>
void run_templated_globalAccess_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);

template <typename object_type>
void run_templated_globalAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_templated_globalAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_templated_globalAccess_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);

	template <typename object_type>
void run_templated_globalAccess_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);

// === Templated Singleton Access ===
// ~~~ Pointer Block ~~~
template <typename object_type>
void run_templated_singletonAccess_pointerBlock_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_singletonAccess_pointerBlock_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_singletonAccess_pointerBlock_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

// ~~~ Pointer Container ~~~
template <typename object_type>
void run_templated_singletonAccess_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_singletonAccess_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_singletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

// ~~~ Pointer Vector ~~~
template <typename object_type>
void run_templated_singletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_singletonAccess_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
	template <typename object_type>
void run_templated_singletonAccess_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);

	// === Templated Cached Singleton Access ===
// ~~~ Pointer Block ~~~
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerBlock_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerBlock_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerBlock_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

// ~~~ Pointer Container ~~~
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

// ~~~ Pointer Vector ~~~
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
	template <typename object_type>
void run_templated_cachedSingletonAccess_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);


// === Object Override (Compatible with Global Static and Singleton) ===
template <typename object_type>
void run_objectOverride_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_objectOverride_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_objectOverride_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);

template <typename object_type>
void run_objectOverride_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id);
                                                             
// === Object Override (Global Access Only) ===
template <typename object_type>
void run_objectOverride_globalAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_objectOverride_globalAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

// === Object Override (Singleton Only) ===
template <typename object_type>
void run_objectOverride_singletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_objectOverride_cachedSingletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_objectOverride_singletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

template <typename object_type>
void run_objectOverride_cachedSingletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id);

// === Malloc (Default allocator for reference) ===
template <typename object_type>
void run_malloc_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index);

template <typename object_type>
void run_malloc_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index);

template <typename object_type>
void run_malloc_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index);