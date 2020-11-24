
#include "lsb_extract.h"   

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

void extract(int16_t *quant)
{
	int i;
	FILE * data2e;
	int LowF_quant[1024];
	int capacity = 0;
	char data[1024 / 8];

	static int is_len_read = 0;
	static int datalength = 0;//（假设提取出来的是这个值）
	static int extract_count = 0;
	
	if (is_len_read == 0)
	{
		//*从temp中提取data length
		for (int i = 0; i < 31; i++)
		{
			datalength += (int)bitAt(quant[i], 1);
			datalength = datalength << 1;
		}
		datalength += (int)bitAt(quant[31], 1);

		printf("\n\nDatalength = %d\n", datalength);
		//*/
	}

	else{
		if ((data2e = fopen(extr_msg_file_name, "a+")) == NULL)    /*打开文件*/
		{
			printf("\nCannot open file\n");
			exit(0);
		}

		//提取datalength
		//计算capacity
		//printf("\n");
		for (i = 0; i<1024; i++)
		if (quant[i] < 0 || quant[i] > 1)//阈值大于一个正奇数 或 小于0或负偶数
		{
			//printf("%d ", quant[i]);
			LowF_quant[capacity] = quant[i];
			capacity++;
		}

		if (capacity != 0 && capacity >= 8)
		{
			//stego_len是本次提取的data的字节数
			int stego_len = min(datalength - extract_count, capacity / 8);
			//提取开始
			int times = 0;
			byte ch[8];
			while (times < stego_len)//提取信息，输出data
			{
				for (i = 0; i < 8; i++)
					ch[i] = LowF_quant[8 * times + i];

				data[times] = 0;
				//！提取data
				for (i = 0; i < 7; i++)
				{
					data[times] += (int)bitAt(ch[i], 1);
					data[times] = data[times] * 2;
				}
				data[times] += (int)bitAt(ch[7], 1);

				fprintf(data2e, "%c", data[times]);
				printf("%c", data[times]);
				times++;
			}
			extract_count += stego_len;
		}
		//提取结束
		fclose(data2e);	
	}
	is_len_read = 1;
}
