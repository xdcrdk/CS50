#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[])
{
    // 定义允许的过滤器类型
    char *filters = "begr";

    // 获取过滤器标志并检查其有效性
    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        printf("Invalid filter.\n");
        return 1;
    }

    // 确保只选择一个过滤器
    if (getopt(argc, argv, filters) != -1)
    {
        printf("Only one filter allowed.\n");
        return 2;
    }

    // 确保正确的使用方式
    if (argc != optind + 2)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 3;
    }

    // 存储文件名
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // 打开输入文件
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    // 打开输出文件
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }

    // 读取输入文件的 BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // 读取输入文件的 BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // 确保输入文件是一个 24 位的未压缩 BMP 文件
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 6;
    }

    // 获取图像的尺寸
    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // 为图像分配内存
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // 确定扫描线的填充字节数
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // 遍历输入文件的每一行扫描线
    for (int i = 0; i < height; i++)
    {
        // 读取一行数据到像素数组
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // 跳过填充字节
        fseek(inptr, padding, SEEK_CUR);
    }

    // 对图像进行处理
    switch (filter)
    {
        // 模糊处理
        case 'b':
            blur(height, width, image);
            break;

        // 边缘检测
        case 'e':
            edges(height, width, image);
            break;

        // 灰度处理
        case 'g':
            grayscale(height, width, image);
            break;

        // 反射处理
        case 'r':
            reflect(height, width, image);
            break;
    }

    // 将修改后的 BITMAPFILEHEADER 写入输出文件
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // 将修改后的 BITMAPINFOHEADER 写入输出文件
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // 将新的像素数据写入输出文件
    for (int i = 0; i < height; i++)
    {
        // 写入一行到输出文件
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // 在每行末尾写入填充字节
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // 释放图像内存
    free(image);

    // 关闭文件
    fclose(inptr);
    fclose(outptr);
    return 0;
}
