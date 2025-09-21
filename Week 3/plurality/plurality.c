#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 候选人的最大数量
#define MAX 9

// 候选人结构体，包含其名字和票数
typedef struct
{
    char *name;
    int votes;
} candidate;

// 候选人数组
candidate candidates[MAX];

// 候选人数量
int candidate_count;

// 函数声明
bool vote(char *name);
void print_winner(void);

int main(int argc, char *argv[])
{
    // 检查命令行参数数量是否正确
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // 初始化候选人数组
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count;
    printf("Number of voters: ");
    scanf("%d", &voter_count);

    // 循环获取每个选民的投票
    for (int i = 0; i < voter_count; i++)
    {
        char name[100];
        printf("Vote: ");
        scanf("%s", name);

        // 检查投票是否合法
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // 输出选举结果
    print_winner();
    return 0;
}

// 更新投票数
bool vote(char *name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0) // 比较名字
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// 输出选举结果
void print_winner(void)
{
    int max_votes = 0;

    // 找到最高票数
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > max_votes)
        {
            max_votes = candidates[i].votes;
        }
    }

    // 输出所有获得最高票数的候选人
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == max_votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}