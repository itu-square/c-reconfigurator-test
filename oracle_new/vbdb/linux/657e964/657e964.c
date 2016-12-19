int _reconfig_CONFIG_TCP_MD5SIG;
int _reconfig_CONFIG_PREEMPT;


#include <assert.h>

__attribute__ ((noinline)) int nondet ()
{
    return 42;
}

static int preempt_counter = 0;

static inline void tcp_free_md5sig_pool (void)
{
    (
     (!_reconfig_CONFIG_TCP_MD5SIG)
     ? free_cpu ()
     : _reconfig_skip ());
}

static inline void tcp_put_md5sig_pool (void)
{
    (
     (_reconfig_CONFIG_PREEMPT)
     ? (preempt_counter --)
     : _reconfig_skip ());
}

void tcp_twsk_destructor ()
{
    if ((_reconfig_CONFIG_TCP_MD5SIG))
    {
        if (nondet ())
         tcp_put_md5sig_pool ();
    }
}

static inline void twsk_destructor ()
{
    tcp_twsk_destructor ();
}

static void inet_twsk_free ()
{
    twsk_destructor ();
}

void inet_twsk_put ()
{
    inet_twsk_free ();
}

static int inet_twdr_do_twkill_work ()
{
    inet_twsk_put ();
    return 0;
}

void inet_twdr_hangman ()
{
    inet_twdr_do_twkill_work ();
}

static inline void __run_timers ()
{
    int preempt_count;
    preempt_count = (preempt_counter);
    inet_twdr_hangman ();
    if (preempt_count != (preempt_counter))
    {
        assert (0);
    }
}

int main (void)
{
    __run_timers ();
    return 0;
}