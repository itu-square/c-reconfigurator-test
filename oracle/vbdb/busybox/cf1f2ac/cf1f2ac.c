int _reconfig_ENABLE_DEBUG_INIT;


#include <stdio.h>
#include <unistd.h>

// !defined(ENABLE_DEBUG_INIT)
void shutdown_signal_V0 (int sig)
{
    char* m;
    if (sig == 15)
    {
        m = "reboot";
    }
    else
    {
        if (sig == 31)
        {
            m = "poweroff";
        }
        else
        {
            m = "halt";
        }
    }
    printf ("Requesting system %s." , m);
}

void open_new_terminal ()
{
    (
     (!_reconfig_ENABLE_DEBUG_INIT)
     ? shutdown_signal_V0 (30)
     : shutdown_signal (30));
}

void run ()
{
    open_new_terminal ();
}

void run_actions ()
{
    run ();
}

int main (int argc , char** argv)
{
    run_actions ();
    return 0;
}