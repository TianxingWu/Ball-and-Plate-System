#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "serial.h"
#include "stdlib.h"

u16 t;  
u16 len;
u16 times=0; //����Ҫ


extern int coords[2];

void recieveData(void)
{
	u8 flag=0;
	char strX[4], strY[4];
	u8 cnt_x=0;
	u8 cnt_y=0;
	u8 *adress = NULL;
	
	if(USART_RX_STA&0x8000)		//�������������
		{	
			LED1=!LED1;
			
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			
			adress = &USART_RX_BUF[0];	//ָ��adress�����ַ���ַ����0-len��һ��
			
			//ȡ��������ַ���ʽ����strX��strY��
			for(t=0;t<len;t++)
			{
				if(*adress>='0' && *adress<='9')
				{
					if(flag==1)
					{
						strX[cnt_x] = *adress;
						cnt_x++;
					}
					else
					{
						strY[cnt_y] = *adress;
						cnt_y++;
					}	
				}
				else
				{
					if(*adress=='#')
						flag = 1;
					if(*adress=='$')
						flag = 2;
				}
				adress++;	
			}
			
			//ת���ַ���Ϊ���ͣ����洢��ȫ�ֱ���coords��
			coords[0] = atoi(strX);
			coords[1] = atoi(strY);
			//��־λ����
			USART_RX_STA=0;
		}
	else
		{
			times++;
			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10); 
		}
}
