#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    // 目录条目信息结构体
    struct dirent de;
    // 文件或者目录的状态信息的结构体
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // 获取文件描述符指向文件的状态信息
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot fstat %s\n", path);
        close(fd);
        return;
    }

    // 判断当前位置属性（目录），目录下的文件可由遍历获取，不需递归
    if (st.type == T_DIR)
    {
        // 目录长度
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
        {
            printf("find: path too long\n");
        }
        else
        {

            // 遍历目录
            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if (de.inum == 0 ||
                    strcmp(de.name, ".") == 0 ||
                    strcmp(de.name, "..") == 0)
                {
                    continue;
                }

                // 将buf替换为当前path，再将它改为文件路径
                strcpy(buf, path);
                p = buf + strlen(buf);
                *p = '/';
                p++;
                strcpy(p, de.name);

                // 读取buf状态信息
                if (stat(buf, &st) < 0)
                {
                    fprintf(2, "find: cannot stat %s\n", buf);
                    continue;
                }

                // 判断文件是否为所查询的文件
                if (strcmp(de.name, name) == 0)
                {
                    printf("%s\n", buf);
                }
                // 递归目录
                if (st.type == T_DIR)
                {
                    find(buf, name);
                }
            }
        }
    }
    close(fd);
    return;
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        // 缺少参数
        fprintf(2,"Missing required arguments\n");
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            // 第一个参数为目录，第二个为需要寻找的目录或文件
            find(argv[1], argv[i]);
        }
    }
    exit(0);
}