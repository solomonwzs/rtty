#include "common.h"
#include <stdio.h>


int
get_pty_master_fd() {
  int fdm;
  int rc;

  fdm = posix_openpt(O_RDWR);
  if (fdm < 0) {
    perror("posix_openpt: ");
    return -1;
  }

  if ((rc = grantpt(fdm)) != 0) {
    perror("grantpt: ");
    return -1;
  }

  if ((rc = unlockpt(fdm)) != 0) {
    perror("unlockpt: ");
    return -1;
  }
  return fdm;
}


int
get_pty_slaver_fd(int fdm) {
  int fds;
  char *name;

  if ((name = ptsname(fdm)) == NULL) {
    perror("ptsname: ");
    return -1;
  }

  if ((fds = open(name, O_RDWR)) < 0) {
    perror("open: ");
    return -1;
  }
  return fds;
}
