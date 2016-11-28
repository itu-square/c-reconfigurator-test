#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"
__attribute__ ((noinline)) int nondet ()
{
    return 42;
}

// defined(CONFIG_SECURITY)
int security_old_inode_init_security_V0 (int* len)
{
    if (nondet ())
    {
        (* len = 0);
        return 0;
    }
    else
         return - 1;
}

// !defined(CONFIG_SECURITY)
int security_old_inode_init_security_V1 (int* len)
{
    return 0;
}

int reiserfs_security_init (int* length)
{
    int error;
    (error = (
              (_reconfig_CONFIG_SECURITY)
              ? security_old_inode_init_security_V0 (length)
              : security_old_inode_init_security_V1 (length)));
    if (error)
    {
        (* length = 0);
        return error;
    }
    int x;
    (x = (* length));
    return 0;
}

int reiserfs_create ()
{
    int retval;
    int length;
    (retval = reiserfs_security_init ((& length)));
    if (retval < 0)
    {
        return retval;
    }
    return 0;
}

int main (void)
{
    reiserfs_create ();
    return 0;
}