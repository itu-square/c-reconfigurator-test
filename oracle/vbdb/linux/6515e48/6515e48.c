int _reconfig_CONFIG_MACH_OMAP_H4;
int _reconfig_CONFIG_ARCH_OMAP2420;


// defined(CONFIG_ARCH_OMAP2420)
void omap2_set_globals_242x_V0 (void)
{
    return;
}

// defined(CONFIG_MACH_OMAP_H4)
static void omap_h4_map_io_V1 (void)
{
    (
     (_reconfig_CONFIG_ARCH_OMAP2420)
     ? omap2_set_globals_242x_V0 ()
     : omap2_set_globals_242x ());
}

int main (int argc , char** argv)
{
    if ((_reconfig_CONFIG_MACH_OMAP_H4))
         omap_h4_map_io_V1 ();
}