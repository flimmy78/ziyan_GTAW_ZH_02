#include <Bsp.h>
	
funMoudle *moudleTestForm[] = {
	
	(void *)0								,(void *)0								,(void *)0								,(void *)0								,&TSL2561MS 							,
	(void *)0								,(void *)0								,(void *)0								,&SHT11MS								,(void *)0								,
	(void *)0								,&LEDArrayCM							,(void *)0								,(void *)0								,(void *)0								,
	(void *)0								,(void *)0								,(void *)0								,(void *)0								,(void *)0								
};

funMoudle *moudleInitForm[] = {
	
	
	(void *)0								,(void *)0								,(void *)0								,(void *)0								,&TSL2561_Init							,
	(void *)0								,(void *)0								,(void *)0								,&SHT11_Init							,(void *)0								,
	(void *)0								,&LEDArray_Init						,(void *)0								,(void *)0								,(void *)0								,
	(void *)0								,(void *)0								,(void *)0								,(void *)0								,(void *)0								
};

void  BSP_Init(void){
	
	bsp_delayinit	();
	testInit			();
	USART1Init1		();
	USART1Init2		();
	USART2Init1		();
	USART2Init2		();
	keyInit			();
	BEEP_Init		();
	LCD144_Init		();
	//Beep_time(500);
	
	mutexUSART1_Active();
	
	moudleInitForm[MOUDLE_ID-1]();
}

void BSP_Test(void){

	//LEDTest		();	Ӳ����֧��
	USART1Debug();
	USART2Trans();
	keyTest ();
	LCD_144_test();
	
	moudleTestForm[MOUDLE_ID-1]();
}
