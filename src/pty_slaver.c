#include <stdio.h>
#include <unistd.h>

#define __USE_BSD
#include <termios.h>

#include "pty_slaver.h"

#define BUFSIZE 1024


int
slaver_echo(int fds) {
  struct termios ori_settings;
  struct termios new_settings;
  int n;
  char buf[BUFSIZE];

  if (tcgetattr(fds, &ori_settings) < 0) {
    perror("tcgetattr: ");
    return -1;
  }

  new_settings = ori_settings;
  cfmakeraw(&new_settings);
  if (tcsetattr(fds, TCSANOW, &new_settings) < 0) {
    perror("tcsetattr: ");
    return -1;
  }

  close(0);
  close(1);
  dup(fds);
  dup(fds);
  while (1) {
    if ((n = read(fds, buf, sizeof(buf) - 1)) > 0) {
      buf[n - 1] = '\0';
      printf("recived: %s\n", buf);
    } else {
      return -1;
    }
  }
  return 0;
}
