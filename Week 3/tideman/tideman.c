#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 最大候选人数量
#define MAX 9

// preferences[i][j] 是偏好i胜过j的选民数量
int preferences[MAX][MAX];

// locked[i][j] 表示i被锁定在j之上
bool locked[MAX][MAX];

// 每对候选人都有一个赢家和一个输家
typedef struct
{
    int winner;  // 胜者
    int loser;   // 败者
} pair;

// 候选人数组
char* candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];  // 所有可能的候选人对

int pair_count;  // 候选人对的数量
int candidate_count;  // 候选人数量

// 函数原型
bool vote(int rank, char* name, int ranks[]);

void record_preferences(int ranks[]);

void add_pairs(void);

void sort_pairs(void);

void lock_pairs(void);

void print_winner(void);

int main(int argc, char* argv[])
{
    // 检查是否传入候选人参数
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // 清空 locked 图
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count;

    // 获取选民数量
    printf("Number of voters: ");
    scanf("%d", &voter_count);

    // 查询每个选民的投票
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] 是选民的第i个偏好
        int ranks[candidate_count];

        // 查询每个候选人的排名
        for (int j = 0; j < candidate_count; j++)
        {
            char name[100];
            printf("Rank %i: ", j + 1);
            getchar();  // 清除之前输入留下的换行符
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;  // 去除字符串末尾的换行符

            // 记录投票，如果无效则返回错误
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// 根据新投票更新排名
bool vote(int rank, char* name, int ranks[])
{
    // 在候选人列表中查找选民选择的候选人
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// 根据选民的排名更新偏好矩阵
void record_preferences(int ranks[])
{
    // 对每对候选人，根据选民的投票更新偏好
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (ranks[i] < ranks[j])
            {
                preferences[ranks[i]][ranks[j]]++;
            }
            else if (ranks[i] > ranks[j])
            {
                preferences[ranks[j]][ranks[i]]++;
            }
        }
    }
}

// 记录每一对候选人的胜负关系
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // 如果i胜过j
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            // 如果j胜过i
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

// 根据胜利幅度对候选人对进行排序
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            int strength_i = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            int strength_j = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            
            // 如果i的胜利幅度小于j，则交换
            if (strength_i < strength_j)
            {
                pair temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
}

// 锁定候选人对的关系，并避免形成环
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        
        // 检查是否会形成环，如果有环则取消锁定
        bool cycle = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[pairs[i].loser][j] && locked[j][pairs[i].winner])
            {
                cycle = true;
                break;
            }
        }
        
        // 如果发现环，则取消锁定这对候选人
        if (cycle)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

// 打印选举的赢家
void print_winner(void)
{
    // 赢家是图中没有其他候选人指向的候选人
    for (int i = 0; i < candidate_count; i++)
    {
        bool winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                winner = false;
                break;
            }
        }
        
        if (winner)
        {
            printf("The winner is: %s\n", candidates[i]);
            return;
        }
    }
}
