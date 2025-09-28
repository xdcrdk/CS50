// 实现拼写检查器
//
// 说明：
// - CS50 原版使用 <sys/resource.h> / getrusage 计时，这在 Linux/macOS 可用，
//   但在 Windows 下没有该头文件与函数。
// - 为了跨平台，这里在 Windows (_WIN32) 下用 GetProcessTimes 仿造 getrusage，
//   保持结构体/函数名一致，使其余代码无需修改。
// - 在非 Windows 平台，仍然使用原生 <sys/resource.h> 与 <sys/time.h>。

#include <ctype.h>
#include <stdio.h>

#if defined(_WIN32)
// ============================
// [Windows 分支] 计时兼容层
// ============================
#include <windows.h>
#include <stdint.h>

// 仿造 POSIX 的 timeval 结构（秒 + 微秒）
struct timeval { long tv_sec; long tv_usec; };

// 仿造 POSIX 的 rusage 结构（用户态/内核态 CPU 时间）
struct rusage {
    struct timeval ru_utime; // user CPU time
    struct timeval ru_stime; // system CPU time
};

#define RUSAGE_SELF 0

// getrusage 的最小可用实现：用 GetProcessTimes 获取当前进程 CPU 时间
static int getrusage(int who, struct rusage *usage) {
    (void)who; // 本实现只支持当前进程，参数未使用
    FILETIME createTime, exitTime, kernelTime, userTime;
    if (!GetProcessTimes(GetCurrentProcess(),
                         &createTime, &exitTime, &kernelTime, &userTime)) {
        return -1;
    }

    ULARGE_INTEGER u, k;
    u.LowPart = userTime.dwLowDateTime;   u.HighPart = userTime.dwHighDateTime;
    k.LowPart = kernelTime.dwLowDateTime; k.HighPart = kernelTime.dwHighDateTime;

    // FILETIME 的单位是 100 纳秒（0.1 微秒），换算成微秒
    uint64_t user_us = u.QuadPart / 10;
    uint64_t kern_us = k.QuadPart / 10;

    usage->ru_utime.tv_sec  = (long)(user_us / 1000000ULL);
    usage->ru_utime.tv_usec = (long)(user_us % 1000000ULL);
    usage->ru_stime.tv_sec  = (long)(kern_us / 1000000ULL);
    usage->ru_stime.tv_usec = (long)(kern_us % 1000000ULL);
    return 0;
}
#else
// ============================
// [Unix 分支] 使用原生头文件
// ============================
#include <sys/resource.h>
#include <sys/time.h>
#endif

#include "dictionary.h"

// 取消可能存在的宏定义（如果上层环境将 calculate/getrusage 定义为宏，避免冲突）
#undef calculate
#undef getrusage

// 默认字典路径（可被命令行参数覆盖）
#define DICTIONARY "dictionaries/large"

// 函数原型：计算两次 rusage 的时间差（秒）
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[])
{
    // 检查命令行参数数量
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./speller [DICTIONARY] text\n");
        return 1;
    }

    // 计时用的 rusage 结构（前后两次）
    struct rusage before, after;

    // 各阶段耗时记录（单位：秒）
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // 确定使用的字典文件：若提供了三参形式，则 argv[1] 为字典路径
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // 加载字典
    getrusage(RUSAGE_SELF, &before);
    bool loaded = load(dictionary);
    getrusage(RUSAGE_SELF, &after);

    // 若加载失败，退出
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // 计算加载字典的耗时
    time_load = calculate(&before, &after);

    // 打开要检查的文本文件（argv[2] 或 argv[1]，取决于是否自定义字典）
    char *text = (argc == 3) ? argv[2] : argv[1];
    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // 打印“拼写错误的词”标题
    printf("\nMISSPELLED WORDS\n\n");

    // 检查流程所需的变量
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1]; // LENGTH 来自 dictionary.h

    // 逐字符读取，拼接成单词后调用 check 函数
    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        // 允许字母与单词中的撇号（且撇号不能在开头）
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // 追加字符到当前单词
            word[index] = c;
            index++;

            // 单词过长（超过 LENGTH）则跳过剩余的字母字符
            if (index > LENGTH)
            {
                // 吃掉这个“超长字母串”的后续部分
                while (fread(&c, sizeof(char), 1, file) && isalpha(c));

                // 重置，准备读取下一个单词
                index = 0;
            }
        }
        // 若包含数字（如 MS Word 的处理），整段视为非单词，跳过
        else if (isdigit(c))
        {
            // 吃掉这个“字母数字串”的后续部分
            while (fread(&c, sizeof(char), 1, file) && isalnum(c));

            // 重置，准备读取下一个单词
            index = 0;
        }
        // 否则遇到分隔符且当前缓存里有字母，说明一个完整单词结束
        else if (index > 0)
        {
            // 在单词末尾补 \0 终止
            word[index] = '\0';

            // 总词数 +1
            words++;

            // 调用 check 检查拼写，并计时
            getrusage(RUSAGE_SELF, &before);
            bool misspelled = !check(word);
            getrusage(RUSAGE_SELF, &after);

            // 累计检查阶段耗时
            time_check += calculate(&before, &after);

            // 若拼写错误，打印该词，并统计
            if (misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // 重置，准备下一个单词
            index = 0;
        }
    }

    // 检查读文件是否发生错误
    if (ferror(file))
    {
        fclose(file);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // 关闭文本文件
    fclose(file);

    // 查询字典中单词总数，并计时
    getrusage(RUSAGE_SELF, &before);
    unsigned int n = size();
    getrusage(RUSAGE_SELF, &after);

    // 计算 size 阶段耗时
    time_size = calculate(&before, &after);

    // 卸载字典，并计时
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();
    getrusage(RUSAGE_SELF, &after);

    // 卸载失败则退出
    if (!unloaded)
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;
    }

    // 计算卸载阶段耗时
    time_unload = calculate(&before, &after);

    // 输出统计信息
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
           time_load + time_check + time_size + time_unload);

    // 成功退出
    return 0;
}

// 计算两次 rusage（before/after）之间的 CPU 时间差（单位：秒）
// - 含用户态（utime）与内核态（stime）两部分的累加
double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}