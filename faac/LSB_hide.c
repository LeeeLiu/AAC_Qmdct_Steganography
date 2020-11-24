
#include "LSB_hide.h"   

#include "stdlib.h"   
#include "ctype.h"   
#include "conio.h"   
#include <string.h>   

typedef signed char byte;

//5个函数的实现

//返回 字节ch里position位置（position=1代表LSB）的比特
bool bitAt(byte ch, short position)
{
	byte temp = ch;
	if (position>8 || position<1)
	{
		printf("Out of Bound! Position must be a number between 1 --- 8\n ");
		return false;
	}
	for (int j = 1; j<position; j++)
		temp = temp >> 1;
	if (temp % 2 == 0)return 0;
	else return 1;
}

//将字节ch里position位置的比特bit修改为value，返回修改后的ch
byte bitSet(byte ch, short position, bool value)
{
	//printf("bitSet前 %d\n", ch);
	byte temp = ch;
	if (position>8 || position<1)
	{
		printf("Out of Bound! Position must be a number between 1 --- 8\n");
		return ch;
	}
	bool bit = bitAt(temp, position);//字节ch里position位置原本是bit
	if (bit^value)//如果bit和value不同
	{
		if (bit == 0)
		{
			byte m = (byte)value;
			for (int i = 1; i<position; i++)
				m = m << 1;
			ch += m;

		//	printf("bitSet后 %d\n", ch);
			return ch;
		}
		else
		{
			byte m = (byte)bit;
			for (int i = 1; i<position; i++)
				m = m << 1;
			ch -= m;
//			printf("bitSet后 %d\n", ch);
			return ch;
		}
	}
	else
	{
	//	printf("bitSet后 %d\n", ch);
		return ch;//如果bit和value相同
	}
		
}

int getValue(byte*A, int num)
{
	int result = 0;
	for (int i = num - 1; i>0; i--)
	{
		result += A[i];
		result = result << 8;
	}
	result += A[0];
	return result;
}

void embed(int *quant)
{	
	int i,c;
	char t;	
	char data[1024/8];//每次从dataFile取出的可以隐藏的数据(最大是1024/8)
	int capacity = 0;//本帧的隐写容量（假设2）
	int LowF_quant[1024];	
	byte temp[32];//*把数据data length的每个比特提取到temp（设定32bit）
	FILE* dataFile;
	static int is_len_writed = 0;//判断消息长度是否已经写进quant
	static int hidden_count = 0;//当前已经隐写的字符数
	static int datalength=0;  //dataFile里数据长度（假设1，接收者已知）
	
	if ((dataFile = fopen(msg_file_name, "r")) == NULL)    /*打开文件*/
	{
		printf("\nCannot open file\n");
		exit(0);
	}
	//计算数据长度
	if (is_len_writed == 0)
	{
		while (fscanf(dataFile, "%c", &t) != EOF)
		{
			datalength++;//记录待隐写数据的总长度
		}
		printf("待隐藏的数据长度是%d\n", datalength);

		int data_len_count = datalength;
		for (i = 31; i >= 0; i--)
		{
			bool v = (bool)(data_len_count % 2);
			temp[i] = bitSet(temp[i], 1, v);//将每个字节的LSB位置设置为指定比特
			data_len_count = data_len_count / 2;
		}
		//将datalength写回quant
		for (i = 0; i < 32; i++)
			quant[i] = temp[i];

	}
	else{
		//计算隐写容量
		for (i = 0; i < 1024; i++)
		if (quant[i] < 0 || quant[i] > 1)//阈值大于一个正奇数 或 小于0或负偶数
		{
			LowF_quant[capacity] = quant[i];
			capacity++;
		}
		//printf("capacity = %d\n", capacity);

		if (capacity != 0 && capacity >= 8)
		{	//stego_len是本次隐写的data的字节数
			int stego_len = min(datalength - hidden_count, capacity / 8);
			fseek(dataFile, hidden_count, SEEK_SET);

			for (i = 0; i < stego_len; i++)
				fscanf(dataFile, "%c", &data[i]);

			//隐写开始
			int times = 0;
			byte ch[8];
			while (times < stego_len)
			{
				//每次取8个系数，存为ch[8]
				for (i = 0; i < 8; i++)
					ch[i] = LowF_quant[8 * times + i];

				for (i = 7; i >= 0; i--)
					ch[i] = bitSet(ch[i], 1, bitAt(data[times], 8 - i));//将一个字符data的每个比特隐藏在ch的LSB中

				for (i = 0; i < 8; i++)
					LowF_quant[8 * times + i] = ch[i];//每次修改8个系数

				times++;
			}

			//将秘密信息写回quant
			for (i = 0, c = 0; i < 1024 && c < stego_len * 8; i++)
			if (quant[i] < 0 || quant[i] > 1)//阈值大于一个正奇数 或 小于0或负偶数
			{
				//printf("%d ", quant[i]);
				quant[i] = LowF_quant[c];
				c++;
			}

			hidden_count += stego_len;//便于下次在dataFile中定位
			//隐写结束
		}
	}

	is_len_writed = 1;
	fclose(dataFile);
}
