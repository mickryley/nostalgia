#include "_shared_defines.inl"

#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR2D	    nostalgia::objects::Vector2D
#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR3D	    nostalgia::objects::Vector3D

#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR2D		nostalgia::objects::Vector2D
#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR3D		nostalgia::objects::Vector3D

#define ALLOCATOR_TEMPLATE_GLOBAL_ACCESS					nostalgia::linear::LinearAllocatorTemplate
#define ALLOCATOR_TEMPLATE_SINGLETON_ACCESS					nostalgia::linear::LinearAllocatorTemplateSingletonAccess
#define ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS	        nostalgia::linear::LinearAllocatorTemplateCachedSingletonAccess

#define ALLOCATOR_SINGLETON_ACCESS							nostalgia::default_malloc::g_default_allocator
#define ALLOCATOR_GLOBAL_ACCESS								nostalgia::default_malloc::g_default_allocator