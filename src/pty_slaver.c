#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

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


int
slaver_tty(int fds, int argc, char **argv) {
  struct termios ori_settings;
  struct termios new_settings;

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
  close(2);

  dup(fds);
  dup(fds);
  dup(fds);

  setsid();
  ioctl(0, TIOCSCTTY, 1);

  char **child_av = malloc((argc + 1) * sizeof(char *));
  for (int i = 0; i < argc; ++i) {
    child_av[i] = strdup(argv[i]);
  }
  child_av[argc] = NULL;
  if (execvp(child_av[0], child_av) < 0) {
    perror("execvp: ");
    return -1;
  }

  return 0;
}
