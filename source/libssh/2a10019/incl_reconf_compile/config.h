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
#define WITH_SFTP 1

/* Define to 1 if you want to enable SSH1 */
/* #undef WITH_SSH1 */

/* Define to 1 if you want to enable server support */
#define WITH_SERVER 1

/* Define to 1 if you want to enable debug output for crypto functions */
/* #undef DEBUG_CRYPTO */

/* Define to 1 if you want to enable calltrace debug output */
#define DEBUG_CALLTRACE 1

/*************************** ENDIAN *****************************/

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
/* #undef WORDS_BIGENDIAN */

/************************* MS Windows ***************************/

#undef _WIN32

#define DEPRECATEDIN_1_1_0(x)
#define DEPRECATEDIN_0_9_8(x)
