/*
 * This file is part of the SSH Library
 *
 * Copyright (c) 2003-2008 by Aris Adamantiadis
 *
 * The SSH Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * The SSH Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the SSH Library; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

/**
 * @file sftp.h
 *
 * @brief SFTP handling functions
 *
 * SFTP commands are channeled by the ssh sftp subsystem. Every packet is
 * sent/read using a SFTP_PACKET type structure. Related to these packets,
 * most of the server answers are messages having an ID and a message
 * specific part. It is described by SFTP_MESSAGE when reading a message,
 * the sftp system puts it into the queue, so the process having asked for
 * it can fetch it, while continuing to read for other messages (it is
 * inspecified in which order messages may be sent back to the client
 *
 * @defgroup ssh_sftp SFTP Functions
 * @{
 */

#ifndef SFTP_H
#define SFTP_H

#include <sys/types.h>

#include "libssh.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define SFTP_DEPRECATED __attribute__ ((deprecated))
#else
#define SFTP_DEPRECATED
#endif

#ifdef _WIN32
#ifndef uid_t
  typedef uint32_t uid_t;
#endif /* uid_t */
#ifndef gid_t
  typedef uint32_t gid_t;
#endif /* gid_t */
#ifdef _MSC_VER
#ifndef mode_t
  typedef uint32_t mode_t;
#endif /* mode_t */
#ifndef ssize_t
  typedef _W64 signed int ssize_t;
#endif /* ssize_t */
#endif /* _MSC_VER */
#endif /* _WIN32 */

typedef struct sftp_ext_struct *sftp_ext;

typedef struct sftp_session_struct {
    SSH_SESSION *session;
    ssh_channel channel;
    int server_version;
    int client_version;
    int version;
    struct request_queue *queue;
    uint32_t id_counter;
    int errnum;
    void **handles;
    struct sftp_ext_struct *ext;
} SFTP_SESSION ;

typedef struct {
    SFTP_SESSION *sftp;
    uint8_t type;
    ssh_buffer payload;
} SFTP_PACKET;

/* file handler */
typedef struct sftp_file{
    SFTP_SESSION *sftp;
    char *name;
    uint64_t offset;
    ssh_string handle;
    int eof;
    int nonblocking;
}  SFTP_FILE ;

typedef struct sftp_dir {
    SFTP_SESSION *sftp;
    char *name;
    ssh_string handle; /* handle to directory */
    ssh_buffer buffer; /* contains raw attributes from server which haven't been parsed */
    uint32_t count; /* counts the number of following attributes structures into buffer */
    int eof; /* end of directory listing */
} SFTP_DIR;

typedef struct {
    SFTP_SESSION *sftp;
    uint8_t packet_type;
    ssh_buffer payload;
    uint32_t id;
} SFTP_MESSAGE;

/* this is a bunch of all data that could be into a message */
typedef struct sftp_client_message{
    SFTP_SESSION *sftp;
    uint8_t type;
    uint32_t id;
    char *filename; /* can be "path" */
    uint32_t flags;
    struct sftp_attributes *attr;
    ssh_string handle;
    uint64_t offset;
    uint32_t len;
    int attr_num;
    ssh_buffer attrbuf; /* used by sftp_reply_attrs */
    ssh_string data; /* can be newpath of rename() */
} SFTP_CLIENT_MESSAGE;

typedef struct request_queue{
    struct request_queue *next;
    SFTP_MESSAGE *message;
} REQUEST_QUEUE;

/* SSH_FXP_MESSAGE described into .7 page 26 */
typedef struct {
  uint32_t id;
  uint32_t status;
    ssh_string error;
    ssh_string lang;
    char *errormsg;
    char *langmsg;
} STATUS_MESSAGE;

/* don't worry much of these aren't really used */
typedef struct sftp_attributes{
    char *name;
    char *longname; /* some weird stuff */
    uint32_t flags;
    uint8_t type;
    uint64_t size;
    uint32_t uid;
    uint32_t gid;
    char *owner;
    char *group;
    uint32_t permissions;
    uint64_t atime64;
    uint32_t atime;
    uint32_t atime_nseconds;
    uint64_t createtime;
    uint32_t createtime_nseconds;
    uint64_t mtime64;
    uint32_t mtime;
    uint32_t mtime_nseconds;
    ssh_string acl;
    uint32_t extended_count;
    ssh_string extended_type;
    ssh_string extended_data;
} SFTP_ATTRIBUTES;

typedef struct sftp_statvfs_struct {
  uint64_t f_bsize; /* file system block size */
  uint64_t f_frsize; /* fundamental fs block size */
  uint64_t f_blocks; /* number of blocks (unit f_frsize) */
  uint64_t f_bfree; /* free blocks in file system */
  uint64_t f_bavail; /* free blocks for non-root */
  uint64_t f_files; /* total file inodes */
  uint64_t f_ffree; /* free file inodes */
  uint64_t f_favail; /* free file inodes for to non-root */
  uint64_t f_fsid; /* file system id */
  uint64_t f_flag; /* bit mask of f_flag values */
  uint64_t f_namemax; /* maximum filename length */
} SFTP_STATVFS;

#define LIBSFTP_VERSION 3

/**
 * @brief Start a new sftp session.
 *
 * @param session       The ssh session to use.
 *
 * @return              A new sftp session or NULL on error.
 */
LIBSSH_API SFTP_SESSION *sftp_new(SSH_SESSION *session);

/**
 * @brief Close and deallocate a sftp session.
 *
 * @param sftp          The sftp session handle to free.
 */
LIBSSH_API void sftp_free(SFTP_SESSION *sftp);

/**
 * @brief Initialize the sftp session with the server.
 *
 * @param sftp          The sftp session to initialize.
 *
 * @return              0 on success, < 0 on error with ssh error set.
 */
LIBSSH_API int sftp_init(SFTP_SESSION *sftp);

/**
 * @brief Get the last sftp error.
 *
 * Use this function to get the latest error set by a posix like sftp function.
 *
 * @param sftp          The sftp session where the error is saved.
 *
 * @return              The saved error (see server responses), < 0 if an error
 *                      in the function occured.
 */
LIBSSH_API int sftp_get_error(SFTP_SESSION *sftp);

/**
 * @brief Get the count of extensions provided by the server.
 *
 * @param  sftp         The sftp session to use.
 *
 * @return The count of extensions provided by the server, 0 on error or
 *         not available.
 */
LIBSSH_API unsigned int sftp_extensions_get_count(SFTP_SESSION *sftp);

/**
 * @brief Get the name of the extension provided by the server.
 *
 * @param  sftp         The sftp session to use.
 *
 * @param  index        The index number of the extension name you want.
 *
 * @return              The name of the extension.
 */
LIBSSH_API const char *sftp_extensions_get_name(SFTP_SESSION *sftp, unsigned int index);

/**
 * @brief Get the data of the extension provided by the server.
 *
 * This is normally the version number of the extension.
 *
 * @param  sftp         The sftp session to use.
 *
 * @param  index        The index number of the extension data you want.
 *
 * @return              The data of the extension.
 */
LIBSSH_API const char *sftp_extensions_get_data(SFTP_SESSION *sftp, unsigned int index);

/**
 * @brief Check if the given extension is supported.
 *
 * @param  sftp         The sftp session to use.
 *
 * @param  name         The name of the extension.
 *
 * @param  data         The data of the extension.
 *
 * @return 1 if supported, 0 if not.
 *
 * Example:
 *
 * @code
 * sftp_extension_supported(sftp, "statvfs@openssh.com", "2");
 * @endcode
 */
LIBSSH_API int sftp_extension_supported(SFTP_SESSION *sftp, const char *name,
    const char *data);

/**
 * @brief Open a directory used to obtain directory entries.
 *
 * @param session       The sftp session handle to open the directory.
 * @param path          The path of the directory to open.
 *
 * @return              A sftp directory handle or NULL on error with ssh and
 *                      sftp error set.
 *
 * @see                 sftp_readdir
 * @see                 sftp_closedir
 */
LIBSSH_API SFTP_DIR *sftp_opendir(SFTP_SESSION *session, const char *path);

/**
 * @brief Get a single file attributes structure of a directory.
 *
 * @param session      The sftp session handle to read the directory entry.
 * @param dir          The opened sftp directory handle to read from.
 *
 * @return             A file attribute structure or NULL at the end of the
 *                     directory.
 *
 * @see                sftp_opendir()
 * @see                sftp_attribute_free()
 * @see                sftp_closedir()
 */
LIBSSH_API SFTP_ATTRIBUTES *sftp_readdir(SFTP_SESSION *session, SFTP_DIR *dir);

/**
 * @brief Tell if the directory has reached EOF (End Of File).
 *
 * @param dir           The sftp directory handle.
 *
 * @return              1 if the directory is EOF, 0 if not.
 *
 * @see                 sftp_readdir()
 */
LIBSSH_API int sftp_dir_eof(SFTP_DIR *dir);

/**
 * @brief Get information about a file or directory.
 *
 * @param session       The sftp session handle.
 * @param path          The path to the file or directory to obtain the
 *                      information.
 *
 * @return              The sftp attributes structure of the file or directory,
 *                      NULL on error with ssh and sftp error set.
 */
LIBSSH_API SFTP_ATTRIBUTES *sftp_stat(SFTP_SESSION *session, const char *path);

/**
 * @brief Get information about a file or directory.
 *
 * Identical to sftp_stat, but if the file or directory is a symbolic link,
 * then the link itself is stated, not the file that it refers to.
 *
 * @param session       The sftp session handle.
 * @param path          The path to the file or directory to obtain the
 *                      information.
 *
 * @return              The sftp attributes structure of the file or directory,
 *                      NULL on error with ssh and sftp error set.
 */
LIBSSH_API SFTP_ATTRIBUTES *sftp_lstat(SFTP_SESSION *session, const char *path);

/**
 * @brief Get information about a file or directory from a file handle.
 *
 * @param file          The sftp file handle to get the stat information.
 *
 * @return              The sftp attributes structure of the file or directory,
 *                      NULL on error with ssh and sftp error set.
 */
LIBSSH_API SFTP_ATTRIBUTES *sftp_fstat(SFTP_FILE *file);

/**
 * @brief Free a sftp attribute structure.
 *
 * @param file          The sftp attribute structure to free.
 */
LIBSSH_API void sftp_attributes_free(SFTP_ATTRIBUTES *file);

/**
 * @brief Close a directory handle opened by sftp_opendir().
 *
 * @param dir           The sftp directory handle to close.
 *
 * @return              Returns SSH_NO_ERROR or SSH_ERROR if an error occured.
 */
LIBSSH_API int sftp_closedir(SFTP_DIR *dir);

/**
 * @deprecated          Use sftp_closedir() instead.
 */
LIBSSH_API int sftp_dir_close(SFTP_DIR *dir) SFTP_DEPRECATED;

/**
 * @brief Close an open file handle.
 *
 * @param file          The open sftp file handle to close.
 *
 * @return              Returns SSH_NO_ERROR or SSH_ERROR if an error occured.
 *
 * @see                 sftp_open()
 */
LIBSSH_API int sftp_close(SFTP_FILE *file);

/**
 * @deprecated          Use sftp_close() instead.
 */
LIBSSH_API int sftp_file_close(SFTP_FILE *file) SFTP_DEPRECATED;

/**
 * @brief Open a file on the server.
 *
 * @param session       The sftp session handle.
 *
 * @param file          The file to be opened.
 *
 * @param access        Is one of O_RDONLY, O_WRONLY or O_RDWR which request
 *                      opening  the  file  read-only,write-only or read/write.
 *                      Acesss may also be bitwise-or'd with one or  more of
 *                      the following:
 *                      O_CREAT - If the file does not exist it will be
 *                      created.
 *                      O_EXCL - When  used with O_CREAT, if the file already
 *                      exists it is an error and the open will fail.
 *                      O_TRUNC - If the file already exists it will be
 *                      truncated.
 *
 * @param mode          Mode specifies the permissions to use if a new file is
 *                      created.  It  is  modified  by  the process's umask in
 *                      the usual way: The permissions of the created file are
 *                      (mode & ~umask)
 *
 * @return              A sftp file handle, NULL on error with ssh and sftp
 *                      error set.
 */
LIBSSH_API SFTP_FILE *sftp_open(SFTP_SESSION *session, const char *file, int flags,
    mode_t mode);

LIBSSH_API void sftp_file_set_nonblocking(SFTP_FILE *handle);

LIBSSH_API void sftp_file_set_blocking(SFTP_FILE *handle);

/**
 * @brief Read from a file using an opened sftp file handle.
 *
 * @param file          The opened sftp file handle to be read from.
 *
 * @param buf           Pointer to buffer to recieve read data.
 *
 * @param count         Size of the buffer in bytes.
 *
 * @return              Number of bytes written, < 0 on error with ssh and sftp
 *                      error set.
 */
LIBSSH_API ssize_t sftp_read(SFTP_FILE *file, void *buf, size_t count);

/**
 * @brief Start an asynchronous read from a file using an opened sftp file handle.
 *
 * Its goal is to avoid the slowdowns related to the request/response pattern
 * of a synchronous read. To do so, you must call 2 functions:
 *
 * sftp_async_read_begin() and sftp_async_read().
 *
 * The first step is to call sftp_async_read_begin(). This function returns a
 * request identifier. The second step is to call sftp_async_read() using the
 * returned identifier.
 *
 * @param file          The opened sftp file handle to be read from.
 *
 * @param len           Size to read in bytes.
 *
 * @return              An identifier corresponding to the sent request, < 0 on
 *                      error.
 *
 * @warning             When calling this function, the internal offset is
 *                      updated corresponding to the len parameter.
 *
 * @warning             A call to sftp_async_read_begin() sends a request to
 *                      the server. When the server answers, libssh allocates
 *                      memory to store it until sftp_async_read() is called.
 *                      Not calling sftp_async_read() will lead to memory
 *                      leaks.
 *
 * @see                 sftp_async_read()
 * @see                 sftp_open()
 */
LIBSSH_API int sftp_async_read_begin(SFTP_FILE *file, uint32_t len);

/**
 * @brief Wait for an asynchronous read to complete and save the data.
 *
 * @param file          The opened sftp file handle to be read from.
 *
 * @param data          Pointer to buffer to recieve read data.
 *
 * @param len           Size of the buffer in bytes. It should be bigger or
 *                      equal to the length parameter of the
 *                      sftp_async_read_begin() call.
 *
 * @param id            The identifier returned by the sftp_async_read_begin()
 *                      function.
 *
 * @return              Number of bytes read, 0 on EOF, SSH_ERROR if an error
 *                      occured, SSH_AGAIN if the file is opened in nonblocking
 *                      mode and the request hasn't been executed yet.
 *
 * @warning             A call to this function with an invalid identifier
 *                      will never return.
 *
 * @see sftp_async_read_begin()
 */
LIBSSH_API int sftp_async_read(SFTP_FILE *file, void *data, uint32_t len, uint32_t id);

/**
 * @brief Write to a file using an opened sftp file handle.
 *
 * @param file          Open sftp file handle to write to.
 *
 * @param buf           Pointer to buffer to write data.
 *
 * @param count         Size of buffer in bytes.
 *
 * @return              Number of bytes written, < 0 on error with ssh and sftp
 *                      error set.
 *
 * @see                 sftp_open()
 * @see                 sftp_read()
 * @see                 sftp_close()
 */
LIBSSH_API ssize_t sftp_write(SFTP_FILE *file, const void *buf, size_t count);

/**
 * @brief Seek to a specific location in a file.
 *
 * @param file         Open sftp file handle to seek in.
 *
 * @param new_offset   Offset in bytes to seek.
 *
 * @return             0 on success, < 0 on error.
 */
LIBSSH_API int sftp_seek(SFTP_FILE *file, uint32_t new_offset);

/**
 * @brief Seek to a specific location in a file. This is the
 * 64bit version.
 *
 * @param file         Open sftp file handle to seek in.
 *
 * @param new_offset   Offset in bytes to seek.
 *
 * @return             0 on success, < 0 on error.
 */
LIBSSH_API int sftp_seek64(SFTP_FILE *file, uint64_t new_offset);

/**
 * @brief Report current byte position in file.
 *
 * @param file          Open sftp file handle.
 *
 * @return              The offset of the current byte relative to the beginning
 *                      of the file associated with the file descriptor. < 0 on
 *                      error.
 */
LIBSSH_API unsigned long sftp_tell(SFTP_FILE *file);

/**
 * @brief Report current byte position in file.
 *
 * @param file          Open sftp file handle.
 *
 * @return              The offset of the current byte relative to the beginning
 *                      of the file associated with the file descriptor. < 0 on
 *                      error.
 */
LIBSSH_API uint64_t sftp_tell64(SFTP_FILE *file);

/**
 * @brief Rewinds the position of the file pointer to the beginning of the
 * file.
 *
 * @param file          Open sftp file handle.
 */
LIBSSH_API void sftp_rewind(SFTP_FILE *file);

/**
 * @deprecated          Use sftp_unlink() instead.
 */
LIBSSH_API int sftp_rm(SFTP_SESSION *sftp, const char *file) SFTP_DEPRECATED;

/**
 * @brief Unlink (delete) a file.
 *
 * @param sftp          The sftp session handle.
 *
 * @param file          The file to unlink/delete.
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_unlink(SFTP_SESSION *sftp, const char *file);

/**
 * @brief Remove a directoy.
 *
 * @param sftp          The sftp session handle.
 *
 * @param directory     The directory to remove.
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_rmdir(SFTP_SESSION *sftp, const char *directory);

/**
 * @brief Create a directory.
 *
 * @param sftp          The sftp session handle.
 *
 * @param directory     The directory to create.
 *
 * @param mode          Specifies the permissions to use. It is modified by the
 *                      process's umask in the usual way:
 *                      The permissions of the created file are (mode & ~umask)
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_mkdir(SFTP_SESSION *sftp, const char *directory, mode_t mode);

/**
 * @brief Rename or move a file or directory.
 *
 * @param sftp          The sftp session handle.
 *
 * @param original      The original url (source url) of file or directory to
 *                      be moved.
 *
 * @param newname       The new url (destination url) of the file or directory
 *                      after the move.
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_rename(SFTP_SESSION *sftp, const char *original, const  char *newname);

/**
 * @brief Set file attributes on a file, directory or symbolic link.
 *
 * @param sftp          The sftp session handle.
 *
 * @param file          The file which attributes should be changed.
 *
 * @param attr          The file attributes structure with the attributes set
 *                      which should be changed.
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_setstat(SFTP_SESSION *sftp, const char *file, SFTP_ATTRIBUTES *attr);

/**
 * @brief Change the file owner and group
 *
 * @param sftp          The sftp session handle.
 *
 * @param file          The file which owner and group should be changed.
 *
 * @param owner         The new owner which should be set.
 *
 * @param group         The new group which should be set.
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_chown(SFTP_SESSION *sftp, const char *file, uid_t owner, gid_t group);

/**
 * @brief Change permissions of a file
 *
 * @param sftp          The sftp session handle.
 *
 * @param file          The file which owner and group should be changed.
 *
 * @param mode          Specifies the permissions to use. It is modified by the
 *                      process's umask in the usual way:
 *                      The permissions of the created file are (mode & ~umask)
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_chmod(SFTP_SESSION *sftp, const char *file, mode_t mode);

/**
 * @brief Change the last modification and access time of a file.
 *
 * @param sftp          The sftp session handle.
 *
 * @param file          The file which owner and group should be changed.
 *
 * @param times         A timeval structure which contains the desired access
 *                      and modification time.
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_utimes(SFTP_SESSION *sftp, const char *file, const struct timeval *times);

/**
 * @brief Create a symbolic link.
 *
 * @param  sftp         The sftp session handle.
 *
 * @param  target       Specifies the target of the symlink.
 *
 * @param  dest         Specifies the path name of the symlink to be created.
 *
 * @return              0 on success, < 0 on error with ssh and sftp error set.
 */
LIBSSH_API int sftp_symlink(SFTP_SESSION *sftp, const char *target, const char *dest);

/**
 * @brief Read the value of a symbolic link.
 *
 * @param  sftp         The sftp session handle.
 *
 * @param  path         Specifies the path name of the symlink to be read.
 *
 * @return              The target of the link, NULL on error.
 */
LIBSSH_API char *sftp_readlink(SFTP_SESSION *sftp, const char *path);

/**
 * @brief Get information about a mounted file system.
 *
 * @param  sftp         The sftp session handle.
 *
 * @param  path         The pathname of any file within the mounted file system.
 *
 * @return A statvfs structure or NULL on error.
 */
LIBSSH_API SFTP_STATVFS *sftp_statvfs(SFTP_SESSION *sftp, const char *path);

/**
 * @brief Get information about a mounted file system.
 *
 * @param  file         An opened file.
 *
 * @return A statvfs structure or NULL on error.
 */
LIBSSH_API SFTP_STATVFS *sftp_fstatvfs(SFTP_FILE *file);

/**
 * @brief Free the memory of an allocated statvfs.
 *
 * @param  statvfs      The statvfs to free.
 */
LIBSSH_API void sftp_statvfs_free(SFTP_STATVFS *statvfs);

/**
 * @brief Canonicalize a sftp path.
 *
 * @param sftp          The sftp session handle.
 *
 * @param path          The path to be canonicalized.
 *
 * @return              The canonicalize path, NULL on error.
 */
LIBSSH_API char *sftp_canonicalize_path(SFTP_SESSION *sftp, const char *path);

/**
 * @brief Get the version of the SFTP protocol supported by the server
 *
 * @param sftp          The sftp session handle.
 *
 * @return              The server version.
 */
LIBSSH_API int sftp_server_version(SFTP_SESSION *sftp);

#ifdef WITH_SERVER
/**
 * @brief Create a new sftp server session.
 *
 * @param session       The ssh session to use.
 *
 * @param chan          The ssh channel to use.
 *
 * @return              A new sftp server session.
 */
LIBSSH_API SFTP_SESSION *sftp_server_new(SSH_SESSION *session, ssh_channel chan);

/**
 * @brief Intialize the sftp server.
 *
 * @param sftp         The sftp session to init.
 *
 * @return             0 on success, < 0 on error.
 */
LIBSSH_API int sftp_server_init(SFTP_SESSION *sftp);
#endif  /* WITH_SERVER */

/* this is not a public interface */
#define SFTP_HANDLES 256
SFTP_PACKET *sftp_packet_read(SFTP_SESSION *sftp);
int sftp_packet_write(SFTP_SESSION *sftp,uint8_t type, ssh_buffer payload);
void sftp_packet_free(SFTP_PACKET *packet);
int buffer_add_attributes(ssh_buffer buffer, SFTP_ATTRIBUTES *attr);
SFTP_ATTRIBUTES *sftp_parse_attr(SFTP_SESSION *session, ssh_buffer buf,int expectname);
/* sftpserver.c */

SFTP_CLIENT_MESSAGE *sftp_get_client_message(SFTP_SESSION *sftp);
void sftp_client_message_free(SFTP_CLIENT_MESSAGE *msg);
int sftp_reply_name(SFTP_CLIENT_MESSAGE *msg, const char *name,
    SFTP_ATTRIBUTES *attr);
int sftp_reply_handle(SFTP_CLIENT_MESSAGE *msg, ssh_string handle);
ssh_string sftp_handle_alloc(SFTP_SESSION *sftp, void *info);
int sftp_reply_attr(SFTP_CLIENT_MESSAGE *msg, SFTP_ATTRIBUTES *attr);
void *sftp_handle(SFTP_SESSION *sftp, ssh_string handle);
int sftp_reply_status(SFTP_CLIENT_MESSAGE *msg, uint32_t status, const char *message);
int sftp_reply_names_add(SFTP_CLIENT_MESSAGE *msg, const char *file,
    const char *longname, SFTP_ATTRIBUTES *attr);
int sftp_reply_names(SFTP_CLIENT_MESSAGE *msg);
int sftp_reply_data(SFTP_CLIENT_MESSAGE *msg, const void *data, int len);
void sftp_handle_remove(SFTP_SESSION *sftp, void *handle);

/* SFTP commands and constants */
#define SSH_FXP_INIT 1
#define SSH_FXP_VERSION 2
#define SSH_FXP_OPEN 3
#define SSH_FXP_CLOSE 4
#define SSH_FXP_READ 5
#define SSH_FXP_WRITE 6
#define SSH_FXP_LSTAT 7
#define SSH_FXP_FSTAT 8
#define SSH_FXP_SETSTAT 9
#define SSH_FXP_FSETSTAT 10
#define SSH_FXP_OPENDIR 11
#define SSH_FXP_READDIR 12
#define SSH_FXP_REMOVE 13
#define SSH_FXP_MKDIR 14
#define SSH_FXP_RMDIR 15
#define SSH_FXP_REALPATH 16
#define SSH_FXP_STAT 17
#define SSH_FXP_RENAME 18
#define SSH_FXP_READLINK 19
#define SSH_FXP_SYMLINK 20

#define SSH_FXP_STATUS 101
#define SSH_FXP_HANDLE 102
#define SSH_FXP_DATA 103
#define SSH_FXP_NAME 104
#define SSH_FXP_ATTRS 105

#define SSH_FXP_EXTENDED 200
#define SSH_FXP_EXTENDED_REPLY 201

/* attributes */
/* sftp draft is completely braindead : version 3 and 4 have different flags for same constants */
/* and even worst, version 4 has same flag for 2 different constants */
/* follow up : i won't develop any sftp4 compliant library before having a clarification */

#define SSH_FILEXFER_ATTR_SIZE 0x00000001
#define SSH_FILEXFER_ATTR_PERMISSIONS 0x00000004
#define SSH_FILEXFER_ATTR_ACCESSTIME 0x00000008
#define SSH_FILEXFER_ATTR_ACMODTIME  0x00000008
#define SSH_FILEXFER_ATTR_CREATETIME 0x00000010
#define SSH_FILEXFER_ATTR_MODIFYTIME 0x00000020
#define SSH_FILEXFER_ATTR_ACL 0x00000040
#define SSH_FILEXFER_ATTR_OWNERGROUP 0x00000080
#define SSH_FILEXFER_ATTR_SUBSECOND_TIMES 0x00000100
#define SSH_FILEXFER_ATTR_EXTENDED 0x80000000
#define SSH_FILEXFER_ATTR_UIDGID 0x00000002

/* types */
#define SSH_FILEXFER_TYPE_REGULAR 1
#define SSH_FILEXFER_TYPE_DIRECTORY 2
#define SSH_FILEXFER_TYPE_SYMLINK 3
#define SSH_FILEXFER_TYPE_SPECIAL 4
#define SSH_FILEXFER_TYPE_UNKNOWN 5

/* server responses */
#define SSH_FX_OK 0
#define SSH_FX_EOF 1
#define SSH_FX_NO_SUCH_FILE 2
#define SSH_FX_PERMISSION_DENIED 3
#define SSH_FX_FAILURE 4
#define SSH_FX_BAD_MESSAGE 5
#define SSH_FX_NO_CONNECTION 6
#define SSH_FX_CONNECTION_LOST 7
#define SSH_FX_OP_UNSUPPORTED 8
#define SSH_FX_INVALID_HANDLE 9
#define SSH_FX_NO_SUCH_PATH 10
#define SSH_FX_FILE_ALREADY_EXISTS 11
#define SSH_FX_WRITE_PROTECT 12
#define SSH_FX_NO_MEDIA 13

/* file flags */
#define SSH_FXF_READ 0x01
#define SSH_FXF_WRITE 0x02
#define SSH_FXF_APPEND 0x04
#define SSH_FXF_CREAT 0x08
#define SSH_FXF_TRUNC 0x10
#define SSH_FXF_EXCL 0x20
#define SSH_FXF_TEXT 0x40

/* rename flags */
#define SSH_FXF_RENAME_OVERWRITE  0x00000001
#define SSH_FXF_RENAME_ATOMIC     0x00000002
#define SSH_FXF_RENAME_NATIVE     0x00000004

#define SFTP_OPEN SSH_FXP_OPEN
#define SFTP_CLOSE SSH_FXP_CLOSE
#define SFTP_READ SSH_FXP_READ
#define SFTP_WRITE SSH_FXP_WRITE
#define SFTP_LSTAT SSH_FXP_LSTAT
#define SFTP_FSTAT SSH_FXP_FSTAT
#define SFTP_SETSTAT SSH_FXP_SETSTAT
#define SFTP_FSETSTAT SSH_FXP_FSETSTAT
#define SFTP_OPENDIR SSH_FXP_OPENDIR
#define SFTP_READDIR SSH_FXP_READDIR
#define SFTP_REMOVE SSH_FXP_REMOVE
#define SFTP_MKDIR SSH_FXP_MKDIR
#define SFTP_RMDIR SSH_FXP_RMDIR
#define SFTP_REALPATH SSH_FXP_REALPATH
#define SFTP_STAT SSH_FXP_STAT
#define SFTP_RENAME SSH_FXP_RENAME
#define SFTP_READLINK SSH_FXP_READLINK
#define SFTP_SYMLINK SSH_FXP_SYMLINK

/* openssh flags */
#define SSH_FXE_STATVFS_ST_RDONLY 0x1 /* read-only */
#define SSH_FXE_STATVFS_ST_NOSUID 0x2 /* no setuid */

#ifdef __cplusplus
} ;
#endif

#endif /* SFTP_H */

/** @} */
/* vim: set ts=2 sw=2 et cindent: */
