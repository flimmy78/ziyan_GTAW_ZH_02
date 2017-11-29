#include "BMP180.h"

//***BMP085ʹ��
float result_UT = 0.0;
float result_UP = 0.0;
float result_UA = 0.0;

short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

extern ARM_DRIVER_USART Driver_USART1;								//�豸�����⴮��һ�豸����

osThreadId tid_BMP180MS_Thread;
osThreadDef(BMP180MS_Thread,osPriorityNormal,1,1024);

//*********************************************************
//����BMP085�ڲ�����,��������
//*********************************************************
uint16_t Multiple_read(uint8_t ST_Address)
{   
	uint8_t msb, lsb;
	uint16_t _data;
	IIC_Start();                          //��ʼ�ź�
	IIC_Send_Byte(BMP180_SlaveAddress);    //�����豸��ַ+д�ź�
	while(IIC_Wait_Ack());
	IIC_Send_Byte(ST_Address);             //���ʹ洢��Ԫ��ַ
	while(IIC_Wait_Ack());                                 
	IIC_Start();                          //��ʼ�ź�
	IIC_Send_Byte(BMP180_SlaveAddress+1);         //�����豸��ַ+���ź�
	while(IIC_Wait_Ack());                                        

	msb = IIC_Read_Byte(1);                 //BUF[0]�洢                
	lsb = IIC_Read_Byte(0);                //���һ��������Ҫ��NOACK

	IIC_Stop();                           //ֹͣ�ź�
	delay_ms(5);
	_data = msb << 8;
	_data |= lsb;	
	return _data;
}
//********************************************************************
uint16_t bmp180ReadTemp(void)
{

    IIC_Start();                  //��ʼ�ź�
    IIC_Send_Byte(BMP180_SlaveAddress);   //�����豸��ַ+д�ź�   BMP180_SlaveAddress  0xee	��ѹ������������ַ
	while(IIC_Wait_Ack());
    IIC_Send_Byte(0xF4);	          // write register address
    while(IIC_Wait_Ack());
	IIC_Send_Byte(0x2E);       	// write register data for temp
    while(IIC_Wait_Ack());
	IIC_Stop();                   //����ֹͣ�ź�
	delay_ms(10);	// max time is 4.5ms
	return Multiple_read(0xF6);
}
//*************************************************************
uint16_t bmp180ReadPressure(void)
{
	//uint16_t pressure = 0;

    IIC_Start();                   //��ʼ�ź�
    IIC_Send_Byte(BMP180_SlaveAddress);   //�����豸��ַ+д�ź�
    while(IIC_Wait_Ack());
	IIC_Send_Byte(0xF4);	          // write register address
    while(IIC_Wait_Ack());
	IIC_Send_Byte(0x34);       	  // write register data for pressure
    while(IIC_Wait_Ack());
	IIC_Stop();                    //����ֹͣ�ź�
	delay_ms(20);    	                  // max time is 4.5ms
	
	//pressure = Multiple_read(0xF6);
	//pressure &= 0x0FFFF;
	
	return Multiple_read(0xF6);//pressure;	
}

//**************************************************************

//��ʼ��BMP085��������Ҫ��ο�pdf�����޸�**************
void BMP180_Init(void)
{
	IIC_Init();
	
	ac1 = Multiple_read(0xAA);
	ac2 = Multiple_read(0xAC);
	ac3 = Multiple_read(0xAE);
	ac4 = Multiple_read(0xB0);
	ac5 = Multiple_read(0xB2);
	ac6 = Multiple_read(0xB4);
	b1 =  Multiple_read(0xB6);
	b2 =  Multiple_read(0xB8);
	mb =  Multiple_read(0xBA);
	mc =  Multiple_read(0xBC);
	md =  Multiple_read(0xBE);
}
//***********************************************************************
void bmp180Convert()
{
	unsigned int ut;
	unsigned long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;

	ut = bmp180ReadTemp();	   // ��ȡ�¶�
	up = bmp180ReadPressure();  // ��ȡѹǿ    return pressure;	
	//*************
	x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	result_UT = ((b5 + 8) >> 4);
	//*************		
	b6 = b5 - 4000;
	                         // Calculate B3
	x1 = (b2 * (b6 * b6)>>12)>>11;
	x2 = (ac2 * b6)>>11;
	x3 = x1 + x2;
	b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;	
	                         // Calculate B4
	x1 = (ac3 * b6)>>13;
	x2 = (b1 * ((b6 * b6)>>12))>>16;
	x3 = ((x1 + x2) + 2)>>2;
	b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
	
	b7 = ((unsigned long)(up - b3) * (50000>>OSS));
	if (b7 < 0x80000000)
	p = (b7<<1)/b4;
	else
	p = (b7/b4)<<1;
	
	x1 = (p>>8) * (p>>8);
	x1 = (x1 * 3038)>>16;
	x2 = (-7357 * p)>>16;
	result_UP = p+((x1 + x2 + 3791)>>4);
}

/****************************************************************
�������������ƣ�BMP085_Get_Altitude			    
�������������ܣ���ȡ���θ߶�ֵ
��������ڲ�������
���������ڲ�����altitude //int��  2�ֽڣ���ǰ���θ߶�ֵ
�������� ע��	 ���صĸ߶�ֵ��λΪ���ף�����ʱ�ٻ���ɴ�С��������Ϊ��λ�ĸ߶�ֵ
*****************************************************************/
double BMP085_Get_Altitude(float pressure)//��ȡ��ѹֵ
{
	double altitude;				
	altitude=44330.0*(1-pow((double)pressure/101325,1/5.255));  	//����оƬ�ֲ��ṩ�Ĺ�ʽ���㺣�θ߶�
	altitude*=100;	 		//ת�������׵�λ�ĸ߶�ֵ������ʱ�ٻ���ɴ�С���ĸ߶�ֵ����߾���
	return altitude;
}

void BMP180MS_Thread(const void *argument){
	
#if(MOUDLE_DEBUG == 1)
	char disp[30];
#endif	
	
	for(;;){
	
		bmp180Convert();
		result_UA = (float)BMP085_Get_Altitude((float)result_UP)/100;		
#if(MOUDLE_DEBUG == 1)	
		sprintf(disp,"\r\nAtmosphere = %.3fKPa  temperature = %.1f��  Altitude = %.3fm\r\n", result_UP/1000,result_UT/10,result_UA);
		Driver_USART1.Send(disp,strlen(disp));
#endif		
		osDelay(1000);
	}
}

void BMP180MS(void){

	tid_BMP180MS_Thread = osThreadCreate(osThread(BMP180MS_Thread),NULL);
}