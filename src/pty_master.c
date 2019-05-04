#include <stdio.h>
#include <unistd.h>

#include "pty_master.h"

#define BUFSIZE 1024
#define INPUT_MSG "Input: "


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
