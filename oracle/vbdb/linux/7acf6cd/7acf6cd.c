int _reconfig_CONFIG_UNIX98_PTYS;
int _reconfig_CONFIG_DEVPTS_MULTIPLE_INSTANCES;

__attribute__ ((noinline)) int nondet ()
{
    return 42;
}

int some_int = 1;

void pts_sb_from_inode (int* inode)
{
// defined(CONFIG_DEVPTS_MULTIPLE_INSTANCES)
    int x_V0;
    if ((_reconfig_CONFIG_DEVPTS_MULTIPLE_INSTANCES))
         x_V0 = * inode;
}

void devpts_pty_kill (int* inode)
{
    pts_sb_from_inode (inode);
}

void pty_close (int* driver_data)
{
    if ((_reconfig_CONFIG_UNIX98_PTYS))
         devpts_pty_kill (driver_data);
}

int tty_release (int* driver_data)
{
    pty_close (driver_data);
    return 0;
}

// defined(CONFIG_UNIX98_PTYS)
int ptmx_open_V1 ()
{
    int* driver_data;
    if (nondet ())
    {
        goto err_release;
    }
    driver_data = & some_int;
    return 0;
    err_release : tty_release (driver_data);
    return - 1;
}

int main ()
{
    if ((_reconfig_CONFIG_UNIX98_PTYS))
         ptmx_open_V1 ();
    return 0;
}