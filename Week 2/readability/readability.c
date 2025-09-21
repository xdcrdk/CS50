#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(const char *text); // 计算字母数量的函数
int count_words(const char *text); // 计算单词数量的函数
int count_sentences(const char *text); // 计算句子数量的函数

int main(void)
{
    // 获取用户输入的文本
    char text[1000];
    printf("Text: ");
    fgets(text, sizeof(text), stdin);

    // 去除换行符
    text[strcspn(text, "\n")] = 0;

    // 计算字母、单词和句子的数量
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // 计算 Coleman-Liau 指数
    float L = (float) letters / words * 100;
    float S = (float) sentences / words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    // 四舍五入并输出年级水平
    int grade = round(index);
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

    return 0;
}

// 计算字母数量
int count_letters(const char *text)
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i])) // 检查字符是否为字母
        {
            count++;
        }
    }
    return count;
}

// 计算单词数量
int count_words(const char *text)
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isspace(text[i]) || text[i] == '\n') // 检查是否为空格或者换行符
        {
            count++;
        }
    }
    // 假设文本至少有一个单词
    return count + 1;
}

// 计算句子数量
int count_sentences(const char *text)
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') // 检查句号、感叹号和问号
        {
            count++;
        }
    }
    return count;
}
