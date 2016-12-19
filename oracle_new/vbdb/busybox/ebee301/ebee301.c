int _reconfig_BB_AR;
int _reconfig_BB_CP_MV;


// defined(BB_AR)
void copySubFile_V0 (int src , int dst)
{
    printf ("Copy readSize bytes between two file descriptors");
}

// defined(BB_CP_MV)
void copyFile_V1 ()
{
    int src;
    int dst;
    src = rand () % 100;
    dst = rand () % 200;
    (
     (_reconfig_BB_AR)
     ? copySubFile_V0 (src , dst)
     : copySubFile (src , dst));
}

int main (int argc , char** argv)
{
    if ((_reconfig_BB_CP_MV))
         copyFile_V1 ();
    return 0;
}