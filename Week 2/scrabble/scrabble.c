#include <ctype.h>
#include <stdio.h>
#include <string.h>

// 每个字母的得分
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// 计算单词得分的函数
int compute_score(char word[]);

int main(void)
{
    // 获取两个玩家的输入单词
    char word1[100], word2[100];
    printf("Player 1: ");
    fgets(word1, sizeof(word1), stdin);  // 获取玩家1输入
    printf("Player 2: ");
    fgets(word2, sizeof(word2), stdin);  // 获取玩家2输入

   // 如果输入中包含换行符，则去除它
    word1[strcspn(word1, "\n")] = 0;
    word2[strcspn(word2, "\n")] = 0;

    // 计算两个单词的得分
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // 输出获胜者
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }

    return 0;
}

int compute_score(char word[])
{
   int score = 0;

    // 遍历单词中的每个字符
    for (int i = 0; word[i] != '\0'; i++)
    {
        char c = tolower(word[i]); // 将字符转换为小写字母

        // 确保字符是字母
        if (c >= 'a' && c <= 'z')
        {
            score += POINTS[c - 'a']; // 根据字母的索引来获取得分
        }
    }

    return score;
}
