int _reconfig_BB_FEATURE_LS_FILETYPES;
int _reconfig_BB_FEATURE_LS_USERNAME;


#include <stdio.h>
#include <stdlib.h>

void list_single ()
{
// defined(BB_FEATURE_LS_FILETYPES)
    int info_V0;
    if ((_reconfig_BB_FEATURE_LS_USERNAME))
         (
                                             (_reconfig_BB_FEATURE_LS_FILETYPES)
                                             ? (info_V0 = 0)
                                             : (info = 0));
    if ((_reconfig_BB_FEATURE_LS_FILETYPES))
    {
        if (rand () % 2)
         printf ("%d" , info_V0);
    }
}

void showfiles ()
{
    list_single ();
}

int main (int argc , char** argv)
{
    showfiles ();
    return 0;
}