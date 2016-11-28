#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

// defined(CONFIG_REGULATOR_MAX8660) && defined(CONFIG_OF)
int max8660_pdata_from_dt_V0 (int* pdata)
{
    (* pdata ++);
    return 0;
}

// defined(CONFIG_REGULATOR_MAX8660) && !defined(CONFIG_OF)
int max8660_pdata_from_dt_V1 (int** pdata)
{
    return 0;
}

// defined(CONFIG_REGULATOR_MAX8660)
int max8660_probe_V2 ()
{
    int pdata_of;
    (pdata_of = 0);
    (
     (_reconfig_CONFIG_REGULATOR_MAX8660 && _reconfig_CONFIG_OF)
     ? max8660_pdata_from_dt_V0 ((& pdata_of))
     : max8660_pdata_from_dt_V1 ((& pdata_of)));
    return 0;
}

int main ()
{
    if ((_reconfig_CONFIG_REGULATOR_MAX8660))
         max8660_probe_V2 ();
    return 0;
}