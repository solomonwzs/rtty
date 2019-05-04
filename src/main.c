#define _XOPEN_SOURCE 600
#include <stdlib.h>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define PTMX    "/dev/ptmx"
#define BUFSIZE 1024

extern int grantpt (int __fd) __THROW;
extern int unlockpt (int __fd) __THROW;
extern char *ptsname (int __fd) __THROW __wur;

int
main(int argc, char **argv) {
  int mfd = open(PTMX, O_RDWR);
  grantpt(mfd);
  unlockpt(mfd);
  fprintf(stderr, "%s\n", ptsname(mfd));

  char buf[BUFSIZE];
  int pid = fork();
  if (pid) {
    while (1) {
      int n = read(mfd, buf, BUFSIZE);
      if (n > 0) {
        write(1, buf, n);
      } else {
        exit(1);
      }
    }
  } else {
    while (1) {
      int n = read(0, buf, BUFSIZE);
      if (n > 0) {
        write(mfd, buf, n);
      } else {
        exit(1);
      }
    }
  }

  return 0;
}
