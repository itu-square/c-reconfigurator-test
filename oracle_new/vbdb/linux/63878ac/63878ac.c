int _reconfig_CONFIG_ARCH_OMAP3;
int _reconfig_CONFIG_PM;

extern void __assert_fail (const char* __assertion , const char* __file , unsigned int __line , const char* __function) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));

__attribute__ ((noinline)) int nondet ()
{
    return 42;
}

// defined(CONFIG_ARCH_OMAP3)
static int* _omap3_sram_configure_core_dpll_V0;

// defined(CONFIG_ARCH_OMAP3)
int omap3_configure_core_dpll_V0 ()
{
    (
     (_omap3_sram_configure_core_dpll_V0)
     ? (void) (0)
     : __assert_fail ("_omap3_sram_configure_core_dpll" , "63878ac.c" , 16 , __PRETTY_FUNCTION__));
    return * _omap3_sram_configure_core_dpll_V0;
}

// defined(CONFIG_ARCH_OMAP3) && defined(CONFIG_PM)
static int some_int_V1 = 1;

// defined(CONFIG_ARCH_OMAP3) && defined(CONFIG_PM)
void omap3_sram_restore_context_V1 (void)
{
    _omap3_sram_configure_core_dpll_V0 = & some_int_V1;
}

// defined(CONFIG_ARCH_OMAP3)
int omap3_core_dpll_m2_set_rate_V0 ()
{
    omap3_configure_core_dpll_V0 ();
    return 0;
}

// defined(CONFIG_ARCH_OMAP3)
int _omap2_init_reprogram_sdrc_V0 (void)
{
    int v;
    v = omap3_core_dpll_m2_set_rate_V0 ();
    return v;
}

int omap34xx_sram_init (void)
{
    if ((_reconfig_CONFIG_ARCH_OMAP3 && _reconfig_CONFIG_PM))
         omap3_sram_restore_context_V1 ();
    return 0;
}

int omap_sram_init (void)
{
    if (nondet ())
         omap34xx_sram_init ();
    return 0;
}

void omap_sdrc_init ()
{
    omap_sram_init ();
    if (nondet ())
    {
        if ((_reconfig_CONFIG_ARCH_OMAP3))
         _omap2_init_reprogram_sdrc_V0 ();
    }
}

void omap3pandora_init (void)
{
    omap_sdrc_init ();
}

int main (void)
{
    omap3pandora_init ();
    return 0;
}