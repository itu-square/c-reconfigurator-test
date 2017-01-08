int _reconfig_CONFIG_PPC_64K_PAGES;
int _reconfig_CONFIG_FORCE_MAX_ZONEORDER;
int _reconfig_CONFIG_SLAB;
int _reconfig_CONFIG_PPC_16K_PAGES;
int _reconfig_CONFIG_PPC_256K_PAGES;
int _reconfig_CONFIG_SLOB;
int _reconfig_CONFIG_LOCKDEP;

// !defined(CONFIG_FORCE_MAX_ZONEORDER) && !defined(CONFIG_PPC_256K_PAGES) && !defined(CONFIG_PPC_64K_PAGES) && defined(CONFIG_PPC_16K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V0 [ (
                          (11 + 14 - 1) <= 25
                          ? (11 + 14 - 1)
                          : 25) + 1 ];

// !defined(CONFIG_FORCE_MAX_ZONEORDER) && !defined(CONFIG_PPC_256K_PAGES) && !defined(CONFIG_PPC_64K_PAGES) && !defined(CONFIG_PPC_16K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V1 [ (
                          (11 + 12 - 1) <= 25
                          ? (11 + 12 - 1)
                          : 25) + 1 ];

// !defined(CONFIG_FORCE_MAX_ZONEORDER) && !defined(CONFIG_PPC_256K_PAGES) && defined(CONFIG_PPC_64K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V2 [ (
                          (11 + 16 - 1) <= 25
                          ? (11 + 16 - 1)
                          : 25) + 1 ];

// !defined(CONFIG_FORCE_MAX_ZONEORDER) && defined(CONFIG_PPC_256K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V3 [ (
                          (11 + 18 - 1) <= 25
                          ? (11 + 18 - 1)
                          : 25) + 1 ];

// defined(CONFIG_FORCE_MAX_ZONEORDER) && !defined(CONFIG_PPC_256K_PAGES) && !defined(CONFIG_PPC_64K_PAGES) && defined(CONFIG_PPC_16K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V4 [ (
                          (CONFIG_FORCE_MAX_ZONEORDER + 14 - 1) <= 25
                          ? (CONFIG_FORCE_MAX_ZONEORDER + 14 - 1)
                          : 25) + 1 ];

// defined(CONFIG_FORCE_MAX_ZONEORDER) && !defined(CONFIG_PPC_256K_PAGES) && !defined(CONFIG_PPC_64K_PAGES) && !defined(CONFIG_PPC_16K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V5 [ (
                          (CONFIG_FORCE_MAX_ZONEORDER + 12 - 1) <= 25
                          ? (CONFIG_FORCE_MAX_ZONEORDER + 12 - 1)
                          : 25) + 1 ];

// defined(CONFIG_FORCE_MAX_ZONEORDER) && !defined(CONFIG_PPC_256K_PAGES) && defined(CONFIG_PPC_64K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V6 [ (
                          (CONFIG_FORCE_MAX_ZONEORDER + 16 - 1) <= 25
                          ? (CONFIG_FORCE_MAX_ZONEORDER + 16 - 1)
                          : 25) + 1 ];

// defined(CONFIG_FORCE_MAX_ZONEORDER) && defined(CONFIG_PPC_256K_PAGES) && defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V7 [ (
                          (CONFIG_FORCE_MAX_ZONEORDER + 18 - 1) <= 25
                          ? (CONFIG_FORCE_MAX_ZONEORDER + 18 - 1)
                          : 25) + 1 ];

// !defined(CONFIG_PPC_256K_PAGES) && !defined(CONFIG_PPC_64K_PAGES) && defined(CONFIG_PPC_16K_PAGES) && !defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V8 [ (14 + 1) + 1 ];

// !defined(CONFIG_PPC_256K_PAGES) && !defined(CONFIG_PPC_64K_PAGES) && !defined(CONFIG_PPC_16K_PAGES) && !defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V9 [ (12 + 1) + 1 ];

// !defined(CONFIG_PPC_256K_PAGES) && defined(CONFIG_PPC_64K_PAGES) && !defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V10 [ (16 + 1) + 1 ];

// defined(CONFIG_PPC_256K_PAGES) && !defined(CONFIG_SLAB) && !defined(CONFIG_SLOB)
int* kmalloc_caches_V11 [ (18 + 1) + 1 ];

// defined(CONFIG_LOCKDEP)
static void init_node_lock_keys_V12 ()
{
    int i;
    for (i = 1; i < (
                     (_reconfig_CONFIG_PPC_256K_PAGES)
                     ? 18
                     : (
                        (_reconfig_CONFIG_PPC_64K_PAGES)
                        ? 16
                        : (
                           (_reconfig_CONFIG_PPC_16K_PAGES)
                           ? 14
                           : 12))) + (
                                      (_reconfig_CONFIG_FORCE_MAX_ZONEORDER)
                                      ? CONFIG_FORCE_MAX_ZONEORDER
                                      : 11); i ++)
    {
        int* cache;
        cache = (
                 (!_reconfig_CONFIG_FORCE_MAX_ZONEORDER && !_reconfig_CONFIG_PPC_256K_PAGES && !_reconfig_CONFIG_PPC_64K_PAGES && _reconfig_CONFIG_PPC_16K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                 ? kmalloc_caches_V0 [ i ]
                 : (
                    (!_reconfig_CONFIG_FORCE_MAX_ZONEORDER && !_reconfig_CONFIG_PPC_256K_PAGES && !_reconfig_CONFIG_PPC_64K_PAGES && !_reconfig_CONFIG_PPC_16K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                    ? kmalloc_caches_V1 [ i ]
                    : (
                       (!_reconfig_CONFIG_FORCE_MAX_ZONEORDER && !_reconfig_CONFIG_PPC_256K_PAGES && _reconfig_CONFIG_PPC_64K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                       ? kmalloc_caches_V2 [ i ]
                       : (
                          (!_reconfig_CONFIG_FORCE_MAX_ZONEORDER && _reconfig_CONFIG_PPC_256K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                          ? kmalloc_caches_V3 [ i ]
                          : (
                             (_reconfig_CONFIG_FORCE_MAX_ZONEORDER && !_reconfig_CONFIG_PPC_256K_PAGES && !_reconfig_CONFIG_PPC_64K_PAGES && _reconfig_CONFIG_PPC_16K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                             ? kmalloc_caches_V4 [ i ]
                             : (
                                (_reconfig_CONFIG_FORCE_MAX_ZONEORDER && !_reconfig_CONFIG_PPC_256K_PAGES && !_reconfig_CONFIG_PPC_64K_PAGES && !_reconfig_CONFIG_PPC_16K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                                ? kmalloc_caches_V5 [ i ]
                                : (
                                   (_reconfig_CONFIG_FORCE_MAX_ZONEORDER && !_reconfig_CONFIG_PPC_256K_PAGES && _reconfig_CONFIG_PPC_64K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                                   ? kmalloc_caches_V6 [ i ]
                                   : (
                                      (_reconfig_CONFIG_FORCE_MAX_ZONEORDER && _reconfig_CONFIG_PPC_256K_PAGES && _reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                                      ? kmalloc_caches_V7 [ i ]
                                      : (
                                         (!_reconfig_CONFIG_PPC_256K_PAGES && !_reconfig_CONFIG_PPC_64K_PAGES && _reconfig_CONFIG_PPC_16K_PAGES && !_reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                                         ? kmalloc_caches_V8 [ i ]
                                         : (
                                            (!_reconfig_CONFIG_PPC_256K_PAGES && !_reconfig_CONFIG_PPC_64K_PAGES && !_reconfig_CONFIG_PPC_16K_PAGES && !_reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                                            ? kmalloc_caches_V9 [ i ]
                                            : (
                                               (!_reconfig_CONFIG_PPC_256K_PAGES && _reconfig_CONFIG_PPC_64K_PAGES && !_reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                                               ? kmalloc_caches_V10 [ i ]
                                               : (
                                                  (_reconfig_CONFIG_PPC_256K_PAGES && !_reconfig_CONFIG_SLAB && !_reconfig_CONFIG_SLOB)
                                                  ? kmalloc_caches_V11 [ i ]
                                                  : kmalloc_caches [ i ]))))))))))));
        if (!cache) continue;
        int n;
        n = * cache;
    }
}

// !defined(CONFIG_LOCKDEP)
void init_node_lock_keys_V13 ()
{
}

// defined(CONFIG_LOCKDEP)
void init_lock_keys_V12 (void)
{
    init_node_lock_keys_V12 ();
}

// !defined(CONFIG_LOCKDEP)
void init_lock_keys_V13 (void)
{
}

void kmem_cache_init_late (void)
{
    (
     (_reconfig_CONFIG_LOCKDEP)
     ? init_lock_keys_V12 ()
     : init_lock_keys_V13 ());
}

int main (void)
{
    if ((!_reconfig_CONFIG_SLOB))
         kmem_cache_init_late ();
    return 0;
}