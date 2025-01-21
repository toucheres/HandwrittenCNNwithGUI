#include "dynamic_lib.h"
#define DLL_API _declspec(dllexport)
DLL_API int add_dll(int a, int b)   //实现两个整数相加
{
	return a + b;
}

// 卷积操作函数
void Conv2d(int w, int h, int k, double *input_matrix, double *kernel, double *out_matrix)
{
	for (int i = 0; i < w - k + 1; i++) // 遍历输入矩阵的行
		for (int j = 0; j < h - k + 1; j++)
		{																												// 遍历输入矩阵的列
			out_matrix[i * (w - k + 1) + j] = 0;																		// 初始化输出矩阵
			for (int row = i; row < i + 3; row++)																		// 遍历卷积核的行
				for (int col = j; col < j + 3; col++)																	// 遍历卷积核的列
					out_matrix[i * (w - k + 1) + j] += input_matrix[row * w + col] * kernel[(row - i) * k + (col - j)]; // 计算卷积结果
		}
}

// 最大池化操作函数
void MaxPool2d(int w, int h, int k, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w / k; i++) // 遍历输入矩阵的行
		for (int j = 0; j < h / k; j++)
		{													   // 遍历输入矩阵的列
			int max_num = -999;								   // 初始化最大值
			for (int row = k * i; row < k * i + k; row++)	   // 遍历池化窗口的行
				for (int col = k * j; col < k * j + k; col++)  // 遍历池化窗口的列
					if (input_matrix[row * w + col] > max_num) // 更新最大值
						max_num = input_matrix[row * w + col];
			output_matrix[i * (w / 2) + j] = max_num; // 将最大值赋给输出矩阵
		}
}

// important ReLU 激活函数
void Relu(int w, int h, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w; i++)																		 // 遍历输入矩阵的行
		for (int j = 0; j < h; j++)																	 // 遍历输入矩阵的列
			output_matrix[i * w + j] = max(input_matrix[i * w + j], input_matrix[i * w + j] * 0.05); // 计算 ReLU 激活结果
}

// 矩阵扩展函数
void MatrixExtensionImproved(int w, int h, double *input_matrix1, double *input_matrix2, double *output_matrix)
{
	for (int i = 0; i < w; i++)									 // 遍历输入矩阵1的行
		for (int j = 0; j < h; j++)								 // 遍历输入矩阵1的列
			output_matrix[i * w + j] = input_matrix1[i * w + j]; // 将输入矩阵1的值赋给输出矩阵

	for (int i = 0; i < w; i++)											 // 遍历输入矩阵2的行
		for (int j = 0; j < h; j++)										 // 遍历输入矩阵2的列
			output_matrix[w * h + i * w + j] = input_matrix2[i * w + j]; // 将输入矩阵2的值赋给输出矩阵
}

// important 矩阵乘法函数
void MatrixMultiply(int w, int h, int out_deminsion, double *input_matrix, double *para_layer, double *output_matrix)
{
	for (int i = 0; i < w; i++) // 遍历输入矩阵的行
		for (int j = 0; j < out_deminsion; j++)
		{																								 // 遍历输出矩阵的列
			output_matrix[i * w + j] = 0;																 // 初始化输出矩阵
			for (int k = 0; k < h; k++)																	 // 遍历输入矩阵的列
				output_matrix[i * w + j] += input_matrix[i * w + k] * para_layer[k * out_deminsion + j]; // 计算矩阵乘法结果
		}
}

// 矩阵分割函数
void MatrixSplit(double *input_matrix, double *splited_matrix1, double *splited_matrix2)
{
	for (int idx = 0; idx < 1152; idx++) // 遍历输入矩阵
		if (idx < 576)					 // 前一半赋给分割矩阵1
			splited_matrix1[idx] = input_matrix[idx];
		else // 后一半赋给分割矩阵2
			splited_matrix2[idx - 576] = input_matrix[idx];
}

// 矩阵反向传播函数
void MatrixBackPropagation(int w, int h, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w; i++)										  // 遍历输入矩阵的行
		for (int j = 0; j < h; j++)									  // 遍历输入矩阵的列
			output_matrix[i * h + j] -= lr * input_matrix[i * h + j]; // 计算反向传播结果
}

// 矩阵反向传播乘法函数
void MatrixBackPropagationMultiply(int w, int h, double *para, double *grad, double *rgrad)
{
	for (int i = 0; i < w; i++)					  // 遍历输入矩阵的行
		for (int j = 0; j < h; j++)				  // 遍历输入矩阵的列
			rgrad[i * h + j] = para[i] * grad[j]; // 计算反向传播乘法结果
}

// 计算矩阵梯度函数
void CalculateMatrixGrad(int w, int h, double *input_matrix, double *grad, double *output_matrix)
{
	for (int i = 0; i < w; i++)
	{						  // 遍历输入矩阵的行
		output_matrix[i] = 0; // 初始化输出矩阵
		for (int j = 0; j < h; j++)
		{														   // 遍历输入矩阵的列
			output_matrix[i] += input_matrix[i * h + j] * grad[j]; // 计算矩阵梯度
		}
	}
}

// ReLU 反向传播函数
void ReluBackPropagation(int w, double *input_matrix, double *grad, double *output_matrix)
{
	for (int i = 0; i < w; i++) // 遍历输入矩阵的行
		if (input_matrix[i] > 0)
			output_matrix[i] = 1 * grad[i]; // 正值部分梯度为1
		else
			output_matrix[i] = 0.05 * grad[i]; // 负值部分梯度为0.05
}

// 填充操作函数
void Padding(int w, int stride, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w + 2 * stride; i++)			 // 遍历输出矩阵的行
		for (int j = 0; j < w + 2 * stride; j++)		 // 遍历输出矩阵的列
			output_matrix[i * (w + 2 * stride) + j] = 0; // 初始化输出矩阵
}

// 翻转卷积核函数
void OverturnKernel(int k, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < k; i++)														// 遍历输入矩阵的行
		for (int j = 0; j < k; j++)													// 遍历输入矩阵的列
			output_matrix[(k - 1 - i) * k + (k - 1 - j)] = input_matrix[i * k + j]; // 翻转卷积核
}

// 释放内存函数
void MemoryFree(double *x)
{
	free(x);  // 释放内存
	x = NULL; // 将指针置为空
}

// 初始化网络参数函数
DLL_API void init(struct parameter *para)
{
	srand(time(NULL));
	// 初始化随机数种子
	for (int i = 0; i < 3; i++)										 // 遍历卷积核的行
		for (int j = 0; j < 3; j++)									 // 遍历卷积核的列
			para->conv_kernel11[i][j] = (rand() / (RAND_MAX + 1.0)); // 初始化卷积核1第1层
																	 // 0-1的随机数
	for (int i = 0; i < 3; i++)										 // 遍历卷积核的行
		for (int j = 0; j < 3; j++)									 // 遍历卷积核的列
			para->conv_kernel12[i][j] = (rand() / (RAND_MAX + 1.0)); // 初始化卷积核1第2层

	for (int i = 0; i < 3; i++)											 // 遍历卷积核的行
		for (int j = 0; j < 3; j++)										 // 遍历卷积核的列
			para->conv_kernel21[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // 初始化卷积核1第3层
																		 // 0-0.2随机数
	for (int i = 0; i < 3; i++)											 // 遍历卷积核的行
		for (int j = 0; j < 3; j++)										 // 遍历卷积核的列
			para->conv_kernel22[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // 初始化卷积核2第1层

	for (int i = 0; i < 3; i++)											 // 遍历卷积核的行
		for (int j = 0; j < 3; j++)										 // 遍历卷积核的列
			para->conv_kernel31[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // 初始化卷积核2第2层

	for (int i = 0; i < 3; i++)											 // 遍历卷积核的行
		for (int j = 0; j < 3; j++)										 // 遍历卷积核的列
			para->conv_kernel32[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // 初始化卷积核2第3层

	for (int i = 0; i < 1152; i++)											   // 遍历全连接层1的行
		for (int j = 0; j < 180; j++)										   // 遍历全连接层1的列
			para->fc_hidden_layer1[i][j] = (rand() / (RAND_MAX + 1.0)) / 1000; // 初始化全连接层1
																			   // 随机数0-0.001
	for (int i = 0; i < 180; i++)											   // 遍历全连接层2的行
		for (int j = 0; j < 45; j++)										   // 遍历全连接层2的列
			para->fc_hidden_layer2[i][j] = (rand() / (RAND_MAX + 1.0)) / 100;  // 初始化全连接层2
																			   // 随机数0-0.01
	for (int i = 0; i < 45; i++)											   // 遍历全连接层3的行
		for (int j = 0; j < 10; j++)										   // 遍历全连接层3的列
			para->fc_hidden_layer3[i][j] = (rand() / (RAND_MAX + 1.0)) / 10;   // 初始化全连接层3
																			   // 随机数0-0.1
}
// readed
// important 前向传播--将上一层的输出作为下一层的输入--图片->概率图->数字
void forward(double *input_matrix, struct parameter *para, struct result *data)
{
	// 卷积
	// 卷积核1:
	// 第一层卷积操作，输入为30x30，卷积核大小为3x3，输出为28x28
	Conv2d(30, 30, 3, input_matrix, &para->conv_kernel11[0][0], &data->first_conv1[0][0]);
	// 第二层卷积操作，输入为28x28，卷积核大小为3x3，输出为26x26
	Conv2d(28, 28, 3, &data->first_conv1[0][0], &para->conv_kernel21[0][0], &data->sencond_conv1[0][0]);
	// 第三层卷积操作，输入为26x26，卷积核大小为3x3，输出为24x24
	Conv2d(26, 26, 3, &data->sencond_conv1[0][0], &para->conv_kernel31[0][0], &data->third_conv1[0][0]);

	// 卷积核2:
	// 第一层卷积操作，输入为30x30，卷积核大小为3x3，输出为28x28
	Conv2d(30, 30, 3, input_matrix, &para->conv_kernel12[0][0], &data->first_conv2[0][0]);
	// 第二层卷积操作，输入为28x28，卷积核大小为3x3，输出为26x26
	Conv2d(28, 28, 3, &data->first_conv2[0][0], &para->conv_kernel22[0][0], &data->sencond_conv2[0][0]);
	// 第三层卷积操作，输入为26x26，卷积核大小为3x3，输出为24x24
	Conv2d(26, 26, 3, &data->sencond_conv2[0][0], &para->conv_kernel32[0][0], &data->third_conv2[0][0]);

	// 将两个通道的卷积结果进行拼接，得到1x1152的向量
	MatrixExtensionImproved(24, 24, &data->third_conv1[0][0], &data->third_conv2[0][0], &data->flatten_conv[0][0]);

	// 全连接
	// 全连接层1，输入为1x1152，输出为1x180
	MatrixMultiply(1, 1152, 180, &data->flatten_conv[0][0], &para->fc_hidden_layer1[0][0], &data->first_fc[0][0]);
	// ReLU激活函数，输入为1x180，输出为1x180
	Relu(1, 180, &data->first_fc[0][0], &data->first_relu[0][0]);
	// 全连接层2，输入为1x180，输出为1x45
	MatrixMultiply(1, 180, 45, &data->first_relu[0][0], &para->fc_hidden_layer2[0][0], &data->second_fc[0][0]);
	// ReLU激活函数，输入为1x45，输出为1x45
	Relu(1, 45, &data->second_fc[0][0], &data->second_relu[0][0]);
	// 全连接层3，输入为1x45，输出为1x10
	MatrixMultiply(1, 45, 10, &data->second_relu[0][0], &para->fc_hidden_layer3[0][0], &data->outmlp[0][0]);
	double probability = 0;

	// 计算概率，输出结果
	// 计算softmax概率
	for (int i = 0; i < 10; i++)
		probability += exp(data->outmlp[0][i]);
	for (int i = 0; i < 10; i++)
	{
		data->result[i] = exp(data->outmlp[0][i]) / probability;
		result[i] = data->result[i];
	}
}
// important 对参数权重关于结果进行求导，迭代参数权重
void backward(int label, struct parameter *para, struct result *data)
{
	// 计算输出层的梯度
	int double_len = sizeof(double);
	double *out_grad;
	out_grad = (double *)malloc(10 * double_len);
	for (int i = 0; i < 10; i++)
		if (i == label)
			out_grad[i] = data->result[i] - 1;
		else
			out_grad[i] = data->result[i] - 0;

	// 计算全连接层3的权重梯度
	double *out_wgrad;
	out_wgrad = (double *)malloc(450 * double_len);
	MatrixBackPropagationMultiply(45, 10, &data->second_relu[0][0], out_grad, out_wgrad);

	// 计算全连接层3的输入梯度
	double *second_rgrad;
	second_rgrad = (double *)malloc(45 * double_len);
	CalculateMatrixGrad(45, 10, &para->fc_hidden_layer3[0][0], out_grad, second_rgrad);
	MemoryFree(out_grad);

	// 计算ReLU激活函数的梯度
	double *second_grad;
	second_grad = (double *)malloc(180 * double_len);
	ReluBackPropagation(45, &data->second_fc[0][0], second_rgrad, second_grad);
	MemoryFree(second_rgrad);

	// 计算全连接层2的权重梯度
	double *second_wgrad;
	second_wgrad = (double *)malloc(8100 * double_len);
	MatrixBackPropagationMultiply(180, 45, &data->first_relu[0][0], second_grad, second_wgrad);

	// 计算全连接层2的输入梯度
	double *first_rgrad;
	first_rgrad = (double *)malloc(180 * double_len);
	CalculateMatrixGrad(180, 45, &para->fc_hidden_layer2[0][0], second_grad, first_rgrad);
	MemoryFree(second_grad);

	// 计算ReLU激活函数的梯度
	double *first_grad;
	first_grad = (double *)malloc(180 * double_len);
	ReluBackPropagation(180, &data->first_fc[0][0], first_rgrad, first_grad);
	MemoryFree(first_rgrad);

	// 计算全连接层1的权重梯度
	double *first_wgrad;
	first_wgrad = (double *)malloc(207360 * double_len);
	MatrixBackPropagationMultiply(1152, 180, &data->flatten_conv[0][0], first_grad, first_wgrad);

	// 计算全连接层1的输入梯度
	double *all_conv_grad;
	all_conv_grad = (double *)malloc(1152 * double_len);
	CalculateMatrixGrad(1152, 180, &para->fc_hidden_layer1[0][0], first_grad, all_conv_grad);
	MemoryFree(first_grad);

	// 将卷积层的梯度分割成两个通道
	double *third_conv_grad1;
	third_conv_grad1 = (double *)malloc(576 * double_len);
	double *third_conv_grad2;
	third_conv_grad2 = (double *)malloc(576 * double_len);
	MatrixSplit(all_conv_grad, third_conv_grad1, third_conv_grad2);
	MemoryFree(all_conv_grad);

	// 计算第三层卷积核的梯度
	double *third_kernel_grad;
	third_kernel_grad = (double *)malloc(9 * double_len);
	Conv2d(26, 26, 24, &data->sencond_conv1[0][0], third_conv_grad1, third_kernel_grad);

	// 翻转第三层卷积核
	double *third_kernel_overturn;
	third_kernel_overturn = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel31[0][0], third_kernel_overturn);

	// 对第三层卷积梯度进行填充
	double *third_conv_grad_padding1;
	third_conv_grad_padding1 = (double *)malloc(784 * double_len);
	Padding(26, 1, third_conv_grad1, third_conv_grad_padding1);
	MemoryFree(third_conv_grad1);

	// 计算第二层卷积核的梯度
	double *second_conv_grad1;
	second_conv_grad1 = (double *)malloc(676 * double_len);
	Conv2d(28, 28, 3, third_conv_grad_padding1, third_kernel_overturn, second_conv_grad1);
	MemoryFree(third_kernel_overturn);
	MemoryFree(third_conv_grad_padding1);

	// 计算第二层卷积核的梯度
	double *second_kernel_grad;
	second_kernel_grad = (double *)malloc(9 * double_len);
	Conv2d(28, 28, 26, &data->first_conv1[0][0], second_conv_grad1, second_kernel_grad);

	// 翻转第二层卷积核
	double *second_kernel_overturn;
	second_kernel_overturn = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel21[0][0], second_kernel_overturn);

	// 对第二层卷积梯度进行填充
	double *second_conv_grad_padding1;
	second_conv_grad_padding1 = (double *)malloc(900 * double_len);
	Padding(28, 1, second_conv_grad1, second_conv_grad_padding1);
	MemoryFree(second_conv_grad1);

	// 计算第一层卷积核的梯度
	double *first_conv_grad;
	first_conv_grad = (double *)malloc(784 * double_len);
	Conv2d(30, 30, 3, second_conv_grad_padding1, second_kernel_overturn, first_conv_grad);
	MemoryFree(second_kernel_overturn);
	MemoryFree(second_conv_grad_padding1);

	// 计算第一层卷积核的梯度
	double *first_kernel_grad;
	first_kernel_grad = (double *)malloc(9 * double_len);
	Conv2d(30, 30, 28, &data->mnist_data[0][0], first_conv_grad, first_kernel_grad);
	MemoryFree(first_conv_grad);

	// 计算第三层卷积核的梯度
	double *third_kernel_grad2;
	third_kernel_grad2 = (double *)malloc(9 * double_len);
	Conv2d(26, 26, 24, &data->sencond_conv2[0][0], third_conv_grad2, third_kernel_grad2);

	// 翻转第三层卷积核
	double *third_kernel_overturn2;
	third_kernel_overturn2 = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel32[0][0], third_kernel_overturn2);

	// 对第三层卷积梯度进行填充
	double *third_conv_grad_padding2;
	third_conv_grad_padding2 = (double *)malloc(784 * double_len);
	Padding(26, 1, third_conv_grad2, third_conv_grad_padding2);
	MemoryFree(third_conv_grad2);

	// 计算第二层卷积核的梯度
	double *second_conv_grad2;
	second_conv_grad2 = (double *)malloc(676 * double_len);
	Conv2d(28, 28, 3, third_conv_grad_padding2, third_kernel_overturn2, second_conv_grad2);
	MemoryFree(third_conv_grad_padding2);

	// 计算第二层卷积核的梯度
	double *second_kernel_grad2;
	second_kernel_grad2 = (double *)malloc(9 * double_len);
	Conv2d(28, 28, 26, &data->first_conv2[0][0], second_conv_grad2, second_kernel_grad2);

	// 翻转第二层卷积核
	double *second_kernel_overturn2;
	second_kernel_overturn2 = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel22[0][0], second_kernel_overturn2);

	// 对第二层卷积梯度进行填充
	double *second_conv_grad_padding2;
	second_conv_grad_padding2 = (double *)malloc(900 * double_len);
	Padding(28, 1, second_conv_grad2, second_conv_grad_padding2);
	MemoryFree(second_conv_grad2);

	// 计算第一层卷积核的梯度
	double *first_conv_grad2;
	first_conv_grad2 = (double *)malloc(784 * double_len);
	Conv2d(30, 30, 3, second_conv_grad_padding2, second_kernel_overturn2, first_conv_grad2);
	MemoryFree(second_kernel_overturn2);
	MemoryFree(second_conv_grad_padding2);

	// 计算第一层卷积核的梯度
	double *first_kernel_grad2;
	first_kernel_grad2 = (double *)malloc(9 * double_len);
	Conv2d(30, 30, 28, &data->mnist_data[0][0], first_conv_grad2, first_kernel_grad2);

	// 更新卷积核参数
	MatrixBackPropagation(3, 3, first_kernel_grad, &para->conv_kernel11[0][0]);
	MatrixBackPropagation(3, 3, second_kernel_grad, &para->conv_kernel21[0][0]);
	MatrixBackPropagation(3, 3, third_kernel_grad, &para->conv_kernel31[0][0]);
	MatrixBackPropagation(3, 3, first_kernel_grad2, &para->conv_kernel12[0][0]);
	MatrixBackPropagation(3, 3, second_kernel_grad2, &para->conv_kernel22[0][0]);
	MatrixBackPropagation(3, 3, third_kernel_grad2, &para->conv_kernel32[0][0]);

	// 更新全连接层参数
	MatrixBackPropagation(1152, 180, first_wgrad, &para->fc_hidden_layer1[0][0]);
	MatrixBackPropagation(180, 45, second_wgrad, &para->fc_hidden_layer2[0][0]);
	MatrixBackPropagation(45, 10, out_wgrad, &para->fc_hidden_layer3[0][0]);

	// 释放内存
	MemoryFree(first_kernel_grad);
	MemoryFree(second_kernel_grad);
	MemoryFree(third_kernel_grad);
	MemoryFree(first_kernel_grad2);
	MemoryFree(second_kernel_grad2);
	MemoryFree(third_kernel_grad2);
	MemoryFree(first_wgrad);  // 释放全连接层1权重梯度的内存
	MemoryFree(second_wgrad); // 释放全连接层2权重梯度的内存
	MemoryFree(out_wgrad);	  // 释放全连接层3权重梯度的内存
	return;					  // 返回
}
// 数据加载函数
DLL_API BOOL DataLoader()
{
	for (int num = 0; num < 10; num++)
	{ // 遍历数字0到9
		for (int i = 0; i < SAMPLE_NUM; i++)
		{												  // 遍历每个数字的样本
			char *e = (char *)malloc(sizeof(char) * 120); // 分配120字节的内存用于存储图像数据
			int *l = (int *)malloc(sizeof(int) * 960);	  // 分配960个整数的内存用于存储二值化后的图像数据
			if (e == NULL || l == NULL)
			{ // 检查内存分配是否成功
				perror("内存分配失败！\n");
				free(e);	  // 释放已分配的内存
				free(l);	  // 释放已分配的内存
				return FALSE; // 返回FALSE表示内存分配失败
			}

			char route_name[50] = "Training_set/"; // 定义训练集文件路径
			char file_name[15];
			sprintf(file_name, "%d/%d.bmp", num, i + 1); // 生成文件名
			strcat(route_name, file_name);				 // 拼接文件路径

			FILE *fp = fopen(route_name, "rb"); // 打开文件
			if (!fp)
			{ // 检查文件是否成功打开
				perror("未能打开训练集数据. 检查'Training_set'文件夹是否存在，并且文件夹中有图片!\n");
				free(e); // 释放已分配的内存
				free(l);
				return FALSE; // 返回FALSE表示文件打开失败
			}

			fseek(fp, 62, SEEK_SET);		 // 跳过文件头部的62字节
			fread(e, sizeof(char), 120, fp); // 读取120字节的图像数据
			fclose(fp);						 // 关闭文件
			// TODO 转为二进制
			// XXX 输入为30x30无灰度bmp图
			int y = 0;
			for (int r = 0; r < 120; r++)
			{ // 遍历读取的图像数据
				for (int u = 1; u < 9; u++)
				{											// 遍历每个字节的每一位
					l[y] = (int)((e[r] >> (8 - u)) & 0x01); // 将每一位转换为二值化数据
					y++;
					if (y > 960)
						break;
				}
			}
			// TODO 存储二进制到Sample
			int g = 0;
			for (int u = 0; u < 30; u++)
			{ // 遍历0-29(1-30)行
				y = 0;
				for (int j = 0; j < 32; j++)
				{ // 遍历0-29(1-30)列
					if (j != 30 && j != 31)
					{												 // 跳过第30和第31列
						Sample[num * SAMPLE_NUM + i].a[u][y] = l[g]; // 将二值化数据存储到样本数组中
						y++;
					}
					g++;
				}
			}

			int q = Sample[num * SAMPLE_NUM + i].a[0][0];
			if (q == 1)
			{

				// TODO why如果第一个像素为1，则将图像数据取反
				// 如果第一个像素为1，则将图像数据取反
				for (int b = 0; b < 30; b++)
				{
					for (int n = 0; n < 30; n++)
					{
						Sample[num * SAMPLE_NUM + i].a[b][n] = 1 - Sample[num * SAMPLE_NUM + i].a[b][n];
					}
				}
			}

			Sample[num * SAMPLE_NUM + i].number = num; // 设置样本的标签
			free(e);								   // 释放已分配的内存
			free(l);
		}
	}
	return TRUE; // 返回TRUE表示数据加载成功
}

// 读取网络参数文件函数
DLL_API BOOL read_file(struct parameter *parameter_dest)
{
	FILE *fp;
	fp = fopen("network_parameter.txt", "rb"); // 以二进制模式打开网络参数文件
	if (fp == NULL)
	{ // 检查文件是否成功打开
		printf("文件打开失败，请检查网络参数文件是否在训练集文件夹内！\n");
		return FALSE; // 返回FALSE表示文件打开失败
	}
	struct parameter *parameter_tmp = NULL;
	parameter_tmp = (struct parameter *)malloc(sizeof(struct parameter)); // 分配内存用于临时存储网络参数
	fread(parameter_tmp, sizeof(struct parameter), 1, fp);				  // 从文件中读取网络参数
	(*parameter_dest) = (*parameter_tmp);								  // 将读取的网络参数赋值给目标参数
	fclose(fp);															  // 关闭文件
	free(parameter_tmp);												  // 释放临时存储的内存
	parameter_tmp = NULL;

	return TRUE; // 返回TRUE表示文件读取成功
}

// 将网络参数写入文件函数
DLL_API BOOL write_para_to_file(struct parameter *parameter_file)
{
	FILE *fp;
	fp = fopen("network_parameter.txt", "wb"); // 以二进制模式打开网络参数文件
	struct parameter *parameter_tmp;
	parameter_tmp = (struct parameter *)malloc(sizeof(struct parameter)); // 分配内存用于临时存储网络参数

	(*parameter_tmp) = (*parameter_file);					// 将网络参数赋值给临时参数
	fwrite(parameter_tmp, sizeof(struct parameter), 1, fp); // 将网络参数写入文件

	fclose(fp);			 // 关闭文件
	free(parameter_tmp); // 释放临时存储的内存
	parameter_tmp = NULL;

	return TRUE; // 返回TRUE表示文件写入成功
}

// 打印网络参数到文件函数
void printf_file2(struct parameter *parameter4)
{
	FILE *fp;
	fp = fopen("NetworkParameters.bin", "wb"); // 以二进制模式打开网络参数文件
	struct parameter *parameter1;
	parameter1 = (struct parameter *)malloc(sizeof(struct parameter)); // 分配内存用于临时存储网络参数
	(*parameter1) = (*parameter4);									   // 将网络参数赋值给临时参数
	fwrite(parameter1, sizeof(struct parameter), 1, fp);			   // 将网络参数写入文件
	fclose(fp);														   // 关闭文件
	free(parameter1);												   // 释放临时存储的内存
	parameter1 = NULL;
	return;
}

// 交叉熵损失函数
double Cross_entropy(double *a, int m)
{
	double u = 0;
	u = (-log10(a[m])); // 计算交叉熵损失
	return u;
}

// 显示进度条函数
void show_progress_bar(int progress, int total)
{
	int bar_width = 50;								  // 进度条宽度
	float percent_complete = (float)progress / total; // 计算完成百分比
	int position = bar_width * percent_complete;	  // 计算进度条位置

	printf("[");
	for (int i = 0; i < bar_width; ++i)
	{ // 绘制进度条
		if (i < position)
		{
			printf("=");
		}
		else if (i == position)
		{
			printf(">");
		}
		else
		{
			printf(" ");
		}
	}
	printf("] %d%%\r", (int)(percent_complete * 100)); // 显示百分比
	fflush(stdout);									   // 刷新输出缓冲区
}

// important 训练，学习
DLL_API void train(int epochs, struct parameter *para, struct result *data)
{
	double corss_loss = 2;						 // 初始化交叉熵损失
	for (int epoch = 0; epoch < epochs; epoch++) // 遍历每个训练周期
	{
		lr = pow((corss_loss / 10), 1.7); // 根据交叉熵损失计算学习率
		if (lr > 0.01)					  // 限制学习率的最大值
		{
			lr = 0.01;
		}

		show_progress_bar(epoch + 1, epochs); // 显示训练进度条
		if ((epoch + 1) % 10 == 0)			  // 每10个周期输出一次交叉熵损失和学习率
		{
			fflush(stdout); // 刷新输出缓冲区
			printf("\t交叉熵损失: %lf  学习率:%.10lf\n", corss_loss, lr);
			if (lr < 0.0000000001) // 如果学习率过小，则保存网络参数
				printf_file2(para);
		}

		// 随机交换样本，打乱顺序
		int a, b;
		srand(time(NULL)); // 初始化随机数种子
		struct sample *sample_tmp = NULL;
		sample_tmp = (struct sample *)malloc(sizeof(struct sample)); // 分配内存用于临时存储样本
		for (int q = 0; q < SAMPLE_NUM * 10; q++)					 // 随机交换样本
		{
			a = (int)((rand() / (RAND_MAX + 1.0)) * 300);
			b = (int)((rand() / (RAND_MAX + 1.0)) * 300);
			if (a >= 0 && a < 300 && (a != b) && b >= 0 && b < 300)
			{
				(*sample_tmp) = Sample[a];
				Sample[a] = Sample[b];
				Sample[b] = (*sample_tmp);
			}
			else
				continue;
		}

		for (int i = 0; i < SAMPLE_NUM * 10; i++) // 遍历每个样本
		{
			corss_loss = 0; // 初始化交叉熵损失
			(*sample_tmp) = Sample[i];
			int y = sample_tmp->number;					   // 获取样本标签
			forward(&sample_tmp->a[0][0], para, data);	   // 前向传播
			backward(y, para, data);					   // 反向传播
			double g = Cross_entropy(&data->result[0], y); // 计算交叉熵损失
			if (g > corss_loss)
				corss_loss = g; // 更新交叉熵损失
		}
		free(sample_tmp); // 释放临时存储的样本内存
		sample_tmp = NULL;
	}
	printf("\n");
	return;
}

DLL_API void test_network(struct parameter *parameter2, struct result *data2)
{
	setlocale(LC_ALL, "");
	char e[120];
	int l[960];
	double data[30][30];
	for (int i = 0; i < 10; i++) // 遍历每个测试样本
	{
		FILE *fp;
		char s[30];
		sprintf(s, "%s%d%s", "Test_set/", i, ".bmp"); // 生成测试样本文件名
		printf("\n打开的文件名:%s", s);
		fp = fopen(s, "rb"); // 打开测试样本文件
		if (!fp)
		{
			perror("不能打开文件!\n");
			return;
		}
		fseek(fp, 62, SEEK_SET);		 // 跳过文件头部的62字节
		fread(e, sizeof(char), 120, fp); // 读取120字节的图像数据
		fclose(fp);
		int y = 0;
		for (int r = 0; r < 120; r++) // 遍历读取的图像数据
		{
			for (int u = 1; u < 9; u++) // 遍历每个字节的每一位
			{
				l[y] = (int)((e[r]) >> (8 - u) & 0x01); // 将每一位转换为二值化数据
				y++;
				if (y > 960)
					break;
			};
		};
		y = 0;
		int g = 0;
		for (int u = 0; u < 30; u++) // 遍历30行
		{
			y = 0;
			for (int j = 0; j < 32; j++) // 遍历32列
			{
				if ((j != 30) && (j != 31)) // 跳过第30和第31列
				{
					data[u][y] = l[g]; // 将二值化数据存储到样本数组中
					y++;
				};
				g++;
			}
		}
		int q = data[0][0];
		if (q == 1) // 如果第一个像素为1，则将图像数据取反
		{
			int n = 0;
			int z = 0;
			for (int b = 0; b < 30; b++)
			{
				n = 0;
				for (;;)
				{
					if (n >= 30)
						break;
					if (data[z][n] == 0)
						data[z][n] = 1;
					else if (data[z][n] == 1)
						data[z][n] = 0;
					n++;
				}
				z++;
			}
		}
		forward(&data[0][0], parameter2, data2); // 前向传播

		double sum = 0;
		int k = 0;
		for (int j = 0; j < 10; j++) // 遍历每个类别
		{
			if (result[j] > sum)
			{
				sum = result[j];
				k = j; // 找到概率最大的类别
			}
			else
				continue;
		}
		printf("\n");
		for (int i = 0; i < 10; i++) // 输出每个类别的预测概率
		{
			printf("\t预测值是%d的概率：%lf\n", i, result[i]);
		}

		printf("\033[1;31;43m最终预测值: %d \033[0m\n", k); // 输出最终预测结果
	}
	return;
}

// int main(int argc, char const *argv[])
// {
// 	// 控制台输出utf8
// 	system("chcp 65001");
// 	int h = DataLoader(); // 加载训练数据
// 	if (h == TRUE)
// 	{
// 		printf("训练数据读取成功！\n");
// 	}
// 	else if (h == FALSE)
// 	{
// 		printf("训练集读取失败！程序自动退出\n");
// 		return 0;
// 	}

// 	printf("=============================================\n");
// 	printf("开始训练网络\n");
// 	// 参数,分配到堆上的参数
// 	struct parameter *storage;
// 	(storage) = (struct parameter *)malloc(sizeof(struct parameter)); // 分配内存用于存储网络参数
// 	struct result *data;
// 	(data) = (struct result *)malloc(sizeof(struct result)); // 分配内存用于存储结果

// 	// XXX 消息循环
// 	char g;
// 	do
// 	{
// 		printf("请问您是否希望从已训练的网络参数文件中读取网络参数？(是请按 y，否请按 n): ");
// 		setbuf(stdin, NULL);	  // 清空输入缓冲区
// 		g = getchar();			  // 获取用户输入
// 		while (getchar() != '\n') // 清空输入缓冲区中的剩余字符
// 			;
// 		if (g == 'y') // 如果用户输入 'y'
// 		{
// 			int h = read_file(storage); // 尝试读取网络参数文件
// 			if (h == FALSE)				// 如果读取失败
// 			{
// 				printf("参数包不存在！开始自动随机初始化网络参数\n");
// 				init(storage);				 // 随机初始化网络参数
// 				write_para_to_file(storage); // 将初始化的网络参数写入文件
// 				printf("网络参数初始化完毕！\n");
// 				printf("网络参数已保存到 network_parameter.txt 文件中\n");
// 			}
// 			else if (h == TRUE) // 如果读取成功
// 			{
// 				printf("参数读取成功!\n");
// 			}
// 		}
// 		else if (g == 'n') // 如果用户输入 'n'
// 		{
// 			init(storage);				 // 随机初始化网络参数
// 			write_para_to_file(storage); // 将初始化的网络参数写入文件
// 			printf("参数初始化完毕！\n");
// 		}
// 	} while (g != 'y' && g != 'n'); // 循环直到用户输入 'y' 或 'n'
// 	printf("=============================================\n"); // 打印分隔线

// 	// important 学习循环
// 	int epoch;
// 	char v;
// 	do
// 	{
// 		printf("请输入预训练的次数：");
// 		scanf("%d", &epoch);
// 		printf("开始训练\n");
// 		train(epoch, storage, data);
// 		printf("开始测试\n");
// 		test_network(storage, data);
// 		write_para_to_file(storage);
// 		do
// 		{
// 			printf("继续训练请按回车，退出请按q: ");
// 			setbuf(stdin, NULL);	  // 清空输入缓冲区
// 			v = getchar();			  // 获取用户输入
// 			while (getchar() != '\n') // 清空输入缓冲区中的剩余字符
// 				;
// 			if (v == 'q') // 如果用户输入 'q'
// 			{
// 				write_para_to_file(storage); // 将网络参数写入文件
// 				return 0;					 // 退出程序
// 			}
// 			else if (v == '\n') // 如果用户按下回车键
// 			{
// 				break; // 退出循环，继续训练
// 			}
// 			else // 如果用户输入其他字符
// 			{
// 				printf("输入错误，请重新输入！\n"); // 提示输入错误
// 			}
// 		} while (1); // 循环直到用户输入正确
// 	} while (v != 'q'); // 循环直到用户输入 'q'

// 	free(storage); // 释放存储网络参数的内存
// 	free(data);	   // 释放存储结果的内存

// 	return 0; // 返回0，表示程序正常结束
// }