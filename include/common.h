#ifndef __INCLUDE_COMMON_H__
#define __INCLUDE_COMMON_H__

#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <fcntl.h>

// This call Creates the master side of the pty. It opens the device /dev/ptmx
// to get the file descriptor belonging to the master side.
extern int posix_openpt(int flags);

// The file descriptor got from posix_openpt() is passed to grantpt() to change
// the access rights on the slave side: the user identifier of the device is
// set to the user identifier of the calling process. The group is set to an
// unspecified value (e.g. "tty") and the access rights are set to crx--w----.
extern int grantpt(int fd);

// After grantpt(), the file descriptor is passed to unlockpt() to unlock the
// slave side.
extern int unlockpt(int fd);

// In order to be able to open the slave side, we need to get its file name
// through ptsname().
extern char *ptsname(int fd);

extern int get_pty_master_fd();
extern int get_pty_slaver_fd(int fdm);

#endif
