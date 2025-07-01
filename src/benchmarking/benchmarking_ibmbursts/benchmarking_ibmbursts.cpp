#include "benchmarking_ibmbursts.h"

#include "allocators/linear_bump/allocator_linear.h"
#include "allocators/stack_lifo/allocator_stack.h"

#include "objects/objects_stack.h"
#include "objects/objects_linear.h"
#include "objects/objects.h"

#include "allocators/allocator_meta.h"
#include "benchmarking/benchmark_meta.h"

#include "benchmarking/exporting/benchmarking_exporting.h"

#include "timer.h"
#include "log.h"

#include <array>
#include <memory_resource>


namespace nostalgia::benchmarking::IBMBursts {

#pragma region Benchmarking Linear Allocators
	void benchmark_IBMBursts_None_V2D(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Cached Singleton V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Cached Singleton V2D");
		timer::Timer _timer = timer::Timer("Total Timer Cached Singleton V2D");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			std::vector<void*> rawPointers;
			rawPointers.reserve(iterations); // Prevent realloc

			_allocateTimer.start();

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//volatile auto* vec = new objects::Vector2D(1.0f, 2.0f);
				void* mem = std::malloc(sizeof(objects::Vector2D));
				if (!mem) throw std::bad_alloc(); // Safety check
				rawPointers.push_back(mem);
				new (mem) objects::Vector2D(1.0f, 2.0f); // Placement new
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			//linear::s_linearAllocator.rewind();
			for (void* mem : rawPointers) {
				reinterpret_cast<objects::Vector2D*>(mem)->~Vector2D(); // Call destructor
				std::free(mem); // Free memory
			}
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Static_V2D(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				volatile auto* vec =
					new linear::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Singleton_V2D(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Singleton V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Singleton V2D");
		timer::Timer _timer = timer::Timer("Total Timer Singleton V2D");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				volatile auto* vec =
					new linear::objects::Vector2D_LocalOverride_SingletonAccess(1.0f, 2.0f);
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Static_V2D_ContPointer(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D Cont Pointer");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D Cont Pointer");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			auto** vec = new linear::objects::Vector2D_LocalOverride_StaticAccess * [iterations];

			for (size_t j = 0; j < iterations; j++) {
				vec[j] = new linear::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			delete[] vec;
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Static_V2D_CustomPointerVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D Cont Pointer");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D Cont Pointer");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			//auto** vec = new linear::objects::Vector2D_LocalOverride_StaticAccess * [iterations];
			linear::PointerLinearVector_ObjectOverride<linear::objects::Vector2D_LocalOverride_StaticAccess> vec(iterations);

			for (size_t j = 0; j < iterations; j++) {
				//vec[j] = new linear::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
				vec.emplace_back(1.0f, 2.0f);
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			//delete[] vec;
			vec.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Singleton_V2D_ContPointer(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Singleton V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Singleton V2D");
		timer::Timer _timer = timer::Timer("Total Timer Singleton V2D");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			auto** vec = new linear::objects::Vector2D_LocalOverride_SingletonAccess * [iterations];

			for (size_t j = 0; j < iterations; j++) {
				vec[j] = new linear::objects::Vector2D_LocalOverride_SingletonAccess(1.0f, 2.0f);
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			//std::free(vec); 
			delete[] vec;
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Static_V2D_ContPointVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D Cont Pointer Vector");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D Cont Pointer Vector");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D Cont Vector");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			std::vector<linear::objects::Vector2D_LocalOverride_StaticAccess*> vecs;
			vecs.reserve(iterations); // Reserve space to prevent reallocations

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object								
				vecs.emplace_back(new linear::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f)); // Store the pointer in the vector
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vecs.clear();
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Singleton_V2D_ContPointVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Singleton V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Singleton V2D");
		timer::Timer _timer = timer::Timer("Total Timer Singleton V2D");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			std::vector<linear::objects::Vector2D_LocalOverride_SingletonAccess*> vecs;
			vecs.reserve(iterations); // Reserve space to prevent reallocations

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				vecs.emplace_back(new linear::objects::Vector2D_LocalOverride_SingletonAccess(1.0f, 2.0f)); // Store the pointer in the vector
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vecs.clear();
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_CachedSingleton_V2D(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Cached Singleton V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Cached Singleton V2D");
		timer::Timer _timer = timer::Timer("Total Timer Cached Singleton V2D");


		// Extra code required for cached singleton
		auto& _allocator = linear::SingletonLinearAllocator::getInstance();

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				void* mem = _allocator.allocate(sizeof(linear::objects::Vector2D_LocalOverride_SingletonAccess));
				volatile auto* vec = new (mem) linear::objects::Vector2D_LocalOverride_SingletonAccess(1.0f, 2.0f);

			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Template_V2D_ContPoint_Inplace(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorTemplate<V2D> _allocator;

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			V2D* vecs = _allocator.allocate(iterations);
			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Template_V2D_ContPoint(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorTemplate<V2D> _allocator;

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			auto** vec = new V2D * [iterations];

			//V2D* vecs = _allocator.allocate(iterations);
			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vec[j] = _allocator.create(1.0f, 2.0f); // Allocate a Vector2D object
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			delete[] vec;
			_allocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Template_V2D_ContPointVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorTemplate<V2D> _allocator;

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			std::vector<V2D*> vecs;
			vecs.reserve(iterations);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vecs.push_back(_allocator.create(1.0f, 2.0f)); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			_allocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_STL_Template_V2D_ContPointVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorStlTemplate<V2D> _allocator;

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			std::vector<V2D*> vecs;
			vecs.reserve(iterations);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vecs.push_back(_allocator.create(1.0f, 2.0f)); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			_allocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_STL_Template_V2D_ContVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;

			vecs.reserve(iterations);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vecs.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vecs.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_STL_Template_V2D_CustomVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorStlTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			//std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;
			linear::LinearVector_StlTemplateAllocator<V2D> vec(iterations * sizeof(V2D), _allocator);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vec.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_PMR_Monotonic_PmrVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		//linear::LinearAllocatorStlTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			//std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;
			//linear::LinearVector_StlTemplateAllocator<V2D> vec(iterations * sizeof(V2D), _allocator);
			size_t byteCount = iterations * sizeof(V2D); // Allocate a buffer for the PMR vector
			std::unique_ptr<std::byte[]> buffer = std::make_unique<std::byte[]>(byteCount);
			auto memResource = std::make_unique<std::pmr::monotonic_buffer_resource>(buffer.get(), byteCount);

			std::pmr::polymorphic_allocator<V2D> alloc(memResource.get());
			//std::pmr::vector<V2D> vec(iterations, alloc);
			//std::pmr::vector<V2D> vec(iterations, V2D{}, alloc);
			std::vector<V2D, std::pmr::polymorphic_allocator<V2D>> vec(iterations, V2D{}, alloc);

			//std::pmr::vector<V2D> vec(std::pmr::polymorphic_allocator<V2D>{&resource});

			//std::pmr::monotonic_buffer_resource pmrResource(buffer, sizeof(buffer));
			//std::pmr::vector<V2D> vec(&pmrResource);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new

				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vec.clear(); // Clear the vector
			//linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_PMR_Monotonic_HardCoded_PmrVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;

		constexpr size_t buffer_size = sizeof(V2D) * 1000;
		static std::array<std::byte, buffer_size> buffer; // Hardcoded buffer size for PMR vector
		std::pmr::monotonic_buffer_resource resource(buffer.data(), buffer.size());

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			//std::pmr::vector<V2D> vec(&resource); //MSVC Only
			//std::pmr::vector<V2D> vec(std::pmr::polymorphic_allocator<V2D>{&resource});
			std::vector<V2D, std::pmr::polymorphic_allocator<V2D>> vec(std::pmr::polymorphic_allocator<V2D>{&resource});


			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new

				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vec.clear(); // Clear the vector
			resource.release();
			//linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_PMR_Unsync_PmrVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		//linear::LinearAllocatorStlTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			//std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;
			//linear::LinearVector_StlTemplateAllocator<V2D> vec(iterations * sizeof(V2D), _allocator);
			//size_t byteCount = iterations * sizeof(V2D); // Allocate a buffer for the PMR vector
			//std::unique_ptr<std::byte[]> buffer = std::make_unique<std::byte[]>(byteCount);

			std::pmr::unsynchronized_pool_resource pool;
			//std::pmr::vector<V2D> vec(iterations, &pool);
			auto alloc = std::pmr::polymorphic_allocator<V2D>{ &pool };
			//std::pmr::vector<V2D> vec(iterations, V2D{}, alloc);
			std::vector<V2D, std::pmr::polymorphic_allocator<V2D>> vec(iterations, V2D{}, alloc);
			//std::pmr::vector<V2D> vec(iterations, V2D{}, std::pmr::polymorphic_allocator<V2D>{&pool});
			//std::pmr::vector<V2D> vec(std::pmr::polymorphic_allocator<V2D>{&resource});

			//auto memResource = std::make_unique<std::pmr::monotonic_buffer_resource>(buffer.get(), byteCount);

			//std::pmr::polymorphic_allocator<V2D> alloc(memResource.get());
			//std::pmr::vector<V2D> vec(iterations, alloc);

			//std::pmr::monotonic_buffer_resource pmrResource(buffer, sizeof(buffer));
			//std::pmr::vector<V2D> vec(&pmrResource);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new

				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vec.clear(); // Clear the vector
			//linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts_Template_V2D_CustomVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			//std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;
			linear::LinearVector_TemplateAllocator<V2D> vec(iterations * sizeof(V2D), _allocator);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			vec.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			_deallocateTimer.pause();
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();
	}

	void benchmark_IBMBursts(int iterations = 1000, int passes = 5000, int size = 8) {

		constexpr size_t bufferSize = 1024 * 1024; // 1 MB
		char* buffer = new char[bufferSize]; // Mallocate a buffer of 1 MB
		nostalgia::linear::LinearAllocator _allocator(buffer, bufferSize);



		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer");
		timer::Timer _timer = timer::Timer("Total Timer");

		_timer.start();
		for (size_t i = 0; i < passes; i++)
		{
			_allocateTimer.start();
			for (size_t j = 0; j < iterations; j++) {
				_allocator.allocate(size); // Allocate 16 bytes
			}
			_allocateTimer.pause();
			_deallocateTimer.start();

			_allocator.rewind(); // Rewind the allocator after each pass

			_deallocateTimer.pause();
			/*for (size_t j = 0; j < iterations; j++) {
				_allocator.deallocate(); // Deallocate the last allocated block
			}*/
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();


	}

	std::vector<BenchmarkMetadata> linearBenchmarks = {
	{
		.label = "IBM Bursts Malloc",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_None_V2D(); }
	},
	{
		.label = "IBM Bursts (Manual Alloc only)",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts(); }
	},
	{
		.label = "IBM Bursts Static Object",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D(); }
	},
	{
		.label = "IBM Bursts Singleton Object",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Singleton_V2D(); }
	},
	{
		.label = "IBM Bursts Cached Singleton Object",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_CachedSingleton_V2D(); }
	},
	{
		.label = "IBM Bursts Static Object + Cont: Point Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D_ContPointVector(); }
	},
	{
		.label = "IBM Bursts Singleton Object + Cont: Point",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D_ContPointer(); }
	},
	{
		.label = "IBM Bursts Singleton Object + Cont: Point Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Singleton_V2D_ContPointVector(); }
	},
	{
		.label = "IBM Bursts Singleton Object + Cont: Point",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Singleton_V2D_ContPointer(); }
	},
	{
		.label = "IBM Bursts Templated Allocator Object + Cont: Point (In Place)",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_ContPoint_Inplace(); }
	},
	{
		.label = "IBM Bursts Templated Allocator Object + Cont: Point",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_ContPoint(); }
	},
	{
		.label = "IBM Bursts Templated Allocator Object + Cont: Point Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_ContPointVector(); }
	},
	{
		.label = "IBM Bursts STL Templated Allocator Object + Cont: Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_STL_Template_V2D_ContVector(); }
	},
	{
		.label = "IBM Bursts STL Templated Allocator Object + Cont: Point Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_STL_Template_V2D_ContPointVector(); }
	},
	{
		.label = "IBM Bursts STL Templated Allocator Object + Cont: Custom Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_STL_Template_V2D_CustomVector(); }
	},
	{
		.label = "IBM Bursts Templated Allocator Object + Cont: Custom Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_CustomVector(); }
	},
	{
		.label = "IBM Bursts Static Allocator Object + Cont: Custom Pointer Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D_CustomPointerVector(); }
	},
	{
		.label = "IBM Bursts PMR Default unsync + Cont: PMR Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_PMR_Unsync_PmrVector(); }
	},
	{
		.label = "IBM Bursts PMR Default monotonic_buffer_resource + Cont: PMR Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_PMR_Monotonic_PmrVector(); }
	},
	{
		.label = "IBM Bursts PMR Default hardcoded monotonic_buffer_resource + Cont: PMR Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_PMR_Monotonic_HardCoded_PmrVector(); }
	}




	};
#pragma endregion
	void benchmark_IBMBursts_Stack(int iterations = 1000, int passes = 5000, int size = 8) {

		constexpr size_t bufferSize = 1024 * 1024; // 1 MB
		char* buffer = new char[bufferSize]; // Mallocate a buffer of 1 MB
		nostalgia::stack::StackAllocator _allocator(buffer, bufferSize);

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer");
		timer::Timer _timer = timer::Timer("Total Timer");

		_timer.start();
		for (size_t i = 0; i < passes; i++)
		{
			_allocateTimer.start();
			for (size_t j = 0; j < iterations; j++) {
				_allocator.allocate(size); // Allocate 16 bytes
			}
			_allocateTimer.pause();

			_deallocateTimer.start();

			_allocator.rewind(); // Rewind the allocator after each pass
			
			_deallocateTimer.pause();
			/*for (size_t j = 0; j < iterations; j++) {
				_allocator.deallocate(); // Deallocate the last allocated block
			}*/
		}
		_timer.stop();
		_allocateTimer.print();
		_deallocateTimer.print();

	}

	void benchmark_IBMBursts_Stack_Static_V2D_ContPointer(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer _allocateTimer = timer::Timer("Allocate Timer Static V2D Cont Pointer");
		timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer Static V2D Cont Pointer");
		timer::Timer _timer = timer::Timer("Total Timer Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			_allocateTimer.start();
			auto** vec = new stack::objects::Vector2D_LocalOverride_StaticAccess * [iterations];

			for (size_t j = 0; j < iterations; j++) {
				vec[j] = new stack::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
			}
			_allocateTimer.pause();

			// Deallocation Iterations
			_deallocateTimer.start();
			for (int i = static_cast<int>(iterations) - 1; i >= 0; --i){
				delete vec[i]; // Deallocate each Vector2D object
			}
			delete[] vec;
			_deallocateTimer.pause();
		}
		_timer.stop();
		//_allocateTimer.print();
		//_deallocateTimer.print();
		exporting::BenchmarkResult result = exporting::BenchmarkResult{
			.totalTime = _timer.print(),
			.allocateTime = _allocateTimer.print(),
			.deallocateTime = _deallocateTimer.print(),
			.iterations = iterations,
			.label = "IBM Bursts Stack Allocator Object + Cont: Point",
			.description = "5000x (1000x alloc + 1000x free)"
		};
		std::vector<exporting::BenchmarkResult> results = { result };
		exporting::exportResultsToFile(results, "benchmark_results.txt");
	}


#pragma region Benchmarking Stack Allocators
	std::vector<BenchmarkMetadata> stackBenchmarks = {
	{
		.label = "IBM Bursts Stack Allocator Object + Cont: Point Vector",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Stack(); }
	},
	{
		.label = "IBM Bursts Stack Allocator Object + Cont: Point",
		.description = "5000x (1000x alloc + 1000x free)",
		.testFlags = allocatorTestFlags::NONE,
		.run = []() { benchmark_IBMBursts_Stack_Static_V2D_ContPointer(); }
	}
	};
#pragma endregion

	void benchmark_IBMBursts_linearAllocators() {

		// Run all requested benchmarks
		for (const auto& benchmark : linearBenchmarks) {
			log::print("======================");
			log::print("Running Linear Allocator Benchmark: {}", benchmark.label);
			log::print("======================");
			benchmark.run();
		}
	}

	void benchmark_IBMBursts_stackAllocators() {

		// Run all requested benchmarks
		for (const auto& benchmark : stackBenchmarks) {
			log::print("======================");
			log::print("Running Stack Allocator Benchmark: {}", benchmark.label);
			log::print("======================");
			benchmark.run();


		}
	}

}