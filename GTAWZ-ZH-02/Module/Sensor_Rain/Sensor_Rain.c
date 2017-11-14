#include "Sensor_Rain.h"

uint8_t isRain;

osThreadId tid_RAINMS_Thread;
osThreadDef(RAINMS_Thread,osPriorityNormal,1,512);

extern ARM_DRIVER_USART Driver_USART1;								//设备驱动库串口一设备声明

void RAIN_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.
}

void RAINMS_Thread(const void *argument){
	
#if(MOUDLE_DEBUG == 1)
	char disp[30];
#endif
	
	for(;;){
		
		isRain	= RAIN_DATA;
		
#if(MOUDLE_DEBUG == 1)	
		sprintf(disp,"\n\ris it rainning now? : %d\n\r", &isRain);			
		Driver_USART1.Send(disp,strlen(disp));
#endif	
		osDelay(1000);
	}
}

void RAINMS(void){

	tid_RAINMS_Thread = osThreadCreate(osThread(RAINMS_Thread),NULL);
}
