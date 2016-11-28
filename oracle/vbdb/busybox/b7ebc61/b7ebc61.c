#include "/home/alex/reconfigurator_old/vbdb/busybox/simple-target/REconfig.c"

#include <stdlib.h>
static unsigned int all_fmt;

int main (int argc , char** argv)
{
    (all_fmt = (1U << 11));
    if (rand () % 2)
    {
        (
         (_reconfig_CONFIG_FEATURE_LS_TIMESTAMPS)
         ? (all_fmt &= ~ (3U << 23))
         : all_fmt &= (~ TIME_MASK));
    }
    return 0;
}