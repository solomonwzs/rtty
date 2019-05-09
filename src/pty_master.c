#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "pty_master.h"

#define BUFSIZE 1024
#define INPUT_MSG "Input: "

#define copy(_fd_dst, _fd_src, _buf) do {\
  int __n;\
  __n = read(_fd_src, _buf, sizeof(_buf));\
  if (__n > 0) {\
    write(_fd_dst, _buf, __n);\
  } else {\
    perror("read: ");\
    return -1;\
  }\
} while (0)


int
echo_input() {
  char buf[BUFSIZE];
  fprintf(stdout, "Input: ");

  if (fgets(buf, sizeof(buf), stdin) == NULL) {
    perror("fgets: ");
    return 1;
  }
  fprintf(stdout, "Output: %s", buf);
  return 0;
}


int
master_stdio(int fdm) {
  int n;
  char buf[BUFSIZE];

  while (1) {
    write(1, INPUT_MSG, sizeof(INPUT_MSG));
    if ((n = read(0, buf, sizeof(buf))) > 0) {
      write(fdm, buf, n);
      if ((n = read(fdm, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        fprintf(stdout, "Output: %s", buf);
      } else {
        return -1;
      }
    } else {
      return -1;
    }
  }
  return 0;
}


int
master_tty(int fdm, int fdi, int fdo) {
  char buf[BUFSIZE];
  fd_set fd_in;
  int rc;
  int nfds;

  if (fdm > fdi) {
    nfds = fdm + 1;
  } else {
    nfds = fdi + 1;
  }

  while (1) {
    FD_ZERO(&fd_in);
    FD_SET(fdm, &fd_in);
    FD_SET(fdi, &fd_in);

    rc = select(nfds, &fd_in, NULL, NULL, NULL);
    switch (rc) {
      case -1:
        perror("select: ");
        return -1;
      default: {
        if (FD_ISSET(fdi, &fd_in)) {
          copy(fdm, fdi, buf);
        }

        if (FD_ISSET(fdm, &fd_in)) {
          copy(fdo, fdm, buf);
        }
      }
    }
  }
  return 0;
}
