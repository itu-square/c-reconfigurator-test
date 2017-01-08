int _reconfig_CONFIG_BCM47XX;
int _reconfig_CONFIG_SSB_DRIVER_EXTIF;

// defined(CONFIG_SSB_DRIVER_EXTIF)
int ssb_extif_gpio_in_V0 ()
{
    return 0;
}

// defined(CONFIG_BCM47XX)
int bcm47xx_gpio_get_value_V1 ()
{
    return (
            (_reconfig_CONFIG_SSB_DRIVER_EXTIF)
            ? ssb_extif_gpio_in_V0 ()
            : ssb_extif_gpio_in ());
}

int main (int argc , char** argv)
{
    if ((_reconfig_CONFIG_BCM47XX))
         bcm47xx_gpio_get_value_V1 ();
    return 0;
}