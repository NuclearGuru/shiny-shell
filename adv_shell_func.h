#ifndef ADV_SHELL_FUNC_H
#define ADV_SHELL_FUNC_H

inline int run(int argc, char** argv);
int cmp_str(const char *a, const char *b, int depth);
inline char **prepare_argv(char *buf, int *buflen, int *argc);

#endif
