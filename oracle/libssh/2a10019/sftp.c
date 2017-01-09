int _reconfig__WIN32;
int _reconfig_WITH_SERVER;
int _reconfig_WITH_SFTP;


#include "config.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include "libssh/priv.h"
#include "libssh/ssh2.h"
#include "libssh/sftp.h"

struct sftp_ext_struct
{
    unsigned int count;
    char** name;
    char** data;
}
;

// defined(WITH_SFTP)
static int sftp_enqueue_V0 (SFTP_SESSION* session , SFTP_MESSAGE* msg);

// defined(WITH_SFTP)
static void sftp_message_free_V0 (SFTP_MESSAGE* msg);

// defined(WITH_SFTP)
static void sftp_set_error_V0 (SFTP_SESSION* sftp , int errnum);

// defined(WITH_SFTP)
static void status_msg_free_V0 (STATUS_MESSAGE* status);

// defined(WITH_SFTP)
static sftp_ext sftp_ext_new_V0 (void)
{
    sftp_ext ext;
    ext = malloc (sizeof (struct sftp_ext_struct));
    if (ext == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (ext);
    return ext;
}

// defined(WITH_SFTP)
static void sftp_ext_free_V0 (sftp_ext ext)
{
    unsigned int i;
    if (ext == NULL)
    {
        return;
    }
    if (ext -> count)
    {
        for (i = 0; i < ext -> count; i ++)
        {
            SAFE_FREE (ext -> name [ i ]);
            SAFE_FREE (ext -> data [ i ]);
        }
        SAFE_FREE (ext -> name);
        SAFE_FREE (ext -> data);
    }
    SAFE_FREE (ext);
}

// defined(WITH_SFTP)
SFTP_SESSION* sftp_new_V0 (SSH_SESSION* session)
{
    SFTP_SESSION* sftp;
    enter_function ();
    if (session == NULL)
    {
        leave_function ();
        return NULL;
    }
    sftp = malloc (sizeof (SFTP_SESSION));
    if (sftp == NULL)
    {
        leave_function ();
        return NULL;
    }
    ZERO_STRUCTP (sftp);
    sftp -> ext = sftp_ext_new_V0 ();
    if (sftp -> ext == NULL)
    {
        SAFE_FREE (sftp);
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    sftp -> session = session;
    sftp -> channel = channel_new (session);
    if (sftp -> channel == NULL)
    {
        SAFE_FREE (sftp);
        leave_function ();
        return NULL;
    }
    if (channel_open_session (sftp -> channel))
    {
        channel_free (sftp -> channel);
        SAFE_FREE (sftp);
        leave_function ();
        return NULL;
    }
    if (channel_request_sftp (sftp -> channel))
    {
        sftp_free (sftp);
        leave_function ();
        return NULL;
    }
    leave_function ();
    return sftp;
}

// defined(WITH_SFTP) && defined(WITH_SERVER)
SFTP_SESSION* sftp_server_new_V1 (SSH_SESSION* session , ssh_channel chan)
{
    SFTP_SESSION* sftp;
    sftp = NULL;
    sftp = malloc (sizeof (SFTP_SESSION));
    if (sftp == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (sftp);
    sftp -> session = session;
    sftp -> channel = chan;
    return sftp;
}

// defined(WITH_SFTP) && defined(WITH_SERVER)
int sftp_server_init_V1 (SFTP_SESSION* sftp)
{
    ssh_session session;
    session = sftp -> session;
    SFTP_PACKET* packet;
    packet = NULL;
    ssh_buffer reply;
    reply = NULL;
    uint32_t version;
    _enter_function (sftp -> channel -> session);
    packet = sftp_packet_read (sftp);
    if (packet == NULL)
    {
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    if (packet -> type != SSH_FXP_INIT)
    {
        ssh_set_error (session , SSH_FATAL , "Packet read of type %d instead of SSH_FXP_INIT" , packet -> type);
        sftp_packet_free (packet);
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    ssh_log (session , SSH_LOG_PACKET , "Received SSH_FXP_INIT");
    buffer_get_u32 (packet -> payload , & version);
    version = ntohl (version);
    ssh_log (session , SSH_LOG_PACKET , "Client version: %d" , version);
    sftp -> client_version = version;
    sftp_packet_free (packet);
    reply = buffer_new ();
    if (reply == NULL)
    {
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    if (buffer_add_u32 (reply , ntohl (LIBSFTP_VERSION)) < 0)
    {
        buffer_free (reply);
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    if (sftp_packet_write (sftp , SSH_FXP_VERSION , reply) < 0)
    {
        buffer_free (reply);
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    buffer_free (reply);
    ssh_log (session , SSH_LOG_RARE , "Server version sent");
    if (version > LIBSFTP_VERSION)
    {
        sftp -> version = LIBSFTP_VERSION;
    }
    else
    {
        sftp -> version = version;
    }
    _leave_function (sftp -> channel -> session);
    return 0;
}

// defined(WITH_SFTP)
void sftp_free_V0 (SFTP_SESSION* sftp)
{
    struct request_queue* ptr;
    if (sftp == NULL)
    {
        return;
    }
    channel_send_eof (sftp -> channel);
    ptr = sftp -> queue;
    while (ptr)
    {
        struct request_queue* old;
        sftp_message_free (ptr -> message);
        old = ptr -> next;
        SAFE_FREE (ptr);
        ptr = old;
    }
    channel_free (sftp -> channel);
    sftp_ext_free_V0 (sftp -> ext);
    ZERO_STRUCTP (sftp);
    SAFE_FREE (sftp);
}

// defined(WITH_SFTP)
int sftp_packet_write_V0 (SFTP_SESSION* sftp , uint8_t type , ssh_buffer payload)
{
    int size;
    if (buffer_prepend_data (payload , & type , sizeof (uint8_t)) < 0)
    {
        return - 1;
    }
    size = htonl (buffer_get_len (payload));
    if (buffer_prepend_data (payload , & size , sizeof (uint32_t)) < 0)
    {
        return - 1;
    }
    size = channel_write (sftp -> channel , buffer_get (payload) , buffer_get_len (payload));
    if (size < 0)
    {
        return - 1;
    }
    else
    {
        if ((uint32_t) size != buffer_get_len (payload))
        {
            ssh_log (sftp -> session , SSH_LOG_PACKET , "Had to write %d bytes, wrote only %d" , buffer_get_len (payload) , size);
        }
    }
    return size;
}

// defined(WITH_SFTP)
SFTP_PACKET* sftp_packet_read_V0 (SFTP_SESSION* sftp)
{
    SFTP_PACKET* packet;
    packet = NULL;
    uint32_t size;
    _enter_function (sftp -> channel -> session);
    packet = malloc (sizeof (SFTP_PACKET));
    if (packet == NULL)
    {
        return NULL;
    }
    packet -> sftp = sftp;
    packet -> payload = buffer_new ();
    if (packet -> payload == NULL)
    {
        SAFE_FREE (packet);
        return NULL;
    }
    if (channel_read_buffer (sftp -> channel , packet -> payload , 4 , 0) <= 0)
    {
        buffer_free (packet -> payload);
        SAFE_FREE (packet);
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    if (buffer_get_u32 (packet -> payload , & size) < 0)
    {
        buffer_free (packet -> payload);
        SAFE_FREE (packet);
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    size = ntohl (size);
    if (channel_read_buffer (sftp -> channel , packet -> payload , 1 , 0) <= 0)
    {
        buffer_free (packet -> payload);
        SAFE_FREE (packet);
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    buffer_get_u8 (packet -> payload , & packet -> type);
    if (size > 1)
    {
        if (channel_read_buffer (sftp -> channel , packet -> payload , size - 1 , 0) <= 0)
        {
            buffer_free (packet -> payload);
            SAFE_FREE (packet);
            _leave_function (sftp -> channel -> session);
            return NULL;
        }
    }
    _leave_function (sftp -> channel -> session);
    return packet;
}

// defined(WITH_SFTP)
static void sftp_set_error_V0 (SFTP_SESSION* sftp , int errnum)
{
    if (sftp != NULL)
    {
        sftp -> errnum = errnum;
    }
}

// defined(WITH_SFTP)
int sftp_get_error_V0 (SFTP_SESSION* sftp)
{
    if (sftp == NULL)
    {
        return - 1;
    }
    return sftp -> errnum;
}

// defined(WITH_SFTP)
static SFTP_MESSAGE* sftp_message_new_V0 (SFTP_SESSION* sftp)
{
    SFTP_MESSAGE* msg;
    msg = NULL;
    _enter_function (sftp -> channel -> session);
    msg = malloc (sizeof (SFTP_MESSAGE));
    if (msg == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (msg);
    msg -> payload = buffer_new ();
    if (msg -> payload == NULL)
    {
        SAFE_FREE (msg);
        return NULL;
    }
    msg -> sftp = sftp;
    _leave_function (sftp -> channel -> session);
    return msg;
}

// defined(WITH_SFTP)
static void sftp_message_free_V0 (SFTP_MESSAGE* msg)
{
    SFTP_SESSION* sftp;
    if (msg == NULL)
    {
        return;
    }
    sftp = msg -> sftp;
    _enter_function (sftp -> channel -> session);
    buffer_free (msg -> payload);
    SAFE_FREE (msg);
    _leave_function (sftp -> channel -> session);
}

// defined(WITH_SFTP)
static SFTP_MESSAGE* sftp_get_message_V0 (SFTP_PACKET* packet)
{
    SFTP_SESSION* sftp;
    sftp = packet -> sftp;
    SFTP_MESSAGE* msg;
    msg = NULL;
    _enter_function (sftp -> channel -> session);
    msg = sftp_message_new_V0 (sftp);
    if (msg == NULL)
    {
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    msg -> sftp = packet -> sftp;
    msg -> packet_type = packet -> type;
    if ((packet -> type != SSH_FXP_STATUS) && (packet -> type != SSH_FXP_HANDLE) && (packet -> type != SSH_FXP_DATA) && (packet -> type != SSH_FXP_ATTRS) && (packet -> type != SSH_FXP_NAME) && (packet -> type != SSH_FXP_EXTENDED_REPLY))
    {
        ssh_set_error (packet -> sftp -> session , SSH_FATAL , "Unknown packet type %d" , packet -> type);
        sftp_message_free_V0 (msg);
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    if (buffer_get_u32 (packet -> payload , & msg -> id) != sizeof (uint32_t))
    {
        ssh_set_error (packet -> sftp -> session , SSH_FATAL , "Invalid packet %d: no ID" , packet -> type);
        sftp_message_free_V0 (msg);
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    ssh_log (packet -> sftp -> session , SSH_LOG_PACKET , "Packet with id %d type %d" , msg -> id , msg -> packet_type);
    if (buffer_add_data (msg -> payload , buffer_get_rest (packet -> payload) , buffer_get_rest_len (packet -> payload)) < 0)
    {
        sftp_message_free_V0 (msg);
        _leave_function (sftp -> channel -> session);
        return NULL;
    }
    _leave_function (sftp -> channel -> session);
    return msg;
}

// defined(WITH_SFTP)
static int sftp_read_and_dispatch_V0 (SFTP_SESSION* sftp)
{
    SFTP_PACKET* packet;
    packet = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    _enter_function (sftp -> channel -> session);
    packet = sftp_packet_read_V0 (sftp);
    if (packet == NULL)
    {
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    msg = sftp_get_message_V0 (packet);
    sftp_packet_free (packet);
    if (msg == NULL)
    {
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    if (sftp_enqueue (sftp , msg) < 0)
    {
        sftp_message_free_V0 (msg);
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    _leave_function (sftp -> channel -> session);
    return 0;
}

// defined(WITH_SFTP)
void sftp_packet_free_V0 (SFTP_PACKET* packet)
{
    if (packet == NULL)
    {
        return;
    }
    buffer_free (packet -> payload);
    free (packet);
}

// defined(WITH_SFTP)
int sftp_init_V0 (SFTP_SESSION* sftp)
{
    SFTP_PACKET* packet;
    packet = NULL;
    ssh_buffer buffer;
    buffer = NULL;
    ssh_string ext_name_s;
    ext_name_s = NULL;
    ssh_string ext_data_s;
    ext_data_s = NULL;
    char* ext_name;
    ext_name = NULL;
    char* ext_data;
    ext_data = NULL;
    uint32_t version;
    version = htonl (LIBSFTP_VERSION);
    _enter_function (sftp -> channel -> session);
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    if ((buffer_add_u32 (buffer , version) < 0) || sftp_packet_write_V0 (sftp , SSH_FXP_INIT , buffer) < 0)
    {
        buffer_free (buffer);
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    buffer_free (buffer);
    packet = sftp_packet_read_V0 (sftp);
    if (packet == NULL)
    {
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    if (packet -> type != SSH_FXP_VERSION)
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Received a %d messages instead of SSH_FXP_VERSION" , packet -> type);
        sftp_packet_free_V0 (packet);
        _leave_function (sftp -> channel -> session);
        return - 1;
    }
    buffer_get_u32 (packet -> payload , & version);
    version = ntohl (version);
    ssh_log (sftp -> session , SSH_LOG_RARE , "SFTP server version %d" , version);
    ext_name_s = buffer_get_ssh_string (packet -> payload);
    while (ext_name_s != NULL)
    {
        int count;
        count = sftp -> ext -> count;
        char** tmp;
        ext_data_s = buffer_get_ssh_string (packet -> payload);
        if (ext_data_s == NULL)
        {
            string_free (ext_name_s);
            break;
        }
        ext_name = string_to_char (ext_name_s);
        ext_data = string_to_char (ext_data_s);
        if (ext_name == NULL || ext_data == NULL)
        {
            SAFE_FREE (ext_name);
            SAFE_FREE (ext_data);
            string_free (ext_name_s);
            string_free (ext_data_s);
            return - 1;
        }
        ssh_log (sftp -> session , SSH_LOG_RARE , "SFTP server extension: %s, version: %s" , ext_name , ext_data);
        count ++;
        tmp = realloc (sftp -> ext -> name , count * sizeof (char*));
        if (tmp == NULL)
        {
            SAFE_FREE (ext_name);
            SAFE_FREE (ext_data);
            string_free (ext_name_s);
            string_free (ext_data_s);
            return - 1;
        }
        tmp [ count - 1 ] = ext_name;
        sftp -> ext -> name = tmp;
        tmp = realloc (sftp -> ext -> data , count * sizeof (char*));
        if (tmp == NULL)
        {
            SAFE_FREE (ext_name);
            SAFE_FREE (ext_data);
            string_free (ext_name_s);
            string_free (ext_data_s);
            return - 1;
        }
        tmp [ count - 1 ] = ext_data;
        sftp -> ext -> data = tmp;
        sftp -> ext -> count = count;
        string_free (ext_name_s);
        string_free (ext_data_s);
        ext_name_s = buffer_get_ssh_string (packet -> payload);
    }
    sftp_packet_free_V0 (packet);
    sftp -> version = sftp -> server_version = version;
    _leave_function (sftp -> channel -> session);
    return 0;
}

// defined(WITH_SFTP)
unsigned int sftp_extensions_get_count_V0 (SFTP_SESSION* sftp)
{
    if (sftp == NULL || sftp -> ext == NULL)
    {
        return 0;
    }
    return sftp -> ext -> count;
}

// defined(WITH_SFTP)
const char* sftp_extensions_get_name_V0 (SFTP_SESSION* sftp , unsigned int idx)
{
    if (sftp == NULL || sftp -> ext == NULL || sftp -> ext -> name == NULL)
    {
        return NULL;
    }
    if (idx > sftp -> ext -> count)
    {
        return NULL;
    }
    return sftp -> ext -> name [ idx ];
}

// defined(WITH_SFTP)
const char* sftp_extensions_get_data_V0 (SFTP_SESSION* sftp , unsigned int idx)
{
    if (sftp == NULL || sftp -> ext == NULL || sftp -> ext -> data == NULL)
    {
        return NULL;
    }
    if (idx > sftp -> ext -> count)
    {
        return NULL;
    }
    return sftp -> ext -> data [ idx ];
}

// defined(WITH_SFTP)
int sftp_extension_supported_V0 (SFTP_SESSION* sftp , const char* name , const char* data)
{
    int i;
    int n;
    n = sftp_extensions_get_count_V0 (sftp);
    for (i = 0; i < n; i ++)
    {
        if (strcmp (sftp_extensions_get_name_V0 (sftp , i) , name) == 0 && strcmp (sftp_extensions_get_data_V0 (sftp , i) , data) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// defined(WITH_SFTP)
static REQUEST_QUEUE* request_queue_new_V0 (SFTP_MESSAGE* msg)
{
    REQUEST_QUEUE* queue;
    queue = NULL;
    queue = malloc (sizeof (REQUEST_QUEUE));
    if (queue == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (queue);
    queue -> message = msg;
    return queue;
}

// defined(WITH_SFTP)
static void request_queue_free_V0 (REQUEST_QUEUE* queue)
{
    if (queue == NULL)
    {
        return;
    }
    ZERO_STRUCTP (queue);
    SAFE_FREE (queue);
}

// defined(WITH_SFTP)
static int sftp_enqueue_V0 (SFTP_SESSION* sftp , SFTP_MESSAGE* msg)
{
    REQUEST_QUEUE* queue;
    queue = NULL;
    REQUEST_QUEUE* ptr;
    queue = request_queue_new_V0 (msg);
    if (queue == NULL)
    {
        return - 1;
    }
    ssh_log (sftp -> session , SSH_LOG_PACKET , "Queued msg type %d id %d" , msg -> id , msg -> packet_type);
    if (sftp -> queue == NULL)
    {
        sftp -> queue = queue;
    }
    else
    {
        ptr = sftp -> queue;
        while (ptr -> next)
        {
            ptr = ptr -> next;
        }
        ptr -> next = queue;
    }
    return 0;
}

// defined(WITH_SFTP)
static SFTP_MESSAGE* sftp_dequeue_V0 (SFTP_SESSION* sftp , uint32_t id)
{
    REQUEST_QUEUE* prev;
    prev = NULL;
    REQUEST_QUEUE* queue;
    SFTP_MESSAGE* msg;
    if (sftp -> queue == NULL)
    {
        return NULL;
    }
    queue = sftp -> queue;
    while (queue)
    {
        if (queue -> message -> id == id)
        {
            if (prev == NULL)
            {
                sftp -> queue = queue -> next;
            }
            else
            {
                prev -> next = queue -> next;
            }
            msg = queue -> message;
            request_queue_free_V0 (queue);
            ssh_log (sftp -> session , SSH_LOG_PACKET , "Dequeued msg id %d type %d" , msg -> id , msg -> packet_type);
            return msg;
        }
        prev = queue;
        queue = queue -> next;
    }
    return NULL;
}

// defined(WITH_SFTP)
static inline uint32_t sftp_get_new_id_V0 (SFTP_SESSION* session)
{
    return ++ session -> id_counter;
}

// defined(WITH_SFTP)
static STATUS_MESSAGE* parse_status_msg_V0 (SFTP_MESSAGE* msg)
{
    STATUS_MESSAGE* status;
    if (msg -> packet_type != SSH_FXP_STATUS)
    {
        ssh_set_error (msg -> sftp -> session , SSH_FATAL , "Not a ssh_fxp_status message passed in!");
        return NULL;
    }
    status = malloc (sizeof (STATUS_MESSAGE));
    if (status == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (status);
    status -> id = msg -> id;
    if ((buffer_get_u32 (msg -> payload , & status -> status) != 4) || (status -> error = buffer_get_ssh_string (msg -> payload)) == NULL || (status -> lang = buffer_get_ssh_string (msg -> payload)) == NULL)
    {
        string_free (status -> error);
        SAFE_FREE (status);
        ssh_set_error (msg -> sftp -> session , SSH_FATAL , "Invalid SSH_FXP_STATUS message");
        return NULL;
    }
    status -> status = ntohl (status -> status);
    status -> errormsg = string_to_char (status -> error);
    status -> langmsg = string_to_char (status -> lang);
    if (status -> errormsg == NULL || status -> langmsg == NULL)
    {
        status_msg_free (status);
        return NULL;
    }
    return status;
}

// defined(WITH_SFTP)
static void status_msg_free_V0 (STATUS_MESSAGE* status)
{
    if (status == NULL)
    {
        return;
    }
    string_free (status -> error);
    string_free (status -> lang);
    SAFE_FREE (status -> errormsg);
    SAFE_FREE (status -> langmsg);
    SAFE_FREE (status);
}

// defined(WITH_SFTP)
static SFTP_FILE* parse_handle_msg_V0 (SFTP_MESSAGE* msg)
{
    SFTP_FILE* file;
    if (msg -> packet_type != SSH_FXP_HANDLE)
    {
        ssh_set_error (msg -> sftp -> session , SSH_FATAL , "Not a ssh_fxp_handle message passed in!");
        return NULL;
    }
    file = malloc (sizeof (SFTP_FILE));
    if (file == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (file);
    file -> handle = buffer_get_ssh_string (msg -> payload);
    if (file -> handle == NULL)
    {
        ssh_set_error (msg -> sftp -> session , SSH_FATAL , "Invalid SSH_FXP_HANDLE message");
        SAFE_FREE (file);
        return NULL;
    }
    file -> sftp = msg -> sftp;
    file -> offset = 0;
    file -> eof = 0;
    return file;
}

// defined(WITH_SFTP)
SFTP_DIR* sftp_opendir_V0 (SFTP_SESSION* sftp , const char* path)
{
    SFTP_MESSAGE* msg;
    msg = NULL;
    SFTP_FILE* file;
    file = NULL;
    SFTP_DIR* dir;
    dir = NULL;
    STATUS_MESSAGE* status;
    ssh_string path_s;
    ssh_buffer payload;
    uint32_t id;
    payload = buffer_new ();
    if (payload == NULL)
    {
        return NULL;
    }
    path_s = string_from_char (path);
    if (path_s == NULL)
    {
        buffer_free (payload);
        return NULL;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (payload , id) < 0 || buffer_add_ssh_string (payload , path_s) < 0)
    {
        buffer_free (payload);
        string_free (path_s);
        return NULL;
    }
    string_free (path_s);
    if (sftp_packet_write_V0 (sftp , SSH_FXP_OPENDIR , payload) < 0)
    {
        buffer_free (payload);
        return NULL;
    }
    buffer_free (payload);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    switch (msg -> packet_type)
    {
        case SSH_FXP_STATUS : status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return NULL;
        }
        sftp_set_error_V0 (sftp , status -> status);
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return NULL;
        case SSH_FXP_HANDLE : file = parse_handle_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (file != NULL)
        {
            dir = malloc (sizeof (SFTP_DIR));
            if (dir == NULL)
            {
                return NULL;
            }
            ZERO_STRUCTP (dir);
            dir -> sftp = sftp;
            dir -> name = strdup (path);
            if (dir -> name == NULL)
            {
                SAFE_FREE (dir);
                SAFE_FREE (file);
                return NULL;
            }
            dir -> handle = file -> handle;
            SAFE_FREE (file);
        }
        return dir;
        default : ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d during opendir!" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return NULL;
}

// defined(WITH_SFTP)
static SFTP_ATTRIBUTES* sftp_parse_attr_4_V0 (SFTP_SESSION* sftp , ssh_buffer buf , int expectnames)
{
    SFTP_ATTRIBUTES* attr;
    ssh_string owner;
    owner = NULL;
    ssh_string group;
    group = NULL;
    uint32_t flags;
    flags = 0;
    int ok;
    ok = 0;
    (void) expectnames;
    attr = malloc (sizeof (SFTP_ATTRIBUTES));
    if (attr == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (attr);
    do
    {
        if (buffer_get_u32 (buf , & flags) != 4)
        {
            break;
        }
        flags = ntohl (flags);
        attr -> flags = flags;
        if (flags & SSH_FILEXFER_ATTR_SIZE)
        {
            if (buffer_get_u64 (buf , & attr -> size) != 8)
            {
                break;
            }
            attr -> size = ntohll (attr -> size);
        }
        if (flags & SSH_FILEXFER_ATTR_OWNERGROUP)
        {
            if ((owner = buffer_get_ssh_string (buf)) == NULL || (attr -> owner = string_to_char (owner)) == NULL)
            {
                break;
            }
            if ((group = buffer_get_ssh_string (buf)) == NULL || (attr -> group = string_to_char (group)) == NULL)
            {
                break;
            }
        }
        if (flags & SSH_FILEXFER_ATTR_PERMISSIONS)
        {
            if (buffer_get_u32 (buf , & attr -> permissions) != 4)
            {
                break;
            }
            attr -> permissions = ntohl (attr -> permissions);
            switch (attr -> permissions & S_IFMT)
            {
                case S_IFSOCK : case S_IFBLK : case S_IFCHR : case S_IFIFO : attr -> type = SSH_FILEXFER_TYPE_SPECIAL;
                break;
                case S_IFLNK : attr -> type = SSH_FILEXFER_TYPE_SYMLINK;
                break;
                case S_IFREG : attr -> type = SSH_FILEXFER_TYPE_REGULAR;
                break;
                case S_IFDIR : attr -> type = SSH_FILEXFER_TYPE_DIRECTORY;
                break;
                default : attr -> type = SSH_FILEXFER_TYPE_UNKNOWN;
                break;
            }
        }
        if (flags & SSH_FILEXFER_ATTR_ACCESSTIME)
        {
            if (buffer_get_u64 (buf , & attr -> atime64) != 8)
            {
                break;
            }
            attr -> atime64 = ntohll (attr -> atime64);
        }
        if (flags & SSH_FILEXFER_ATTR_SUBSECOND_TIMES)
        {
            if (buffer_get_u32 (buf , & attr -> atime_nseconds) != 4)
            {
                break;
            }
            attr -> atime_nseconds = ntohl (attr -> atime_nseconds);
        }
        if (flags & SSH_FILEXFER_ATTR_CREATETIME)
        {
            if (buffer_get_u64 (buf , & attr -> createtime) != 8)
            {
                break;
            }
            attr -> createtime = ntohll (attr -> createtime);
        }
        if (flags & SSH_FILEXFER_ATTR_SUBSECOND_TIMES)
        {
            if (buffer_get_u32 (buf , & attr -> createtime_nseconds) != 4)
            {
                break;
            }
            attr -> createtime_nseconds = ntohl (attr -> createtime_nseconds);
        }
        if (flags & SSH_FILEXFER_ATTR_MODIFYTIME)
        {
            if (buffer_get_u64 (buf , & attr -> mtime64) != 8)
            {
                break;
            }
            attr -> mtime64 = ntohll (attr -> mtime64);
        }
        if (flags & SSH_FILEXFER_ATTR_SUBSECOND_TIMES)
        {
            if (buffer_get_u32 (buf , & attr -> mtime_nseconds) != 4)
            {
                break;
            }
            attr -> mtime_nseconds = ntohl (attr -> mtime_nseconds);
        }
        if (flags & SSH_FILEXFER_ATTR_ACL)
        {
            if ((attr -> acl = buffer_get_ssh_string (buf)) == NULL)
            {
                break;
            }
        }
        if (flags & SSH_FILEXFER_ATTR_EXTENDED)
        {
            if (buffer_get_u32 (buf , & attr -> extended_count) != 4)
            {
                break;
            }
            attr -> extended_count = ntohl (attr -> extended_count);
            while (attr -> extended_count && (attr -> extended_type = buffer_get_ssh_string (buf)) && (attr -> extended_data = buffer_get_ssh_string (buf)))
            {
                attr -> extended_count --;
            }
            if (attr -> extended_count)
            {
                break;
            }
        }
        ok = 1;
    }
    while (0);
    if (ok == 0)
    {
        string_free (owner);
        string_free (group);
        string_free (attr -> acl);
        string_free (attr -> extended_type);
        string_free (attr -> extended_data);
        SAFE_FREE (attr -> owner);
        SAFE_FREE (attr -> group);
        SAFE_FREE (attr);
        ssh_set_error (sftp -> session , SSH_FATAL , "Invalid ATTR structure");
        return NULL;
    }
    return attr;
}

// defined(WITH_SFTP)
static SFTP_ATTRIBUTES* sftp_parse_attr_3_V0 (SFTP_SESSION* sftp , ssh_buffer buf , int expectname)
{
    ssh_string longname;
    longname = NULL;
    ssh_string name;
    name = NULL;
    SFTP_ATTRIBUTES* attr;
    uint32_t flags;
    flags = 0;
    int ok;
    ok = 0;
    attr = malloc (sizeof (SFTP_ATTRIBUTES));
    if (attr == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (attr);
    do
    {
        if (expectname)
        {
            if ((name = buffer_get_ssh_string (buf)) == NULL || (attr -> name = string_to_char (name)) == NULL)
            {
                break;
            }
            string_free (name);
            ssh_log (sftp -> session , SSH_LOG_RARE , "Name: %s" , attr -> name);
            if ((longname = buffer_get_ssh_string (buf)) == NULL || (attr -> longname = string_to_char (longname)) == NULL)
            {
                break;
            }
            string_free (longname);
        }
        if (buffer_get_u32 (buf , & flags) != sizeof (uint32_t))
        {
            break;
        }
        flags = ntohl (flags);
        attr -> flags = flags;
        ssh_log (sftp -> session , SSH_LOG_RARE , "Flags: %.8lx\n" , (long unsigned int) flags);
        if (flags & SSH_FILEXFER_ATTR_SIZE)
        {
            if (buffer_get_u64 (buf , & attr -> size) != sizeof (uint64_t))
            {
                break;
            }
            attr -> size = ntohll (attr -> size);
            ssh_log (sftp -> session , SSH_LOG_RARE , "Size: %llu\n" , (long long unsigned int) attr -> size);
        }
        if (flags & SSH_FILEXFER_ATTR_UIDGID)
        {
            if (buffer_get_u32 (buf , & attr -> uid) != sizeof (uint32_t))
            {
                break;
            }
            if (buffer_get_u32 (buf , & attr -> gid) != sizeof (uint32_t))
            {
                break;
            }
            attr -> uid = ntohl (attr -> uid);
            attr -> gid = ntohl (attr -> gid);
        }
        if (flags & SSH_FILEXFER_ATTR_PERMISSIONS)
        {
            if (buffer_get_u32 (buf , & attr -> permissions) != sizeof (uint32_t))
            {
                break;
            }
            attr -> permissions = ntohl (attr -> permissions);
            switch (attr -> permissions & S_IFMT)
            {
                case S_IFSOCK : case S_IFBLK : case S_IFCHR : case S_IFIFO : attr -> type = SSH_FILEXFER_TYPE_SPECIAL;
                break;
                case S_IFLNK : attr -> type = SSH_FILEXFER_TYPE_SYMLINK;
                break;
                case S_IFREG : attr -> type = SSH_FILEXFER_TYPE_REGULAR;
                break;
                case S_IFDIR : attr -> type = SSH_FILEXFER_TYPE_DIRECTORY;
                break;
                default : attr -> type = SSH_FILEXFER_TYPE_UNKNOWN;
                break;
            }
        }
        if (flags & SSH_FILEXFER_ATTR_ACMODTIME)
        {
            if (buffer_get_u32 (buf , & attr -> atime) != sizeof (uint32_t))
            {
                break;
            }
            attr -> atime = ntohl (attr -> atime);
            if (buffer_get_u32 (buf , & attr -> mtime) != sizeof (uint32_t))
            {
                break;
            }
            attr -> mtime = ntohl (attr -> mtime);
        }
        if (flags & SSH_FILEXFER_ATTR_EXTENDED)
        {
            if (buffer_get_u32 (buf , & attr -> extended_count) != sizeof (uint32_t))
            {
                break;
            }
            attr -> extended_count = ntohl (attr -> extended_count);
            while (attr -> extended_count && (attr -> extended_type = buffer_get_ssh_string (buf)) && (attr -> extended_data = buffer_get_ssh_string (buf)))
            {
                attr -> extended_count --;
            }
            if (attr -> extended_count)
            {
                break;
            }
        }
        ok = 1;
    }
    while (0);
    if (!ok)
    {
        string_free (name);
        string_free (longname);
        string_free (attr -> extended_type);
        string_free (attr -> extended_data);
        SAFE_FREE (attr -> name);
        SAFE_FREE (attr -> longname);
        SAFE_FREE (attr);
        ssh_set_error (sftp -> session , SSH_FATAL , "Invalid ATTR structure");
        return NULL;
    }
    return attr;
}

// defined(WITH_SFTP)
int buffer_add_attributes_V0 (ssh_buffer buffer , SFTP_ATTRIBUTES* attr)
{
    uint32_t flags;
    flags = (
             attr
             ? attr -> flags
             : 0);
    flags &= (SSH_FILEXFER_ATTR_SIZE | SSH_FILEXFER_ATTR_UIDGID | SSH_FILEXFER_ATTR_PERMISSIONS | SSH_FILEXFER_ATTR_ACMODTIME);
    if (buffer_add_u32 (buffer , htonl (flags)) < 0)
    {
        return - 1;
    }
    if (attr)
    {
        if (flags & SSH_FILEXFER_ATTR_SIZE)
        {
            if (buffer_add_u64 (buffer , htonll (attr -> size)) < 0)
            {
                return - 1;
            }
        }
        if (flags & SSH_FILEXFER_ATTR_UIDGID)
        {
            if (buffer_add_u32 (buffer , htonl (attr -> uid)) < 0 || buffer_add_u32 (buffer , htonl (attr -> gid)) < 0)
            {
                return - 1;
            }
        }
        if (flags & SSH_FILEXFER_ATTR_PERMISSIONS)
        {
            if (buffer_add_u32 (buffer , htonl (attr -> permissions)) < 0)
            {
                return - 1;
            }
        }
        if (flags & SSH_FILEXFER_ATTR_ACMODTIME)
        {
            if (buffer_add_u32 (buffer , htonl (attr -> atime)) < 0 || buffer_add_u32 (buffer , htonl (attr -> mtime)) < 0)
            {
                return - 1;
            }
        }
    }
    return 0;
}

// defined(WITH_SFTP)
SFTP_ATTRIBUTES* sftp_parse_attr_V0 (SFTP_SESSION* session , ssh_buffer buf , int expectname)
{
    switch (session -> version)
    {
        case 4 : return sftp_parse_attr_4_V0 (session , buf , expectname);
        case 3 : return sftp_parse_attr_3_V0 (session , buf , expectname);
        default : ssh_set_error (session -> session , SSH_FATAL , "Version %d unsupported by client" , session -> server_version);
        return NULL;
    }
    return NULL;
}

// defined(WITH_SFTP)
int sftp_server_version_V0 (SFTP_SESSION* sftp)
{
    return sftp -> server_version;
}

// defined(WITH_SFTP)
SFTP_ATTRIBUTES* sftp_readdir_V0 (SFTP_SESSION* sftp , SFTP_DIR* dir)
{
    SFTP_MESSAGE* msg;
    msg = NULL;
    STATUS_MESSAGE* status;
    SFTP_ATTRIBUTES* attr;
    ssh_buffer payload;
    uint32_t id;
    if (dir -> buffer == NULL)
    {
        payload = buffer_new ();
        if (payload == NULL)
        {
            return NULL;
        }
        id = sftp_get_new_id_V0 (sftp);
        if (buffer_add_u32 (payload , id) < 0 || buffer_add_ssh_string (payload , dir -> handle) < 0)
        {
            buffer_free (payload);
            return NULL;
        }
        if (sftp_packet_write_V0 (sftp , SSH_FXP_READDIR , payload) < 0)
        {
            buffer_free (payload);
            return NULL;
        }
        buffer_free (payload);
        ssh_log (sftp -> session , SSH_LOG_PACKET , "Sent a ssh_fxp_readdir with id %d" , id);
        while (msg == NULL)
        {
            if (sftp_read_and_dispatch_V0 (sftp) < 0)
            {
                return NULL;
            }
            msg = sftp_dequeue_V0 (sftp , id);
        }
        switch (msg -> packet_type)
        {
            case SSH_FXP_STATUS : status = parse_status_msg_V0 (msg);
            sftp_message_free_V0 (msg);
            if (status == NULL)
            {
                return NULL;
            }
            sftp_set_error_V0 (sftp , status -> status);
            switch (status -> status)
            {
                case SSH_FX_EOF : dir -> eof = 1;
                status_msg_free_V0 (status);
                return NULL;
                default : break;
            }
            ssh_set_error (sftp -> session , SSH_FATAL , "Unknown error status: %d" , status -> status);
            status_msg_free_V0 (status);
            return NULL;
            case SSH_FXP_NAME : buffer_get_u32 (msg -> payload , & dir -> count);
            dir -> count = ntohl (dir -> count);
            dir -> buffer = msg -> payload;
            msg -> payload = NULL;
            sftp_message_free_V0 (msg);
            break;
            default : ssh_set_error (sftp -> session , SSH_FATAL , "Unsupported message back %d" , msg -> packet_type);
            sftp_message_free_V0 (msg);
            return NULL;
        }
    }
    if (dir -> count == 0)
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Count of files sent by the server is zero, which is invalid, or " "libsftp bug");
        return NULL;
    }
    ssh_log (sftp -> session , SSH_LOG_RARE , "Count is %d" , dir -> count);
    attr = sftp_parse_attr_V0 (sftp , dir -> buffer , 1);
    if (attr == NULL)
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Couldn't parse the SFTP attributes");
        return NULL;
    }
    dir -> count --;
    if (dir -> count == 0)
    {
        buffer_free (dir -> buffer);
        dir -> buffer = NULL;
    }
    return attr;
}

// defined(WITH_SFTP)
int sftp_dir_eof_V0 (SFTP_DIR* dir)
{
    return dir -> eof;
}

// defined(WITH_SFTP)
void sftp_attributes_free_V0 (SFTP_ATTRIBUTES* file)
{
    if (file == NULL)
    {
        return;
    }
    string_free (file -> acl);
    string_free (file -> extended_data);
    string_free (file -> extended_type);
    SAFE_FREE (file -> name);
    SAFE_FREE (file -> longname);
    SAFE_FREE (file -> group);
    SAFE_FREE (file -> owner);
    SAFE_FREE (file);
}

// defined(WITH_SFTP)
static int sftp_handle_close_V0 (SFTP_SESSION* sftp , ssh_string handle)
{
    STATUS_MESSAGE* status;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_buffer buffer;
    buffer = NULL;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , handle) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_CLOSE , buffer) < 0)
    {
        buffer_free (buffer);
        return - 1;
    }
    buffer_free (buffer);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    switch (msg -> packet_type)
    {
        case SSH_FXP_STATUS : status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_OK : status_msg_free_V0 (status);
            return 0;
            break;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
        default : ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d during sftp_handle_close!" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_file_close_V0 (SFTP_FILE* file)
{
    return sftp_close (file);
}

// defined(WITH_SFTP)
int sftp_close_V0 (SFTP_FILE* file)
{
    int err;
    err = SSH_NO_ERROR;
    SAFE_FREE (file -> name);
    if (file -> handle)
    {
        err = sftp_handle_close_V0 (file -> sftp , file -> handle);
        string_free (file -> handle);
    }
    SAFE_FREE (file);
    return err;
}

// defined(WITH_SFTP)
int sftp_dir_close_V0 (SFTP_DIR* dir)
{
    return sftp_closedir (dir);
}

// defined(WITH_SFTP)
int sftp_closedir_V0 (SFTP_DIR* dir)
{
    int err;
    err = SSH_NO_ERROR;
    SAFE_FREE (dir -> name);
    if (dir -> handle)
    {
        err = sftp_handle_close_V0 (dir -> sftp , dir -> handle);
        string_free (dir -> handle);
    }
    buffer_free (dir -> buffer);
    SAFE_FREE (dir);
    return err;
}

// defined(WITH_SFTP)
SFTP_FILE* sftp_open_V0 (SFTP_SESSION* sftp , const char* file , int flags , mode_t mode)
{
    SFTP_MESSAGE* msg;
    msg = NULL;
    STATUS_MESSAGE* status;
    SFTP_ATTRIBUTES attr;
    SFTP_FILE* handle;
    ssh_string filename;
    ssh_buffer buffer;
    uint32_t sftp_flags;
    sftp_flags = 0;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return NULL;
    }
    filename = string_from_char (file);
    if (filename == NULL)
    {
        buffer_free (buffer);
        return NULL;
    }
    ZERO_STRUCT (attr);
    attr . permissions = mode;
    attr . flags = SSH_FILEXFER_ATTR_PERMISSIONS;
    if (flags == O_RDONLY)
         sftp_flags |= SSH_FXF_READ;
    if (flags & O_WRONLY)
         sftp_flags |= SSH_FXF_WRITE;
    if (flags & O_RDWR)
         sftp_flags |= (SSH_FXF_WRITE | SSH_FXF_READ);
    if (flags & O_CREAT)
         sftp_flags |= SSH_FXF_CREAT;
    if (flags & O_TRUNC)
         sftp_flags |= SSH_FXF_TRUNC;
    if (flags & O_EXCL)
         sftp_flags |= SSH_FXF_EXCL;
    ssh_log (sftp -> session , SSH_LOG_PACKET , "Opening file %s with sftp flags %x" , file , sftp_flags);
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , filename) < 0)
    {
        buffer_free (buffer);
        string_free (filename);
        return NULL;
    }
    string_free (filename);
    if (buffer_add_u32 (buffer , htonl (sftp_flags)) < 0 || buffer_add_attributes_V0 (buffer , & attr) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_OPEN , buffer) < 0)
    {
        buffer_free (buffer);
        return NULL;
    }
    buffer_free (buffer);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    switch (msg -> packet_type)
    {
        case SSH_FXP_STATUS : status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return NULL;
        }
        sftp_set_error_V0 (sftp , status -> status);
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return NULL;
        case SSH_FXP_HANDLE : handle = parse_handle_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        return handle;
        default : ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d during open!" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return NULL;
}

// defined(WITH_SFTP)
void sftp_file_set_nonblocking_V0 (SFTP_FILE* handle)
{
    handle -> nonblocking = 1;
}

// defined(WITH_SFTP)
void sftp_file_set_blocking_V0 (SFTP_FILE* handle)
{
    handle -> nonblocking = 0;
}

// defined(WITH_SFTP)
ssize_t sftp_read_V0 (SFTP_FILE* handle , void* buf , size_t count)
{
    SFTP_SESSION* sftp;
    sftp = handle -> sftp;
    SFTP_MESSAGE* msg;
    msg = NULL;
    STATUS_MESSAGE* status;
    ssh_string datastring;
    ssh_buffer buffer;
    int id;
    if (handle -> eof)
    {
        return 0;
    }
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    id = sftp_get_new_id_V0 (handle -> sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , handle -> handle) < 0 || buffer_add_u64 (buffer , htonll (handle -> offset)) < 0 || buffer_add_u32 (buffer , htonl (count)) < 0 || sftp_packet_write_V0 (handle -> sftp , SSH_FXP_READ , buffer) < 0)
    {
        buffer_free (buffer);
        return - 1;
    }
    buffer_free (buffer);
    while (msg == NULL)
    {
        if (handle -> nonblocking)
        {
            if (channel_poll (handle -> sftp -> channel , 0) == 0)
            {
                return 0;
            }
        }
        if (sftp_read_and_dispatch_V0 (handle -> sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (handle -> sftp , id);
    }
    switch (msg -> packet_type)
    {
        case SSH_FXP_STATUS : status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_EOF : handle -> eof = 1;
            status_msg_free_V0 (status);
            return 0;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
        case SSH_FXP_DATA : datastring = buffer_get_ssh_string (msg -> payload);
        sftp_message_free_V0 (msg);
        if (datastring == NULL)
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received invalid DATA packet from sftp server");
            return - 1;
        }
        if (string_len (datastring) > count)
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received a too big DATA packet from sftp server: " "%zu and asked for %zu" , string_len (datastring) , count);
            string_free (datastring);
            return - 1;
        }
        count = string_len (datastring);
        handle -> offset += count;
        memcpy (buf , datastring -> string , count);
        string_free (datastring);
        return count;
        default : ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d during read!" , msg -> packet_type);
        sftp_message_free_V0 (msg);
        return - 1;
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_async_read_begin_V0 (SFTP_FILE* file , uint32_t len)
{
    SFTP_SESSION* sftp;
    sftp = file -> sftp;
    ssh_buffer buffer;
    uint32_t id;
    _enter_function (sftp -> channel -> session);
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , file -> handle) < 0 || buffer_add_u64 (buffer , htonll (file -> offset)) < 0 || buffer_add_u32 (buffer , htonl (len)) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_READ , buffer) < 0)
    {
        buffer_free (buffer);
        return - 1;
    }
    buffer_free (buffer);
    file -> offset += len;
    _leave_function (sftp -> channel -> session);
    return id;
}

// defined(WITH_SFTP)
int sftp_async_read_V0 (SFTP_FILE* file , void* data , uint32_t size , uint32_t id)
{
    SFTP_SESSION* sftp;
    sftp = file -> sftp;
    SFTP_MESSAGE* msg;
    msg = NULL;
    STATUS_MESSAGE* status;
    ssh_string datastring;
    int err;
    err = SSH_OK;
    uint32_t len;
    _enter_function (sftp -> channel -> session);
    if (file -> eof)
    {
        _leave_function (sftp -> channel -> session);
        return 0;
    }
    while (msg == NULL)
    {
        if (file -> nonblocking)
        {
            if (channel_poll (sftp -> channel , 0) == 0)
            {
                return SSH_AGAIN;
            }
        }
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            _leave_function (sftp -> channel -> session);
            return SSH_ERROR;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    switch (msg -> packet_type)
    {
        case SSH_FXP_STATUS : status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            _leave_function (sftp -> channel -> session);
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        if (status -> status != SSH_FX_EOF)
        {
            ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server : %s" , status -> errormsg);
            _leave_function (sftp -> channel -> session);
            err = SSH_ERROR;
        }
        else
        {
            file -> eof = 1;
        }
        status_msg_free_V0 (status);
        _leave_function (sftp -> channel -> session);
        return err;
        case SSH_FXP_DATA : datastring = buffer_get_ssh_string (msg -> payload);
        sftp_message_free_V0 (msg);
        if (datastring == NULL)
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received invalid DATA packet from sftp server");
            _leave_function (sftp -> channel -> session);
            return SSH_ERROR;
        }
        if (string_len (datastring) > size)
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received a too big DATA packet from sftp server: " "%zu and asked for %u" , string_len (datastring) , size);
            string_free (datastring);
            _leave_function (sftp -> channel -> session);
            return SSH_ERROR;
        }
        len = string_len (datastring);
        memcpy (data , datastring -> string , len);
        string_free (datastring);
        _leave_function (sftp -> channel -> session);
        return len;
        default : ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d during read!" , msg -> packet_type);
        sftp_message_free_V0 (msg);
        _leave_function (sftp -> channel -> session);
        return SSH_ERROR;
    }
    _leave_function (sftp -> channel -> session);
    return SSH_ERROR;
}

// defined(WITH_SFTP)
ssize_t sftp_write_V0 (SFTP_FILE* file , const void* buf , size_t count)
{
    SFTP_SESSION* sftp;
    sftp = file -> sftp;
    SFTP_MESSAGE* msg;
    msg = NULL;
    STATUS_MESSAGE* status;
    ssh_string datastring;
    ssh_buffer buffer;
    uint32_t id;
    int len;
    int packetlen;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    datastring = string_new (count);
    if (datastring == NULL)
    {
        buffer_free (buffer);
        return - 1;
    }
    string_fill (datastring , buf , count);
    id = sftp_get_new_id_V0 (file -> sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , file -> handle) < 0 || buffer_add_u64 (buffer , htonll (file -> offset)) < 0 || buffer_add_ssh_string (buffer , datastring) < 0)
    {
        buffer_free (buffer);
        string_free (datastring);
        return - 1;
    }
    string_free (datastring);
    len = sftp_packet_write_V0 (file -> sftp , SSH_FXP_WRITE , buffer);
    packetlen = buffer_get_len (buffer);
    buffer_free (buffer);
    if (len < 0)
    {
        return - 1;
    }
    else
    {
        if (len != packetlen)
        {
            ssh_log (sftp -> session , SSH_LOG_PACKET , "Could not write as much data as expected");
        }
    }
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (file -> sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (file -> sftp , id);
    }
    switch (msg -> packet_type)
    {
        case SSH_FXP_STATUS : status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_OK : file -> offset += count;
            status_msg_free_V0 (status);
            return count;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        file -> offset += count;
        status_msg_free_V0 (status);
        return - 1;
        default : ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d during write!" , msg -> packet_type);
        sftp_message_free_V0 (msg);
        return - 1;
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_seek_V0 (SFTP_FILE* file , uint32_t new_offset)
{
    if (file == NULL)
    {
        return - 1;
    }
    file -> offset = new_offset;
    return 0;
}

// defined(WITH_SFTP)
int sftp_seek64_V0 (SFTP_FILE* file , uint64_t new_offset)
{
    if (file == NULL)
    {
        return - 1;
    }
    file -> offset = new_offset;
    return 0;
}

// defined(WITH_SFTP)
unsigned long sftp_tell_V0 (SFTP_FILE* file)
{
    return (unsigned long) file -> offset;
}

// defined(WITH_SFTP)
uint64_t sftp_tell64_V0 (SFTP_FILE* file)
{
    return (uint64_t) file -> offset;
}

// defined(WITH_SFTP)
void sftp_rewind_V0 (SFTP_FILE* file)
{
    file -> offset = 0;
}

// defined(WITH_SFTP)
int sftp_rm_V0 (SFTP_SESSION* sftp , const char* file)
{
    return sftp_unlink (sftp , file);
}

// defined(WITH_SFTP)
int sftp_unlink_V0 (SFTP_SESSION* sftp , const char* file)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_string filename;
    ssh_buffer buffer;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    filename = string_from_char (file);
    if (filename == NULL)
    {
        buffer_free (buffer);
        return - 1;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , filename) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_REMOVE , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (filename);
    }
    string_free (filename);
    buffer_free (buffer);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp))
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_STATUS)
    {
        status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_OK : status_msg_free_V0 (status);
            return 0;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
    }
    else
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to remove file" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_rmdir_V0 (SFTP_SESSION* sftp , const char* directory)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_string filename;
    ssh_buffer buffer;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    filename = string_from_char (directory);
    if (filename == NULL)
    {
        buffer_free (buffer);
        return - 1;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , filename) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_RMDIR , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (filename);
        return - 1;
    }
    buffer_free (buffer);
    string_free (filename);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_STATUS)
    {
        status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_OK : status_msg_free_V0 (status);
            return 0;
            break;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
    }
    else
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to remove directory" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_mkdir_V0 (SFTP_SESSION* sftp , const char* directory , mode_t mode)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    SFTP_ATTRIBUTES* errno_attr;
    errno_attr = NULL;
    SFTP_ATTRIBUTES attr;
    ssh_buffer buffer;
    ssh_string path;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    path = string_from_char (directory);
    if (path == NULL)
    {
        buffer_free (buffer);
        return - 1;
    }
    ZERO_STRUCT (attr);
    attr . permissions = mode;
    attr . flags = SSH_FILEXFER_ATTR_PERMISSIONS;
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , path) < 0 || buffer_add_attributes_V0 (buffer , & attr) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_MKDIR , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (path);
    }
    buffer_free (buffer);
    string_free (path);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_STATUS)
    {
        status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_FAILURE : errno_attr = sftp_lstat (sftp , directory);
            if (errno_attr != NULL)
            {
                SAFE_FREE (errno_attr);
                sftp_set_error_V0 (sftp , SSH_FX_FILE_ALREADY_EXISTS);
            }
            break;
            case SSH_FX_OK : status_msg_free_V0 (status);
            return 0;
            break;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
    }
    else
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to make directory" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_rename_V0 (SFTP_SESSION* sftp , const char* original , const char* newname)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_buffer buffer;
    ssh_string oldpath;
    ssh_string newpath;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    oldpath = string_from_char (original);
    if (oldpath == NULL)
    {
        buffer_free (buffer);
        return - 1;
    }
    newpath = string_from_char (newname);
    if (newpath == NULL)
    {
        buffer_free (buffer);
        string_free (oldpath);
        return - 1;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , oldpath) < 0 || buffer_add_ssh_string (buffer , newpath) < 0 || buffer_add_u32 (buffer , SSH_FXF_RENAME_OVERWRITE) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_RENAME , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (oldpath);
        string_free (newpath);
        return - 1;
    }
    buffer_free (buffer);
    string_free (oldpath);
    string_free (newpath);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_STATUS)
    {
        status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_OK : status_msg_free_V0 (status);
            return 0;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
    }
    else
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to rename" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_setstat_V0 (SFTP_SESSION* sftp , const char* file , SFTP_ATTRIBUTES* attr)
{
    uint32_t id;
    id = sftp_get_new_id_V0 (sftp);
    ssh_buffer buffer;
    buffer = buffer_new ();
    ssh_string path;
    path = string_from_char (file);
    SFTP_MESSAGE* msg;
    msg = NULL;
    STATUS_MESSAGE* status;
    status = NULL;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    path = string_from_char (file);
    if (path == NULL)
    {
        buffer_free (buffer);
        return - 1;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , path) < 0 || buffer_add_attributes_V0 (buffer , attr) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_SETSTAT , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (path);
        return - 1;
    }
    buffer_free (buffer);
    string_free (path);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_STATUS)
    {
        status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_OK : status_msg_free_V0 (status);
            return 0;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
    }
    else
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to set stats" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return - 1;
}

// defined(WITH_SFTP)
int sftp_chown_V0 (SFTP_SESSION* sftp , const char* file , uid_t owner , gid_t group)
{
    SFTP_ATTRIBUTES attr;
    ZERO_STRUCT (attr);
    attr . uid = owner;
    attr . gid = group;
    attr . flags = SSH_FILEXFER_ATTR_OWNERGROUP;
    return sftp_setstat_V0 (sftp , file , & attr);
}

// defined(WITH_SFTP)
int sftp_chmod_V0 (SFTP_SESSION* sftp , const char* file , mode_t mode)
{
    SFTP_ATTRIBUTES attr;
    ZERO_STRUCT (attr);
    attr . permissions = mode;
    attr . flags = SSH_FILEXFER_ATTR_PERMISSIONS;
    return sftp_setstat_V0 (sftp , file , & attr);
}

// defined(WITH_SFTP)
int sftp_utimes_V0 (SFTP_SESSION* sftp , const char* file , const struct timeval* times)
{
    SFTP_ATTRIBUTES attr;
    ZERO_STRUCT (attr);
    attr . atime = times [ 0 ] . tv_sec;
    attr . atime_nseconds = times [ 0 ] . tv_usec;
    attr . mtime = times [ 1 ] . tv_sec;
    attr . mtime_nseconds = times [ 1 ] . tv_usec;
    attr . flags |= SSH_FILEXFER_ATTR_ACCESSTIME | SSH_FILEXFER_ATTR_MODIFYTIME | SSH_FILEXFER_ATTR_SUBSECOND_TIMES;
    return sftp_setstat_V0 (sftp , file , & attr);
}

// defined(WITH_SFTP)
int sftp_symlink_V0 (SFTP_SESSION* sftp , const char* target , const char* dest)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_string target_s;
    ssh_string dest_s;
    ssh_buffer buffer;
    uint32_t id;
    if (sftp == NULL || target == NULL || dest == NULL)
    {
        return - 1;
    }
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return - 1;
    }
    target_s = string_from_char (target);
    if (target_s == NULL)
    {
        buffer_free (buffer);
        return - 1;
    }
    dest_s = string_from_char (dest);
    if (dest_s == NULL)
    {
        string_free (target_s);
        buffer_free (buffer);
        return - 1;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0)
    {
        buffer_free (buffer);
        string_free (dest_s);
        string_free (target_s);
        return - 1;
    }
    if (ssh_get_openssh_version (sftp -> session))
    {
        if (buffer_add_ssh_string (buffer , target_s) < 0 || buffer_add_ssh_string (buffer , dest_s) < 0)
        {
            buffer_free (buffer);
            string_free (dest_s);
            string_free (target_s);
            return - 1;
        }
    }
    else
    {
        if (buffer_add_ssh_string (buffer , dest_s) < 0 || buffer_add_ssh_string (buffer , target_s) < 0)
        {
            buffer_free (buffer);
            string_free (dest_s);
            string_free (target_s);
            return - 1;
        }
    }
    if (sftp_packet_write_V0 (sftp , SSH_FXP_SYMLINK , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (dest_s);
        string_free (target_s);
        return - 1;
    }
    buffer_free (buffer);
    string_free (dest_s);
    string_free (target_s);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return - 1;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_STATUS)
    {
        status = parse_status_msg_V0 (msg);
        sftp_message_free_V0 (msg);
        if (status == NULL)
        {
            return - 1;
        }
        sftp_set_error_V0 (sftp , status -> status);
        switch (status -> status)
        {
            case SSH_FX_OK : status_msg_free_V0 (status);
            return 0;
            default : break;
        }
        ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
        status_msg_free_V0 (status);
        return - 1;
    }
    else
    {
        ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to set stats" , msg -> packet_type);
        sftp_message_free_V0 (msg);
    }
    return - 1;
}

// defined(WITH_SFTP)
char* sftp_readlink_V0 (SFTP_SESSION* sftp , const char* path)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_string path_s;
    path_s = NULL;
    ssh_string link_s;
    link_s = NULL;
    ssh_buffer buffer;
    char* lnk;
    uint32_t ignored;
    uint32_t id;
    if (sftp == NULL || path == NULL)
    {
        return NULL;
    }
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return NULL;
    }
    path_s = string_from_char (path);
    if (path_s == NULL)
    {
        buffer_free (buffer);
        return NULL;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , path_s) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_READLINK , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (path_s);
        return NULL;
    }
    buffer_free (buffer);
    string_free (path_s);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_NAME)
    {
        buffer_get_u32 (msg -> payload , & ignored);
        link_s = buffer_get_ssh_string (msg -> payload);
        sftp_message_free_V0 (msg);
        if (link_s == NULL)
        {
            return NULL;
        }
        lnk = string_to_char (link_s);
        string_free (link_s);
        return lnk;
    }
    else
    {
        if (msg -> packet_type == SSH_FXP_STATUS)
        {
            status = parse_status_msg_V0 (msg);
            sftp_message_free_V0 (msg);
            if (status == NULL)
            {
                return NULL;
            }
            ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
            status_msg_free_V0 (status);
        }
        else
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to set stats" , msg -> packet_type);
            sftp_message_free_V0 (msg);
        }
    }
    return NULL;
}

// defined(WITH_SFTP)
static SFTP_STATVFS* sftp_parse_statvfs_V0 (SFTP_SESSION* sftp , ssh_buffer buf)
{
    SFTP_STATVFS* statvfs;
    uint64_t tmp;
    int ok;
    ok = 0;
    statvfs = malloc (sizeof (SFTP_STATVFS));
    if (statvfs == NULL)
    {
        return NULL;
    }
    ZERO_STRUCTP (statvfs);
    do
    {
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_bsize = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_frsize = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_blocks = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_bfree = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_bavail = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_files = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_ffree = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_favail = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_fsid = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_flag = ntohll (tmp);
        if (buffer_get_u64 (buf , & tmp) != sizeof (uint64_t))
        {
            break;
        }
        statvfs -> f_namemax = ntohll (tmp);
        ok = 1;
    }
    while (0);
    if (!ok)
    {
        SAFE_FREE (statvfs);
        ssh_set_error (sftp -> session , SSH_FATAL , "Invalid statvfs structure");
        return NULL;
    }
    return statvfs;
}

// defined(WITH_SFTP)
SFTP_STATVFS* sftp_statvfs_V0 (SFTP_SESSION* sftp , const char* path)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_string pathstr;
    ssh_string ext;
    ssh_buffer buffer;
    uint32_t id;
    if (sftp == NULL || path == NULL)
    {
        return NULL;
    }
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return NULL;
    }
    ext = string_from_char ("statvfs@openssh.com");
    if (ext == NULL)
    {
        buffer_free (buffer);
        return NULL;
    }
    pathstr = string_from_char (path);
    if (pathstr == NULL)
    {
        buffer_free (buffer);
        string_free (ext);
        return NULL;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , ext) < 0 || buffer_add_ssh_string (buffer , pathstr) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_EXTENDED , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (ext);
        string_free (pathstr);
        return NULL;
    }
    buffer_free (buffer);
    string_free (ext);
    string_free (pathstr);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_EXTENDED_REPLY)
    {
        SFTP_STATVFS* buf;
        buf = sftp_parse_statvfs_V0 (sftp , msg -> payload);
        sftp_message_free_V0 (msg);
        if (buf == NULL)
        {
            return NULL;
        }
        return buf;
    }
    else
    {
        if (msg -> packet_type == SSH_FXP_STATUS)
        {
            status = parse_status_msg_V0 (msg);
            sftp_message_free_V0 (msg);
            if (status == NULL)
            {
                return NULL;
            }
            ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
            status_msg_free_V0 (status);
        }
        else
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to get statvfs" , msg -> packet_type);
            sftp_message_free_V0 (msg);
        }
    }
    return NULL;
}

// defined(WITH_SFTP)
SFTP_STATVFS* sftp_fstatvfs_V0 (SFTP_FILE* file)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    SFTP_SESSION* sftp;
    ssh_string ext;
    ssh_buffer buffer;
    uint32_t id;
    if (sftp == NULL || file == NULL)
    {
        return NULL;
    }
    sftp = file -> sftp;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return NULL;
    }
    ext = string_from_char ("fstatvfs@openssh.com");
    if (ext == NULL)
    {
        buffer_free (buffer);
        return NULL;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , ext) < 0 || buffer_add_ssh_string (buffer , file -> handle) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_EXTENDED , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (ext);
        return NULL;
    }
    buffer_free (buffer);
    string_free (ext);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_EXTENDED_REPLY)
    {
        SFTP_STATVFS* buf;
        buf = sftp_parse_statvfs_V0 (sftp , msg -> payload);
        sftp_message_free_V0 (msg);
        if (buf == NULL)
        {
            return NULL;
        }
        return buf;
    }
    else
    {
        if (msg -> packet_type == SSH_FXP_STATUS)
        {
            status = parse_status_msg_V0 (msg);
            sftp_message_free_V0 (msg);
            if (status == NULL)
            {
                return NULL;
            }
            ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
            status_msg_free_V0 (status);
        }
        else
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to set stats" , msg -> packet_type);
            sftp_message_free_V0 (msg);
        }
    }
    return NULL;
}

// defined(WITH_SFTP)
void sftp_statvfs_free_V0 (SFTP_STATVFS* statvfs)
{
    if (statvfs == NULL)
    {
        return;
    }
    SAFE_FREE (statvfs);
}

// defined(WITH_SFTP)
char* sftp_canonicalize_path_V0 (SFTP_SESSION* sftp , const char* path)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_string name;
    name = NULL;
    ssh_string pathstr;
    ssh_buffer buffer;
    char* cname;
    uint32_t ignored;
    uint32_t id;
    if (sftp == NULL || path == NULL)
    {
        return NULL;
    }
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return NULL;
    }
    pathstr = string_from_char (path);
    if (pathstr == NULL)
    {
        buffer_free (buffer);
        return NULL;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , pathstr) < 0 || sftp_packet_write_V0 (sftp , SSH_FXP_REALPATH , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (pathstr);
        return NULL;
    }
    buffer_free (buffer);
    string_free (pathstr);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_NAME)
    {
        buffer_get_u32 (msg -> payload , & ignored);
        name = buffer_get_ssh_string (msg -> payload);
        sftp_message_free_V0 (msg);
        if (name == NULL)
        {
            return NULL;
        }
        cname = string_to_char (name);
        string_free (name);
        return cname;
    }
    else
    {
        if (msg -> packet_type == SSH_FXP_STATUS)
        {
            status = parse_status_msg_V0 (msg);
            sftp_message_free_V0 (msg);
            if (status == NULL)
            {
                return NULL;
            }
            ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
            status_msg_free_V0 (status);
        }
        else
        {
            ssh_set_error (sftp -> session , SSH_FATAL , "Received message %d when attempting to set stats" , msg -> packet_type);
            sftp_message_free_V0 (msg);
        }
    }
    return NULL;
}

// defined(WITH_SFTP)
static SFTP_ATTRIBUTES* sftp_xstat_V0 (SFTP_SESSION* sftp , const char* path , int param)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_string pathstr;
    ssh_buffer buffer;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return NULL;
    }
    pathstr = string_from_char (path);
    if (pathstr == NULL)
    {
        buffer_free (buffer);
        return NULL;
    }
    id = sftp_get_new_id_V0 (sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , pathstr) < 0 || sftp_packet_write_V0 (sftp , param , buffer) < 0)
    {
        buffer_free (buffer);
        string_free (pathstr);
        return NULL;
    }
    buffer_free (buffer);
    string_free (pathstr);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_ATTRS)
    {
        return sftp_parse_attr_V0 (sftp , msg -> payload , 0);
    }
    else
    {
        if (msg -> packet_type == SSH_FXP_STATUS)
        {
            status = parse_status_msg_V0 (msg);
            sftp_message_free_V0 (msg);
            if (status == NULL)
            {
                return NULL;
            }
            sftp_set_error_V0 (sftp , status -> status);
            ssh_set_error (sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
            status_msg_free_V0 (status);
            return NULL;
        }
    }
    ssh_set_error (sftp -> session , SSH_FATAL , "Received mesg %d during stat()" , msg -> packet_type);
    sftp_message_free_V0 (msg);
    return NULL;
}

// defined(WITH_SFTP)
SFTP_ATTRIBUTES* sftp_stat_V0 (SFTP_SESSION* session , const char* path)
{
    return sftp_xstat_V0 (session , path , SSH_FXP_STAT);
}

// defined(WITH_SFTP)
SFTP_ATTRIBUTES* sftp_lstat_V0 (SFTP_SESSION* session , const char* path)
{
    return sftp_xstat_V0 (session , path , SSH_FXP_LSTAT);
}

// defined(WITH_SFTP)
SFTP_ATTRIBUTES* sftp_fstat_V0 (SFTP_FILE* file)
{
    STATUS_MESSAGE* status;
    status = NULL;
    SFTP_MESSAGE* msg;
    msg = NULL;
    ssh_buffer buffer;
    uint32_t id;
    buffer = buffer_new ();
    if (buffer == NULL)
    {
        return NULL;
    }
    id = sftp_get_new_id_V0 (file -> sftp);
    if (buffer_add_u32 (buffer , id) < 0 || buffer_add_ssh_string (buffer , file -> handle) < 0 || sftp_packet_write_V0 (file -> sftp , SSH_FXP_FSTAT , buffer) < 0)
    {
        buffer_free (buffer);
        return NULL;
    }
    buffer_free (buffer);
    while (msg == NULL)
    {
        if (sftp_read_and_dispatch_V0 (file -> sftp) < 0)
        {
            return NULL;
        }
        msg = sftp_dequeue_V0 (file -> sftp , id);
    }
    if (msg -> packet_type == SSH_FXP_ATTRS)
    {
        return sftp_parse_attr_V0 (file -> sftp , msg -> payload , 0);
    }
    else
    {
        if (msg -> packet_type == SSH_FXP_STATUS)
        {
            status = parse_status_msg_V0 (msg);
            sftp_message_free_V0 (msg);
            if (status == NULL)
            {
                return NULL;
            }
            ssh_set_error (file -> sftp -> session , SSH_REQUEST_DENIED , "SFTP server: %s" , status -> errormsg);
            status_msg_free_V0 (status);
            return NULL;
        }
    }
    ssh_set_error (file -> sftp -> session , SSH_FATAL , "Received msg %d during fstat()" , msg -> packet_type);
    sftp_message_free_V0 (msg);
    return NULL;
}