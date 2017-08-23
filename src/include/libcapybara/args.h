#ifndef LIBCAPYBARA_ARGS_H
#define LIBCAPYBARA_ARGS_H

#define ARGS_INNER(A,B) A ## B
#define ARGS(A,B) ARGS_INNER(A,B)

#define ARGN(n, LIST) ARGS(HANDLE_CFG_, n) LIST

#define HANDLE_CFG_1(_1,_2,_3)
#endif
