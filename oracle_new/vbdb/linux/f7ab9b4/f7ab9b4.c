int _reconfig_CONFIG_DRM_I915;
int _reconfig_CONFIG_SHMEM;
int _reconfig_CONFIG_TMPFS;

typedef int filler_t (int);

int some_fun (int n)
{
    return n;
}

// !defined(CONFIG_TMPFS)
filler_t* f_V0 = ((void*) 0);

// defined(CONFIG_TMPFS)
filler_t* f_V1 = & some_fun;

// defined(CONFIG_SHMEM) || defined(CONFIG_TMPFS)
filler_t* shmem_get_inode_V2 ()
{
    return (
            (!_reconfig_CONFIG_TMPFS)
            ? f_V0
            : f_V1);
}

// !defined(CONFIG_TMPFS) && !defined(CONFIG_SHMEM)
filler_t* ramfs_get_inode_V3 ()
{
    return & some_fun;
}

filler_t* shmem_file_setup ()
{
    return (
            (!_reconfig_CONFIG_TMPFS && !_reconfig_CONFIG_SHMEM)
            ? ramfs_get_inode_V3 ()
            : shmem_get_inode_V2 ());
}

// defined(CONFIG_DRM_I915)
int drm_gem_object_init_V4 (filler_t** readpage)
{
    * readpage = shmem_file_setup ();
    return 0;
}

// defined(CONFIG_DRM_I915)
void i915_gem_alloc_object_V4 (filler_t** readpage)
{
    if (drm_gem_object_init_V4 (readpage) != 0)
        ;
}

// defined(CONFIG_DRM_I915)
void do_read_cache_page_V4 (filler_t* filler)
{
    filler (0);
}

// defined(CONFIG_DRM_I915)
void read_cache_page_gfp_V4 (filler_t** readpage)
{
    filler_t* filler;
    filler = * readpage;
    do_read_cache_page_V4 (filler);
}

// defined(CONFIG_DRM_I915)
int i915_gem_object_get_pages_gtt_V4 (filler_t** readpage)
{
    read_cache_page_gfp_V4 (readpage);
    return 0;
}

// defined(CONFIG_DRM_I915)
int i915_gem_object_bind_to_gtt_V4 (filler_t** readpage)
{
    return i915_gem_object_get_pages_gtt_V4 (readpage);
}

// defined(CONFIG_DRM_I915)
int i915_gem_object_pin_V4 (filler_t** readpage)
{
    return i915_gem_object_bind_to_gtt_V4 (readpage);
}

// defined(CONFIG_DRM_I915)
int intel_init_ring_buffer_V4 (filler_t** readpage)
{
    i915_gem_alloc_object_V4 (readpage);
    return i915_gem_object_pin_V4 (readpage);
}

// defined(CONFIG_DRM_I915)
int intel_init_render_ring_buffer_V4 (filler_t** readpage)
{
    return intel_init_ring_buffer_V4 (readpage);
}

// defined(CONFIG_DRM_I915)
int i915_gem_init_ringbuffer_V4 (filler_t** readpage)
{
    return intel_init_render_ring_buffer_V4 (readpage);
}

// defined(CONFIG_DRM_I915)
int i915_load_modeset_init_V4 (filler_t** readpage)
{
    return i915_gem_init_ringbuffer_V4 (readpage);
}

// defined(CONFIG_DRM_I915)
int i915_driver_load_V4 ()
{
    filler_t* readpage;
    return i915_load_modeset_init_V4 (& readpage);
}

int main ()
{
    if ((_reconfig_CONFIG_DRM_I915))
         i915_driver_load_V4 ();
    return 0;
}