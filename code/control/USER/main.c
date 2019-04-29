#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "serial.h"
#include "timer.h"
#include "pid.h"

//����ȫ�ֱ���
PID_TypeDef PID_x, PID_y;//����PID�ṹ��PID_x��PID_y

int coords[2];//��ǰ��������

u16 targetX = 240;//��ǰx����
u16 targetY = 240;//��ǰy����

int main(void)
{
	u16 pwmval_x, pwmval_y;//����ռ�ձȵı���(CCR�Ĵ�����ֵ)

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	TIM3_PWM_Init(9999,143);	 //Ƶ��Ϊ��72*10^6/(9999+1)/(143+1)=50Hz
	
	pid_init(0.6, 0, 18, &PID_x);
	pid_init(0.6, 0, 18, &PID_y);
	
	//��ʼ��ʱ����ˮƽ
	coords[0] = 240;
	coords[1] = 240;
	
 	while(1)
	{
		//1���Ӵ��ڶ�ȡ��ǰ����ֵ������coords������
		recieveData();
		//2��ʹ��pid������ƶ��PWMռ�ձȵĲ���
		pwmval_x = 730 + better_pid(coords[0],targetX, &PID_x);
		pwmval_y = 730 + better_pid(coords[1],targetY, &PID_y);
		//3���ֱ𽫿��Ʋ�����ֵ����ʱ��3��PWMͨ��1��ͨ��2
		TIM_SetCompare1(TIM3,pwmval_x);
		TIM_SetCompare2(TIM3,pwmval_y);
//		printf("x:%d\r\n", pwmval_x);
//		printf("y:%d\r\n", pwmval_y);

		
	}	 
}

