int _reconfig_CONFIG_ARM_LPAE;

static int do_page_fault ()
{
    return 0;
}

static int do_sect_fault ()
{
    return 0;
}

int main ()
{
    do_page_fault ();
    if ((!_reconfig_CONFIG_ARM_LPAE))
         do_sect_fault ();
    return 0;
}