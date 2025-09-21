#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// 函数声明
int is_valid_key(const char *key);
void encrypt_text(const char *plaintext, const char *key);

int main(int argc, char *argv[])
{
    // 检查命令行参数数量是否为 2
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // 获取密钥并验证其有效性
    const char *key = argv[1];
    if (!is_valid_key(key))
    {
        return 1;
    }

    // 获取用户输入的明文
    char plaintext[1000];
    printf("plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);

    // 去除换行符
    plaintext[strcspn(plaintext, "\n")] = 0;

    // 输出密文
    printf("ciphertext: ");
    encrypt_text(plaintext, key);
    printf("\n");

    return 0;
}

// 验证密钥的有效性
int is_valid_key(const char *key)
{
    // 检查密钥长度是否为 26
    if (strlen(key) != ALPHABET_SIZE)
    {
        printf("Key must contain 26 characters.\n");
        return 0;
    }

    // 检查密钥是否只包含字母字符，且不包含重复字母
    char seen[ALPHABET_SIZE] = {0};
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 0;
        }

        char c = toupper(key[i]);
        if (seen[c - 'A'])
        {
            printf("Key must not contain repeated characters.\n");
            return 0;
        }
        seen[c - 'A'] = 1;
    }

    return 1;
}

// 使用替换密码加密文本
void encrypt_text(const char *plaintext, const char *key)
{
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        char c = plaintext[i];

        // 处理小写字母
        if (islower(c))
        {
            printf("%c", tolower(key[c - 'a']));
        }
        // 处理大写字母
        else if (isupper(c))
        {
            printf("%c", toupper(key[c - 'A']));
        }
        // 非字母字符直接输出
        else
        {
            printf("%c", c);
        }
    }
}
