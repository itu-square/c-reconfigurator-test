#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

// defined(CONFIG_IOSCHED_CFQ)
static long cfq_scale_slice_V0 ()
{
    long d;
    (d = 1);
    (d = d * (
              (_reconfig_CONFIG_BLK_CGROUP)
              ? 500
              : CFQ_WEIGHT_DEFAULT));
    return d;
}

int main ()
{
    if ((_reconfig_CONFIG_IOSCHED_CFQ))
         cfq_scale_slice_V0 ();
    return 0;
}