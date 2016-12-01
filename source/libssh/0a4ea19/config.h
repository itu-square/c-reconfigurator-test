/* Name of package */
#define PACKAGE "libssh"

/* Version number of package */
#define VERSION "0.5.90"

/* #undef LOCALEDIR */
#define DATADIR "/usr/share/libssh"
#define LIBDIR "/usr/lib"
#define PLUGINDIR "/usr/lib/libssh-4"
#define SYSCONFDIR "/usr/etc"
#define BINARYDIR "/home/alex/repos/reconfigurator-examples/Libssh_src_pki_c/libssh-err/build"
#define SOURCEDIR "/home/alex/repos/reconfigurator-examples/Libssh_src_pki_c/libssh-err"

/************************** HEADER FILES *************************/

/* Define to 1 if you have the <argp.h> header file. */
#define HAVE_ARGP_H 1

/* Define to 1 if you have the <pty.h> header file. */
#define HAVE_PTY_H 1

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <openssl/aes.h> header file. */
#define HAVE_OPENSSL_AES_H 1

/* Define to 1 if you have the <wspiapi.h> header file. */
/* #undef HAVE_WSPIAPI_H */

/* Define to 1 if you have the <openssl/blowfish.h> header file. */
#define HAVE_OPENSSL_BLOWFISH_H 1

/* Define to 1 if you have the <openssl/des.h> header file. */
#define HAVE_OPENSSL_DES_H 1

/* Define to 1 if you have the <openssl/ecdh.h> header file. */
#define HAVE_OPENSSL_ECDH_H 1

/* Define to 1 if you have the <openssl/ec.h> header file. */
#define HAVE_OPENSSL_EC_H 1

/* Define to 1 if you have the <openssl/ecdsa.h> header file. */
#define HAVE_OPENSSL_ECDSA_H 1

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have eliptic curve cryptography in openssl */
#define HAVE_OPENSSL_ECC 1

/* Define to 1 if you have eliptic curve cryptography in gcrypt */
/* #undef HAVE_GCRYPT_ECC */

/* Define to 1 if you have eliptic curve cryptography */
#define HAVE_ECC 1

/*************************** FUNCTIONS ***************************/

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the `_snprintf' function. */
/* #undef HAVE__SNPRINTF */

/* Define to 1 if you have the `_snprintf_s' function. */
/* #undef HAVE__SNPRINTF_S */

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the `_vsnprintf' function. */
/* #undef HAVE__VSNPRINTF */

/* Define to 1 if you have the `_vsnprintf_s' function. */
/* #undef HAVE__VSNPRINTF_S */

/* Define to 1 if you have the `strncpy' function. */
#define HAVE_STRNCPY 1

/* Define to 1 if you have the `cfmakeraw' function. */
#define HAVE_CFMAKERAW 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 1

/* Define to 1 if you have the `ntohll' function. */
/* #undef HAVE_NTOHLL */

/*************************** LIBRARIES ***************************/

/* Define to 1 if you have the `crypto' library (-lcrypto). */
#define HAVE_LIBCRYPTO 1

/* Define to 1 if you have the `gcrypt' library (-lgcrypt). */
/* #undef HAVE_LIBGCRYPT */

/* Define to 1 if you have the `pthread' library (-lpthread). */
#define HAVE_PTHREAD 1


/**************************** OPTIONS ****************************/

/* Define to 1 if you want to enable ZLIB */
#define WITH_ZLIB 1

/* Define to 1 if you want to enable SFTP */
#define WITH_SFTP 1

/* Define to 1 if you want to enable SSH1 */
/* #undef WITH_SSH1 */

/* Define to 1 if you want to enable server support */
#define WITH_SERVER 1

/* Define to 1 if you want to enable debug output for crypto functions */
/* #undef DEBUG_CRYPTO */

/* Define to 1 if you want to enable pcap output support (experimental) */
#define WITH_PCAP 1

/* Define to 1 if you want to enable calltrace debug output */
#define DEBUG_CALLTRACE 1

/*************************** ENDIAN *****************************/

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
/* #undef WORDS_BIGENDIAN */

#define DEPRECATEDIN_0_9_8(x)
#define DEPRECATEDIN_1_1_0(x)

#ifdef RECONFIGURATOR
typedef signed char 	int8_t;
typedef unsigned char 	uint8_t;
typedef signed int 	int16_t;
typedef unsigned int 	uint16_t;
typedef signed long int 	int32_t;
typedef unsigned long int 	uint32_t;
typedef signed long long int 	int64_t;
typedef unsigned long long int 	uint64_t;
typedef struct struct_size_t time_t;
typedef struct struct_mode_t mode_t;
typedef struct struct_pthread_once_t pthread_once_t;
typedef struct struct_pthread_key_t pthread_key_t;
typedef struct struct_pthread_t pthread_t;
typedef struct struct_va_list va_list;
typedef struct struct_ssh_key ssh_key;
typedef struct struct_ssh_signature ssh_signature;
typedef struct struct_ssh_auth_callback ssh_auth_callback;
typedef struct struct_off_t off_t;

typedef struct struct_ssh_buffer ssh_buffer;
typedef struct struct_ssh_string ssh_string;
typedef struct struct_ssh_session ssh_session;
typedef struct struct_SHACTX SHACTX;

//typedef struct struct_gcry_sexp_t gcry_sexp_t;
typedef struct struct_SOCKET SOCKET;
typedef struct struct_size_t size_t;
typedef struct struct_ssize_t ssize_t;
typedef struct struct_fd_set fd_set;
typedef struct struct_nfds_t nfds_t;

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
