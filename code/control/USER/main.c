#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "serial.h"
#include "timer.h"
#include "pid.h"

//定义全局变量
PID_TypeDef PID_x, PID_y;//两个PID结构体PID_x和PID_y

int coords[2];//当前坐标数组

u16 targetX = 240;//当前x坐标
u16 targetY = 240;//当前y坐标

int main(void)
{
	u16 pwmval_x, pwmval_y;//控制占空比的变量(CCR寄存器数值)

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	TIM3_PWM_Init(9999,143);	 //频率为：72*10^6/(9999+1)/(143+1)=50Hz
	
	pid_init(0.6, 0, 18, &PID_x);
	pid_init(0.6, 0, 18, &PID_y);
	
	//初始化时保持水平
	coords[0] = 240;
	coords[1] = 240;
	
 	while(1)
	{
		//1、从串口读取当前坐标值，存入coords数组中
		recieveData();
		//2、使用pid计算控制舵机PWM占空比的参数
		pwmval_x = 730 + better_pid(coords[0],targetX, &PID_x);
		pwmval_y = 730 + better_pid(coords[1],targetY, &PID_y);
		//3、分别将控制参数赋值给定时器3的PWM通道1、通道2
		TIM_SetCompare1(TIM3,pwmval_x);
		TIM_SetCompare2(TIM3,pwmval_y);
//		printf("x:%d\r\n", pwmval_x);
//		printf("y:%d\r\n", pwmval_y);

		
	}	 
}

