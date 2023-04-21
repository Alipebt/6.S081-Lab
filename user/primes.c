/*
        ______           ______           ______           ______           ______
2  ->  | p  2 |         |      |         |      |         |      |         |      |
3  ->  |      |  3  ->  | p  3 |         |      |         |      |         |      |
4  ->  | drop |         |      |         |      |         |      |         |      |
5  ->  |      |  5  ->  |      |  5  ->  | p  5 |         |      |         |      |
6  ->  | drop |         |      |         |      |         |      |         |      |
7  ->  |      |  7  ->  |      |  7  ->  |      |  7  ->  | p  7 |         |      |
8  ->  | drop |         |      |         |      |         |      |         |      |
9  ->  |      |  9  ->  |      |  9  ->  | drop |         |      |         |      |
10 ->  | drop |         |      |         |      |         |      |         |      |
11 ->  |      |  11 ->  |      |  11 ->  |      |  11 ->  |      |  11 ->  | p 11 |
   ->  |      |         |      |         |      |         |      |         |      |

*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void selectPrime(int p2p[])
{
    int prime;
    int num;

    int p2c[2];
    pipe(p2c);

    // 接收的第一个数必为素数
    int n = read(p2p[0], &prime, sizeof(int));

    if (n > 0)
    {
        // 防止当 read 返回 0 时，输出 prime = 0
        printf("prime %d\n", prime);

        if (fork() == 0)
        {
            // 当前管道的读端关闭
            close(p2p[0]);
            // 右管道的写端关闭
            close(p2c[1]);

            selectPrime(p2c);
        }
        else
        {
            while ((n = read(p2p[0], &num, sizeof(int))))
            {
                if (num % prime != 0)
                {
                    write(p2c[1], &num, sizeof(int));
                }
            }
            
            // 当前管道的读端关闭
            close(p2p[0]);

            // 右管道关闭
            close(p2c[0]);
            close(p2c[1]);
        }
    }

    return;
}

int main()
{
    // 管道的定义以及初始化
    int p2c[2];
    pipe(p2c);

    if (fork() == 0)
    {
        // 右管道的写端关闭
        close(p2c[1]);
        // 管道筛选
        selectPrime(p2c);
    }
    else
    {
        // 当前管道的读端关闭
        close(p2c[0]);

        for (int i = 2; i <= 35; i++)
        {
            // 向右管道写 2～35
            write(p2c[1], &i, sizeof(int));
        }

        // 当前管道的写端关闭
        close(p2c[1]);
    }

    exit(0);
}