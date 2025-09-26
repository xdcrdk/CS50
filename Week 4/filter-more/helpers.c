#include "helpers.h"

// 将图像转换为灰度
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // 遍历每个像素
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 计算灰度值（红、绿、蓝三个通道的加权平均值）
            int gray = (int)(0.299 * image[i][j].rgbtRed + 0.587 * image[i][j].rgbtGreen + 0.114 * image[i][j].rgbtBlue);

            // 将每个通道的值都设置为灰度值
            image[i][j].rgbtRed = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtBlue = gray;
        }
    }
    return;
}

// 水平翻转图像
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // 遍历每一行
    for (int i = 0; i < height; i++)
    {
        // 对每一行中的像素进行左右交换
        for (int j = 0; j < width / 2; j++)
        {
            // 交换像素 image[i][j] 和 image[i][width - j - 1]
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return 0;
}

// 对图像进行模糊处理
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // 创建一个新的图像数组，用于存储模糊后的像素
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    
    // 遍历每个像素
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 初始化 RGB 值
            int red = 0, green = 0, blue = 0;
            int count = 0;

            // 遍历 3x3 邻域
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    // 检查邻域是否超出图像边界
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // 累加 RGB 值
                        red += image[ni][nj].rgbtRed;
                        green += image[ni][nj].rgbtGreen;
                        blue += image[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            // 计算平均 RGB 值
            new_image[i][j].rgbtRed = red / count;
            new_image[i][j].rgbtGreen = green / count;
            new_image[i][j].rgbtBlue = blue / count;
        }
    }

    // 将模糊后的图像复制回原图像
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }

    // 释放新图像数组的内存
    free(new_image);
    return 0;
}

// 边缘检测
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // 创建一个新的图像数组，用于存储处理后的像素
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    
    // Sobel 算子的两个卷积核，用于计算梯度
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // 遍历每个像素，跳过边界
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            // 初始化 RGB 值
            int redX = 0, greenX = 0, blueX = 0;
            int redY = 0, greenY = 0, blueY = 0;

            // 对 3x3 邻域应用 Sobel 算子
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int pixelRed = image[i + di][j + dj].rgbtRed;
                    int pixelGreen = image[i + di][j + dj].rgbtGreen;
                    int pixelBlue = image[i + di][j + dj].rgbtBlue;

                    // 计算梯度
                    redX += pixelRed * Gx[di + 1][dj + 1];
                    greenX += pixelGreen * Gx[di + 1][dj + 1];
                    blueX += pixelBlue * Gx[di + 1][dj + 1];

                    redY += pixelRed * Gy[di + 1][dj + 1];
                    greenY += pixelGreen * Gy[di + 1][dj + 1];
                    blueY += pixelBlue * Gy[di + 1][dj + 1];
                }
            }

            // 计算总梯度
            int red = (int)sqrt(redX * redX + redY * redY);
            int green = (int)sqrt(greenX * greenX + greenY * greenY);
            int blue = (int)sqrt(blueX * blueX + blueY * blueY);

            // 限制最大值为 255
            new_image[i][j].rgbtRed = (red > 255) ? 255 : red;
            new_image[i][j].rgbtGreen = (green > 255) ? 255 : green;
            new_image[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
        }
    }

    // 将处理后的图像复制回原图像
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }

    // 释放新图像数组的内存
    free(new_image);
    return 0;
}
