#include "/home/alex/reconfigurator_old/vbdb/busybox/simple-target/REconfig.c"

#include <stdio.h>
#include <stdlib.h>
// defined(CONFIG_FEATURE_HTTPD_CGI)
static const char request_GET_V0 [ ] = "GET";

void sendHeaders (int responseNum)
{
// defined(CONFIG_LFS)
    long long total_V1;
    if ((_reconfig_CONFIG_LFS))
         ((total_V1 = 10LL));
// !defined(CONFIG_LFS)
    long total_V2;
    if ((!_reconfig_CONFIG_LFS))
         ((total_V2 = 10L));
    printf ("%ld\r\n" , (
                         (_reconfig_CONFIG_LFS)
                         ? total_V1
                         : total_V2));
}

void handleIncoming (const char* request)
{
// defined(CONFIG_FEATURE_HTTPD_BASIC_AUTH)
    int random_V3;
    if ((_reconfig_CONFIG_FEATURE_HTTPD_BASIC_AUTH))
         ((random_V3 = rand () % 2));
// defined(CONFIG_FEATURE_HTTPD_BASIC_AUTH)
    int http_unauthorized_V3;
    if ((_reconfig_CONFIG_FEATURE_HTTPD_BASIC_AUTH))
    {
        ((http_unauthorized_V3 = random_V3));
        if (http_unauthorized_V3)
        {
            sendHeaders (401);
        }
    }
    if ((_reconfig_CONFIG_FEATURE_HTTPD_CGI))
    {
        if (request != request_GET_V0)
        {
            sendHeaders (501);
        }
    }
}

int main (int argc , char** argv)
{
    const char request [ ];
    (request [ 0 ] = 'P');
    (request [ 1 ] = 'O');
    (request [ 2 ] = 'S');
    (request [ 3 ] = 'T');
    handleIncoming (request);
    return 0;
}