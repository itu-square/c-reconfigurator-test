#include "/home/alex/reconfigurator_old/vbdb/busybox/simple-target/REconfig.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void build_dep (void)
{
    char* dt;
    (dt = NULL);
    if (rand () % 2)
    {
        (dt = "-i");
    }
    if ((_reconfig_ENABLE_FEATURE_MODPROBE_MULTIPLE_OPTIONS))
    {
        if (dt)
        {
        }
    }
// !defined(ENABLE_FEATURE_MODPROBE_MULTIPLE_OPTIONS)
    char buf_V0 [ 50 ];
    if ((!_reconfig_ENABLE_FEATURE_MODPROBE_MULTIPLE_OPTIONS))
    {
        strcpy (buf_V0 , "module option(s): ");
        strcat (buf_V0 , dt);
    }
}

int main (int argc , char** argv)
{
    build_dep ();
    return 0;
}