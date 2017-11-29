#include "LDchip.h"
#include "Reg_RW.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "Driver_USART.h"
#include "delay.h"

/*************�˿���Ϣ********************
 * ����˵��
 * RST      PB6
 * CS   		PB8
 * WR/SPIS  PB9
 * P2/SDCK  PB3
 * P1/SDO   PB4
 * P0/SDI   PB5
 * IRQ      PC1
 * A0			PB7
 * RD       PA0
*****************************************/

osThreadId tid_PhoneticsMS_Thread;
osThreadDef(PhoneticsMS_Thread,osPriorityNormal,1,4096);

/************************************************************************************
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
*********************************************************************************/
uint8_t phoneticsNUM;

uint8 nAsrStatus=0;

extern ARM_DRIVER_USART Driver_USART1;								//�豸�����⴮��һ�豸����

void LD3320_Init(void);

uint8 RunASR(void);
void ProcessInt0(void);
void LD3320_EXTI_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_GPIO_Cfg(void);
void LED_gpio_cfg(void);

/***********************************************************
* ��    �ƣ� LD3320_main(void)
* ��    �ܣ� ������LD3320�������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/

void LD3320_main(void)
{
    uint8 nAsrRes=0;
//    PrintCom(USART1," ����������....\r\n"); /*text.....*/
//    PrintCom(USART1,"���1����ˮ��\r\n"); /*text.....*/
//    PrintCom(USART1,"2����˸\r\n"); /*text.....*/
//    PrintCom(USART1,"3����������\r\n"); /*text.....*/
//    PrintCom(USART1,"4��ȫ��\r\n"); /*text.....*/
	
	 uint8_t cntdn_FLG = 0;
	 uint32_t val_cnt = 0;
	
	 nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR

    while(1)
    {
        switch(nAsrStatus)
        {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;
        case LD_ASR_NONE:
            nAsrStatus=LD_ASR_RUNING;
            if (RunASR()==0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR���ӹؼ��������ASR����
            {
                nAsrStatus = LD_ASR_ERROR;
            }
            break;

        case LD_ASR_FOUNDOK:
            nAsrRes = LD_GetResult( );	//	һ��ASRʶ�����̽�����ȥȡASRʶ����
//            PrintCom(USART1,"\r\nʶ����:");			 /*text.....*/
//            USART_SendData(USART1,nAsrRes+0x30); /*text.....*/

            switch(nAsrRes)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
            {
            case CODE_ZMKM:			
//                PrintCom(USART1,"����ˮ�ơ�����ʶ��ɹ�\r\n"); /*text.....*/
#if(MOUDLE_DEBUG == 1)	
						Driver_USART1.Send("cmd1 ֥�鿪�� identify\r\n",13);
#endif
						phoneticsNUM = 1;
						cntdn_FLG = 1;val_cnt = 0;
                break;
            case CODE_ZMGM:	
//                PrintCom(USART1,"����˸������ʶ��ɹ�\r\n"); /*text.....*/
#if(MOUDLE_DEBUG == 1)	
						Driver_USART1.Send("cmd2 ֥����� identify\r\n",13);
#endif
						phoneticsNUM = 2;
						cntdn_FLG = 1;val_cnt = 0;
                break;
            case CODE_PGKM:		

//                PrintCom(USART1,"����������������ʶ��ɹ�\r\n"); /*text.....*/
#if(MOUDLE_DEBUG == 1)	
						Driver_USART1.Send("cmd3 ƻ������ identify\r\n",13);
#endif
						phoneticsNUM = 3;
						cntdn_FLG = 1;val_cnt = 0;
                break;
            case CODE_PGGM:		

//                PrintCom(USART1,"��ȫ������ʶ��ɹ�\r\n"); /*text.....*/
#if(MOUDLE_DEBUG == 1)	
						Driver_USART1.Send("cmd4 ƻ������ identify\r\n",13);
#endif
						phoneticsNUM = 4;
						cntdn_FLG = 1;val_cnt = 0;
                break;
            case CODE_1234:		

//                PrintCom(USART1,"��ȫ������ʶ��ɹ�\r\n"); /*text.....*/
#if(MOUDLE_DEBUG == 1)	
						Driver_USART1.Send("cmd5 һ������ identify\r\n",13);
#endif
						phoneticsNUM = 5;
						cntdn_FLG = 1;val_cnt = 0;
                break;
            case CODE_2234:		

//                PrintCom(USART1,"��ȫ������ʶ��ɹ�\r\n"); /*text.....*/
#if(MOUDLE_DEBUG == 1)	
						Driver_USART1.Send("cmd6 �������� identify\r\n",13);
#endif
						phoneticsNUM = 6;
						cntdn_FLG = 1;val_cnt = 0;
                break;
            case CODE_3234:		

//                PrintCom(USART1,"��ȫ������ʶ��ɹ�\r\n"); /*text.....*/
#if(MOUDLE_DEBUG == 1)	
						Driver_USART1.Send("cmd7 �������� identify\r\n",13);
#endif
						phoneticsNUM = 7;
						cntdn_FLG = 1;val_cnt = 0;
                break;
            default:
                break;
            }
            nAsrStatus = LD_ASR_NONE;
            break;

        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
        }//switch
        //�����������ʱ���֣��û���ɾ����
        //Board_text(nAsrRes );	/*text....................*/
		  
		  if(cntdn_FLG){
		  
				val_cnt++;
				if(val_cnt >= 10000000){
				
					cntdn_FLG = val_cnt = 0;
					phoneticsNUM = 0;
				}
		  }
    }// while

}
/***********************************************************
* ��    �ƣ�LD3320_Init(void)
* ��    �ܣ�ģ�������˿ڳ�ʼ����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_Init(void)
{
    LD3320_GPIO_Cfg();
    LD3320_EXTI_Cfg();
    LD3320_Spi_cfg();
    //LED_gpio_cfg();

    LD_reset();
}

/***********************************************************
* ��    �ƣ� void Delay_( int i)
* ��    �ܣ� ����ʱ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void Delay_( int i)
{
    while( i--)
    {

    }
}
/***********************************************************
* ��    �ƣ�	LD3320_delay(unsigned long uldata)
* ��    �ܣ�	����ʱ����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void  LD3320_delay(unsigned long uldata)
{
    unsigned int j  =  0;
    unsigned int g  =  0;
    for (j=0; j<5; j++)
    {
        for (g=0; g<uldata; g++)
        {
            Delay_(120);
        }
    }
}

/***********************************************************
* ��    �ƣ�	RunASR(void)
* ��    �ܣ�	����ASR
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
uint8 RunASR(void)
{
    uint8 i=0;
    uint8 asrflag=0;
    for (i=0; i<5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
    {
        LD_AsrStart();			//��ʼ��ASR
        LD3320_delay(100);
        if (LD_AsrAddFixed()==0)	//���ӹؼ����ﵽLD3320оƬ��
        {
            LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
            continue;
        }

        LD3320_delay(10);

        if (LD_AsrRun() == 0)
        {
            LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
            continue;
        }

        asrflag=1;
        break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
    }

    return asrflag;
}

/***********************************************************
* ��    �ƣ�LD3320_GPIO_Cfg(void)
* ��    �ܣ���ʼ����Ҫ�õ���IO��
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_GPIO_Cfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // ����PA8 ���	 8M ����
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

        /*    MCO    configure */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        RCC_MCOConfig( RCC_MCO_HSE);		//8M
    }
    //io������
    {
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC,ENABLE);
        //LD_CS	/RSET
//		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;//;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOB,&GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;//;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOC,&GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;//;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
//		//LD_RD
//		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
        //GPIO_SetBits(GPIOB,GPIO_Pin_7);	/*A0Ĭ������*/
//	GPIO_SetBits(GPIOA,GPIO_Pin_0);	/*RDĬ������*/
    }
}
/***********************************************************
* ��    �ƣ�LD3320_Spi_cfg(void)
* ��    �ܣ�����SPI���ܺͶ˿ڳ�ʼ��
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_Spi_cfg(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //spi�˿�����
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC,ENABLE);	   //ʹ��SPI3����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC,ENABLE);	   //ʹ��SPI3����ʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    //P0/P1/P2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			//spis Ƭѡ	WR
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    LD_CS_H();

    //spi��������
    SPI_Cmd(SPI1, DISABLE);
    /* SPI3 ���� */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //��������NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //�����ʿ��� SYSCLK/128
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7
    SPI_Init(SPI1, &SPI_InitStructure);
    /* ʹ��SPI3 */
    SPI_Cmd(SPI1, ENABLE);

}
/***********************************************************
* ��    �ƣ� LD3320_EXTI_Cfg(void)
* ��    �ܣ� �ⲿ�жϹ������ú���ض˿�����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_EXTI_Cfg(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //�ж���������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //�ⲿ�ж�������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EXTI_Line12);

    GPIO_SetBits(GPIOC,GPIO_Pin_12);	 //Ĭ�������ж�����

    EXTI_ClearFlag(EXTI_Line12);
    EXTI_ClearITPendingBit(EXTI_Line12);
    //�ж�Ƕ������
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* ��    �ƣ�  EXTI  _IRQHandler(void)
* ��    �ܣ� �ⲿ�жϺ���
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12)!= RESET )
    {
        ProcessInt0();
//        PrintCom(USART1,"�����ж�\r\n");	/*text........................*/
        EXTI_ClearFlag(EXTI_Line12);
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}
/***********************************************************
* ��    �ƣ�void LED_gpio_cfg(void)
* ��    �ܣ�LED�˿�����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LED_gpio_cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
}

void Phonetics_Init(void){

	LD3320_Init();
}

void PhoneticsMS_Thread(const void *argument){

	LD3320_main();
}

void Phonetics(void){

	tid_PhoneticsMS_Thread =  osThreadCreate(osThread(PhoneticsMS_Thread),NULL);
}
