int _reconfig_CONFIG_PROC_PAGE_MONITOR;


#include <errno.h>
#include <stdlib.h>

// defined(CONFIG_PROC_PAGE_MONITOR)
long* pm_buffer_V0;

// defined(CONFIG_PROC_PAGE_MONITOR)
unsigned long pm_pos_V0;

// defined(CONFIG_PROC_PAGE_MONITOR)
unsigned long pm_len_V0;

// defined(CONFIG_PROC_PAGE_MONITOR)
static int add_to_pagemap_V0 ()
{
    pm_buffer_V0 [ pm_pos_V0 ++ ] = 0;
    if (pm_pos_V0 >= pm_len_V0)
         return 1;
    return 0;
}

// defined(CONFIG_PROC_PAGE_MONITOR)
static int pagemap_pte_range_V0 ()
{
    int err;
    err = 0;
    while (1)
    {
        err = add_to_pagemap_V0 ();
        if (err)
         return err;
    }
    return err;
}

// defined(CONFIG_PROC_PAGE_MONITOR)
int walk_page_range_V0 ()
{
    return pagemap_pte_range_V0 ();
}

// defined(CONFIG_PROC_PAGE_MONITOR)
static int pagemap_read_V0 ()
{
    pm_len_V0 = sizeof (long) * 10;
    pm_buffer_V0 = malloc (pm_len_V0);
    if (!pm_buffer_V0)
         return - ENOMEM;
    pm_pos_V0 = 0;
    walk_page_range_V0 ();
    return 0;
}

int main (int argc , char** argv)
{
    if ((_reconfig_CONFIG_PROC_PAGE_MONITOR))
         pagemap_read_V0 ();
    return 0;
}