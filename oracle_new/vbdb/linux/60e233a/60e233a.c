int _reconfig_CONFIG_HOTPLUG;


#include <errno.h>
#include <stdio.h>

static char buf [ 2048 ];

static int buflen = 0;

// defined(CONFIG_HOTPLUG)
int add_uevent_var_V0 ()
{
    int len;
    len = sprintf (& buf [ buflen ] , "MODALIAS=");
    buflen += len + 1;
    return 0;
}

// !defined(CONFIG_HOTPLUG)
int add_uevent_var_V1 ()
{
    return 0;
}

int input_add_uevent_modalias_var ()
{
    if ((
         (_reconfig_CONFIG_HOTPLUG)
         ? add_uevent_var_V0 ()
         : add_uevent_var_V1 ()))
         return - ENOMEM;
    buf [ buflen - 1 ] = 'x';
    return 0;
}

int main (int argc , char** argv)
{
    input_add_uevent_modalias_var ();
    return 0;
}