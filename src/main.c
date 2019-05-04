#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "pty_master.h"
#include "pty_slaver.h"

#define INPUT_MSG "Input: "
#define BUFSIZE 1024


int
main(int argc, char **argv) {
  int fdm;
  int fds;

  if ((fdm = get_pty_master_fd()) < 0) {
    return -1;
  }
  if ((fds = get_pty_slaver_fd(fdm)) < 0) {
    close(fdm);
    return -1;
  }

  int pid = fork();
  if (pid != 0) {
    close(fds);
    master_stdio(fdm);
    close(fdm);
  } else {
    close(fdm);
    slaver_echo(fds);
    close(fds);
  }

  return 0;
}
