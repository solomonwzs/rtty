#ifndef __INCLUDE_PTY_MASTER_H__
#define __INCLUDE_PTY_MASTER_H__

extern int echo_input();

extern int master_stdio(int fdm);

extern int master_tty(int fdm, int fdi, int fdo);

#endif
