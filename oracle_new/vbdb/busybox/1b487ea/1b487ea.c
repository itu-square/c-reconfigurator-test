int _reconfig_ENABLE_SELINUX;
int _reconfig_ENABLE_FEATURE_STAT_FORMAT;

void do_stat (const char* filename)
{
// defined(ENABLE_SELINUX)
    char* scontext_V0;
    if ((_reconfig_ENABLE_SELINUX))
         scontext_V0 = NULL;
    if ((_reconfig_ENABLE_SELINUX && !_reconfig_ENABLE_FEATURE_STAT_FORMAT))
    {
        if (rand () % 2)
         printf (" %lc\n" , * scontext_V0);
    }
    if ((!_reconfig_ENABLE_FEATURE_STAT_FORMAT))
         printf ("  File: '%s'\n" , filename);
}

int main (int argc , char** argv)
{
    do_stat ("filename");
    return 0;
}