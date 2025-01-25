#pragma once
/*****************************************************************************
File name: Handwritten-digit-recognition
Description: CNN手写数字识别
Author: XXX
Version: 3.1
Date: 1999年13月32日
*****************************************************************************/
#include <stdio.h>  // 标准输入输出库
#include <stdlib.h> // 标准库，包含内存分配、进程控制、转换等函数
#include <math.h>   // 数学库，包含数学函数
#include <string.h> // 字符串处理库
#include <time.h>   // 时间库，包含时间相关函数
#include <locale.h>
// #include <unistd.h> // POSIX 标准库，包含对操作系统 API 的访问

// 定义宏函数 max，用于返回两个数中的较大值
#define  max___(a, b) (((a) > (b)) ? (a) : (b))

// 定义样本数量
#define SAMPLE_NUM 30

// 定义布尔类型
#define BOOL int
#define TRUE 1
#define FALSE 0

#define  DLL_API _declspec(dllexport)
//DLL_API int add_dll(int x, int y);
//DLL_API BOOL DataLoader();


// 定义网络参数结构体(参数)
struct parameter
{
    double conv_kernel11[3][3];         // 第一层卷积核1
    double conv_kernel21[3][3];         // 第二层卷积核1
    double conv_kernel31[3][3];         // 第三层卷积核1
    double conv_kernel12[3][3];         // 第一层卷积核2
    double conv_kernel22[3][3];         // 第二层卷积核2
    double conv_kernel32[3][3];         // 第三层卷积核2
    double fc_hidden_layer1[1152][180]; // 全连接层1
    double fc_hidden_layer2[180][45];   // 全连接层2
    double fc_hidden_layer3[45][10];    // 全连接层3
};

// 定义网络结果结构体
struct result
{
    double mnist_data[30][30];    // 输入数据
    double first_conv1[28][28];   // 第一层卷积结果1
    double sencond_conv1[26][26]; // 第二层卷积结果1
    double third_conv1[24][24];   // 第三层卷积结果1
    double first_conv2[28][28];   // 第一层卷积结果2
    double sencond_conv2[26][26]; // 第二层卷积结果2
    double third_conv2[24][24];   // 第三层卷积结果2
    double flatten_conv[1][1152]; // 扁平化后的卷积结果
    double first_fc[1][180];      // 第一层全连接层结果
    double first_relu[1][180];    // 第一层 ReLU 激活结果
    double second_fc[1][45];      // 第二层全连接层结果
    double second_relu[1][45];    // 第二层 ReLU 激活结果
    double outmlp[1][10];         // 输出层结果
    double result[10];            // 最终结果
};

// 定义样本结构体
struct sample
{
    double a[30][30]; // 样本数据
    // TODO 为什么是double类型？
    int number;            // 样本标签
}; // 样本数组 存储0-9各30张图
//DLL_API void init(struct parameter *para);
//DLL_API BOOL read_file(struct parameter *parameter_dest);
//DLL_API BOOL write_para_to_file(struct parameter *parameter_file);
//DLL_API void train(int epochs, int numall);
//DLL_API void test_network(struct parameter *parameter2, struct result *data2);
//DLL_API int recongise(const char *path_bmp);
//DLL_API int train_data_loader(int* numeach);
#ifdef __cplusplus
extern "C" {
#endif

    DLL_API int add_dll(int x, int y);
    DLL_API BOOL DataLoader();
    DLL_API void init(struct parameter* para);
    DLL_API BOOL read_file(struct parameter* parameter_dest);
    DLL_API BOOL write_para_to_file(struct parameter* parameter_file);
    DLL_API void train(int epochs, int numall);
    DLL_API void test_network(struct parameter* parameter2, struct result* data2);
    DLL_API int recongise(const char* path_bmp);
    DLL_API int train_data_loader(int* numeach);

#ifdef __cplusplus
}
#endif