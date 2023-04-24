#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char *argv_new[MAXARG];
    int i;
    char c, *p, buf[MAXARG];

    // 将xargs后的参数全部传到新argv中
    for (i = 1; i < argc; i++)
    {
        argv_new[i - 1] = argv[i];
    }

    p = buf;

    // 每次读取一个字节，并记录到buf中
    while (read(0, &c, sizeof(char)) > 0)
    {
        *p = c;
        // 当遇到换行时结束此行，并执行exec
        if (c == '\n')
        {
            // 将\n换为结束符
            *p = 0;
            // buf也添加到新argv里
            argv_new[i - 1] = buf;

            if (fork())
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