#include "_shared_defines.inl"

#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR2D	    nostalgia::linear::objects::Vector2D_LocalOverride_StaticAccess
#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR3D	    nostalgia::linear::objects::Vector3D_LocalOverride_StaticAccess

#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR2D		nostalgia::linear::objects::Vector2D_LocalOverride_SingletonAccess
#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR3D		nostalgia::linear::objects::Vector3D_LocalOverride_SingletonAccess

#define ALLOCATOR_TEMPLATE_GLOBAL_ACCESS					nostalgia::linear::LinearAllocatorTemplate
#define ALLOCATOR_TEMPLATE_SINGLETON_ACCESS					nostalgia::linear::LinearAllocatorTemplateSingletonAccess
#define ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS	        nostalgia::linear::LinearAllocatorTemplateCachedSingletonAccess  

#define ALLOCATOR_STL_TEMPLATE_GLOBAL_ACCESS		        nostalgia::linear::LinearAllocatorSTLTemplate

#define ALLOCATOR_SINGLETON_ACCESS							nostalgia::linear::SingletonLinearAllocator::get_instance()
#define ALLOCATOR_GLOBAL_ACCESS								nostalgia::linear::g_linear_allocator