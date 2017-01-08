int _reconfig_CONFIG_I2C_DESIGNWARE_PLATFORM;
int _reconfig_CONFIG_I2C_DESIGNWARE_PCI;


// defined(CONFIG_I2C_DESIGNWARE_PLATFORM)
int i2c_dw_init_V0 ()
{
    return 0;
}

// defined(CONFIG_I2C_DESIGNWARE_PCI)
int i2c_dw_init_V1 ()
{
    return 0;
}

int main (void)
{
    if ((_reconfig_CONFIG_I2C_DESIGNWARE_PCI || _reconfig_CONFIG_I2C_DESIGNWARE_PLATFORM))
         (
                                                                                            (_reconfig_CONFIG_I2C_DESIGNWARE_PLATFORM)
                                                                                            ? i2c_dw_init_V0 ()
                                                                                            : i2c_dw_init_V1 ());
    return 0;
}