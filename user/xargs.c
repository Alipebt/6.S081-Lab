#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARGS 10

int main(int argc, char *argv[])
{
    int pid;
    char *argv_new[MAXARGS];
    char c, buf[512];
    char *p = buf;
    int i;

    for (i = 1; i < argc; i++)
    {
        argv_new[i - 1] = argv[i];
    }

    while (read(0, &c, sizeof(char)))
    {
        *p = c;
        if (c == '\n')
        {
            *p = 0;
            argv_new[i - 1] = buf;
            if ((pid = fork()))
            {
                wait(0);
            }
            else
            {
                exec(argv[1], argv_new);
            }
        }
        p++;
    }

    exit(0);
}