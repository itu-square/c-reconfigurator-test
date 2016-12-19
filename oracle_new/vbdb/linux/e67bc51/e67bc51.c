#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

// defined(CONFIG_TRACING)
void trace_dump_stack_V0 (int skip)
{
    return;
}

// !defined(CONFIG_TRACING)
static inline void trace_dump_stack_V1 (void)
{
}

int main (int argc , char** argv)
{
    (
     (_reconfig_CONFIG_TRACING)
     ? trace_dump_stack_V0 (0)
     : trace_dump_stack_V1 (0));
    return 0;
}