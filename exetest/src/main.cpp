#include <main.h>
int main(int argc, char const *argv[])
{
    // 控制台输出utf8
    system("chcp 65001");
    int h = DataLoader(); // 加载训练数据
    if (h == TRUE)
    {
        printf("训练数据读取成功！\n");
    }
    else if (h == FALSE)
    {
        printf("训练集读取失败！程序自动退出\n");
        return 0;
    }

    printf("=============================================\n");
    printf("开始训练网络\n");
    // 参数,分配到堆上的参数
    struct parameter *storage;
    (storage) = (struct parameter *)malloc(sizeof(struct parameter)); // 分配内存用于存储网络参数
    struct result *data;
    (data) = (struct result *)malloc(sizeof(struct result)); // 分配内存用于存储结果

    // XXX 消息循环
    char g;
    do
    {
        printf("请问您是否希望从已训练的网络参数文件中读取网络参数？(是请按 y，否请按 n): ");
        setbuf(stdin, NULL);      // 清空输入缓冲区
        g = getchar();            // 获取用户输入
        while (getchar() != '\n') // 清空输入缓冲区中的剩余字符
            ;
        if (g == 'y') // 如果用户输入 'y'
        {
            int h = read_file(storage); // 尝试读取网络参数文件
            if (h == FALSE)             // 如果读取失败
            {
                printf("参数包不存在！开始自动随机初始化网络参数\n");
                init(storage);               // 随机初始化网络参数
                write_para_to_file(storage); // 将初始化的网络参数写入文件
                printf("网络参数初始化完毕！\n");
                printf("网络参数已保存到 network_parameter.txt 文件中\n");
            }
            else if (h == TRUE) // 如果读取成功
            {
                printf("参数读取成功!\n");
            }
        }
        else if (g == 'n') // 如果用户输入 'n'
        {
            init(storage);               // 随机初始化网络参数
            write_para_to_file(storage); // 将初始化的网络参数写入文件
            printf("参数初始化完毕！\n");
        }
    } while (g != 'y' && g != 'n'); // 循环直到用户输入 'y' 或 'n'
    printf("=============================================\n"); // 打印分隔线

    // important 学习循环
    int epoch;
    char v;
    do
    {
        printf("请输入预训练的次数：");
        scanf("%d", &epoch);
        printf("开始训练\n");
        train(epoch, storage, data);
        printf("开始测试\n");
        test_network(storage, data);
        write_para_to_file(storage);
        do
        {
            std::wcout << L"继续训练请按回车,退出请按q: \n" ;
            setbuf(stdin, NULL);      // 清空输入缓冲区
            v = getchar();            // 获取用户输入
            while (getchar() != '\n') // 清空输入缓冲区中的剩余字符
                ;
            if (v == 'q') // 如果用户输入 'q'
            {
                write_para_to_file(storage); // 将网络参数写入文件
                return 0;                    // 退出程序
            }
            else if (v == '\n') // 如果用户按下回车键
            {
                break; // 退出循环，继续训练
            }
            else // 如果用户输入其他字符
            {
                std::wcout << L"输入错误，请重新输入！\n"; // 提示输入错误
            }
        } while (1); // 循环直到用户输入正确
    } while (v != 'q'); // 循环直到用户输入 'q'

    free(storage); // 释放存储网络参数的内存
    free(data);    // 释放存储结果的内存

    return 0; // 返回0，表示程序正常结束
}
