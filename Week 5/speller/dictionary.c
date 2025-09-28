// 拼写检查字典的核心实现

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"


// 单链表结点
typedef struct node
{
    char word[LENGTH + 1]; // 存放一个字典单词（含结尾 '\0'）
    struct node *next;     // 指向同桶中的下一个结点
} node;


const unsigned int N = (1u << 16); // 桶数量

static node *table[N]; // 哈希表

static unsigned int word_count = 0; // 当前已加载的单词数量

static bool loaded = false; // 标记是否已成功加载过字典

// 把 C 字符串转为小写
static inline void tolower_copy(char *dst, const char *src)
{
    for (int i = 0; src[i] != '\0'; i++)
    {
        unsigned char c = (unsigned char)src[i];
        dst[i] = (char)tolower(c);
        if (dst[i] == '\0') break;
        dst[i + 1] = '\0';
    }
}

// 哈希函数
// 遍历小写字符，采用“乘-异或-轻度自混合”的滚动哈希，最后用按位与 (&) 代替取模（因为 N 为 2 的幂）。
unsigned int hash(const char *word)
{
    unsigned int h = 0;
    for (const unsigned char *p = (const unsigned char *)word; *p; p++)
    {
        // 统一转小写
        unsigned char c = *p;
        if (c >= 'A' && c <= 'Z') c = (unsigned char)(c - 'A' + 'a');

        // 乘以一个奇数常量再异或字符值；随后做一次右移自混合，增加高位参与
        h = (h * 131u) ^ c;
        h ^= (h >> 16);
    }
    return h & (N - 1); // 代替 % N，等价且更快（N 为 2 的幂）
}

// 查询函数，若单词在字典中返回 true，否则 false
bool check(const char *word)
{
    // 把查询词转为小写
    char lower[LENGTH + 1];
    int len = 0;
    for (; word[len] != '\0' && len < LENGTH; len++)
    {
        unsigned char c = (unsigned char)word[len];
        lower[len] = (char)tolower(c);
    }
    lower[len] = '\0';

    // 根据哈希值找到桶，遍历链表查找
    unsigned int idx = hash(lower);
    for (node *cur = table[idx]; cur != NULL; cur = cur->next)
    {
        // 字典中全为小写，因此直接用 strcmp 即可
        if (strcmp(cur->word, lower) == 0)
        {
            return true;
        }
    }
    return false;
}

// 加载函数，成功返回 true，失败返回 false ==========
bool load(const char *dictionary)
{
    // 打开字典文件（只读）
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }

    // 每次读入一个单词
    char buffer[LENGTH + 1];

    while (fscanf(fp, "%45s", buffer) == 1)
    {
        // 为该单词分配结点
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            // 分配失败，关闭文件并返回 false
            fclose(fp);
            return false;
        }

        // 把 buffer 作为小写词拷贝到结点
        tolower_copy(n->word, buffer);

        // 头插到对应桶
        unsigned int idx = hash(n->word);
        n->next = table[idx];
        table[idx] = n;

        word_count++;
    }

    fclose(fp);
    loaded = true;
    return true;
}

// 返回字典中单词的数量（若尚未加载则为 0）
unsigned int size(void)
{
    return loaded ? word_count : 0;
}

// 卸载字典（释放全部内存）成功返回 true
bool unload(void)
{
    // 遍历每个桶，逐结点释放
    for (unsigned int i = 0; i < N; i++)
    {
        node *cur = table[i];
        while (cur != NULL)
        {
            node *next = cur->next;
            free(cur);
            cur = next;
        }
        table[i] = NULL;
    }

    // 重置全局状态
    word_count = 0;
    loaded = false;
    return true;
}
