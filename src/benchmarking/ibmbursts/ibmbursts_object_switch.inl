
// Define the object types and their parameters
void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {
	log::print("Running IBM Bursts benchmark with allocator: {} and parameters: iterations={}, passes={}, object_id_index={}",
		allocator.label, iterations, passes, object_id_index);

	// ObjectID index is used to select the object type for the benchmark
	switch(static_cast<nostalgia::ObjectID>(object_id_index)) {
		case nostalgia::ObjectID::Vector2D:
#define PARAMETERISED_OBJECT_BASIC OBJECT_BASIC_VECTOR2D
#define PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR2D
#define PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR2D
#define CONSTRUCTOR_PARAMETERS 1.0f, 2.0f

#include "ibmbursts_object_pass.inl"
#include "benchmarking/defines/_clear_object_switch.inl"
			break;
		case nostalgia::ObjectID::Vector3D:
#define PARAMETERISED_OBJECT_BASIC OBJECT_BASIC_VECTOR3D
#define PARAMETERISED_OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR3D
#define PARAMETERISED_OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR3D
#define CONSTRUCTOR_PARAMETERS 1.0f, 2.0f, 3.0f

#include "ibmbursts_object_pass.inl"
#include "benchmarking/defines/_clear_object_switch.inl"
			break;
		default:
		// Unused, but needed to avoid compilation errors
#define CONSTRUCTOR_PARAMETERS 
			log::print(LogFlags::Error, "Invalid ObjectID index: {}", object_id_index);
			return;
	}
}
