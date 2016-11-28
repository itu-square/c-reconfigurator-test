#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

// defined(CONFIG_CRYPTO_BLKCIPHER)
void* crypto_alloc_ablkcipher_V0 ()
{
    return (void*) 0;
}

// defined(CONFIG_CRYPTO_TEST)
static void test_cipher_V1 ()
{
    (
     (_reconfig_CONFIG_CRYPTO_BLKCIPHER)
     ? crypto_alloc_ablkcipher_V0 ()
     : crypto_alloc_ablkcipher ());
}

int main (int argc , char** argv)
{
    if ((_reconfig_CONFIG_CRYPTO_TEST))
         test_cipher_V1 ();
}