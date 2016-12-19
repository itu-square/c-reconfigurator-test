#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

// defined(CONFIG_I2C)
static int i2c_master_send_V0 = 0;

// defined(CONFIG_SND_SOC_DA7210) || defined(CONFIG_SND_FSI_DA7210)
static int da7210_init_V1 ()
{
    int codec_hw_write;
    (codec_hw_write = (
                       (_reconfig_CONFIG_I2C)
                       ? i2c_master_send_V0
                       : i2c_master_send));
    return 0;
}

// defined(CONFIG_SND_SOC_AK4642) || defined(CONFIG_SND_FSI_AK4642)
static int ak4642_init_V2 ()
{
    int codec_hw_write;
    (codec_hw_write = (
                       (_reconfig_CONFIG_I2C)
                       ? i2c_master_send_V0
                       : i2c_master_send));
    return 0;
}

int main (int argc , char** argv)
{
    if ((_reconfig_CONFIG_SND_SOC_DA7210 || _reconfig_CONFIG_SND_FSI_DA7210))
         da7210_init_V1 ();
    if ((_reconfig_CONFIG_SND_SOC_AK4642 || _reconfig_CONFIG_SND_FSI_AK4642))
         ak4642_init_V2 ();
    return 0;
}