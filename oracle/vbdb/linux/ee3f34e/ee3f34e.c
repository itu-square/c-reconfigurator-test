#include "/home/alex/reconfigurator_old/vbdb/linux/simple-target/REconfig.c"

#include <stdbool.h>
#include <stdlib.h>
_Bool IS_ERR (const void* ptr)
{
    return (ptr == (void*) - 1);
}

_Bool IS_ERR_OR_NULL (const void* ptr)
{
    return (!ptr) || IS_ERR (ptr);
}

// defined(CONFIG_IP_SCTP) && defined(CONFIG_IPV6)
static int some_int_V0 = 1;

// defined(CONFIG_IP_SCTP) && defined(CONFIG_IPV6)
int* ip6_dst_lookup_flow_V0 ()
{
    return ((& some_int_V0));
}

// defined(CONFIG_IP_SCTP) && defined(CONFIG_IPV6)
void sctp_v6_get_dst_V0 ()
{
    int* dst;
    (dst = NULL);
    (dst = ip6_dst_lookup_flow_V0 ());
    if ((!IS_ERR (dst)))
    {
        (dst = NULL);
    }
    if ((!IS_ERR (dst)))
    {
        char* rt;
        (rt = (char*) dst);
        (
         (!_reconfig_SCTP_DEBUG && _reconfig_CONFIG_SCTP_DBG_MSG || _reconfig_SCTP_DEBUG && SCTP_DEBUG)
         ? (((* rt) ++))
         : _reconfig_skip ());
    }
}

int main ()
{
    if ((_reconfig_CONFIG_IP_SCTP && _reconfig_CONFIG_IPV6))
         sctp_v6_get_dst_V0 ();
    return 0;
}