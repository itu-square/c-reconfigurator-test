#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"
__attribute__ ((noinline)) int nondet ()
{
    return 42;
}

// defined(CONFIG_NETPOLL)
int netpoll_setup_V0 ()
{
    int err;
    if ((_reconfig_CONFIG_IPV6))
    {
        (err = - 1);
        if (nondet ()) goto put;
    }
    else goto put;
    put : return err;
}

int main (void)
{
    if ((_reconfig_CONFIG_NETPOLL))
         netpoll_setup_V0 ();
    return 0;
}