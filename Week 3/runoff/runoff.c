#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 最大选民和候选人数量
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] 代表第 i 位选民的选择第 j 候选人
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// 候选人结构体，包含名字、投票数和是否被淘汰的状态
typedef struct
{
    char name[100];
    int votes;
    bool eliminated;
} candidate;

// 候选人数组
candidate candidates[MAX_CANDIDATES];

// 选民和候选人数量
int voter_count;
int candidate_count;

// 函数原型
bool vote(int voter, int rank, char name[]);

void tabulate(void);

bool print_winner(void);

int find_min(void);

bool is_tie(int min);

void eliminate(int min);

int main(int argc, char *argv[])
{
    // 检查命令行参数是否正确
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // 初始化候选人数组
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }

    // 设置他们的名字、开始时投票数为0，且未被淘汰
    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i].name, argv[i + 1]);
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    printf("Number of voters: ");
    if (scanf("%d", &voter_count) != 1 || voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // 循环收集选民的投票
    for (int i = 0; i < voter_count; i++)
    {
        // 循环获取每一位选民的排名
        for (int j = 0; j < candidate_count; j++)
        {
            char name[100];
            printf("Rank %i: ", j + 1);
            getchar();  // 清除之前输入留下的换行符
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;  // 去除字符串末尾的换行符

            // 记录投票，除非是无效投票
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
        printf("\n");
    }

    // 循环进行多轮选举，直到选出一个赢家
    while (true)
    {
        // 根据剩余的候选人统计投票
        tabulate();

        // 检查选举是否结束，是否有赢家
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // 淘汰得票最少的候选人
        int min = find_min();
        bool tie = is_tie(min);

        // 如果是平局，所有未淘汰的候选人都算赢家
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // 淘汰得票最少的候选人
        eliminate(min);

        // 重置所有候选人的投票数为零
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// 如果投票有效，记录选民的偏好
bool vote(int voter, int rank, char name[])
{
    // 查找候选人是否存在
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// 统计所有未被淘汰候选人的得票数
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int candidate_index = preferences[i][j];
            if (!candidates[candidate_index].eliminated)
            {
                candidates[candidate_index].votes++;
                break;
            }
        }
    }
}

// 打印选举的赢家（如果有的话）
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // 如果某个候选人得票超过一半，宣布为赢家
        if (candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// 找到剩余候选人中得票最少的那一个
int find_min(void)
{
    int min = MAX_VOTERS;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes < min)
        {
            min = candidates[i].votes;
        }
    }
    return min;
}

// 如果选举在所有剩余候选人中存在平局，返回true，否则返回false
bool is_tie(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes > min)
        {
            return false;
        }
    }
    return true;
}

// 淘汰得票最少的候选人
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}