#define HAVE_LIBCRYPTO 1

#undef _WIN32

struct ssh_buffer_struct { };

typedef struct ssh_buffer_struct* ssh_buffer;

typedef struct struct_va_list va_list;
typedef struct struct_ssh_key ssh_key;
typedef struct struct_ssh_signature ssh_signature;
typedef struct struct_ssh_auth_callback ssh_auth_callback;
typedef struct struct_off_t off_t;
typedef struct struct_ssh_public_key ssh_public_key;
typedef struct struct_ssh_private_key ssh_private_key;
typedef struct struct_ssh_string ssh_string;
typedef struct struct_size_t size_t;
typedef struct struct_ssh_session ssh_session;
typedef unsigned long int 	uint32_t;
typedef struct struct_SHACTX SHACTX;

struct ssh_signature_struct {};

enum ssh_keytypes_e{
  SSH_KEYTYPE_UNKNOWN=0,
  SSH_KEYTYPE_DSS=1,
  SSH_KEYTYPE_RSA,
  SSH_KEYTYPE_RSA1,
  SSH_KEYTYPE_ECDSA
};

enum ssh_keycmp_e {
  SSH_KEY_CMP_PUBLIC = 0,
  SSH_KEY_CMP_PRIVATE
};

#define DEPRECATEDIN_1_1_0(x)
