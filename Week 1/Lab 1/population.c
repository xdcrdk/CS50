#include <stdio.h>

int main(void)
{
    int start, end;
    
    // 获取有效的初始数量
    do
    {
        printf("Start size: ");
        scanf("%d", &start);
    } while (start < 9);  // 确保初始数量不少于 9
    
    // 获取有效的目标数量
    do
    {
        printf("End size: ");
        scanf("%d", &end);
    } while (end < start);  // 确保目标数量大于或等于初始数量
    
    int years = 0;
    
    // 模拟种群增长，直到种群达到或超过目标数量
    while (start < end)
    {
        int new_llamas = start / 3;  // 计算每年新出生的llama数量
        int lost_llamas = start / 4; // 计算每年去世的llama数量
        start = start + new_llamas - lost_llamas;  // 更新种群数量
        
        years++;  // 每年增长一次，年数加 1
    }
    
    // 输出所需的年数
    printf("Years: %d\n", years);
    
    return 0;
}
