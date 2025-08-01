#include "_shared_defines.inl"
// Defines for [Pool_Slab] Allocator
#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR2D		nostalgia::pool::objects::Vector2D_LocalOverride_StaticAccess
#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR3D		nostalgia::pool::objects::Vector3D_LocalOverride_StaticAccess

#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR2D		nostalgia::pool::objects::Vector2D_LocalOverride_SingletonAccess
#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR3D		nostalgia::pool::objects::Vector3D_LocalOverride_SingletonAccess

#define ALLOCATOR_TEMPLATE_GLOBAL_ACCESS					nostalgia::pool::PoolAllocatorTemplate
#define ALLOCATOR_TEMPLATE_SINGLETON_ACCESS					nostalgia::pool::PoolAllocatorTemplateSingletonAccess
#define ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS			nostalgia::pool::PoolAllocatorTemplateCachedSingletonAccess

#define ALLOCATOR_STL_TEMPLATE_GLOBAL_ACCESS		        nostalgia::linear::LinearAllocatorSTLTemplate

#define ALLOCATOR_SINGLETON_ACCESS							nostalgia::pool::SingletonPoolAllocator::get_instance()
#define ALLOCATOR_GLOBAL_ACCESS								nostalgia::pool::g_pool_allocator