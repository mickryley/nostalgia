#include "benchmarking_ibmbursts.h"

#include "allocators/linear_bump/allocator_linear.h"
#include "allocators/stack_lifo/allocator_stack.h"
#include "allocators/pool_slab/allocator_pool.h"

#include "objects/objects_freelist.h"
#include "objects/objects_pool.h"
#include "objects/objects_stack.h"
#include "objects/objects_linear.h"
#include "objects/objects.h"

#include "allocators/info/allocator_meta.h"
#include "benchmarking/info/benchmark_meta.h"

#include "benchmarking/exporter/benchmarking_exporting.h"

#include "implementations/implementation_meta.h"

#include "timer.h"
#include "log.h"

#include <array>
#include <memory_resource>


namespace nostalgia::benchmarking::IBMBursts {

#pragma region Benchmarking Linear Allocators

	void benchmark_IBMBursts_Static_V2D_CustomPointerVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D Cont Pointer");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D Cont Pointer");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			//auto** vec = new linear::objects::Vector2D_LocalOverride_StaticAccess * [iterations];
			linear::PointerLinearVector_ObjectOverride<linear::objects::Vector2D_LocalOverride_StaticAccess> vec(iterations);

			for (size_t j = 0; j < iterations; j++) {
				//vec[j] = new linear::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
				vec.emplace_back(1.0f, 2.0f);
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			//delete[] vec;
			vec.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts_STL_Template_V2D_ContPointVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorStlTemplate<V2D> _allocator;

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			std::vector<V2D*> vecs;
			vecs.reserve(iterations);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vecs.push_back(_allocator.create(1.0f, 2.0f)); // Full allocation via allocator
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			_allocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts_STL_Template_V2D_ContVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;

			vecs.reserve(iterations);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vecs.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			vecs.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts_STL_Template_V2D_CustomVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorStlTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			//std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;
			linear::LinearVector_StlTemplateAllocator<V2D> vec(iterations * sizeof(V2D), _allocator);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			vec.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts_PMR_Monotonic_PmrVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		//linear::LinearAllocatorStlTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
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
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			vec.clear(); // Clear the vector
			//linear::s_linearAllocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts_PMR_Monotonic_HardCoded_PmrVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;

		constexpr size_t buffer_size = sizeof(V2D) * 1000;
		static std::array<std::byte, buffer_size> buffer; // Hardcoded buffer size for PMR vector
		std::pmr::monotonic_buffer_resource resource(buffer.data(), buffer.size());

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			//std::pmr::vector<V2D> vec(&resource); //MSVC Only
			//std::pmr::vector<V2D> vec(std::pmr::polymorphic_allocator<V2D>{&resource});
			std::vector<V2D, std::pmr::polymorphic_allocator<V2D>> vec(std::pmr::polymorphic_allocator<V2D>{&resource});


			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new

				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			vec.clear(); // Clear the vector
			resource.release();
			//linear::s_linearAllocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts_PMR_Unsync_PmrVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		//linear::LinearAllocatorStlTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
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
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			vec.clear(); // Clear the vector
			//linear::s_linearAllocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts_Template_V2D_CustomVector(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D");

		using V2D = objects::Vector2D;
		linear::LinearAllocatorTemplate<V2D> _allocator;


		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			//std::vector<V2D, linear::LinearAllocatorStlTemplate<V2D>> vecs;
			linear::LinearVector_TemplateAllocator<V2D> vec(iterations * sizeof(V2D), _allocator);

			for (size_t j = 0; j < iterations; j++) {
				// Allocate a Vector2D object
				//new (&vecs[j]) V2D(1.0f, 2.0f); // Placement new
				vec.emplace_back(1.0f, 2.0f); // Full allocation via allocator
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			vec.clear(); // Clear the vector
			linear::s_linearAllocator.rewind();
			deallocate_timer.pause();
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();
	}

	void benchmark_IBMBursts(int iterations = 1000, int passes = 5000, int size = 8) {

		constexpr size_t buffer_size = 1024 * 1024; // 1 MB
		char* buffer = new char[buffer_size]; // Mallocate a buffer of 1 MB
		nostalgia::linear::LinearAllocator _allocator(buffer, buffer_size);



		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer");
		timer::Timer overall_timer = timer::Timer("Total Timer");

		_timer.start();
		for (size_t i = 0; i < passes; i++)
		{
			allocate_timer.start();
			for (size_t j = 0; j < iterations; j++) {
				_allocator.allocate(size); // Allocate 16 bytes
			}
			allocate_timer.pause();
			deallocate_timer.start();

			_allocator.rewind(); // Rewind the allocator after each pass

			deallocate_timer.pause();
			/*for (size_t j = 0; j < iterations; j++) {
				_allocator.deallocate(); // Deallocate the last allocated block
			}*/
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();


	}

	std::vector<ImplementationDetails> linearBenchmarks = {
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",
		//.label = "IBM Bursts Malloc",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_None_V2D(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts (Manual Alloc only)",
		//.description = "5000x (1000x alloc + 1000x free)",
		//////.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Static Object",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Singleton Object",
		////.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Singleton_V2D(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Cached Singleton Object",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_CachedSingleton_V2D(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Static Object + Cont: Point Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D_ContPointVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//		.label = "IBM Bursts Singleton Object + Cont: Point",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D_ContPointer(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Singleton Object + Cont: Point Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Singleton_V2D_ContPointVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Singleton Object + Cont: Point",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Singleton_V2D_ContPointer(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Templated Allocator Object + Cont: Point (In Place)",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_ContPoint_Inplace(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Templated Allocator Object + Cont: Point",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_ContPoint(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Templated Allocator Object + Cont: Point Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_ContPointVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts STL Templated Allocator Object + Cont: Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_STL_Template_V2D_ContVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts STL Templated Allocator Object + Cont: Point Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_STL_Template_V2D_ContPointVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts STL Templated Allocator Object + Cont: Custom Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_STL_Template_V2D_CustomVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Templated Allocator Object + Cont: Custom Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Template_V2D_CustomVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Static Allocator Object + Cont: Custom Pointer Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_Static_V2D_CustomPointerVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts PMR Default unsync + Cont: PMR Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_PMR_Unsync_PmrVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts PMR Default monotonic_buffer_resource + Cont: PMR Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::ImplementationID::NONE,
		.run = []() { benchmark_IBMBursts_PMR_Monotonic_PmrVector(); }
	},
	{
		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Linear,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts PMR Default hardcoded monotonic_buffer_resource + Cont: PMR Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::NONE,
		.run = []() { benchmark_IBMBursts_PMR_Monotonic_HardCoded_PmrVector(); }
	}




	};

#pragma endregion

#pragma region Benchmarking Stack Allocators

	void benchmark_IBMBursts_Stack(int iterations = 1000, int passes = 5000, int size = 8) {

		constexpr size_t buffer_size = 1024 * 1024; // 1 MB
		char* buffer = new char[buffer_size]; // Mallocate a buffer of 1 MB
		nostalgia::stack::StackAllocator _allocator(buffer, buffer_size);

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer");
		timer::Timer overall_timer = timer::Timer("Total Timer");

		_timer.start();
		for (size_t i = 0; i < passes; i++)
		{
			allocate_timer.start();
			for (size_t j = 0; j < iterations; j++) {
				_allocator.allocate(size); // Allocate 16 bytes
			}
			allocate_timer.pause();

			deallocate_timer.start();

			_allocator.rewind(); // Rewind the allocator after each pass
			
			deallocate_timer.pause();
			/*for (size_t j = 0; j < iterations; j++) {
				_allocator.deallocate(); // Deallocate the last allocated block
			}*/
		}
		_timer.stop();
		allocate_timer.print();
		deallocate_timer.print();

	}

	void benchmark_IBMBursts_Stack_Static_V2D_ContPointer(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D Cont Pointer");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D Cont Pointer");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			auto** vec = new stack::objects::Vector2D_LocalOverride_StaticAccess * [iterations];

			for (size_t j = 0; j < iterations; j++) {
				vec[j] = new stack::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			for (int i = static_cast<int>(iterations) - 1; i >= 0; --i){
				delete vec[i]; // Deallocate each Vector2D object
			}
			delete[] vec;
			deallocate_timer.pause();
		}
		_timer.stop();
		//allocate_timer.print();
		//deallocate_timer.print();
		exporting::BenchmarkResult result = exporting::BenchmarkResult{
			.total_time = overall_timer.print(),
			.allocate_time = allocate_timer.print(),
			.deallocate_time = deallocate_timer.print(),
			.iterations = iterations,
			//.label = "IBM Bursts Stack Allocator Object + Cont: Point",
			//.description = "5000x (1000x alloc + 1000x free)"
		};
		std::vector<exporting::BenchmarkResult> results = { result };
		exporting::exportResultsToFile(results, "benchmark_results.txt");
	}


	std::vector<ImplementationDetails> stackBenchmarks = {
	{

		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Stack,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Stack Allocator Object + Cont: Point Vector",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::NONE,
		.run = []() { benchmark_IBMBursts_Stack(); }
	},
	{

		.id = ImplementationID::NONE,
		.allocator = AllocatorID::Stack,
		.parameters = "5000x (1000x alloc + 1000x free)",

		//.label = "IBM Bursts Stack Allocator Object + Cont: Point",
		//.description = "5000x (1000x alloc + 1000x free)",
		//.compatible_flags = AllocatorFlags::NONE,
		.run = []() { benchmark_IBMBursts_Stack_Static_V2D_ContPointer(); }
	}
	};
#pragma endregion

#pragma region Benchmarking Pool Allocators


	void benchmark_IBMBursts_Pool_Static_V2D_ContPointer(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Static V2D Cont Pointer");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Static V2D Cont Pointer");
		timer::Timer overall_timer = timer::Timer("Total Timer Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			auto** vec = new pool::objects::Vector2D_LocalOverride_StaticAccess * [iterations];

			for (size_t j = 0; j < iterations; j++) {
				vec[j] = new pool::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			for (int i = static_cast<int>(iterations) - 1; i >= 0; --i) {
				delete vec[i]; // Deallocate each Vector2D object
			}
			delete[] vec;
			deallocate_timer.pause();
		}
		_timer.stop();
		//allocate_timer.print();
		//deallocate_timer.print();
		exporting::BenchmarkResult result = exporting::BenchmarkResult{
			.total_time = overall_timer.print(),
			.allocate_time = allocate_timer.print(),
			.deallocate_time = deallocate_timer.print(),
			.iterations = iterations,
			//.label = "IBM Bursts Pool Allocator Static Object + Cont: Point",
			//.description = "5000x (1000x alloc + 1000x free)"
		};
		std::vector<exporting::BenchmarkResult> results = { result };
		exporting::exportResultsToFile(results, "benchmark_results.txt");
	}

	std::vector<ImplementationDetails> poolBenchmarks = {
		{

			.id = ImplementationID::NONE,
			.allocator = AllocatorID::Pool,
			.parameters = "5000x (1000x alloc + 1000x free)",

			//.label = "IBM Bursts Pool Allocator Static Object + Cont: Point Vector",
			//.description = "5000x (1000x alloc + 1000x free)",
			//.compatible_flags = AllocatorFlags::NONE,
			.run = []() { benchmark_IBMBursts_Pool_Static_V2D_ContPointer(); }
		}/*,
		{
			.label = "IBM Bursts Pool Allocator Object + Cont: Point",
			.description = "5000x (1000x alloc + 1000x free)",
			.compatible_flags = AllocatorFlags::NONE,
			.run = []() { benchmark_IBMBursts_Stack_Static_V2D_ContPointer(); }
		}*/
	};
#pragma endregion

#pragma region Benchmarking Free List Allocators
	void benchmark_IBMBursts_Freelist_Static_V2D_ContPointer(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Freelist Static V2D Cont Pointer");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Freelist Static V2D Cont Pointer");
		timer::Timer overall_timer = timer::Timer("Total Timer Freelist Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			auto** vec = new freelist::objects::Vector2D_LocalOverride_StaticAccess * [iterations];

			for (size_t j = 0; j < iterations; j++) {
				vec[j] = new freelist::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			for (int k = static_cast<int>(iterations) - 1; k >= 0; --k) {
				delete vec[k]; // Deallocate each Vector2D object
			}
			delete[] vec;
			deallocate_timer.pause();
		}
		_timer.stop();
		//allocate_timer.print();
		//deallocate_timer.print();
		exporting::BenchmarkResult result = exporting::BenchmarkResult{
			.total_time = overall_timer.print(),
			.allocate_time = allocate_timer.print(),
			.deallocate_time = deallocate_timer.print(),
			.iterations = iterations,
			//.label = "IBM Bursts Freelist Allocator Static Object + Cont: Point",
			//.description = "5000x (1000x alloc + 1000x free)"
		};
		std::vector<exporting::BenchmarkResult> results = { result };
		exporting::exportResultsToFile(results, "benchmark_results.txt");
	}

	void benchmark_IBMBursts_Freelist_Static_V2D_ContPointer_ForwardDealloc(int iterations = 1000, int passes = 5000, int size = 16) {

		// Initialise Main Timers
		timer::Timer allocate_timer = timer::Timer("Allocate Timer Freelist Static V2D Cont Pointer");
		timer::Timer deallocate_timer = timer::Timer("Deallocate Timer Freelist Static V2D Cont Pointer");
		timer::Timer overall_timer = timer::Timer("Total Timer Freelist Static V2D Cont Pointer");

		_timer.start();
		// Alloc + Dealloc Passes
		for (size_t i = 0; i < passes; i++)
		{
			// Allocation Iterations
			allocate_timer.start();
			auto** vec = new freelist::objects::Vector2D_LocalOverride_StaticAccess * [iterations];

			for (size_t j = 0; j < iterations; j++) {
				vec[j] = new freelist::objects::Vector2D_LocalOverride_StaticAccess(1.0f, 2.0f);
			}
			allocate_timer.pause();

			// Deallocation Iterations
			deallocate_timer.start();
			for (size_t k = 0; k < iterations; k++) {
				delete vec[k]; // Deallocate each Vector2D object
			}
			delete[] vec;
			deallocate_timer.pause();
		}
		_timer.stop();
		//allocate_timer.print();
		//deallocate_timer.print();
		exporting::BenchmarkResult result = exporting::BenchmarkResult{
			.total_time = overall_timer.print(),
			.allocate_time = allocate_timer.print(),
			.deallocate_time = deallocate_timer.print(),
			.iterations = iterations,
			//.label = "IBM Bursts Freelist Allocator Static Object + Cont: Point",
			//.description = "5000x (1000x alloc + 1000x free)"
		};
		std::vector<exporting::BenchmarkResult> results = { result };
		exporting::exportResultsToFile(results, "benchmark_results.txt");
	}

	std::vector<ImplementationDetails> freelistBenchmarks = {
		{
			.id = ImplementationID::NONE,
			.allocator = AllocatorID::Freelist,
			.parameters = "5000x (1000x alloc + 1000x free)",

			//.label = "IBM Bursts Freelist Allocator Static Object + Cont: Point Vector",
			//.description = "5000x (1000x alloc + 1000x free)",
			//.compatible_flags = AllocatorFlags::NONE,
			.run = []() { benchmark_IBMBursts_Freelist_Static_V2D_ContPointer(); }
		}/*,
		{
			.label = "IBM Bursts Freelist Allocator Static Object + Cont: Point Vector - Forward Dealloc",
			.description = "5000x (1000x alloc + 1000x free)",
			.compatible_flags = AllocatorFlags::NONE,
			.run = []() { benchmark_IBMBursts_Freelist_Static_V2D_ContPointer_ForwardDealloc(); }
		}*/
	};
#pragma endregion

	void benchmark_IBMBursts_linearAllocators() {

		// Run all requested benchmarks
		for (const auto& benchmark : linearBenchmarks) {
			log::print("======================");
			log::print("Running Linear Allocator Benchmark: {}", benchmark.label());
			log::print("======================");
			benchmark.run();
		}
	}

	void benchmark_IBMBursts_stackAllocators() {

		// Run all requested benchmarks
		for (const auto& benchmark : stackBenchmarks) {
			log::print("======================");
			log::print("Running Stack Allocator Benchmark: {}", benchmark.label());
			log::print("======================");
			benchmark.run();
		}
	}

	void benchmark_IBMBurstg_pool_allocators() {

		// Run all requested benchmarks
		for (const auto& benchmark : poolBenchmarks) {
			log::print("======================");
			log::print("Running Pool Allocator Benchmark: {}", benchmark.label());
			log::print("======================");
			benchmark.run();
		}
	}

	void benchmark_IBMBursts_freelistAllocators() {

		// Run all requested benchmarks
		for (const auto& benchmark : freelistBenchmarks) {
			log::print("======================");
			log::print("Running Freelist Allocator Benchmark: {}", benchmark.label());
			log::print("======================");
			benchmark.run();
		}
	}

}