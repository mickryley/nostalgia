#include "_shared_defines.inl"
// Defines for [Stack_Lifo] Allocator
#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR2D		nostalgia::stack::objects::Vector2D_LocalOverride_StaticAccess
#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR3D		nostalgia::stack::objects::Vector3D_LocalOverride_StaticAccess

#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR2D		nostalgia::stack::objects::Vector2D_LocalOverride_SingletonAccess
#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR3D		nostalgia::stack::objects::Vector3D_LocalOverride_SingletonAccess

#define ALLOCATOR_TEMPLATE_GLOBAL_ACCESS					nostalgia::stack::StackAllocatorTemplate
#define ALLOCATOR_TEMPLATE_SINGLETON_ACCESS					nostalgia::stack::StackAllocatorTemplateSingletonAccess
#define ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS			nostalgia::stack::StackAllocatorTemplateCachedSingletonAccess

#define ALLOCATOR_SINGLETON_ACCESS							nostalgia::stack::SingletonStackAllocator::get_instance()
#define ALLOCATOR_GLOBAL_ACCESS								nostalgia::stack::g_stack_allocator