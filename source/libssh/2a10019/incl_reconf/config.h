#undef _WIN32

typedef struct sftp_session_struct { } SFTP_SESSION ;
typedef struct { } SFTP_MESSAGE;
typedef struct { } STATUS_MESSAGE;
typedef struct sftp_ext_struct *sftp_ext;
typedef struct ssh_session_struct SSH_SESSION;
typedef struct ssh_channel_struct* ssh_channel;
typedef struct ssh_session_struct* ssh_session;

typedef struct { } SFTP_PACKET;
typedef struct ssh_buffer_struct* ssh_buffer;
typedef unsigned char 	uint8_t;

typedef struct struct_ssh_string ssh_string;

typedef struct request_queue { } REQUEST_QUEUE;
typedef struct sftp_file { } SFTP_FILE ;
typedef struct sftp_dir { } SFTP_DIR;
typedef struct sftp_attributes { } SFTP_ATTRIBUTES;
typedef struct sftp_statvfs_struct { } SFTP_STATVFS;

typedef int mode_t;
typedef long ssize_t;
typedef struct struct_size_t size_t;

typedef unsigned long int uint32_t;
typedef unsigned long long int uint64_t;

typedef uint32_t uid_t;
typedef uint32_t gid_t;

struct timeval { };
