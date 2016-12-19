#include "/home/alex/reconfigurator_old/vbdb/busybox/simple-target/REconfig.c"

#include <stdio.h>
#include <stdlib.h>
int main (int argc , char** argv)
{
    int r;
    (r = rand () % 2);
    int m_has_modinfo;
    int exit_status;
    (exit_status = - 1);
    int someInt;
    (someInt = 1);
// defined(CONFIG_FEATURE_INSMOD_VERSION_CHECKING)
    int k_crcs_V0;
    if (r)
         (m_has_modinfo = 0);
    else
         (m_has_modinfo = 1);
    if ((_reconfig_CONFIG_FEATURE_INSMOD_VERSION_CHECKING))
         ((k_crcs_V0 = 0));
    if (r)
    {
        (
         (_reconfig_CONFIG_FEATURE_INSMOD_VERSION_CHECKING)
         ? (k_crcs_V0 = someInt)
         : (k_crcs = someInt));
    }
    else
    {
        printf ("Not configured to support old kernels");
        goto out;
    }
    out : return (exit_status);
    return 0;
}