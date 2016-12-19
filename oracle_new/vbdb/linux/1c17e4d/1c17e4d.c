int _reconfig_CONFIG_RCU_FAST_NO_HZ;
int _reconfig_CONFIG_RCU_CPU_STALL_INFO;

extern int printf (const char* __restrict __format , ...);

extern int sprintf (char* __restrict __s , const char* __restrict __format , ...) __attribute__ ((__nothrow__));

// defined(CONFIG_RCU_CPU_STALL_INFO) && defined(CONFIG_RCU_FAST_NO_HZ)
void print_cpu_stall_fast_no_hz_V0 (char* cp , int cpu)
{
    sprintf (cp , "foobar");
}

// defined(CONFIG_RCU_CPU_STALL_INFO) && !defined(CONFIG_RCU_FAST_NO_HZ)
void print_cpu_stall_fast_no_hz_V1 (char* cp , int cpu)
{
}

// defined(CONFIG_RCU_CPU_STALL_INFO)
void print_cpu_stall_info_V2 (int cpu)
{
    char fast_no_hz [ 72 ];
    (
     (_reconfig_CONFIG_RCU_FAST_NO_HZ)
     ? print_cpu_stall_fast_no_hz_V0 (fast_no_hz , cpu)
     : print_cpu_stall_fast_no_hz_V1 (fast_no_hz , cpu));
    printf ("\t%d: %s\n" , cpu , fast_no_hz);
}

// !defined(CONFIG_RCU_CPU_STALL_INFO)
void print_cpu_stall_info_V3 (int cpu)
{
    printf (" %d" , cpu);
}

int main (void)
{
    if ((_reconfig_CONFIG_RCU_CPU_STALL_INFO))
         print_cpu_stall_info_V2 (0);
    return 0;
}