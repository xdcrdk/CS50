// 模拟血型的遗传

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 每个人有两个父母和两个等位基因
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);

void print_family(person *p, int generation);

void free_family(person *p);

char random_allele();

int main(void)
{
    // 为随机数生成器设定种子
    srand(time(0));

    // 创建一个包含三代人的家谱
    person *p = create_family(GENERATIONS);

    // 打印家族树中每个人的血型
    print_family(p, 0);

    // 释放内存
    free_family(p);
}

// 使用 “generations” 创建一个新个体
person *create_family(int generations)
{
    // 为当前个体分配内存，并检查是否分配成功
    person *p = malloc(sizeof(person));
    if (p == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // 如果还需要继续创建更早一代
    if (generations > 1)
    {
        // 通过递归调用 create_family 为当前个体创建两位父母
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // 为当前个体设置父母指针
        p->parents[0] = parent0;
        p->parents[1] = parent1;

        // 基于父母的等位基因，随机给当前个体分配两个等位基因
        p->alleles[0] = parent0->alleles[rand() % 2];
        p->alleles[1] = parent1->alleles[rand() % 2];
    }

    // 如果不需要再创建更早一代
    else
    {
        // 将父母指针设为 NULL
        p->parents[0] = NULL;
        p->parents[1] = NULL;

        // 随机分配等位基因
        p->alleles[0] = random_allele();
        p->alleles[1] = random_allele();
    }

    // 返回新创建的 person
    return p;
}

// 释放 “p” 以及 “p” 的所有祖先占用的内存
void free_family(person *p)
{
    // 处理基例（空指针）
    if (p == NULL)
    {
        return;
    }

    // 递归释放父母
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // 释放当前结点
    free(p);
}

// 打印每位家族成员及其等位基因
void print_family(person *p, int generation)
{
    // 处理基例
    if (p == NULL)
    {
        return;
    }

    // 打印缩进
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // 打印人物信息
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    // 打印当前个体的双亲
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// 随机选择一个血型等位基因
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
