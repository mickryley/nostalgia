nostalgia::BenchmarkID benchmark_id = nostalgia::BenchmarkID::IBM_Bursts;

// === Templated Global Access ===
// ~~~ Pointer Block ~~~
template <typename object_type>
void run_templated_globalAccess_pointerBlock_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(&vec[k], 1);
		}
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_globalAccess_pointerBlock_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();
		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(&vec[k], 1);
		}
		allocator_template.rewind();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_globalAccess_pointerBlock_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
// ~~~ Pointer Container ~~~
template <typename object_type>
void run_templated_globalAccess_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(vec[k], 1);
		}
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_globalAccess_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(vec[k], 1);
		}
		
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_globalAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
// ~~~ Pointer Vector ~~~
template <typename object_type>
void run_templated_globalAccess_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(vec[k], 1);
		}
		
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_globalAccess_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(vec[k], 1);
		}
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_globalAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_GLOBAL_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

// === Templated Singleton Access ===
// ~~~ Pointer Block ~~~
template <typename object_type>
void run_templated_singletonAccess_pointerBlock_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(&vec[k], 1);
		}
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_singletonAccess_pointerBlock_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();
		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(&vec[k], 1);
		}
		allocator_template.rewind();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_singletonAccess_pointerBlock_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

// ~~~ Pointer Container ~~~
template <typename object_type>
void run_templated_singletonAccess_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(vec[k], 1);
		}
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_singletonAccess_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(vec[k], 1);
		}
		
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_singletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
// ~~~ Pointer Vector ~~~
template <typename object_type>
void run_templated_singletonAccess_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(vec[k], 1);
		}
		
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_singletonAccess_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(vec[k], 1);
		}
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_singletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}


// === Templated Cached Singleton Access ===
// ~~~ Pointer Block ~~~
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerBlock_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(&vec[k], 1);
		}
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerBlock_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();
		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(&vec[k], 1);
		}
		allocator_template.rewind();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerBlock_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type* vec = allocator_template.allocate(iterations); 
		for (size_t j = 0; j < iterations; j++) {
			new (&vec[j]) object_type(CONSTRUCTOR_PARAMETERS); 
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

// ~~~ Pointer Container ~~~
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(vec[k], 1);
		}
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(vec[k], 1);
		}
		
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations];

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = allocator_template.create(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		delete[] vec;
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
// ~~~ Pointer Vector ~~~
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			allocator_template.deallocate(vec[k], 1);
		}
		
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			allocator_template.deallocate(vec[k], 1);
		}
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_templated_cachedSingletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS<object_type> allocator_template;

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(allocator_template.create(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		allocator_template.rewind();
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}


// === Object Override (Compatible with Global Static and Singleton) ===
// ~~~ Pointer Block implementation would bypass object override ~~~
// ~~~ Pointer Container ~~~
template <typename object_type>
void run_objectOverride_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new object_type * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new object_type(CONSTRUCTOR_PARAMETERS);    // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			delete vec[k];
		}

		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_objectOverride_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations]; 

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new object_type(CONSTRUCTOR_PARAMETERS);   
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			delete vec[k];
		}

		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
// ~~~ Pointer Vector ~~~
template <typename object_type>
void run_objectOverride_pointerVector_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(new object_type(CONSTRUCTOR_PARAMETERS)); // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			delete vec[k];
		}

		vec.clear();

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_objectOverride_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(new object_type(CONSTRUCTOR_PARAMETERS)); // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {
			delete vec[k];
		}
		
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

// === Object Override (Global Access Only [For Rewind Access]) ===
template <typename object_type>
void run_objectOverride_globalAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new object_type * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new object_type(CONSTRUCTOR_PARAMETERS);    // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		ALLOCATOR_GLOBAL_ACCESS.rewind();
		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_objectOverride_globalAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index,
	nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(new object_type(CONSTRUCTOR_PARAMETERS)); // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		ALLOCATOR_GLOBAL_ACCESS.rewind();
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

// === Object Override (Singleton Only [For Rewind Access]) ===
// ~~~ Pointer Container ~~~
template <typename object_type>
void run_objectOverride_singletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new object_type * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new object_type(CONSTRUCTOR_PARAMETERS);    // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		ALLOCATOR_SINGLETON_ACCESS.rewind();

		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_objectOverride_singletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			vec.emplace_back(new object_type(CONSTRUCTOR_PARAMETERS)); // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		ALLOCATOR_SINGLETON_ACCESS.rewind();
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

// === Object Override (Cached Singleton Only [For Rewind Access]) ===
template <typename object_type>
void run_objectOverride_cachedSingletonAccess_pointerContainer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto& cached_allocator = ALLOCATOR_SINGLETON_ACCESS;

		auto** vec = new object_type * [iterations]; 

		for (size_t j = 0; j < iterations; j++) {
			void* ptr = cached_allocator.allocate(sizeof(object_type));
			vec[j] = new (ptr) object_type(CONSTRUCTOR_PARAMETERS);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		cached_allocator.rewind();

		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_objectOverride_cachedSingletonAccess_pointerVector_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index, nostalgia::ImplementationID i_id) {

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	PRINT_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto& cached_allocator = ALLOCATOR_SINGLETON_ACCESS;

		std::vector<object_type*> vec;
		vec.reserve(iterations);

		for (size_t j = 0; j < iterations; j++) {
			void* ptr = cached_allocator.allocate(sizeof(object_type)); 
			vec.push_back(new (ptr) object_type(CONSTRUCTOR_PARAMETERS));
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		cached_allocator.rewind();
		vec.clear();
		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}


// === Malloc (Default allocator for reference) ===
template <typename object_type>
void run_malloc_pointerContainer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {

	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::NoAllocator_Malloc_PointerContainer_ForwardDeallocation; 

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)					// (Benchmarking) - 1 - Passes
	{
		START_ALLOC_TIMERS();

		auto** vec = new object_type * [iterations]; 	// [ALLOC SPECIFIC] {Implementation Specific}

		for (size_t j = 0; j < iterations; j++) {		// (Benchmarking) - 2 - Iterations - Allocations
			void* ptr = malloc(sizeof(object_type));  	// [ALLOC SPECIFIC] {Implementation Specific}
			if (!ptr) throw std::bad_alloc(); 			// {Implementation Specific}
			vec[j] = new (ptr) object_type(CONSTRUCTOR_PARAMETERS);  // {Implementation Specific}
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {		// (Benchmarking) - 3 - Iterations - Deallocations
			vec[k]->~object_type(); 					// [ALLOC SPECIFIC] {Implementation Specific}
			std::free(vec[k]); 							// [ALLOC SPECIFIC] {Implementation Specific}
		}
		delete[] vec;									// [ALLOC SPECIFIC] {Implementation Specific}

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_malloc_pointerContainer_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {

	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::NoAllocator_Malloc_PointerContainer_ReverseDeallocation;

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)					// (Benchmarking) - 1 - Passes
	{
		START_ALLOC_TIMERS();

		object_type** vec = new object_type * [iterations]; 	// [ALLOC SPECIFIC] {Implementation Specific}

		for (size_t j = 0; j < iterations; j++) {		// (Benchmarking) - 2 - Iterations - Allocations
			void* ptr = malloc(sizeof(object_type));  	// [ALLOC SPECIFIC] {Implementation Specific}
			if (!ptr) throw std::bad_alloc(); 			// {Implementation Specific}
			vec[j] = new (ptr) object_type(CONSTRUCTOR_PARAMETERS);  // {Implementation Specific}
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {			// (Benchmarking) - 3 - Iterations - Deallocations
			vec[k]->~object_type(); 					// [ALLOC SPECIFIC] {Implementation Specific}
			std::free(vec[k]); 							// [ALLOC SPECIFIC] {Implementation Specific}
		}
		delete[] vec;									// [ALLOC SPECIFIC] {Implementation Specific}

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

template <typename object_type>
void run_malloc_pointerVector_reverseDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {

	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::NoAllocator_Malloc_PointerVector_ReverseDeallocation;

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)	
	{
		START_ALLOC_TIMERS();

		std::vector<object_type*> vec;	
		vec.reserve(iterations);						

		for (size_t j = 0; j < iterations; j++) {		
			object_type* ptr = static_cast<object_type*>(malloc(sizeof(object_type))); 
			if (!ptr) throw std::bad_alloc(); 			
			vec.push_back(ptr);
			new (ptr) object_type(CONSTRUCTOR_PARAMETERS);  
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = iterations; k-- > 0;) {							// (Benchmarking) - 3 - Iterations - Deallocations
			vec[k]->~object_type();		// [ALLOC SPECIFIC] {Implementation Specific}
			std::free(vec[k]); 											// [ALLOC SPECIFIC] {Implementation Specific}
		}

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}


#include "allocators/defines/_clear_defines.inl"