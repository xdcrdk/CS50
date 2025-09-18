#include <stdio.h>

static int read_height(void) // 读取高度的函数
{
    int h;
    int ch;
    for (;;)
    {
        printf("Height: ");
        if (scanf("%d", &h) == 1 && h >= 1 && h <= 8)
        {
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            return h;
        }
        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
}

int main(void)
{
    int h = read_height();

    for (int i = 1; i <= h; i++) // 即从上到下打印
    {
        for (int s = 0; s < h - i; s++) // 打印#前的空格
        {
            printf(" ");
        }

        for (int j = 0; j < i; j++) // 打印左侧的#
        {
            printf("#");
        }

        printf("  "); // 打印中间的空格

        for (int j = 0; j < i; j++) // 打印右侧的#
        {
            printf("#");
        }

        printf("\n"); // 每打印完一行后换行
    }

    return 0;
}

