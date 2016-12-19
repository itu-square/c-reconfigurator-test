#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

#include <stdio.h>
typedef unsigned long cpumask_t;

cpumask_t cpu_none_mask = 0;

cpumask_t cpu_online_mask = 0;

// defined(CONFIG_NUMA)
int nr_node_ids_V0;

// defined(CONFIG_NUMA)
cpumask_t* node_to_cpumask_map_V0 [ (1 << 8) ];

// defined(CONFIG_NUMA)
void setup_node_to_cpumask_map_V0 (void)
{
    unsigned int node;
    for ((node = 0); node < nr_node_ids_V0; (node ++))
         ((node_to_cpumask_map_V0 [ node ] = (& cpu_online_mask)));
}

// !defined(CONFIG_NUMA)
static inline const cpumask_t* cpumask_of_node_V1 (int node)
{
    return (& cpu_online_mask);
}

// defined(CONFIG_NUMA) && defined(CONFIG_DEBUG_PER_CPU_MAPS)
const cpumask_t* cpumask_of_node_V2 (int node)
{
    if (node >= nr_node_ids_V0)
    {
        fprintf (stderr , "cpumask_of_node(%d): node > nr_node_ids(%d)\n" , node , nr_node_ids_V0);
        return cpu_none_mask;
    }
    if (((node_to_cpumask_map_V0 [ node ])) == NULL)
    {
        fprintf (stderr , "cpumask_of_node(%d): no node_to_cpumask_map!\n" , node);
        return cpu_online_mask;
    }
    return ((node_to_cpumask_map_V0 [ node ]));
}

// !defined(CONFIG_NUMA)
static inline void setup_node_to_cpumask_map_V1 (void)
{
}

// defined(CONFIG_NUMA) && !defined(CONFIG_DEBUG_PER_CPU_MAPS)
static inline const cpumask_t* cpumask_of_node_V3 (int node)
{
    return ((node_to_cpumask_map_V0 [ node ]));
}

// !defined(CONFIG_X86_32) && defined(CONFIG_NUMA) && defined(CONFIG_PCI) && defined(CONFIG_X86_64)
static int mp_bus_to_node_V4 [ 256 ] =
{
    [ 0 ... 256 - 1 ] = - 1}
;

// defined(CONFIG_NUMA) && defined(CONFIG_PCI) && !defined(CONFIG_X86_64) || defined(CONFIG_X86_32) && defined(CONFIG_NUMA) && defined(CONFIG_PCI)
static unsigned char mp_bus_to_node_V5 [ 256 ] =
{
    [ 0 ... 256 - 1 ] = - 1}
;

// defined(CONFIG_NUMA) && defined(CONFIG_PCI) && !defined(CONFIG_X86_64) || defined(CONFIG_X86_32) && defined(CONFIG_NUMA) && defined(CONFIG_PCI)
int get_mp_bus_to_node_V5 (int busnum)
{
    int node;
    if (busnum < 0 || busnum > (256 - 1))
         return 0;
    (node = ((mp_bus_to_node_V5 [ busnum ])));
    return node;
}

// !defined(CONFIG_X86_32) && defined(CONFIG_NUMA) && defined(CONFIG_PCI) && defined(CONFIG_X86_64)
int get_mp_bus_to_node_V4 (int busnum)
{
    int node;
    (node = - 1);
    if (busnum < 0 || busnum > (256 - 1))
         return node;
    (node = ((mp_bus_to_node_V4 [ busnum ])));
    return node;
}

// !defined(CONFIG_NUMA) && defined(CONFIG_PCI)
static inline int get_mp_bus_to_node_V6 (int busnum)
{
    return 0;
}

// defined(CONFIG_NUMA) && defined(CONFIG_PCI)
static const cpumask_t* cpumask_of_pcibus_V7 (int node)
{
    return
           (node == - 1)
           ? (& cpu_online_mask)
           : (
              (_reconfig_CONFIG_NUMA && _reconfig_CONFIG_DEBUG_PER_CPU_MAPS)
              ? cpumask_of_node_V2 (node)
              : cpumask_of_node_V3 (node));
}

// defined(CONFIG_PCI)
static int local_cpus_show_V8 (int node)
{
    const cpumask_t* mask;
    int len;
    (len = 1);
    if ((_reconfig_CONFIG_NUMA))
         (mask = cpumask_of_pcibus_V7 (node));
    else
         (mask = cpumask_of_node_V1 (node));
    printf ("mask: %ld\n" , (* mask));
    return len;
}

// defined(CONFIG_PCI)
static int dev_attr_show_V8 (int node)
{
    int ret;
    (ret = local_cpus_show_V8 (node));
    return ret;
}

// defined(CONFIG_PCI)
int pcibios_scan_root_V8 ()
{
    return (
            (_reconfig_CONFIG_NUMA && _reconfig_CONFIG_PCI && (!_reconfig_CONFIG_X86_64) || _reconfig_CONFIG_X86_32 && _reconfig_CONFIG_NUMA && _reconfig_CONFIG_PCI)
            ? get_mp_bus_to_node_V5 (0)
            : (
               ((!_reconfig_CONFIG_X86_32) && _reconfig_CONFIG_NUMA && _reconfig_CONFIG_PCI && _reconfig_CONFIG_X86_64)
               ? get_mp_bus_to_node_V4 (0)
               : get_mp_bus_to_node_V6 (0)));
}

int main (int argc , char** argv)
{
    if ((_reconfig_CONFIG_NUMA))
         ((nr_node_ids_V0 = rand () % ((1 << 8) - 1)));
    (
     (_reconfig_CONFIG_NUMA)
     ? setup_node_to_cpumask_map_V0 ()
     : setup_node_to_cpumask_map_V1 ());
// defined(CONFIG_PCI)
    int node_V8;
    if ((_reconfig_CONFIG_PCI))
    {
        ((node_V8 = pcibios_scan_root_V8 ()));
        dev_attr_show_V8 (node_V8);
    }
    return 0;
}