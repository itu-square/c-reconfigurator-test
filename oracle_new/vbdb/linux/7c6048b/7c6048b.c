int _reconfig_CONFIG_ACPI;
int _reconfig_CONFIG_ACPI_VIDEO;
int _reconfig_CONFIG_STUB_POULSBO;
int _reconfig_CONFIG_BACKLIGHT_CLASS_DEVICE;


// defined(CONFIG_BACKLIGHT_CLASS_DEVICE)
void backlight_device_unregister_V0 ()
{
    return;
}

// defined(CONFIG_ACPI_VIDEO) || defined(CONFIG_ACPI) && defined(CONFIG_STUB_POULSBO)
static int acpi_video_bus_put_one_device_V1 ()
{
    (
     (_reconfig_CONFIG_BACKLIGHT_CLASS_DEVICE)
     ? backlight_device_unregister_V0 ()
     : backlight_device_unregister ());
    return 0;
}

int main (int argc , char** argv)
{
    if ((_reconfig_CONFIG_ACPI && _reconfig_CONFIG_STUB_POULSBO))
         (
                                                                   (_reconfig_CONFIG_ACPI_VIDEO)
                                                                   ? acpi_video_bus_put_one_device_V1 ()
                                                                   : acpi_video_bus_put_one_device ());
    return 0;
}