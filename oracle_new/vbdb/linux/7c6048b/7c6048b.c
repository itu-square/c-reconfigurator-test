#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

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
         acpi_video_bus_put_one_device_V1 ();
    return 0;
}