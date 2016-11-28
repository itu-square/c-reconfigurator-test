#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"
typedef long unsigned int size_t;

extern void* malloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__));

extern void free (void* __ptr) __attribute__ ((__nothrow__ , __leaf__));

unsigned char* wbuf;

// defined(CONFIG_JFFS2_FS_WBUF_VERIFY)
unsigned char* wbuf_verify_V0;

int jffs2_nor_wbuf_flash_setup ()
{
    (wbuf = malloc (32));
    if ((!wbuf))
         return - 12;
    return 0;
}

// defined(CONFIG_JFFS2_FS_WBUF_VERIFY)
static int jffs2_verify_write_V0 ()
{
    return (int) ((* wbuf_verify_V0));
}

int __jffs2_flush_wbuf ()
{
    if ((_reconfig_CONFIG_JFFS2_FS_WBUF_VERIFY))
         jffs2_verify_write_V0 ();
    else
    {
        do
        {
        }
        while (0);
    }
    return 0;
}

int jffs2_flash_writev ()
{
    __jffs2_flush_wbuf ();
    return 0;
}

int jffs2_flash_write ()
{
    return jffs2_flash_writev ();
}

int main ()
{
    jffs2_nor_wbuf_flash_setup ();
    jffs2_flash_write ();
    return 0;
}