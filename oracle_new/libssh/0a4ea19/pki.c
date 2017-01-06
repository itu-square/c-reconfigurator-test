#include "/home/alex/reconfigurator/c-reconfigurator-test/target/libssh/0a4ea19/REconfig.c"

#include "config.h"
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "libssh/libssh.h"
#include "libssh/session.h"
#include "libssh/priv.h"
#include "libssh/pki.h"
#include "libssh/pki_priv.h"
#include "libssh/keys.h"
#include "libssh/buffer.h"
#include "libssh/misc.h"
#include "libssh/agent.h"
void _ssh_pki_log (const char* function , const char* format , ...)
{
// defined(DEBUG_CRYPTO)
    char buffer_V0 [ 1024 ];
// defined(DEBUG_CRYPTO)
    va_list va_V0;
    if ((_reconfig_DEBUG_CRYPTO))
    {
        va_start (va_V0 , format);
        vsnprintf (buffer_V0 , ((sizeof (buffer_V0))) , format , va_V0);
        va_end (va_V0);
        ssh_log_function (3 , function , buffer_V0);
    }
    else
    {
        (void) function;
        (void) format;
    }
    return;
}

enum ssh_keytypes_e pki_privatekey_type_from_string (const char* privkey)
{
    if (strncmp (privkey , "-----BEGIN DSA PRIVATE KEY-----" , strlen ("-----BEGIN DSA PRIVATE KEY-----")) == 0)
    {
        return SSH_KEYTYPE_DSS;
    }
    if (strncmp (privkey , "-----BEGIN RSA PRIVATE KEY-----" , strlen ("-----BEGIN RSA PRIVATE KEY-----")) == 0)
    {
        return SSH_KEYTYPE_RSA;
    }
    if (strncmp (privkey , "-----BEGIN EC PRIVATE KEY-----" , strlen ("-----BEGIN EC PRIVATE KEY-----")) == 0)
    {
        return SSH_KEYTYPE_ECDSA;
    }
    return SSH_KEYTYPE_UNKNOWN;
}

ssh_key ssh_key_new (void)
{
    ssh_key ptr;
    (ptr = malloc (sizeof (struct ssh_key_struct)));
    if (ptr == NULL)
    {
        return NULL;
    }
    do
    {
        if ((ptr) != NULL)
         memset ((char*) (ptr) , 0 , (sizeof (* (ptr))));
    }
    while (0);
    return ptr;
}

ssh_key ssh_key_dup (const ssh_key key)
{
    if (key == NULL)
    {
        return NULL;
    }
    return pki_key_dup (key , 0);
}

void ssh_key_clean (ssh_key key)
{
    if (key == NULL)
         return;
    if ((_reconfig_HAVE_LIBGCRYPT))
    {
        if (key -> dsa)
         gcry_sexp_release (key -> dsa);
        if (key -> rsa)
         gcry_sexp_release (key -> rsa);
    }
    else
    {
        if (key -> dsa)
         DSA_free (key -> dsa);
        if (key -> rsa)
         RSA_free (key -> rsa);
    }
    if (key -> ecdsa)
    {
        if ((_reconfig_HAVE_LIBGCRYPT))
         gcry_sexp_release (key -> ecdsa);
        else
         EC_KEY_free (key -> ecdsa);
    }
    (key -> flags = 0x0);
    (key -> type = SSH_KEYTYPE_UNKNOWN);
    (key -> ecdsa_nid = 0);
    (key -> type_c = NULL);
    (key -> dsa = NULL);
    (key -> rsa = NULL);
    (key -> ecdsa = NULL);
}

void ssh_key_free (ssh_key key)
{
    if (key)
    {
        ssh_key_clean (key);
        do
        {
            if ((key) != NULL)
            {
                free (key);
                (key = NULL);
            }
        }
        while (0);
    }
}

enum ssh_keytypes_e ssh_key_type (const ssh_key key)
{
    if (key == NULL)
    {
        return SSH_KEYTYPE_UNKNOWN;
    }
    return key -> type;
}

const char* ssh_key_type_to_char (enum ssh_keytypes_e type)
{
    switch (type)
    {
        case SSH_KEYTYPE_DSS : return "ssh-dss";
        case SSH_KEYTYPE_RSA : return "ssh-rsa";
        case SSH_KEYTYPE_RSA1 : return "ssh-rsa1";
        case SSH_KEYTYPE_ECDSA : return "ssh-ecdsa";
        case SSH_KEYTYPE_UNKNOWN : return NULL;
    }
    return NULL;
}

enum ssh_keytypes_e ssh_key_type_from_name (const char* name)
{
    if (name == NULL)
    {
        return SSH_KEYTYPE_UNKNOWN;
    }
    if (strcmp (name , "rsa1") == 0)
    {
        return SSH_KEYTYPE_RSA1;
    }
    else
    {
        if (strcmp (name , "rsa") == 0)
        {
            return SSH_KEYTYPE_RSA;
        }
        else
        {
            if (strcmp (name , "dsa") == 0)
            {
                return SSH_KEYTYPE_DSS;
            }
            else
            {
                if (strcmp (name , "ssh-rsa1") == 0)
                {
                    return SSH_KEYTYPE_RSA1;
                }
                else
                {
                    if (strcmp (name , "ssh-rsa") == 0)
                    {
                        return SSH_KEYTYPE_RSA;
                    }
                    else
                    {
                        if (strcmp (name , "ssh-dss") == 0)
                        {
                            return SSH_KEYTYPE_DSS;
                        }
                        else
                        {
                            if (strcmp (name , "ssh-ecdsa") == 0 || strcmp (name , "ecdsa") == 0 || strcmp (name , "ecdsa-sha2-nistp256") == 0 || strcmp (name , "ecdsa-sha2-nistp384") == 0 || strcmp (name , "ecdsa-sha2-nistp521") == 0)
                            {
                                return SSH_KEYTYPE_ECDSA;
                            }
                        }
                    }
                }
            }
        }
    }
    return SSH_KEYTYPE_UNKNOWN;
}

int ssh_key_is_public (const ssh_key k)
{
    if (k == NULL)
    {
        return 0;
    }
    return (k -> flags & 0x0001);
}

int ssh_key_is_private (const ssh_key k)
{
    if (k == NULL)
    {
        return 0;
    }
    return (k -> flags & 0x0002);
}

int ssh_key_cmp (const ssh_key k1 , const ssh_key k2 , enum ssh_keycmp_e what)
{
    if (k1 == NULL || k2 == NULL)
    {
        return 1;
    }
    if (k1 -> type != k2 -> type)
    {
        _ssh_pki_log (__FUNCTION__ , "key types don't macth!");
        return 1;
    }
    if (what == SSH_KEY_CMP_PRIVATE)
    {
        if ((!ssh_key_is_private (k1)) || (!ssh_key_is_private (k2)))
        {
            return 1;
        }
    }
    return pki_key_compare (k1 , k2 , what);
}

ssh_signature ssh_signature_new (void)
{
    struct ssh_signature_struct* sig;
    (sig = malloc (sizeof (struct ssh_signature_struct)));
    if (sig == NULL)
    {
        return NULL;
    }
    do
    {
        if ((sig) != NULL)
         memset ((char*) (sig) , 0 , (sizeof (* (sig))));
    }
    while (0);
    return sig;
}

void ssh_signature_free (ssh_signature sig)
{
    if (sig == NULL)
    {
        return;
    }
    switch (sig -> type)
    {
        case SSH_KEYTYPE_DSS : if ((_reconfig_HAVE_LIBGCRYPT))
         gcry_sexp_release (sig -> dsa_sig);
        else
         DSA_SIG_free (sig -> dsa_sig);
        break;
        case SSH_KEYTYPE_RSA : case SSH_KEYTYPE_RSA1 : if ((_reconfig_HAVE_LIBGCRYPT))
         gcry_sexp_release (sig -> rsa_sig);
        else
        {
            do
            {
                if ((sig -> rsa_sig) != NULL)
                {
                    free (sig -> rsa_sig);
                    (sig -> rsa_sig = NULL);
                }
            }
            while (0);
        }
        break;
        case SSH_KEYTYPE_ECDSA : case SSH_KEYTYPE_UNKNOWN : break;
    }
    do
    {
        if ((sig) != NULL)
        {
            free (sig);
            (sig = NULL);
        }
    }
    while (0);
}

int ssh_pki_import_privkey_base64 (const char* b64_key , const char* passphrase , ssh_auth_callback auth_fn , void* auth_data , ssh_key* pkey)
{
    ssh_key key;
    if (b64_key == NULL || pkey == NULL)
    {
        return - 1;
    }
    if (b64_key == NULL || (!* b64_key))
    {
        return - 1;
    }
    _ssh_pki_log (__FUNCTION__ , "Trying to decode privkey passphrase=%s" ,
                                                                            passphrase
                                                                            ? "true"
                                                                            : "false");
    (key = pki_private_key_from_base64 (b64_key , passphrase , auth_fn , auth_data));
    if (key == NULL)
    {
        return - 1;
    }
    (* pkey = key);
    return 0;
}

int ssh_pki_import_privkey_file (const char* filename , const char* passphrase , ssh_auth_callback auth_fn , void* auth_data , ssh_key* pkey)
{
    struct stat sb;
    char* key_buf;
    ssh_key key;
    FILE * file;
    off_t size;
    int rc;
    if (pkey == NULL || filename == NULL || (* filename) == '\0')
    {
        return - 1;
    }
    (rc = stat (filename , (& sb)));
    if (rc < 0)
    {
        _ssh_pki_log (__FUNCTION__ , "Error getting stat of %s: %s" , filename , strerror (errno));
        switch (errno)
        {
            case ENOENT : case EACCES : return - 127;
        }
        return - 1;
    }
    (file = fopen (filename , "rb"));
    if (file == NULL)
    {
        _ssh_pki_log (__FUNCTION__ , "Error opening %s: %s" , filename , strerror (errno));
        return - 127;
    }
    (key_buf = malloc (sb . st_size + 1));
    if (key_buf == NULL)
    {
        fclose (file);
        _ssh_pki_log (__FUNCTION__ , "Out of memory!");
        return - 1;
    }
    (size = fread (key_buf , 1 , sb . st_size , file));
    fclose (file);
    if (size != sb . st_size)
    {
        do
        {
            if ((key_buf) != NULL)
            {
                free (key_buf);
                (key_buf = NULL);
            }
        }
        while (0);
        _ssh_pki_log (__FUNCTION__ , "Error reading %s: %s" , filename , strerror (errno));
        return - 1;
    }
    (key = pki_private_key_from_base64 (key_buf , passphrase , auth_fn , auth_data));
    do
    {
        if ((key_buf) != NULL)
        {
            free (key_buf);
            (key_buf = NULL);
        }
    }
    while (0);
    if (key == NULL)
    {
        return - 1;
    }
    (* pkey = key);
    return 0;
}

ssh_public_key ssh_pki_convert_key_to_publickey (const ssh_key key)
{
    ssh_public_key pub;
    ssh_key tmp;
    if (key == NULL)
    {
        return NULL;
    }
    (tmp = ssh_key_dup (key));
    if (tmp == NULL)
    {
        return NULL;
    }
    (pub = malloc (sizeof (struct ssh_public_key_struct)));
    if (pub == NULL)
    {
        ssh_key_free (tmp);
        return NULL;
    }
    do
    {
        if ((pub) != NULL)
         memset ((char*) (pub) , 0 , (sizeof (* (pub))));
    }
    while (0);
    (pub -> type = tmp -> type);
    (pub -> type_c = tmp -> type_c);
    (pub -> dsa_pub = tmp -> dsa);
    (tmp -> dsa = NULL);
    (pub -> rsa_pub = tmp -> rsa);
    (tmp -> rsa = NULL);
    ssh_key_free (tmp);
    return pub;
}

ssh_private_key ssh_pki_convert_key_to_privatekey (const ssh_key key)
{
    ssh_private_key privkey;
    (privkey = malloc (sizeof (struct ssh_private_key_struct)));
    if (privkey == NULL)
    {
        ssh_key_free (key);
        return NULL;
    }
    (privkey -> type = key -> type);
    (privkey -> dsa_priv = key -> dsa);
    (privkey -> rsa_priv = key -> rsa);
    return privkey;
}

static int pki_import_pubkey_buffer (ssh_buffer buffer , enum ssh_keytypes_e type , ssh_key* pkey)
{
    ssh_key key;
    int rc;
    (key = ssh_key_new ());
    if (key == NULL)
    {
        return - 1;
    }
    (key -> type = type);
    (key -> type_c = ssh_key_type_to_char (type));
    (key -> flags = 0x0001);
    switch (type)
    {
        case SSH_KEYTYPE_DSS :
        {
            ssh_string p;
            ssh_string q;
            ssh_string g;
            ssh_string pubkey;
            (p = buffer_get_ssh_string (buffer));
            if (p == NULL)
            {
                goto fail;
            }
            (q = buffer_get_ssh_string (buffer));
            if (q == NULL)
            {
                ssh_string_burn (p);
                ssh_string_free (p);
                goto fail;
            }
            (g = buffer_get_ssh_string (buffer));
            if (g == NULL)
            {
                ssh_string_burn (p);
                ssh_string_free (p);
                ssh_string_burn (q);
                ssh_string_free (q);
                goto fail;
            }
            (pubkey = buffer_get_ssh_string (buffer));
            if (pubkey == NULL)
            {
                ssh_string_burn (p);
                ssh_string_free (p);
                ssh_string_burn (q);
                ssh_string_free (q);
                ssh_string_burn (g);
                ssh_string_free (g);
                goto fail;
            }
            (rc = pki_pubkey_build_dss (key , p , q , g , pubkey));
            if ((_reconfig_DEBUG_CRYPTO))
            {
                ssh_print_hexa ("p" , ssh_string_data (p) , ssh_string_len (p));
                ssh_print_hexa ("q" , ssh_string_data (q) , ssh_string_len (q));
                ssh_print_hexa ("g" , ssh_string_data (g) , ssh_string_len (g));
            }
            ssh_string_burn (p);
            ssh_string_free (p);
            ssh_string_burn (q);
            ssh_string_free (q);
            ssh_string_burn (g);
            ssh_string_free (g);
            ssh_string_burn (pubkey);
            ssh_string_free (pubkey);
            if (rc == - 1)
            {
                goto fail;
            }
        }
        break;
        case SSH_KEYTYPE_RSA : case SSH_KEYTYPE_RSA1 :
        {
            ssh_string e;
            ssh_string n;
            (e = buffer_get_ssh_string (buffer));
            if (e == NULL)
            {
                goto fail;
            }
            (n = buffer_get_ssh_string (buffer));
            if (n == NULL)
            {
                ssh_string_burn (e);
                ssh_string_free (e);
                goto fail;
            }
            (rc = pki_pubkey_build_rsa (key , e , n));
            if ((_reconfig_DEBUG_CRYPTO))
            {
                ssh_print_hexa ("e" , ssh_string_data (e) , ssh_string_len (e));
                ssh_print_hexa ("n" , ssh_string_data (n) , ssh_string_len (n));
            }
            ssh_string_burn (e);
            ssh_string_free (e);
            ssh_string_burn (n);
            ssh_string_free (n);
            if (rc == - 1)
            {
                goto fail;
            }
        }
        break;
        case SSH_KEYTYPE_ECDSA :
        {
            ssh_string e;
            ssh_string i;
            int nid;
            (i = buffer_get_ssh_string (buffer));
            if (i == NULL)
            {
                goto fail;
            }
            (nid = pki_key_ecdsa_nid_from_name (ssh_string_get_char (i)));
            ssh_string_free (i);
            if (nid == - 1)
            {
                goto fail;
            }
            (e = buffer_get_ssh_string (buffer));
            if (e == NULL)
            {
                goto fail;
            }
            (rc = pki_pubkey_build_ecdsa (key , nid , e));
            ssh_string_burn (e);
            ssh_string_free (e);
            if (rc < 0)
            {
                goto fail;
            }
        }
        break;
        case SSH_KEYTYPE_UNKNOWN : _ssh_pki_log (__FUNCTION__ , "Unknown public key protocol %d" , type);
        goto fail;
    }
    (* pkey = key);
    return 0;
    fail : ssh_key_free (key);
    return - 1;
}

int ssh_pki_import_pubkey_base64 (const char* b64_key , enum ssh_keytypes_e type , ssh_key* pkey)
{
    ssh_buffer buffer;
    ssh_string type_s;
    int rc;
    if (b64_key == NULL || pkey == NULL)
    {
        return - 1;
    }
    (buffer = base64_to_bin (b64_key));
    if (buffer == NULL)
    {
        return - 1;
    }
    (type_s = buffer_get_ssh_string (buffer));
    if (type_s == NULL)
    {
        ssh_buffer_free (buffer);
        return - 1;
    }
    ssh_string_free (type_s);
    (rc = pki_import_pubkey_buffer (buffer , type , pkey));
    ssh_buffer_free (buffer);
    return rc;
}

int ssh_pki_import_pubkey_blob (const ssh_string key_blob , ssh_key* pkey)
{
    ssh_buffer buffer;
    ssh_string type_s;
    (type_s = NULL);
    enum ssh_keytypes_e type;
    int rc;
    if (key_blob == NULL || pkey == NULL)
    {
        return - 1;
    }
    (buffer = ssh_buffer_new ());
    if (buffer == NULL)
    {
        _ssh_pki_log (__FUNCTION__ , "Out of memory!");
        return - 1;
    }
    (rc = buffer_add_data (buffer , ssh_string_data (key_blob) , ssh_string_len (key_blob)));
    if (rc < 0)
    {
        _ssh_pki_log (__FUNCTION__ , "Out of memory!");
        goto fail;
    }
    (type_s = buffer_get_ssh_string (buffer));
    if (type_s == NULL)
    {
        _ssh_pki_log (__FUNCTION__ , "Out of memory!");
        goto fail;
    }
    (type = ssh_key_type_from_name (ssh_string_get_char (type_s)));
    if (type == SSH_KEYTYPE_UNKNOWN)
    {
        _ssh_pki_log (__FUNCTION__ , "Unknown key type found!");
        goto fail;
    }
    ssh_string_free (type_s);
    (rc = pki_import_pubkey_buffer (buffer , type , pkey));
    ssh_buffer_free (buffer);
    return rc;
    fail : ssh_buffer_free (buffer);
    ssh_string_free (type_s);
    return - 1;
}

int ssh_pki_import_pubkey_file (const char* filename , ssh_key* pkey)
{
    enum ssh_keytypes_e type;
    struct stat sb;
    char* key_buf;
    char* p;
    const char* q;
    FILE * file;
    off_t size;
    int rc;
    if (pkey == NULL || filename == NULL || (* filename) == '\0')
    {
        return - 1;
    }
    (rc = stat (filename , (& sb)));
    if (rc < 0)
    {
        _ssh_pki_log (__FUNCTION__ , "Error gettint stat of %s: %s" , filename , strerror (errno));
        switch (errno)
        {
            case ENOENT : case EACCES : return - 127;
        }
        return - 1;
    }
    (file = fopen (filename , "r"));
    if (file == NULL)
    {
        _ssh_pki_log (__FUNCTION__ , "Error opening %s: %s" , filename , strerror (errno));
        return - 127;
    }
    (key_buf = malloc (sb . st_size + 1));
    if (key_buf == NULL)
    {
        fclose (file);
        _ssh_pki_log (__FUNCTION__ , "Out of memory!");
        return - 1;
    }
    (size = fread (key_buf , 1 , sb . st_size , file));
    fclose (file);
    if (size != sb . st_size)
    {
        do
        {
            if ((key_buf) != NULL)
            {
                free (key_buf);
                (key_buf = NULL);
            }
        }
        while (0);
        _ssh_pki_log (__FUNCTION__ , "Error reading %s: %s" , filename , strerror (errno));
        return - 1;
    }
    (q = (p = key_buf));
    while (!isspace ((int) * p))
         (p ++);
    (* p = '\0');
    (type = ssh_key_type_from_name (q));
    if (type == SSH_KEYTYPE_UNKNOWN)
    {
        do
        {
            if ((key_buf) != NULL)
            {
                free (key_buf);
                (key_buf = NULL);
            }
        }
        while (0);
        return - 1;
    }
    (q = (++ p));
    while (!isspace ((int) * p))
         (p ++);
    (* p = '\0');
    (rc = ssh_pki_import_pubkey_base64 (q , type , pkey));
    do
    {
        if ((key_buf) != NULL)
        {
            free (key_buf);
            (key_buf = NULL);
        }
    }
    while (0);
    return rc;
}

int ssh_pki_generate (enum ssh_keytypes_e type , int parameter , ssh_key* pkey)
{
    int rc;
    ssh_key key;
    (key = ssh_key_new ());
    if (key == NULL)
    {
        return - 1;
    }
    (key -> type = type);
    (key -> type_c = ssh_key_type_to_char (type));
    (key -> flags = 0x0002 | 0x0001);
    switch (type)
    {
        case SSH_KEYTYPE_RSA : case SSH_KEYTYPE_RSA1 : (rc = pki_key_generate_rsa (key , parameter));
        if (rc == - 1) goto error;
        break;
        case SSH_KEYTYPE_DSS : (rc = pki_key_generate_dss (key , parameter));
        if (rc == - 1) goto error;
        break;
        case SSH_KEYTYPE_ECDSA : (rc = pki_key_generate_ecdsa (key , parameter));
        if (rc == - 1) goto error;
        break;
        case SSH_KEYTYPE_UNKNOWN : goto error;
    }
    (* pkey = key);
    return 0;
    error : ssh_key_free (key);
    return - 1;
}

int ssh_pki_export_privkey_to_pubkey (const ssh_key privkey , ssh_key* pkey)
{
    ssh_key pubkey;
    if (privkey == NULL || (!ssh_key_is_private (privkey)))
    {
        return - 1;
    }
    (pubkey = pki_key_dup (privkey , 1));
    if (pubkey == NULL)
    {
        return - 1;
    }
    (* pkey = pubkey);
    return 0;
}

int ssh_pki_export_pubkey_blob (const ssh_key key , ssh_string* pblob)
{
    ssh_string blob;
    if (key == NULL)
    {
        return 0;
    }
    (blob = pki_publickey_to_blob (key));
    if (blob == NULL)
    {
        return - 1;
    }
    (* pblob = blob);
    return 0;
}

int ssh_pki_export_pubkey_base64 (const ssh_key key , char** b64_key)
{
    ssh_string key_blob;
    unsigned char* b64;
    if (key == NULL || b64_key == NULL)
    {
        return - 1;
    }
    (key_blob = pki_publickey_to_blob (key));
    if (key_blob == NULL)
    {
        return - 1;
    }
    (b64 = bin_to_base64 (ssh_string_data (key_blob) , ssh_string_len (key_blob)));
    ssh_string_free (key_blob);
    if (b64 == NULL)
    {
        return - 1;
    }
    (* b64_key = (char*) b64);
    return 0;
}

int ssh_pki_export_pubkey_file (const ssh_key key , const char* filename)
{
    char key_buf [ 4096 ];
    char host [ 256 ];
    char* b64_key;
    char* user;
    FILE * fp;
    int rc;
    if (key == NULL || filename == NULL || (* filename) == '\0')
    {
        return - 1;
    }
    (user = ssh_get_local_username ());
    if (user == NULL)
    {
        return - 1;
    }
    (rc = gethostname (host , (sizeof (host))));
    if (rc < 0)
    {
        free (user);
        return - 1;
    }
    (rc = ssh_pki_export_pubkey_base64 (key , (& b64_key)));
    if (rc < 0)
    {
        free (user);
        return - 1;
    }
    (rc = snprintf (key_buf , (sizeof (key_buf)) , "%s %s %s@%s\n" , key -> type_c , b64_key , user , host));
    free (user);
    free (b64_key);
    if (rc < 0)
    {
        return - 1;
    }
    (fp = fopen (filename , "w+"));
    if (fp == NULL)
    {
        return - 1;
    }
    (rc = fwrite (key_buf , strlen (key_buf) , 1 , fp));
    if (rc != 1 || ferror (fp))
    {
        fclose (fp);
        unlink (filename);
        return - 1;
    }
    fclose (fp);
    return 0;
}

int ssh_pki_export_pubkey_rsa1 (const ssh_key key , const char* host , char* rsa1 , size_t rsa1_len)
{
    return pki_export_pubkey_rsa1 (key , host , rsa1 , rsa1_len);
}

int ssh_pki_export_signature_blob (const ssh_signature sig , ssh_string* sig_blob)
{
    ssh_buffer buf;
    (buf = NULL);
    ssh_string str;
    int rc;
    if (sig == NULL || sig_blob == NULL)
    {
        return - 1;
    }
    (buf = ssh_buffer_new ());
    if (buf == NULL)
    {
        return - 1;
    }
    (str = ssh_string_from_char (ssh_key_type_to_char (sig -> type)));
    if (str == NULL)
    {
        ssh_buffer_free (buf);
        return - 1;
    }
    (rc = buffer_add_ssh_string (buf , str));
    ssh_string_free (str);
    if (rc < 0)
    {
        ssh_buffer_free (buf);
        return - 1;
    }
    (str = pki_signature_to_blob (sig));
    if (str == NULL)
    {
        ssh_buffer_free (buf);
        return - 1;
    }
    (rc = buffer_add_ssh_string (buf , str));
    ssh_string_free (str);
    if (rc < 0)
    {
        ssh_buffer_free (buf);
        return - 1;
    }
    (str = ssh_string_new (buffer_get_rest_len (buf)));
    if (str == NULL)
    {
        ssh_buffer_free (buf);
        return - 1;
    }
    ssh_string_fill (str , buffer_get_rest (buf) , buffer_get_rest_len (buf));
    ssh_buffer_free (buf);
    (* sig_blob = str);
    return 0;
}

int ssh_pki_import_signature_blob (const ssh_string sig_blob , const ssh_key pubkey , ssh_signature* psig)
{
    ssh_signature sig;
    enum ssh_keytypes_e type;
    ssh_string str;
    ssh_buffer buf;
    int rc;
    if (sig_blob == NULL || psig == NULL)
    {
        return - 1;
    }
    (buf = ssh_buffer_new ());
    if (buf == NULL)
    {
        return - 1;
    }
    (rc = buffer_add_data (buf , ssh_string_data (sig_blob) , ssh_string_len (sig_blob)));
    if (rc < 0)
    {
        ssh_buffer_free (buf);
        return - 1;
    }
    (str = buffer_get_ssh_string (buf));
    if (str == NULL)
    {
        ssh_buffer_free (buf);
        return - 1;
    }
    (type = ssh_key_type_from_name (ssh_string_get_char (str)));
    ssh_string_free (str);
    (str = buffer_get_ssh_string (buf));
    ssh_buffer_free (buf);
    if (str == NULL)
    {
        return - 1;
    }
    (sig = pki_signature_from_blob (pubkey , str , type));
    ssh_string_free (str);
    if (sig == NULL)
    {
        return - 1;
    }
    (* psig = sig);
    return 0;
}

int ssh_pki_signature_verify_blob (ssh_session session , ssh_string sig_blob , const ssh_key key , unsigned char* digest , size_t dlen)
{
    ssh_signature sig;
    int rc;
    (rc = ssh_pki_import_signature_blob (sig_blob , key , (& sig)));
    if (rc < 0)
    {
        return - 1;
    }
    ssh_log (session , SSH_LOG_FUNCTIONS , "Going to verify a %s type signature" , key -> type_c);
    if (key -> type == SSH_KEYTYPE_ECDSA)
    {
// defined(HAVE_LIBGCRYPT) && !defined(EVP_MAX_MD_SIZE)
        unsigned char ehash_V1 [ 36 ];
// !defined(HAVE_LIBGCRYPT) || defined(EVP_MAX_MD_SIZE)
        unsigned char ehash_V2 [ EVP_MAX_MD_SIZE ];
        int _reconfig_ehash_index;
        for (_reconfig_ehash_index = 0; _reconfig_ehash_index < (
                                                                 (_reconfig_HAVE_LIBGCRYPT && !_reconfig_EVP_MAX_MD_SIZE)
                                                                 ? 36
                                                                 : EVP_MAX_MD_SIZE); _reconfig_ehash_index ++)
        {
            (
             (_reconfig_HAVE_LIBGCRYPT && (!_reconfig_EVP_MAX_MD_SIZE))
             ? ((ehash_V1 [ _reconfig_ehash_index ] = 0))
             : ((ehash_V2 [ _reconfig_ehash_index ] = 0)));
        }
        uint32_t elen;
        evp (key -> ecdsa_nid , digest , dlen , (
                                                 (_reconfig_HAVE_LIBGCRYPT && (!_reconfig_EVP_MAX_MD_SIZE))
                                                 ? ehash_V1
                                                 : ehash_V2) , (& elen));
        (rc = pki_signature_verify (session , sig , key , (
                                                           (_reconfig_HAVE_LIBGCRYPT && (!_reconfig_EVP_MAX_MD_SIZE))
                                                           ? ehash_V1
                                                           : ehash_V2) , elen));
    }
    else
    {
// defined(HAVE_LIBGCRYPT)
        unsigned char hash_V3 [ 20 ];
// !defined(HAVE_LIBGCRYPT)
        unsigned char hash_V4 [ SHA_DIGEST_LENGTH ];
        int _reconfig_hash_index;
        for (_reconfig_hash_index = 0; _reconfig_hash_index < (
                                                               (_reconfig_HAVE_LIBGCRYPT)
                                                               ? 20
                                                               : SHA_DIGEST_LENGTH); _reconfig_hash_index ++)
        {
            (
             (_reconfig_HAVE_LIBGCRYPT)
             ? ((hash_V3 [ _reconfig_hash_index ] = 0))
             : ((hash_V4 [ _reconfig_hash_index ] = 0)));
        }
        sha1 (digest , dlen , (
                               (_reconfig_HAVE_LIBGCRYPT)
                               ? hash_V3
                               : hash_V4));
        if ((_reconfig_DEBUG_CRYPTO))
         ssh_print_hexa ("Hash to be verified with dsa" , (
                                                                                        (_reconfig_HAVE_LIBGCRYPT)
                                                                                        ? hash_V3
                                                                                        : hash_V4) , (
                                                                                                      (_reconfig_HAVE_LIBGCRYPT)
                                                                                                      ? 20
                                                                                                      : SHA_DIGEST_LENGTH));
        (rc = pki_signature_verify (session , sig , key , (
                                                           (_reconfig_HAVE_LIBGCRYPT)
                                                           ? hash_V3
                                                           : hash_V4) , (
                                                                         (_reconfig_HAVE_LIBGCRYPT)
                                                                         ? 20
                                                                         : SHA_DIGEST_LENGTH)));
    }
    ssh_signature_free (sig);
    return rc;
}

ssh_string ssh_pki_do_sign (ssh_session session , ssh_buffer sigbuf , const ssh_key privkey)
{
    struct ssh_crypto_struct* crypto;
    (crypto =
              session -> current_crypto
              ? session -> current_crypto
              : session -> next_crypto);
// defined(HAVE_LIBGCRYPT)
    unsigned char hash_V3 [ 20 ];
// !defined(HAVE_LIBGCRYPT)
    unsigned char hash_V4 [ SHA_DIGEST_LENGTH ];
    int _reconfig_hash_index;
    for (_reconfig_hash_index = 0; _reconfig_hash_index < (
                                                           (_reconfig_HAVE_LIBGCRYPT)
                                                           ? 20
                                                           : SHA_DIGEST_LENGTH); _reconfig_hash_index ++)
    {
        (
         (_reconfig_HAVE_LIBGCRYPT)
         ? ((hash_V3 [ _reconfig_hash_index ] = 0))
         : ((hash_V4 [ _reconfig_hash_index ] = 0)));
    }
    ssh_signature sig;
    ssh_string sig_blob;
    ssh_string session_id;
    SHACTX ctx;
    int rc;
    if (privkey == NULL || (!ssh_key_is_private (privkey)))
    {
        return NULL;
    }
    (session_id = ssh_string_new (crypto -> digest_len));
    if (session_id == NULL)
    {
        return NULL;
    }
    ssh_string_fill (session_id , crypto -> session_id , crypto -> digest_len);
    (ctx = sha1_init ());
    if (ctx == NULL)
    {
        ssh_string_free (session_id);
        return NULL;
    }
    sha1_update (ctx , session_id , ssh_string_len (session_id) + 4);
    ssh_string_free (session_id);
    sha1_update (ctx , buffer_get_rest (sigbuf) , buffer_get_rest_len (sigbuf));
    sha1_final ((
                 (_reconfig_HAVE_LIBGCRYPT)
                 ? hash_V3
                 : hash_V4) , ctx);
    if ((_reconfig_DEBUG_CRYPTO))
         ssh_print_hexa ("Hash being signed" , (
                                                                         (_reconfig_HAVE_LIBGCRYPT)
                                                                         ? hash_V3
                                                                         : hash_V4) , (
                                                                                       (_reconfig_HAVE_LIBGCRYPT)
                                                                                       ? 20
                                                                                       : SHA_DIGEST_LENGTH));
    (sig = pki_do_sign (privkey , (
                                   (_reconfig_HAVE_LIBGCRYPT)
                                   ? hash_V3
                                   : hash_V4) , (
                                                 (_reconfig_HAVE_LIBGCRYPT)
                                                 ? 20
                                                 : SHA_DIGEST_LENGTH)));
    if (sig == NULL)
    {
        return NULL;
    }
    (rc = ssh_pki_export_signature_blob (sig , (& sig_blob)));
    ssh_signature_free (sig);
    if (rc < 0)
    {
        return NULL;
    }
    return sig_blob;
}

ssh_string ssh_pki_do_sign_agent (ssh_session session , struct ssh_buffer_struct* buf , const ssh_key pubkey)
{
    struct ssh_crypto_struct* crypto;
    ssh_string session_id;
    ssh_string sig_blob;
    ssh_buffer sig_buf;
    int rc;
    if (session -> current_crypto)
    {
        (crypto = session -> current_crypto);
    }
    else
    {
        (crypto = session -> next_crypto);
    }
    (session_id = ssh_string_new (crypto -> digest_len));
    if (session_id == NULL)
    {
        return NULL;
    }
    ssh_string_fill (session_id , crypto -> session_id , crypto -> digest_len);
    (sig_buf = ssh_buffer_new ());
    if (sig_buf == NULL)
    {
        ssh_string_free (session_id);
        return NULL;
    }
    (rc = buffer_add_ssh_string (sig_buf , session_id));
    if (rc < 0)
    {
        ssh_string_free (session_id);
        ssh_buffer_free (sig_buf);
        return NULL;
    }
    ssh_string_free (session_id);
    if (buffer_add_buffer (sig_buf , buf) < 0)
    {
        ssh_buffer_free (sig_buf);
        return NULL;
    }
    (sig_blob = ssh_agent_sign_data (session , pubkey , sig_buf));
    ssh_buffer_free (sig_buf);
    return sig_blob;
}

ssh_string ssh_srv_pki_do_sign_sessionid (ssh_session session , const ssh_key privkey)
{
    struct ssh_crypto_struct* crypto;
    (crypto =
              session -> current_crypto
              ? session -> current_crypto
              : session -> next_crypto);
// defined(HAVE_LIBGCRYPT)
    unsigned char hash_V3 [ 20 ];
// !defined(HAVE_LIBGCRYPT)
    unsigned char hash_V4 [ SHA_DIGEST_LENGTH ];
    int _reconfig_hash_index;
    for (_reconfig_hash_index = 0; _reconfig_hash_index < (
                                                           (_reconfig_HAVE_LIBGCRYPT)
                                                           ? 20
                                                           : SHA_DIGEST_LENGTH); _reconfig_hash_index ++)
    {
        (
         (_reconfig_HAVE_LIBGCRYPT)
         ? ((hash_V3 [ _reconfig_hash_index ] = 0))
         : ((hash_V4 [ _reconfig_hash_index ] = 0)));
    }
    ssh_signature sig;
    ssh_string sig_blob;
    SHACTX ctx;
    int rc;
    if (session == NULL || privkey == NULL || (!ssh_key_is_private (privkey)))
    {
        return NULL;
    }
    (ctx = sha1_init ());
    if (ctx == NULL)
    {
        return NULL;
    }
    if (crypto -> session_id == NULL)
    {
        _ssh_set_error (session , SSH_FATAL , __FUNCTION__ , "Missing session_id");
        return NULL;
    }
    sha1_update (ctx , crypto -> session_id , crypto -> digest_len);
    sha1_final ((
                 (_reconfig_HAVE_LIBGCRYPT)
                 ? hash_V3
                 : hash_V4) , ctx);
    if ((_reconfig_DEBUG_CRYPTO))
         ssh_print_hexa ("Hash being signed" , (
                                                                         (_reconfig_HAVE_LIBGCRYPT)
                                                                         ? hash_V3
                                                                         : hash_V4) , (
                                                                                       (_reconfig_HAVE_LIBGCRYPT)
                                                                                       ? 20
                                                                                       : SHA_DIGEST_LENGTH));
    (sig = pki_do_sign_sessionid (privkey , (
                                             (_reconfig_HAVE_LIBGCRYPT)
                                             ? hash_V3
                                             : hash_V4) , (
                                                           (_reconfig_HAVE_LIBGCRYPT)
                                                           ? 20
                                                           : SHA_DIGEST_LENGTH)));
    if (sig == NULL)
    {
        return NULL;
    }
    (rc = ssh_pki_export_signature_blob (sig , (& sig_blob)));
    ssh_signature_free (sig);
    if (rc < 0)
    {
        return NULL;
    }
    return sig_blob;
}