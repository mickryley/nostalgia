#define OBJECT_BASIC_VECTOR2D                   nostalgia::objects::Vector2D
#define OBJECT_BASIC_VECTOR3D                   nostalgia::objects::Vector3D

// #define CONSTRUCTOR_PARAMETERS 1.0f, 2.0f, 3.0f


#define IBM_BURSTS_IMPLEMENTATION_DETAILS std::format("{}x ({}x alloc + {}x free) [{}]", passes, iterations, iterations, object_id_to_string(static_cast<ObjectID>(object_id_index))).c_str()