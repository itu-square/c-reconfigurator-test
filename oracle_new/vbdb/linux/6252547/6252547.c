int _reconfig_CONFIG_OF_IRQ;
int _reconfig_CONFIG_IRQ_DOMAIN;
int _reconfig_CONFIG_TWL4030_CORE;


// defined(CONFIG_IRQ_DOMAIN) || defined(CONFIG_TWL4030_CORE)
int irq_domain_simple_ops_V0 = 1;

// defined(CONFIG_IRQ_DOMAIN) || defined(CONFIG_TWL4030_CORE)
void irq_domain_add_V0 (int* ops)
{
    int irq;
    irq = * ops;
}

// defined(CONFIG_TWL4030_CORE)
int twl_probe_V1 ()
{
    int* ops;
    ops = (void*) 0;
    if ((_reconfig_CONFIG_OF_IRQ))
         ops = & irq_domain_simple_ops_V0;
    irq_domain_add_V0 (ops);
}

int main ()
{
    if ((_reconfig_CONFIG_TWL4030_CORE))
         twl_probe_V1 ();
    return 0;
}