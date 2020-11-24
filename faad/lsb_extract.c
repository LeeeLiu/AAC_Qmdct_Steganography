
#include "lsb_extract.h"   

#include "stdlib.h"   
#include "ctype.h"   
#include "conio.h"   
#include <string.h>   

typedef signed char byte;

//5��������ʵ��

//���� �ֽ�ch��positionλ�ã�position=1����LSB���ı���
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

//���ֽ�ch��positionλ�õı���bit�޸�Ϊvalue�������޸ĺ��ch
byte bitSet(byte ch, short position, bool value)
{
	//printf("bitSetǰ %d\n", ch);
	byte temp = ch;
	if (position>8 || position<1)
	{
		printf("Out of Bound! Position must be a number between 1 --- 8\n");
		return ch;
	}
	bool bit = bitAt(temp, position);//�ֽ�ch��positionλ��ԭ����bit
	if (bit^value)//���bit��value��ͬ
	{
		if (bit == 0)
		{
			byte m = (byte)value;
			for (int i = 1; i<position; i++)
				m = m << 1;
			ch += m;

			//	printf("bitSet�� %d\n", ch);
			return ch;
		}
		else
		{
			byte m = (byte)bit;
			for (int i = 1; i<position; i++)
				m = m << 1;
			ch -= m;
			//			printf("bitSet�� %d\n", ch);
			return ch;
		}
	}
	else
	{
		//	printf("bitSet�� %d\n", ch);
		return ch;//���bit��value��ͬ
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
	static int datalength = 0;//��������ȡ�����������ֵ��
	static int extract_count = 0;
	
	if (is_len_read == 0)
	{
		//*��temp����ȡdata length
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
		if ((data2e = fopen(extr_msg_file_name, "a+")) == NULL)    /*���ļ�*/
		{
			printf("\nCannot open file\n");
			exit(0);
		}

		//��ȡdatalength
		//����capacity
		//printf("\n");
		for (i = 0; i<1024; i++)
		if (quant[i] < 0 || quant[i] > 1)//��ֵ����һ�������� �� С��0��ż��
		{
			//printf("%d ", quant[i]);
			LowF_quant[capacity] = quant[i];
			capacity++;
		}

		if (capacity != 0 && capacity >= 8)
		{
			//stego_len�Ǳ�����ȡ��data���ֽ���
			int stego_len = min(datalength - extract_count, capacity / 8);
			//��ȡ��ʼ
			int times = 0;
			byte ch[8];
			while (times < stego_len)//��ȡ��Ϣ�����data
			{
				for (i = 0; i < 8; i++)
					ch[i] = LowF_quant[8 * times + i];

				data[times] = 0;
				//����ȡdata
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
		//��ȡ����
		fclose(data2e);	
	}
	is_len_read = 1;
}
