int _reconfig_CONFIG_VLAN_8021Q;
int _reconfig_CONFIG_RPS;

#include <assert.h>

__attribute__ ((noinline)) int nondet ()
{
    return 42;
}

// defined(CONFIG_VLAN_8021Q)
_Bool vlan_hwaccel_do_receive_V0 ()
{
    return 1;
}

// !defined(CONFIG_VLAN_8021Q)
_Bool vlan_hwaccel_do_receive_V1 ()
{
    assert (0);
    return 0;
}

int __netif_receive_skb ()
{
    if (nondet ())
    {
        (
         (_reconfig_CONFIG_VLAN_8021Q)
         ? vlan_hwaccel_do_receive_V0 ()
         : vlan_hwaccel_do_receive_V1 ());
    }
    return 0;
}

int netif_receive_skb ()
{
    return __netif_receive_skb ();
}

int main (void)
{
    netif_receive_skb ();
    return 0;
}