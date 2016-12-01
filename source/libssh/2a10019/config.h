/* Name of package */
#define PACKAGE "libssh"

/* Version number of package */
#define VERSION "0.4.0"

/* #undef LOCALEDIR */
#define DATADIR "/usr/share/libssh"
#define LIBDIR "/usr/lib"
#define PLUGINDIR "/usr/lib/libssh-4"
#define SYSCONFDIR "/usr/etc"
#define BINARYDIR "/home/alex/repos/reconfigurator-examples/Libssh_libssh_sftp_c/libssh-err/build"
#define SOURCEDIR "/home/alex/repos/reconfigurator-examples/Libssh_libssh_sftp_c/libssh-err"

/************************** HEADER FILES *************************/

/* Define to 1 if you have the <pty.h> header file. */
#define HAVE_PTY_H 1

/* Define to 1 if you have the <termios.h> header file. */
/* #undef HAVE_TERMIOS_H */

/* Define to 1 if you have the <openssl/aes.h> header file. */
#define HAVE_OPENSSL_AES_H 1

/* Define to 1 if you have the <wspiapi.h> header file. */
/* #undef HAVE_WSPIAPI_H */

/* Define to 1 if you have the <openssl/blowfish.h> header file. */
#define HAVE_OPENSSL_BLOWFISH_H 1

/* Define to 1 if you have the <openssl/des.h> header file. */
#define HAVE_OPENSSL_DES_H 1

/*************************** FUNCTIONS ***************************/

/* Define to 1 if you have the `cfmakeraw' function. */
#define HAVE_CFMAKERAW 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `regcomp' function. */
#define HAVE_REGCOMP 1

/*************************** LIBRARIES ***************************/

/* Define to 1 if you have the `crypto' library (-lcrypto). */
#define HAVE_LIBCRYPTO 1

/* Define to 1 if you have the `gcrypt' library (-lgcrypt). */
/* #undef HAVE_LIBGCRYPT */

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/**************************** OPTIONS ****************************/

/* Define to 1 if you want to enable ZLIB */
#define WITH_LIBZ 1

/* Define to 1 if you want to enable SSH1 */
//#define WITH_SFTP 1

/* Define to 1 if you want to enable SSH1 */
/* #undef WITH_SSH1 */

/* Define to 1 if you want to enable server support */
//#define WITH_SERVER 1

/* Define to 1 if you want to enable debug output for crypto functions */
/* #undef DEBUG_CRYPTO */

/* Define to 1 if you want to enable calltrace debug output */
#define DEBUG_CALLTRACE 1

/*************************** ENDIAN *****************************/

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
/* #undef WORDS_BIGENDIAN */

/************************* MS Windows ***************************/

#ifdef _WIN32
# ifdef _MSC_VER
/* On Microsoft compilers define inline to __inline on all others use inline */
#  undef inline
#  define inline __inline

#  undef strdup
#  define strdup _strdup
# endif // _MSC_VER
#endif /* _WIN32 */

#define DEPRECATEDIN_0_9_8(x)
#define DEPRECATEDIN_1_1_0(x)

#ifdef RECONFIGURATOR
typedef struct struct_SOCKET SOCKET;
typedef struct struct_size_t size_t;
typedef struct struct_ssize_t ssize_t;
typedef struct struct_fd_set fd_set;
typedef struct struct_nfds_t nfds_t;
typedef struct struct_mode_t mode_t;
typedef struct struct_uid_t uid_t;
typedef struct struct_gid_t gid_t;

typedef struct struct_SHA_CTX SHA_CTX;
typedef struct struct_SHA256_CTX SHA256_CTX;
typedef struct struct_MD5_CTX  MD5_CTX;
typedef struct struct_HMAC_CTX HMAC_CTX;
typedef struct struct_BIGNUM BIGNUM;
typedef struct struct_BN_CTX BN_CTX;


typedef struct struct_off_t off_t;
typedef struct struct_gcry_md_hd_t gcry_md_hd_t;
typedef struct struct_gcry_mpi_t gcry_mpi_t;
typedef struct struct_gcry_cipher_hd_t gcry_cipher_hd_t;
typedef struct struct_gcry_sexp_t gcry_sexp_t;

typedef struct struct_ssh_public_key ssh_public_key;
typedef struct struct_ssh_private_key ssh_private_key;

typedef struct struct_DSA DSA;
typedef struct struct_RSA RSA;
typedef struct struct_DSA_SIG DSA_SIG;
typedef struct struct_EC_KEY EC_KEY;
typedef struct struct_ECDSA_SIG ECDSA_SIG;

typedef struct struct_va_list va_list;
#endif
