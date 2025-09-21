#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    // 检查命令行参数数量是否为 2
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // 获取密钥并验证是否为正整数
    int key = atoi(argv[1]);
    if (key <= 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // 获取用户输入的明文
    char plaintext[1000];
    printf("plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);

    // 去除换行符
    plaintext[strcspn(plaintext, "\n")] = 0;

    // 输出密文提示
    printf("ciphertext: ");

    // 遍历明文中的每个字符
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        // 处理小写字母
        if (islower(plaintext[i])) // 检查是否为小写字母
        {
            printf("%c", 'a' + (plaintext[i] - 'a' + key) % 26);
        }
        // 处理大写字母
        else if (isupper(plaintext[i])) // 检查是否为大写字母
        {
            printf("%c", 'A' + (plaintext[i] - 'A' + key) % 26);
        }
        // 非字母字符直接输出
        else
        {
            printf("%c", plaintext[i]);
        }
    }

    printf("\n");
    return 0;
}
