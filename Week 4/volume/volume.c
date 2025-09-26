// 修改音频文件的音量

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// .wav 文件头的字节数
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
     // 检查命令行参数
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // 打开输入文件并确定缩放因子
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]); // 获取缩放因子

    // 复制输入文件的头部到输出文件
    uint8_t header[HEADER_SIZE];
    fread(header, sizeof(uint8_t), HEADER_SIZE, input); // 读取头部
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output); // 写入头部

    // 读取音频样本并更新数据
    int16_t sample;
    while (fread(&sample, sizeof(int16_t), 1, input) == 1)
    {
        // 调整音量（根据因子缩放音频样本）
        sample = (int16_t)(sample * factor);

        // 限制音频样本的范围，防止溢出
        if (sample > INT16_MAX)
        {
            sample = INT16_MAX;
        }
        else if (sample < INT16_MIN)
        {
            sample = INT16_MIN;
        }

        // 写入修改后的样本到输出文件
        fwrite(&sample, sizeof(int16_t), 1, output);
    }

    // 关闭文件
    fclose(input);
    fclose(output);
}
