#include "dynamic_lib.h"
#define DLL_API _declspec(dllexport)
DLL_API int add_dll(int a, int b)   //ʵ�������������
{
	return a + b;
}

// �����������
void Conv2d(int w, int h, int k, double *input_matrix, double *kernel, double *out_matrix)
{
	for (int i = 0; i < w - k + 1; i++) // ��������������
		for (int j = 0; j < h - k + 1; j++)
		{																												// ��������������
			out_matrix[i * (w - k + 1) + j] = 0;																		// ��ʼ���������
			for (int row = i; row < i + 3; row++)																		// ��������˵���
				for (int col = j; col < j + 3; col++)																	// ��������˵���
					out_matrix[i * (w - k + 1) + j] += input_matrix[row * w + col] * kernel[(row - i) * k + (col - j)]; // ���������
		}
}

// ���ػ���������
void MaxPool2d(int w, int h, int k, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w / k; i++) // ��������������
		for (int j = 0; j < h / k; j++)
		{													   // ��������������
			int max_num = -999;								   // ��ʼ�����ֵ
			for (int row = k * i; row < k * i + k; row++)	   // �����ػ����ڵ���
				for (int col = k * j; col < k * j + k; col++)  // �����ػ����ڵ���
					if (input_matrix[row * w + col] > max_num) // �������ֵ
						max_num = input_matrix[row * w + col];
			output_matrix[i * (w / 2) + j] = max_num; // �����ֵ�����������
		}
}

// important ReLU �����
void Relu(int w, int h, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w; i++)																		 // ��������������
		for (int j = 0; j < h; j++)																	 // ��������������
			output_matrix[i * w + j] = max(input_matrix[i * w + j], input_matrix[i * w + j] * 0.05); // ���� ReLU ������
}

// ������չ����
void MatrixExtensionImproved(int w, int h, double *input_matrix1, double *input_matrix2, double *output_matrix)
{
	for (int i = 0; i < w; i++)									 // �����������1����
		for (int j = 0; j < h; j++)								 // �����������1����
			output_matrix[i * w + j] = input_matrix1[i * w + j]; // ���������1��ֵ�����������

	for (int i = 0; i < w; i++)											 // �����������2����
		for (int j = 0; j < h; j++)										 // �����������2����
			output_matrix[w * h + i * w + j] = input_matrix2[i * w + j]; // ���������2��ֵ�����������
}

// important ����˷�����
void MatrixMultiply(int w, int h, int out_deminsion, double *input_matrix, double *para_layer, double *output_matrix)
{
	for (int i = 0; i < w; i++) // ��������������
		for (int j = 0; j < out_deminsion; j++)
		{																								 // ��������������
			output_matrix[i * w + j] = 0;																 // ��ʼ���������
			for (int k = 0; k < h; k++)																	 // ��������������
				output_matrix[i * w + j] += input_matrix[i * w + k] * para_layer[k * out_deminsion + j]; // �������˷����
		}
}

// ����ָ��
void MatrixSplit(double *input_matrix, double *splited_matrix1, double *splited_matrix2)
{
	for (int idx = 0; idx < 1152; idx++) // �����������
		if (idx < 576)					 // ǰһ�븳���ָ����1
			splited_matrix1[idx] = input_matrix[idx];
		else // ��һ�븳���ָ����2
			splited_matrix2[idx - 576] = input_matrix[idx];
}

// �����򴫲�����
void MatrixBackPropagation(int w, int h, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w; i++)										  // ��������������
		for (int j = 0; j < h; j++)									  // ��������������
			output_matrix[i * h + j] -= lr * input_matrix[i * h + j]; // ���㷴�򴫲����
}

// �����򴫲��˷�����
void MatrixBackPropagationMultiply(int w, int h, double *para, double *grad, double *rgrad)
{
	for (int i = 0; i < w; i++)					  // ��������������
		for (int j = 0; j < h; j++)				  // ��������������
			rgrad[i * h + j] = para[i] * grad[j]; // ���㷴�򴫲��˷����
}

// ��������ݶȺ���
void CalculateMatrixGrad(int w, int h, double *input_matrix, double *grad, double *output_matrix)
{
	for (int i = 0; i < w; i++)
	{						  // ��������������
		output_matrix[i] = 0; // ��ʼ���������
		for (int j = 0; j < h; j++)
		{														   // ��������������
			output_matrix[i] += input_matrix[i * h + j] * grad[j]; // ��������ݶ�
		}
	}
}

// ReLU ���򴫲�����
void ReluBackPropagation(int w, double *input_matrix, double *grad, double *output_matrix)
{
	for (int i = 0; i < w; i++) // ��������������
		if (input_matrix[i] > 0)
			output_matrix[i] = 1 * grad[i]; // ��ֵ�����ݶ�Ϊ1
		else
			output_matrix[i] = 0.05 * grad[i]; // ��ֵ�����ݶ�Ϊ0.05
}

// ����������
void Padding(int w, int stride, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < w + 2 * stride; i++)			 // ��������������
		for (int j = 0; j < w + 2 * stride; j++)		 // ��������������
			output_matrix[i * (w + 2 * stride) + j] = 0; // ��ʼ���������
}

// ��ת����˺���
void OverturnKernel(int k, double *input_matrix, double *output_matrix)
{
	for (int i = 0; i < k; i++)														// ��������������
		for (int j = 0; j < k; j++)													// ��������������
			output_matrix[(k - 1 - i) * k + (k - 1 - j)] = input_matrix[i * k + j]; // ��ת�����
}

// �ͷ��ڴ溯��
void MemoryFree(double *x)
{
	free(x);  // �ͷ��ڴ�
	x = NULL; // ��ָ����Ϊ��
}

// ��ʼ�������������
DLL_API void init(struct parameter *para)
{
	srand(time(NULL));
	// ��ʼ�����������
	for (int i = 0; i < 3; i++)										 // ��������˵���
		for (int j = 0; j < 3; j++)									 // ��������˵���
			para->conv_kernel11[i][j] = (rand() / (RAND_MAX + 1.0)); // ��ʼ�������1��1��
																	 // 0-1�������
	for (int i = 0; i < 3; i++)										 // ��������˵���
		for (int j = 0; j < 3; j++)									 // ��������˵���
			para->conv_kernel12[i][j] = (rand() / (RAND_MAX + 1.0)); // ��ʼ�������1��2��

	for (int i = 0; i < 3; i++)											 // ��������˵���
		for (int j = 0; j < 3; j++)										 // ��������˵���
			para->conv_kernel21[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // ��ʼ�������1��3��
																		 // 0-0.2�����
	for (int i = 0; i < 3; i++)											 // ��������˵���
		for (int j = 0; j < 3; j++)										 // ��������˵���
			para->conv_kernel22[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // ��ʼ�������2��1��

	for (int i = 0; i < 3; i++)											 // ��������˵���
		for (int j = 0; j < 3; j++)										 // ��������˵���
			para->conv_kernel31[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // ��ʼ�������2��2��

	for (int i = 0; i < 3; i++)											 // ��������˵���
		for (int j = 0; j < 3; j++)										 // ��������˵���
			para->conv_kernel32[i][j] = (rand() / (RAND_MAX + 1.0)) / 5; // ��ʼ�������2��3��

	for (int i = 0; i < 1152; i++)											   // ����ȫ���Ӳ�1����
		for (int j = 0; j < 180; j++)										   // ����ȫ���Ӳ�1����
			para->fc_hidden_layer1[i][j] = (rand() / (RAND_MAX + 1.0)) / 1000; // ��ʼ��ȫ���Ӳ�1
																			   // �����0-0.001
	for (int i = 0; i < 180; i++)											   // ����ȫ���Ӳ�2����
		for (int j = 0; j < 45; j++)										   // ����ȫ���Ӳ�2����
			para->fc_hidden_layer2[i][j] = (rand() / (RAND_MAX + 1.0)) / 100;  // ��ʼ��ȫ���Ӳ�2
																			   // �����0-0.01
	for (int i = 0; i < 45; i++)											   // ����ȫ���Ӳ�3����
		for (int j = 0; j < 10; j++)										   // ����ȫ���Ӳ�3����
			para->fc_hidden_layer3[i][j] = (rand() / (RAND_MAX + 1.0)) / 10;   // ��ʼ��ȫ���Ӳ�3
																			   // �����0-0.1
}
// readed
// important ǰ�򴫲�--����һ��������Ϊ��һ�������--ͼƬ->����ͼ->����
void forward(double *input_matrix, struct parameter *para, struct result *data)
{
	// ���
	// �����1:
	// ��һ��������������Ϊ30x30������˴�СΪ3x3�����Ϊ28x28
	Conv2d(30, 30, 3, input_matrix, &para->conv_kernel11[0][0], &data->first_conv1[0][0]);
	// �ڶ���������������Ϊ28x28������˴�СΪ3x3�����Ϊ26x26
	Conv2d(28, 28, 3, &data->first_conv1[0][0], &para->conv_kernel21[0][0], &data->sencond_conv1[0][0]);
	// ������������������Ϊ26x26������˴�СΪ3x3�����Ϊ24x24
	Conv2d(26, 26, 3, &data->sencond_conv1[0][0], &para->conv_kernel31[0][0], &data->third_conv1[0][0]);

	// �����2:
	// ��һ��������������Ϊ30x30������˴�СΪ3x3�����Ϊ28x28
	Conv2d(30, 30, 3, input_matrix, &para->conv_kernel12[0][0], &data->first_conv2[0][0]);
	// �ڶ���������������Ϊ28x28������˴�СΪ3x3�����Ϊ26x26
	Conv2d(28, 28, 3, &data->first_conv2[0][0], &para->conv_kernel22[0][0], &data->sencond_conv2[0][0]);
	// ������������������Ϊ26x26������˴�СΪ3x3�����Ϊ24x24
	Conv2d(26, 26, 3, &data->sencond_conv2[0][0], &para->conv_kernel32[0][0], &data->third_conv2[0][0]);

	// ������ͨ���ľ���������ƴ�ӣ��õ�1x1152������
	MatrixExtensionImproved(24, 24, &data->third_conv1[0][0], &data->third_conv2[0][0], &data->flatten_conv[0][0]);

	// ȫ����
	// ȫ���Ӳ�1������Ϊ1x1152�����Ϊ1x180
	MatrixMultiply(1, 1152, 180, &data->flatten_conv[0][0], &para->fc_hidden_layer1[0][0], &data->first_fc[0][0]);
	// ReLU�����������Ϊ1x180�����Ϊ1x180
	Relu(1, 180, &data->first_fc[0][0], &data->first_relu[0][0]);
	// ȫ���Ӳ�2������Ϊ1x180�����Ϊ1x45
	MatrixMultiply(1, 180, 45, &data->first_relu[0][0], &para->fc_hidden_layer2[0][0], &data->second_fc[0][0]);
	// ReLU�����������Ϊ1x45�����Ϊ1x45
	Relu(1, 45, &data->second_fc[0][0], &data->second_relu[0][0]);
	// ȫ���Ӳ�3������Ϊ1x45�����Ϊ1x10
	MatrixMultiply(1, 45, 10, &data->second_relu[0][0], &para->fc_hidden_layer3[0][0], &data->outmlp[0][0]);
	double probability = 0;

	// ������ʣ�������
	// ����softmax����
	for (int i = 0; i < 10; i++)
		probability += exp(data->outmlp[0][i]);
	for (int i = 0; i < 10; i++)
	{
		data->result[i] = exp(data->outmlp[0][i]) / probability;
		result[i] = data->result[i];
	}
}
// important �Բ���Ȩ�ع��ڽ�������󵼣���������Ȩ��
void backward(int label, struct parameter *para, struct result *data)
{
	// �����������ݶ�
	int double_len = sizeof(double);
	double *out_grad;
	out_grad = (double *)malloc(10 * double_len);
	for (int i = 0; i < 10; i++)
		if (i == label)
			out_grad[i] = data->result[i] - 1;
		else
			out_grad[i] = data->result[i] - 0;

	// ����ȫ���Ӳ�3��Ȩ���ݶ�
	double *out_wgrad;
	out_wgrad = (double *)malloc(450 * double_len);
	MatrixBackPropagationMultiply(45, 10, &data->second_relu[0][0], out_grad, out_wgrad);

	// ����ȫ���Ӳ�3�������ݶ�
	double *second_rgrad;
	second_rgrad = (double *)malloc(45 * double_len);
	CalculateMatrixGrad(45, 10, &para->fc_hidden_layer3[0][0], out_grad, second_rgrad);
	MemoryFree(out_grad);

	// ����ReLU��������ݶ�
	double *second_grad;
	second_grad = (double *)malloc(180 * double_len);
	ReluBackPropagation(45, &data->second_fc[0][0], second_rgrad, second_grad);
	MemoryFree(second_rgrad);

	// ����ȫ���Ӳ�2��Ȩ���ݶ�
	double *second_wgrad;
	second_wgrad = (double *)malloc(8100 * double_len);
	MatrixBackPropagationMultiply(180, 45, &data->first_relu[0][0], second_grad, second_wgrad);

	// ����ȫ���Ӳ�2�������ݶ�
	double *first_rgrad;
	first_rgrad = (double *)malloc(180 * double_len);
	CalculateMatrixGrad(180, 45, &para->fc_hidden_layer2[0][0], second_grad, first_rgrad);
	MemoryFree(second_grad);

	// ����ReLU��������ݶ�
	double *first_grad;
	first_grad = (double *)malloc(180 * double_len);
	ReluBackPropagation(180, &data->first_fc[0][0], first_rgrad, first_grad);
	MemoryFree(first_rgrad);

	// ����ȫ���Ӳ�1��Ȩ���ݶ�
	double *first_wgrad;
	first_wgrad = (double *)malloc(207360 * double_len);
	MatrixBackPropagationMultiply(1152, 180, &data->flatten_conv[0][0], first_grad, first_wgrad);

	// ����ȫ���Ӳ�1�������ݶ�
	double *all_conv_grad;
	all_conv_grad = (double *)malloc(1152 * double_len);
	CalculateMatrixGrad(1152, 180, &para->fc_hidden_layer1[0][0], first_grad, all_conv_grad);
	MemoryFree(first_grad);

	// ���������ݶȷָ������ͨ��
	double *third_conv_grad1;
	third_conv_grad1 = (double *)malloc(576 * double_len);
	double *third_conv_grad2;
	third_conv_grad2 = (double *)malloc(576 * double_len);
	MatrixSplit(all_conv_grad, third_conv_grad1, third_conv_grad2);
	MemoryFree(all_conv_grad);

	// ������������˵��ݶ�
	double *third_kernel_grad;
	third_kernel_grad = (double *)malloc(9 * double_len);
	Conv2d(26, 26, 24, &data->sencond_conv1[0][0], third_conv_grad1, third_kernel_grad);

	// ��ת����������
	double *third_kernel_overturn;
	third_kernel_overturn = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel31[0][0], third_kernel_overturn);

	// �Ե��������ݶȽ������
	double *third_conv_grad_padding1;
	third_conv_grad_padding1 = (double *)malloc(784 * double_len);
	Padding(26, 1, third_conv_grad1, third_conv_grad_padding1);
	MemoryFree(third_conv_grad1);

	// ����ڶ������˵��ݶ�
	double *second_conv_grad1;
	second_conv_grad1 = (double *)malloc(676 * double_len);
	Conv2d(28, 28, 3, third_conv_grad_padding1, third_kernel_overturn, second_conv_grad1);
	MemoryFree(third_kernel_overturn);
	MemoryFree(third_conv_grad_padding1);

	// ����ڶ������˵��ݶ�
	double *second_kernel_grad;
	second_kernel_grad = (double *)malloc(9 * double_len);
	Conv2d(28, 28, 26, &data->first_conv1[0][0], second_conv_grad1, second_kernel_grad);

	// ��ת�ڶ�������
	double *second_kernel_overturn;
	second_kernel_overturn = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel21[0][0], second_kernel_overturn);

	// �Եڶ������ݶȽ������
	double *second_conv_grad_padding1;
	second_conv_grad_padding1 = (double *)malloc(900 * double_len);
	Padding(28, 1, second_conv_grad1, second_conv_grad_padding1);
	MemoryFree(second_conv_grad1);

	// �����һ�����˵��ݶ�
	double *first_conv_grad;
	first_conv_grad = (double *)malloc(784 * double_len);
	Conv2d(30, 30, 3, second_conv_grad_padding1, second_kernel_overturn, first_conv_grad);
	MemoryFree(second_kernel_overturn);
	MemoryFree(second_conv_grad_padding1);

	// �����һ�����˵��ݶ�
	double *first_kernel_grad;
	first_kernel_grad = (double *)malloc(9 * double_len);
	Conv2d(30, 30, 28, &data->mnist_data[0][0], first_conv_grad, first_kernel_grad);
	MemoryFree(first_conv_grad);

	// ������������˵��ݶ�
	double *third_kernel_grad2;
	third_kernel_grad2 = (double *)malloc(9 * double_len);
	Conv2d(26, 26, 24, &data->sencond_conv2[0][0], third_conv_grad2, third_kernel_grad2);

	// ��ת����������
	double *third_kernel_overturn2;
	third_kernel_overturn2 = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel32[0][0], third_kernel_overturn2);

	// �Ե��������ݶȽ������
	double *third_conv_grad_padding2;
	third_conv_grad_padding2 = (double *)malloc(784 * double_len);
	Padding(26, 1, third_conv_grad2, third_conv_grad_padding2);
	MemoryFree(third_conv_grad2);

	// ����ڶ������˵��ݶ�
	double *second_conv_grad2;
	second_conv_grad2 = (double *)malloc(676 * double_len);
	Conv2d(28, 28, 3, third_conv_grad_padding2, third_kernel_overturn2, second_conv_grad2);
	MemoryFree(third_conv_grad_padding2);

	// ����ڶ������˵��ݶ�
	double *second_kernel_grad2;
	second_kernel_grad2 = (double *)malloc(9 * double_len);
	Conv2d(28, 28, 26, &data->first_conv2[0][0], second_conv_grad2, second_kernel_grad2);

	// ��ת�ڶ�������
	double *second_kernel_overturn2;
	second_kernel_overturn2 = (double *)malloc(9 * double_len);
	OverturnKernel(3, &para->conv_kernel22[0][0], second_kernel_overturn2);

	// �Եڶ������ݶȽ������
	double *second_conv_grad_padding2;
	second_conv_grad_padding2 = (double *)malloc(900 * double_len);
	Padding(28, 1, second_conv_grad2, second_conv_grad_padding2);
	MemoryFree(second_conv_grad2);

	// �����һ�����˵��ݶ�
	double *first_conv_grad2;
	first_conv_grad2 = (double *)malloc(784 * double_len);
	Conv2d(30, 30, 3, second_conv_grad_padding2, second_kernel_overturn2, first_conv_grad2);
	MemoryFree(second_kernel_overturn2);
	MemoryFree(second_conv_grad_padding2);

	// �����һ�����˵��ݶ�
	double *first_kernel_grad2;
	first_kernel_grad2 = (double *)malloc(9 * double_len);
	Conv2d(30, 30, 28, &data->mnist_data[0][0], first_conv_grad2, first_kernel_grad2);

	// ���¾���˲���
	MatrixBackPropagation(3, 3, first_kernel_grad, &para->conv_kernel11[0][0]);
	MatrixBackPropagation(3, 3, second_kernel_grad, &para->conv_kernel21[0][0]);
	MatrixBackPropagation(3, 3, third_kernel_grad, &para->conv_kernel31[0][0]);
	MatrixBackPropagation(3, 3, first_kernel_grad2, &para->conv_kernel12[0][0]);
	MatrixBackPropagation(3, 3, second_kernel_grad2, &para->conv_kernel22[0][0]);
	MatrixBackPropagation(3, 3, third_kernel_grad2, &para->conv_kernel32[0][0]);

	// ����ȫ���Ӳ����
	MatrixBackPropagation(1152, 180, first_wgrad, &para->fc_hidden_layer1[0][0]);
	MatrixBackPropagation(180, 45, second_wgrad, &para->fc_hidden_layer2[0][0]);
	MatrixBackPropagation(45, 10, out_wgrad, &para->fc_hidden_layer3[0][0]);

	// �ͷ��ڴ�
	MemoryFree(first_kernel_grad);
	MemoryFree(second_kernel_grad);
	MemoryFree(third_kernel_grad);
	MemoryFree(first_kernel_grad2);
	MemoryFree(second_kernel_grad2);
	MemoryFree(third_kernel_grad2);
	MemoryFree(first_wgrad);  // �ͷ�ȫ���Ӳ�1Ȩ���ݶȵ��ڴ�
	MemoryFree(second_wgrad); // �ͷ�ȫ���Ӳ�2Ȩ���ݶȵ��ڴ�
	MemoryFree(out_wgrad);	  // �ͷ�ȫ���Ӳ�3Ȩ���ݶȵ��ڴ�
	return;					  // ����
}
// ���ݼ��غ���
DLL_API BOOL DataLoader()
{
	for (int num = 0; num < 10; num++)
	{ // ��������0��9
		for (int i = 0; i < SAMPLE_NUM; i++)
		{												  // ����ÿ�����ֵ�����
			char *e = (char *)malloc(sizeof(char) * 120); // ����120�ֽڵ��ڴ����ڴ洢ͼ������
			int *l = (int *)malloc(sizeof(int) * 960);	  // ����960���������ڴ����ڴ洢��ֵ�����ͼ������
			if (e == NULL || l == NULL)
			{ // ����ڴ�����Ƿ�ɹ�
				perror("�ڴ����ʧ�ܣ�\n");
				free(e);	  // �ͷ��ѷ�����ڴ�
				free(l);	  // �ͷ��ѷ�����ڴ�
				return FALSE; // ����FALSE��ʾ�ڴ����ʧ��
			}

			char route_name[50] = "Training_set/"; // ����ѵ�����ļ�·��
			char file_name[15];
			sprintf(file_name, "%d/%d.bmp", num, i + 1); // �����ļ���
			strcat(route_name, file_name);				 // ƴ���ļ�·��

			FILE *fp = fopen(route_name, "rb"); // ���ļ�
			if (!fp)
			{ // ����ļ��Ƿ�ɹ���
				perror("δ�ܴ�ѵ��������. ���'Training_set'�ļ����Ƿ���ڣ������ļ�������ͼƬ!\n");
				free(e); // �ͷ��ѷ�����ڴ�
				free(l);
				return FALSE; // ����FALSE��ʾ�ļ���ʧ��
			}

			fseek(fp, 62, SEEK_SET);		 // �����ļ�ͷ����62�ֽ�
			fread(e, sizeof(char), 120, fp); // ��ȡ120�ֽڵ�ͼ������
			fclose(fp);						 // �ر��ļ�
			// TODO תΪ������
			// XXX ����Ϊ30x30�޻Ҷ�bmpͼ
			int y = 0;
			for (int r = 0; r < 120; r++)
			{ // ������ȡ��ͼ������
				for (int u = 1; u < 9; u++)
				{											// ����ÿ���ֽڵ�ÿһλ
					l[y] = (int)((e[r] >> (8 - u)) & 0x01); // ��ÿһλת��Ϊ��ֵ������
					y++;
					if (y > 960)
						break;
				}
			}
			// TODO �洢�����Ƶ�Sample
			int g = 0;
			for (int u = 0; u < 30; u++)
			{ // ����0-29(1-30)��
				y = 0;
				for (int j = 0; j < 32; j++)
				{ // ����0-29(1-30)��
					if (j != 30 && j != 31)
					{												 // ������30�͵�31��
						Sample[num * SAMPLE_NUM + i].a[u][y] = l[g]; // ����ֵ�����ݴ洢������������
						y++;
					}
					g++;
				}
			}

			int q = Sample[num * SAMPLE_NUM + i].a[0][0];
			if (q == 1)
			{

				// TODO why�����һ������Ϊ1����ͼ������ȡ��
				// �����һ������Ϊ1����ͼ������ȡ��
				for (int b = 0; b < 30; b++)
				{
					for (int n = 0; n < 30; n++)
					{
						Sample[num * SAMPLE_NUM + i].a[b][n] = 1 - Sample[num * SAMPLE_NUM + i].a[b][n];
					}
				}
			}

			Sample[num * SAMPLE_NUM + i].number = num; // ���������ı�ǩ
			free(e);								   // �ͷ��ѷ�����ڴ�
			free(l);
		}
	}
	return TRUE; // ����TRUE��ʾ���ݼ��سɹ�
}

// ��ȡ��������ļ�����
DLL_API BOOL read_file(struct parameter *parameter_dest)
{
	system("chcp 65001");
	FILE *fp;
	fp = fopen("network_parameter.txt", "rb"); // �Զ�����ģʽ����������ļ�
	if (fp == NULL)
	{ // ����ļ��Ƿ�ɹ���
		printf("�ļ���ʧ�ܣ�������������ļ��Ƿ���ѵ�����ļ����ڣ�\n");
		return FALSE; // ����FALSE��ʾ�ļ���ʧ��
	}
	struct parameter *parameter_tmp = NULL;
	parameter_tmp = (struct parameter *)malloc(sizeof(struct parameter)); // �����ڴ�������ʱ�洢�������
	fread(parameter_tmp, sizeof(struct parameter), 1, fp);				  // ���ļ��ж�ȡ�������
	(*parameter_dest) = (*parameter_tmp);								  // ����ȡ�����������ֵ��Ŀ�����
	fclose(fp);															  // �ر��ļ�
	free(parameter_tmp);												  // �ͷ���ʱ�洢���ڴ�
	parameter_tmp = NULL;

	return TRUE; // ����TRUE��ʾ�ļ���ȡ�ɹ�
}

// ���������д���ļ�����
DLL_API BOOL write_para_to_file(struct parameter *parameter_file)
{
	FILE *fp;
	fp = fopen("network_parameter.txt", "wb"); // �Զ�����ģʽ����������ļ�
	struct parameter *parameter_tmp;
	parameter_tmp = (struct parameter *)malloc(sizeof(struct parameter)); // �����ڴ�������ʱ�洢�������

	(*parameter_tmp) = (*parameter_file);					// �����������ֵ����ʱ����
	fwrite(parameter_tmp, sizeof(struct parameter), 1, fp); // ���������д���ļ�

	fclose(fp);			 // �ر��ļ�
	free(parameter_tmp); // �ͷ���ʱ�洢���ڴ�
	parameter_tmp = NULL;

	return TRUE; // ����TRUE��ʾ�ļ�д��ɹ�
}

// ��ӡ����������ļ�����
void printf_file2(struct parameter *parameter4)
{
	FILE *fp;
	fp = fopen("NetworkParameters.bin", "wb"); // �Զ�����ģʽ����������ļ�
	struct parameter *parameter1;
	parameter1 = (struct parameter *)malloc(sizeof(struct parameter)); // �����ڴ�������ʱ�洢�������
	(*parameter1) = (*parameter4);									   // �����������ֵ����ʱ����
	fwrite(parameter1, sizeof(struct parameter), 1, fp);			   // ���������д���ļ�
	fclose(fp);														   // �ر��ļ�
	free(parameter1);												   // �ͷ���ʱ�洢���ڴ�
	parameter1 = NULL;
	return;
}

// ��������ʧ����
double Cross_entropy(double *a, int m)
{
	double u = 0;
	u = (-log10(a[m])); // ���㽻������ʧ
	return u;
}

// ��ʾ����������
void show_progress_bar(int progress, int total)
{
	system("chcp 65001");
	int bar_width = 50;								  // ���������
	float percent_complete = (float)progress / total; // ������ɰٷֱ�
	int position = bar_width * percent_complete;	  // ���������λ��

	printf("[");
	for (int i = 0; i < bar_width; ++i)
	{ // ���ƽ�����
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
	printf("] %d%%\r", (int)(percent_complete * 100)); // ��ʾ�ٷֱ�
	fflush(stdout);									   // ˢ�����������
}

// important ѵ����ѧϰ
DLL_API void train(int epochs, struct parameter *para, struct result *data)
{
	system("chcp 65001");
	double corss_loss = 2;						 // ��ʼ����������ʧ
	for (int epoch = 0; epoch < epochs; epoch++) // ����ÿ��ѵ������
	{
		lr = pow((corss_loss / 10), 1.7); // ���ݽ�������ʧ����ѧϰ��
		if (lr > 0.01)					  // ����ѧϰ�ʵ����ֵ
		{
			lr = 0.01;
		}

		show_progress_bar(epoch + 1, epochs); // ��ʾѵ��������
		if ((epoch + 1) % 10 == 0)			  // ÿ10���������һ�ν�������ʧ��ѧϰ��
		{
			fflush(stdout); // ˢ�����������
			printf("\t��������ʧ: %lf  ѧϰ��:%.10lf\n", corss_loss, lr);
			if (lr < 0.0000000001) // ���ѧϰ�ʹ�С���򱣴��������
				printf_file2(para);
		}

		// �����������������˳��
		int a, b;
		srand(time(NULL)); // ��ʼ�����������
		struct sample *sample_tmp = NULL;
		sample_tmp = (struct sample *)malloc(sizeof(struct sample)); // �����ڴ�������ʱ�洢����
		for (int q = 0; q < SAMPLE_NUM * 10; q++)					 // �����������
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

		for (int i = 0; i < SAMPLE_NUM * 10; i++) // ����ÿ������
		{
			corss_loss = 0; // ��ʼ����������ʧ
			(*sample_tmp) = Sample[i];
			int y = sample_tmp->number;					   // ��ȡ������ǩ
			forward(&sample_tmp->a[0][0], para, data);	   // ǰ�򴫲�
			backward(y, para, data);					   // ���򴫲�
			double g = Cross_entropy(&data->result[0], y); // ���㽻������ʧ
			if (g > corss_loss)
				corss_loss = g; // ���½�������ʧ
		}
		free(sample_tmp); // �ͷ���ʱ�洢�������ڴ�
		sample_tmp = NULL;
	}
	printf("\n");
	return;
}

DLL_API void test_network(struct parameter *parameter2, struct result *data2)
{
	system("chcp 65001");
	setlocale(LC_ALL, "");
	char e[120];
	int l[960];
	double data[30][30];
	for (int i = 0; i < 10; i++) // ����ÿ����������
	{
		FILE *fp;
		char s[30];
		sprintf(s, "%s%d%s", "Test_set/", i, ".bmp"); // ���ɲ��������ļ���
		printf("\n�򿪵��ļ���:%s", s);
		fp = fopen(s, "rb"); // �򿪲��������ļ�
		if (!fp)
		{
			perror("���ܴ��ļ�!\n");
			return;
		}
		fseek(fp, 62, SEEK_SET);		 // �����ļ�ͷ����62�ֽ�
		fread(e, sizeof(char), 120, fp); // ��ȡ120�ֽڵ�ͼ������
		fclose(fp);
		int y = 0;
		for (int r = 0; r < 120; r++) // ������ȡ��ͼ������
		{
			for (int u = 1; u < 9; u++) // ����ÿ���ֽڵ�ÿһλ
			{
				l[y] = (int)((e[r]) >> (8 - u) & 0x01); // ��ÿһλת��Ϊ��ֵ������
				y++;
				if (y > 960)
					break;
			};
		};
		y = 0;
		int g = 0;
		for (int u = 0; u < 30; u++) // ����30��
		{
			y = 0;
			for (int j = 0; j < 32; j++) // ����32��
			{
				if ((j != 30) && (j != 31)) // ������30�͵�31��
				{
					data[u][y] = l[g]; // ����ֵ�����ݴ洢������������
					y++;
				};
				g++;
			}
		}
		int q = data[0][0];
		if (q == 1) // �����һ������Ϊ1����ͼ������ȡ��
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
		forward(&data[0][0], parameter2, data2); // ǰ�򴫲�

		double sum = 0;
		int k = 0;
		for (int j = 0; j < 10; j++) // ����ÿ�����
		{
			if (result[j] > sum)
			{
				sum = result[j];
				k = j; // �ҵ������������
			}
			else
				continue;
		}
		printf("\n");
		for (int i = 0; i < 10; i++) // ���ÿ������Ԥ�����
		{
			printf("\tԤ��ֵ��%d�ĸ��ʣ�%lf\n", i, result[i]);
		}

		printf("\033[1;31;43m����Ԥ��ֵ: %d \033[0m\n", k); // �������Ԥ����
	}
	return;
}

// int main(int argc, char const *argv[])
// {
// 	// ����̨���utf8
// 	system("chcp 65001");
// 	int h = DataLoader(); // ����ѵ������
// 	if (h == TRUE)
// 	{
// 		printf("ѵ�����ݶ�ȡ�ɹ���\n");
// 	}
// 	else if (h == FALSE)
// 	{
// 		printf("ѵ������ȡʧ�ܣ������Զ��˳�\n");
// 		return 0;
// 	}

// 	printf("=============================================\n");
// 	printf("��ʼѵ������\n");
// 	// ����,���䵽���ϵĲ���
// 	struct parameter *storage;
// 	(storage) = (struct parameter *)malloc(sizeof(struct parameter)); // �����ڴ����ڴ洢�������
// 	struct result *data;
// 	(data) = (struct result *)malloc(sizeof(struct result)); // �����ڴ����ڴ洢���

// 	// XXX ��Ϣѭ��
// 	char g;
// 	do
// 	{
// 		printf("�������Ƿ�ϣ������ѵ������������ļ��ж�ȡ���������(���밴 y�����밴 n): ");
// 		setbuf(stdin, NULL);	  // ������뻺����
// 		g = getchar();			  // ��ȡ�û�����
// 		while (getchar() != '\n') // ������뻺�����е�ʣ���ַ�
// 			;
// 		if (g == 'y') // ����û����� 'y'
// 		{
// 			int h = read_file(storage); // ���Զ�ȡ��������ļ�
// 			if (h == FALSE)				// �����ȡʧ��
// 			{
// 				printf("�����������ڣ���ʼ�Զ������ʼ���������\n");
// 				init(storage);				 // �����ʼ���������
// 				write_para_to_file(storage); // ����ʼ�����������д���ļ�
// 				printf("���������ʼ����ϣ�\n");
// 				printf("��������ѱ��浽 network_parameter.txt �ļ���\n");
// 			}
// 			else if (h == TRUE) // �����ȡ�ɹ�
// 			{
// 				printf("������ȡ�ɹ�!\n");
// 			}
// 		}
// 		else if (g == 'n') // ����û����� 'n'
// 		{
// 			init(storage);				 // �����ʼ���������
// 			write_para_to_file(storage); // ����ʼ�����������д���ļ�
// 			printf("������ʼ����ϣ�\n");
// 		}
// 	} while (g != 'y' && g != 'n'); // ѭ��ֱ���û����� 'y' �� 'n'
// 	printf("=============================================\n"); // ��ӡ�ָ���

// 	// important ѧϰѭ��
// 	int epoch;
// 	char v;
// 	do
// 	{
// 		printf("������Ԥѵ���Ĵ�����");
// 		scanf("%d", &epoch);
// 		printf("��ʼѵ��\n");
// 		train(epoch, storage, data);
// 		printf("��ʼ����\n");
// 		test_network(storage, data);
// 		write_para_to_file(storage);
// 		do
// 		{
// 			printf("����ѵ���밴�س����˳��밴q: ");
// 			setbuf(stdin, NULL);	  // ������뻺����
// 			v = getchar();			  // ��ȡ�û�����
// 			while (getchar() != '\n') // ������뻺�����е�ʣ���ַ�
// 				;
// 			if (v == 'q') // ����û����� 'q'
// 			{
// 				write_para_to_file(storage); // ���������д���ļ�
// 				return 0;					 // �˳�����
// 			}
// 			else if (v == '\n') // ����û����»س���
// 			{
// 				break; // �˳�ѭ��������ѵ��
// 			}
// 			else // ����û����������ַ�
// 			{
// 				printf("����������������룡\n"); // ��ʾ�������
// 			}
// 		} while (1); // ѭ��ֱ���û�������ȷ
// 	} while (v != 'q'); // ѭ��ֱ���û����� 'q'

// 	free(storage); // �ͷŴ洢����������ڴ�
// 	free(data);	   // �ͷŴ洢������ڴ�

// 	return 0; // ����0����ʾ������������
// }