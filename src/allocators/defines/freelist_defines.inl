#include "_shared_defines.inl"

#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR2D		nostalgia::freelist::objects::Vector2D_LocalOverride_StaticAccess
#define OBJECT_LOCAL_OVERRIDE_GLOBAL_ACCESS_VECTOR3D		nostalgia::freelist::objects::Vector3D_LocalOverride_StaticAccess

#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR2D		nostalgia::freelist::objects::Vector2D_LocalOverride_SingletonAccess
#define OBJECT_LOCAL_OVERRIDE_SINGLETON_ACCESS_VECTOR3D		nostalgia::freelist::objects::Vector3D_LocalOverride_SingletonAccess

#define ALLOCATOR_TEMPLATE_GLOBAL_ACCESS					nostalgia::freelist::FreeAllocatorTemplate
#define ALLOCATOR_TEMPLATE_SINGLETON_ACCESS					nostalgia::freelist::FreeAllocatorTemplateSingletonAccess
#define ALLOCATOR_TEMPLATE_CACHED_SINGLETON_ACCESS			nostalgia::freelist::FreeAllocatorTemplateCachedSingletonAccess

#define ALLOCATOR_SINGLETON_ACCESS							nostalgia::freelist::SingletonFreeAllocator::get_instance()
#define ALLOCATOR_GLOBAL_ACCESS								nostalgia::freelist::g_free_allocator