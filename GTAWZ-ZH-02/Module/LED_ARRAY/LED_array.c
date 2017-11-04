#include "LED_Array.h"

uint8_t FLG_CLO = 0;

osThreadId tid_LEDArrayTest_Thread;

osThreadDef(LEDArrayTest_Thread,osPriorityNormal,1,256);

extern ARM_DRIVER_USART Driver_USART1;

unsigned char DispBuffer[8][16]={

	0
};

const unsigned char disp_liba[8][16] = {

	{0x00,0x00,0x7F,0xFC,0x40,0x04,0x40,0x04,0x5F,0xF4,0x41,0x04,0x41,0x04,0x4F,0xE4},
	{0x41,0x04,0x41,0x44,0x41,0x24,0x5F,0xF4,0x40,0x04,0x40,0x04,0x7F,0xFC,0x40,0x04},/*"?",0*/

	{0x01,0x00,0x01,0x00,0x7F,0xFC,0x01,0x00,0x3F,0xF8,0x02,0x00,0xFF,0xFE,0x04,0x40},
	{0x09,0x20,0x31,0x18,0xCB,0x26,0x05,0xC0,0x19,0x30,0x61,0x08,0x05,0x00,0x02,0x00},/*"?",1*/

	{0x00,0x00,0x3F,0xF8,0x20,0x08,0x20,0x08,0x20,0x08,0x3F,0xF8,0x20,0x80,0x20,0x80},
	{0x3F,0xFC,0x20,0x80,0x20,0x40,0x20,0x40,0x24,0x24,0x28,0x14,0x30,0x0C,0x20,0x04},/*"?",2*/

	{0x02,0x00,0x01,0x00,0x3F,0xFC,0x20,0x04,0x42,0x08,0x02,0x00,0x02,0x00,0xFF,0xFE},
	{0x04,0x20,0x08,0x20,0x18,0x40,0x06,0x40,0x01,0x80,0x02,0x60,0x0C,0x10,0x70,0x08},/*"?",3*/
};

const unsigned char disp_libb[46][16] = {

	{0x10,0x00,0x13,0xF8,0x12,0x08,0x13,0xF8,0xFE,0x08,0x13,0xF8,0x11,0x00,0x13,0xFC},
	{0x1C,0x44,0x32,0x44,0xD2,0xA4,0x12,0x04,0x13,0xF4,0x10,0x04,0x50,0x28,0x20,0x10},/*"?",0*/

	{0x00,0x40,0x20,0x20,0x11,0xFC,0x10,0x00,0x00,0x88,0x00,0x50,0xF3,0xFE,0x12,0x22},
	{0x14,0x24,0x11,0xFC,0x11,0x24,0x11,0x24,0x15,0x34,0x19,0x28,0x10,0x20,0x00,0x20},/*"?",1*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00},/*",",2*/

	{0x10,0x00,0x13,0xF8,0x12,0x08,0x13,0xF8,0xFE,0x08,0x13,0xF8,0x11,0x00,0x13,0xFC},
	{0x1C,0x44,0x32,0x44,0xD2,0xA4,0x12,0x04,0x13,0xF4,0x10,0x04,0x50,0x28,0x20,0x10},/*"?",3*/

	{0x00,0x40,0x20,0x20,0x11,0xFC,0x10,0x00,0x00,0x88,0x00,0x50,0xF3,0xFE,0x12,0x22},
	{0x14,0x24,0x11,0xFC,0x11,0x24,0x11,0x24,0x15,0x34,0x19,0x28,0x10,0x20,0x00,0x20},/*"?",4*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00},/*",",5*/

	{0x00,0x20,0x20,0x20,0x10,0x20,0x13,0xFE,0x82,0x22,0x42,0x24,0x4A,0x20,0x0B,0xFC},
	{0x12,0x84,0x12,0x88,0xE2,0x48,0x22,0x50,0x22,0x20,0x24,0x50,0x24,0x88,0x09,0x06},/*"?",6*/

	{0x00,0x00,0x3F,0xF8,0x24,0x48,0x24,0x48,0x24,0x48,0x3F,0xF8,0x02,0x00,0x04,0x00},
	{0x0F,0xF0,0x10,0x10,0x68,0x20,0x04,0x40,0x02,0x80,0x03,0x00,0x1C,0x00,0xE0,0x00},/*"?",7*/

	{0x10,0x00,0x13,0xF8,0x12,0x08,0x13,0xF8,0xFE,0x08,0x13,0xF8,0x11,0x00,0x13,0xFC},
	{0x1C,0x44,0x32,0x44,0xD2,0xA4,0x12,0x04,0x13,0xF4,0x10,0x04,0x50,0x28,0x20,0x10},/*"?",8*/

	{0x00,0x40,0x20,0x20,0x11,0xFC,0x10,0x00,0x00,0x88,0x00,0x50,0xF3,0xFE,0x12,0x22},
	{0x14,0x24,0x11,0xFC,0x11,0x24,0x11,0x24,0x15,0x34,0x19,0x28,0x10,0x20,0x00,0x20},/*"?",9*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00},/*",",10*/

	{0x00,0x20,0x20,0x20,0x10,0x20,0x13,0xFE,0x82,0x22,0x42,0x24,0x4A,0x20,0x0B,0xFC},
	{0x12,0x84,0x12,0x88,0xE2,0x48,0x22,0x50,0x22,0x20,0x24,0x50,0x24,0x88,0x09,0x06},/*"?",11*/

	{0x00,0x00,0x3F,0xF8,0x24,0x48,0x24,0x48,0x24,0x48,0x3F,0xF8,0x02,0x00,0x04,0x00},
	{0x0F,0xF0,0x10,0x10,0x68,0x20,0x04,0x40,0x02,0x80,0x03,0x00,0x1C,0x00,0xE0,0x00},/*"?",12*/

	{0x09,0x04,0x08,0x88,0x08,0x00,0x13,0xFE,0x12,0x22,0x32,0xAA,0x32,0x72,0x52,0x22},
	{0x93,0xFE,0x10,0x00,0x11,0xFC,0x11,0x04,0x11,0xFC,0x11,0x04,0x11,0xFC,0x11,0x04},/*"?",13*/

	{0x10,0x00,0x13,0xF8,0x12,0x08,0x13,0xF8,0xFE,0x08,0x13,0xF8,0x11,0x00,0x13,0xFC},
	{0x1C,0x44,0x32,0x44,0xD2,0xA4,0x12,0x04,0x13,0xF4,0x10,0x04,0x50,0x28,0x20,0x10},/*"?",14*/

	{0x00,0x40,0x20,0x20,0x11,0xFC,0x10,0x00,0x00,0x88,0x00,0x50,0xF3,0xFE,0x12,0x22},
	{0x14,0x24,0x11,0xFC,0x11,0x24,0x11,0x24,0x15,0x34,0x19,0x28,0x10,0x20,0x00,0x20},/*"?",15*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00},/*",",16*/

	{0x08,0x20,0x08,0x20,0xFF,0xFE,0x08,0x20,0x01,0x00,0x3F,0xF8,0x08,0x20,0x04,0x40},
	{0xFF,0xFE,0x00,0x00,0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10},/*"?",17*/

	{0x10,0x00,0x11,0xFC,0x11,0x04,0x11,0x04,0xFD,0xFC,0x11,0x04,0x11,0x04,0x15,0xFC},
	{0x18,0x00,0x33,0xFE,0xD0,0x20,0x11,0x20,0x11,0x3C,0x11,0x20,0x52,0xA0,0x24,0x7E},/*"?",18*/

	{0x04,0x40,0x04,0x40,0xFF,0xFE,0x04,0x40,0x00,0x20,0x7D,0xFC,0x44,0x88,0x48,0x50},
	{0x51,0xFE,0x49,0x00,0x45,0x00,0x45,0x00,0x55,0x00,0x49,0x00,0x42,0x00,0x44,0x00},/*"?",19*/

	{0x20,0x20,0x13,0xFE,0x92,0x22,0x42,0x20,0x4B,0xFC,0x12,0x84,0xE4,0x48,0x24,0x30},
	{0x29,0xCE,0x04,0x00,0xFF,0xFE,0x08,0x20,0x1C,0x40,0x03,0x80,0x0C,0x70,0x70,0x08},/*"?",20*/

	{0x00,0x00,0x20,0x00,0x17,0xFE,0x10,0x08,0x00,0x08,0x03,0xC8,0xF2,0x48,0x12,0x48},
	{0x12,0x48,0x12,0x48,0x13,0xC8,0x12,0x48,0x14,0x08,0x18,0x08,0x10,0x28,0x00,0x10},/*"?",21*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x18,0x00,0x24,0x00,0x24,0x00,0x18,0x00,0x00,0x00,0x00,0x00},/*"?",22*/
};

const unsigned char ASCII[128][16] = {
	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",1*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"",2*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",3*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",4*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",5*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",6*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",7*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",8*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"	",9*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",10*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",11*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",12*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x1E,0x3F,0x7F,0x6D,0x4C,0x0C,0x0C,0x00,0x00},/*"",13*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",14*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",15*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",16*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",17*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",18*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",19*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",20*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",21*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",22*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",23*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",24*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",25*/
	
	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"SUB",26*/

	{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00},/*"",27*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"",28*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"",29*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"",30*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"",31*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",32*/

	{0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x10,0x10,0x00,0x00},/*"!",33*/

	{0x00,0x12,0x24,0x24,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",34*/

	{0x00,0x00,0x00,0x12,0x12,0x12,0x7E,0x24,0x24,0x24,0x7E,0x24,0x24,0x24,0x00,0x00},/*"#",35*/

	{0x00,0x00,0x08,0x3C,0x4A,0x4A,0x48,0x38,0x0C,0x0A,0x0A,0x4A,0x4A,0x3C,0x08,0x08},/*"$",36*/

	{0x00,0x00,0x00,0x44,0xA4,0xA8,0xA8,0xB0,0x54,0x1A,0x2A,0x2A,0x4A,0x44,0x00,0x00},/*"%",37*/

	{0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x50,0x6E,0xA4,0x94,0x98,0x89,0x76,0x00,0x00},/*"&",38*/

	{0x00,0x60,0x20,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",39*/

	{0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00},/*"(",40*/

	{0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00},/*")",41*/

	{0x00,0x00,0x00,0x00,0x10,0x10,0xD6,0x38,0x38,0xD6,0x10,0x10,0x00,0x00,0x00,0x00},/*"*",42*/

	{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x7F,0x08,0x08,0x08,0x00,0x00,0x00,0x00},/*"+",43*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x20,0x20,0x40},/*",",44*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",45*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00},/*".",46*/

	{0x00,0x00,0x02,0x04,0x04,0x04,0x08,0x08,0x10,0x10,0x10,0x20,0x20,0x40,0x40,0x00},/*"/",47*/

	{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",48*/

	{0x00,0x00,0x00,0x08,0x38,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"1",49*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x02,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00},/*"2",50*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x02,0x04,0x18,0x04,0x02,0x42,0x42,0x3C,0x00,0x00},/*"3",51*/

	{0x00,0x00,0x00,0x04,0x0C,0x0C,0x14,0x24,0x24,0x44,0x7F,0x04,0x04,0x1F,0x00,0x00},/*"4",52*/

	{0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x78,0x44,0x02,0x02,0x42,0x44,0x38,0x00,0x00},/*"5",53*/

	{0x00,0x00,0x00,0x18,0x24,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x22,0x1C,0x00,0x00},/*"6",54*/

	{0x00,0x00,0x00,0x7E,0x42,0x04,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x00,0x00},/*"7",55*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",56*/

	{0x00,0x00,0x00,0x38,0x44,0x42,0x42,0x42,0x46,0x3A,0x02,0x02,0x24,0x18,0x00,0x00},/*"9",57*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00},/*":",58*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10},/*";",59*/

	{0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00},/*"<",60*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00},/*"=",61*/

	{0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00},/*">",62*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x62,0x04,0x08,0x08,0x08,0x00,0x18,0x18,0x00,0x00},/*"?",63*/

	{0x00,0x00,0x00,0x38,0x44,0x5A,0xAA,0xAA,0xAA,0xAA,0xAA,0x5C,0x42,0x3C,0x00,0x00},/*"@",64*/

	{0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,0x24,0x3C,0x44,0x42,0x42,0xE7,0x00,0x00},/*"A",65*/

	{0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},/*"B",66*/

	{0x00,0x00,0x00,0x3E,0x42,0x42,0x80,0x80,0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00},/*"C",67*/

	{0x00,0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},/*"D",68*/

	{0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x42,0x42,0xFC,0x00,0x00},/*"E",69*/

	{0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x40,0x40,0xE0,0x00,0x00},/*"F",70*/

	{0x00,0x00,0x00,0x3C,0x44,0x44,0x80,0x80,0x80,0x8E,0x84,0x44,0x44,0x38,0x00,0x00},/*"G",71*/

	{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"H",72*/

	{0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"I",73*/

	{0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xF0},/*"J",74*/

	{0x00,0x00,0x00,0xEE,0x44,0x48,0x50,0x70,0x50,0x48,0x48,0x44,0x44,0xEE,0x00,0x00},/*"K",75*/

	{0x00,0x00,0x00,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0xFE,0x00,0x00},/*"L",76*/

	{0x00,0x00,0x00,0xEE,0x6C,0x6C,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x54,0xD6,0x00,0x00},/*"M",77*/

	{0x00,0x00,0x00,0xC7,0x62,0x62,0x52,0x52,0x4A,0x4A,0x4A,0x46,0x46,0xE2,0x00,0x00},/*"N",78*/

	{0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},/*"O",79*/

	{0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x42,0x7C,0x40,0x40,0x40,0x40,0xE0,0x00,0x00},/*"P",80*/

	{0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0xB2,0x4C,0x38,0x06,0x00},/*"Q",81*/

	{0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x7C,0x48,0x48,0x44,0x44,0x42,0xE3,0x00,0x00},/*"R",82*/

	{0x00,0x00,0x00,0x3E,0x42,0x42,0x40,0x20,0x18,0x04,0x02,0x42,0x42,0x7C,0x00,0x00},/*"S",83*/

	{0x00,0x00,0x00,0xFE,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00},/*"T",84*/

	{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"U",85*/

	{0x00,0x00,0x00,0xE7,0x42,0x42,0x44,0x24,0x24,0x28,0x28,0x18,0x10,0x10,0x00,0x00},/*"V",86*/

	{0x00,0x00,0x00,0xD6,0x54,0x54,0x54,0x54,0x54,0x6C,0x28,0x28,0x28,0x28,0x00,0x00},/*"W",87*/

	{0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00},/*"X",88*/

	{0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00},/*"Y",89*/

	{0x00,0x00,0x00,0x7E,0x84,0x04,0x08,0x08,0x10,0x20,0x20,0x42,0x42,0xFC,0x00,0x00},/*"Z",90*/

	{0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00},/*"[",91*/

	{0x00,0x00,0x40,0x20,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x04,0x04,0x04,0x02,0x02},/*"\",92*/

	{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00},/*"]",93*/

	{0x00,0x18,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",94*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},/*"_",95*/

	{0x00,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",96*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x0C,0x34,0x44,0x4C,0x36,0x00,0x00},/*"a",97*/

	{0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x00,0x00},/*"b",98*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x40,0x40,0x40,0x22,0x1C,0x00,0x00},/*"c",99*/

	{0x00,0x00,0x00,0x00,0x06,0x02,0x02,0x3E,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00},/*"d",100*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x7E,0x40,0x42,0x3C,0x00,0x00},/*"e",101*/

	{0x00,0x00,0x00,0x00,0x0C,0x12,0x10,0x7C,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"f",102*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x44,0x44,0x38,0x40,0x3C,0x42,0x42,0x3C},/*"g",103*/

	{0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"h",104*/

	{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"i",105*/

	{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x78},/*"j",106*/

	{0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0x4E,0x48,0x50,0x70,0x48,0x44,0xEE,0x00,0x00},/*"k",107*/

	{0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"l",108*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x49,0x49,0x49,0x49,0x49,0xED,0x00,0x00},/*"m",109*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"n",110*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"o",111*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x64,0x42,0x42,0x42,0x64,0x58,0x40,0xE0},/*"p",112*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1A,0x26,0x42,0x42,0x42,0x26,0x1A,0x02,0x07},/*"q",113*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00},/*"r",114*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x42,0x40,0x3C,0x02,0x42,0x7C,0x00,0x00},/*"s",115*/

	{0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x10,0x10,0x12,0x0C,0x00,0x00},/*"t",116*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00},/*"u",117*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x00,0x00},/*"v",118*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDB,0x89,0x4A,0x5A,0x54,0x24,0x24,0x00,0x00},/*"w",119*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x24,0x18,0x18,0x18,0x24,0x6E,0x00,0x00},/*"x",120*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x10,0x10,0x60},/*"y",121*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x08,0x10,0x10,0x22,0x7E,0x00,0x00},/*"z",122*/

	{0x00,0x03,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x03,0x00},/*"{",123*/

	{0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08},/*"|",124*/

	{0x00,0xC0,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x20,0x20,0x20,0xC0,0x00},/*"}",125*/

	{0x20,0x5A,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",126*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}/*"",127*/
};



void DIO1_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13;				 //DI INPUT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.
}

void DIO2_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_13;//
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //DI INPUT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;				 //DI INPUT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.
	
	
	//GPIO_Write(GPIOB,0XFF00);
// GPIO_ResetBits(GPIOB,GPIO_Pin_7);						
// GPIO_ResetBits(GPIOB,GPIO_Pin_8);		
	
}

void RELAY_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;				 //
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;				 //DI INPUT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.

	//GPIO_Write(GPIOB,0XFF00);
 GPIO_ResetBits(GPIOB,GPIO_Pin_7);						
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);		
	
}

//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_12;	 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 //GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //输出高
 //GPIO_SetBits(GPIOA,GPIO_Pin_12);						 //输出高

 GPIO_ResetBits(GPIOA,GPIO_Pin_8);						
 GPIO_ResetBits(GPIOA,GPIO_Pin_12);	
	
}
 
void LEDArray_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.
	
//	EN=1;
//	EN1=1;
//	EN2=1;

}

//OE RCLK SRCLK 前端3.3V   -- 5V,    信号反向 
void SH_595(u8 Date_in)  
{  
 u8 j;  
 for (j=0;j<8;j++)  
 {  
	SCLK=1;//上升沿发生移位
	delay_us(1);
	if ((Date_in&0x01)==0x01)
		DS=0;
	else
		DS=1;	 			 
	SCLK=0; 
	delay_us(1);
	Date_in=Date_in>>1;	
 }  
 
} 

void SH_595R(u8 Date_in)  
{  
 u8 j;  
 for (j=0;j<8;j++)  
 {  
	SCLK1=1;//上升沿发生移位
	delay_us(1); 
	if ((Date_in&0x01)==0x01)
		DS1=0;
	else
		DS1=1;	 			 
	SCLK1=0; 
	delay_us(1);
	Date_in=Date_in>>1;	
 }  
 
} 

void SH_595G(u8 Date_in)  
{  
 u8 j;  
 for (j=0;j<8;j++)  
 {  
	SCLK2=1;//上升沿发生移位
	delay_us(1); 	 
	if ((Date_in&0x01)==0x01)
		DS2=0;
	else
		DS2=1;	 			 
	SCLK2=0; 
	delay_us(1); 	
	Date_in=Date_in>>1;	
 }  
 
} 

void OUT_595(void)//移位寄存器内的数据锁存到输出寄存器并显示
{  
  LCLK=1;
	delay_us(1);	
  LCLK=0;//上升沿将数据送到输出锁存器            
	delay_us(1);
  LCLK=1;  
}

void OUT_595R(void)//移位寄存器内的数据锁存到输出寄存器并显示
{  
  LCLK1=1;     
  delay_us(1);
  LCLK1=0;//上升沿将数据送到输出锁存器     
	delay_us(1);
  LCLK1=1;  
}
void OUT_595G(void)//移位寄存器内的数据锁存到输出寄存器并显示
{  
  LCLK2=1;
  delay_us(1);	
  LCLK2=0;//上升沿将数据送到输出锁存器            
  delay_us(1);	
  LCLK2=1;  
}

void	D_DATA(void)
{

  	ROW_SEL(16);
		SH_595R(0x80); 
		SH_595R(0x40); 
		SH_595R(0x20); 
		SH_595R(0x10); 
		SH_595R(0x08); 
		SH_595R(0x04); 
		SH_595R(0x02); 
		SH_595R(0x01); 
		OUT_595R();
		//delay_ms(10);


}	



unsigned char Shift_Data(u8 dat)
{
    u8 i;  
    u8 tmp=0x00;  
  
    for(i=0;i<8;i++)  
    {  
        tmp=((dat>>i)&0x01)|tmp;  
        if(i<7)  
					tmp=tmp<<1;  
    }  	
		return tmp;
		
}	

void SH16_595(u16 Date_in)  
{  
 u8 j;  
 for (j=0;j<16;j++)  
 {  
	SCLK=1;//上升沿发生移位
	delay_us(1);
	if ((Date_in&0x0001)==0x0001)
		DS=0;
	else
		DS=1;	 			 
	SCLK=0; 
	delay_us(1);
	Date_in=Date_in>>1;	
 }  
 
} 

//1-16行 ，  上下8行 反了
void ROW_SEL(u8 dat)
{
	if((dat<=8)&&(dat>=1))
	{
		
		SH_595(~Shift_Data(1<<(dat-1))); 
		SH_595(Shift_Data(0xFF)); 
		OUT_595();
		}	
	else if((dat<=16)&&(dat>=9))
	{
		SH_595(Shift_Data(0xFF)); 
		SH_595(~Shift_Data(1<<(dat-9))); 		
		OUT_595();	
		}	

}	

void ROW_OFF(void)
{
		SH16_595(0XFFFF); 
		OUT_595();
}	

void DISPLAY_ASC(unsigned char gImage_dat[8][16],char color)
{
	u8 i=0,j=0,k=0;
	EN=1;

	for(i=0;i<2;i++)
		for(j=0;j<8;j++){
			ROW_OFF();	
			
			switch(color){
			
				case 'R':
								EN1=1;
								EN2=0;
								for(k=0;k<8;k++)SH_595R((gImage_dat[7-k][j + i * 8]));
								OUT_595R();					
								break;
				case 'G':
								EN1=0;
								EN2=1;
								for(k=0;k<8;k++)SH_595G((gImage_dat[7-k][j + i * 8]));
								OUT_595G();					
								break;
				case 'X':
								EN1=1;
								EN2=1;
								for(k=0;k<8;k++)SH_595R((gImage_dat[7-k][j + i * 8]));		
								for(k=0;k<8;k++)SH_595G((gImage_dat[7-k][j + i * 8]));
								OUT_595R();	
								OUT_595G();					
								break;
				default :
								for(k=0;k<8;k++)SH_595R((gImage_dat[7-k][j + i * 8]));
								OUT_595R();					
								break;
			}

			ROW_SEL(j+1+8*i);
			delay_us(20);		
		}
}

void DISPLAY_R(unsigned char gImage_dat[8][16])
{
	u8 i=0,j=0;
	EN=1;
	EN1=1;
	EN2=0;
  for(i=0;i<2;i++)
	{
		for(j=0;j<8;j++)
		{
		ROW_OFF();
		SH_595R((gImage_dat[i+6][1+j*2]));	 
		SH_595R((gImage_dat[i+6][0+j*2]));	

		SH_595R((gImage_dat[i+4][1+j*2]));	
		SH_595R((gImage_dat[i+4][0+j*2]));	 	
		
		SH_595R((gImage_dat[i+2][1+j*2]));	
		SH_595R((gImage_dat[i+2][0+j*2]));	
		
		SH_595R((gImage_dat[i+0][1+j*2]));	 
		SH_595R((gImage_dat[i+0][0+j*2]));	

		OUT_595R();		
			
		ROW_SEL(j+1+8*i);

		delay_us(20);		
		}
	}
}

void DISPLAY_G(unsigned char gImage_dat[8][16])
{
	u8 i=0,j=0;
	
	EN=1;
	EN1=0;
	EN2=1;	
	
	for(i=0;i<2;i++)
	{
		for(j=0;j<8;j++)
		{
			ROW_OFF();
			SH_595G((gImage_dat[i+6][1+j*2]));	 
			SH_595G((gImage_dat[i+6][0+j*2]));	

			SH_595G((gImage_dat[i+4][1+j*2]));	
			SH_595G((gImage_dat[i+4][0+j*2]));	 	

			SH_595G((gImage_dat[i+2][1+j*2]));	
			SH_595G((gImage_dat[i+2][0+j*2]));	

			SH_595G((gImage_dat[i+0][1+j*2]));	 
			SH_595G((gImage_dat[i+0][0+j*2]));	

			OUT_595G();					
			ROW_SEL(j+1+8*i);
			delay_us(20);		
		}
	}
}

void DISPLAY_RG(unsigned char gImage_dat[8][16])
{
	u8 i=0,j=0;
	EN=1;
	EN1=1;
	EN2=1;	
  for(i=0;i<2;i++)
	{
		for(j=0;j<8;j++)
		{
		ROW_OFF();

		SH_595R((gImage_dat[i+6][1+j*2]));	 
		SH_595R((gImage_dat[i+6][0+j*2]));	

		SH_595R((gImage_dat[i+4][1+j*2]));	
		SH_595R((gImage_dat[i+4][0+j*2]));	 	
		
		SH_595R((gImage_dat[i+2][1+j*2]));	
		SH_595R((gImage_dat[i+2][0+j*2]));	
		
		SH_595R((gImage_dat[i+0][1+j*2]));	 
		SH_595R((gImage_dat[i+0][0+j*2]));	

			
		SH_595G((gImage_dat[i+6][1+j*2]));	 
		SH_595G((gImage_dat[i+6][0+j*2]));	

		SH_595G((gImage_dat[i+4][1+j*2]));	
		SH_595G((gImage_dat[i+4][0+j*2]));	 	
		
		SH_595G((gImage_dat[i+2][1+j*2]));	
		SH_595G((gImage_dat[i+2][0+j*2]));	
		
		SH_595G((gImage_dat[i+0][1+j*2]));	 
		SH_595G((gImage_dat[i+0][0+j*2]));	

		OUT_595R();	
		OUT_595G();		
			
		ROW_SEL(j+1+8*i);

		delay_us(20);			
		}
	}
}

void DiSP_ASC(char *str,char color,char speed){

	uint8_t datlen = strlen(str);
	uint8_t loop,loopa,loopb,loopc,disp_loop;
	
	unsigned char temp[16];
	
	if(datlen < 9)
		for(loopa = 0;loopa < datlen;loopa ++){
			
			memcpy(DispBuffer[loopa],ASCII[*str++],16);
			for(disp_loop = 0;disp_loop < speed;disp_loop ++)DISPLAY_ASC(DispBuffer,color);
		}else{
		
			for(loop = 0;loop < datlen;loop ++){
			
				memcpy(temp,ASCII[*str++],16);
				for(loopa = 0;loopa < 8;loopa ++){
					
					for(loopb = 0;loopb < 7;loopb ++){
						
						for(loopc = 0;loopc < 16;loopc ++){
							
							DispBuffer[loopb][loopc] *= 2;
							DispBuffer[loopb][loopc] |= DispBuffer[loopb + 1][loopc] >> 7;
						}
					}
					
					for(loopc = 0;loopc < 16;loopc ++){

						DispBuffer[7][loopc] *= 2;
						DispBuffer[7][loopc] |= temp[loopc] >> 7;
						temp[loopc] *= 2;
					}
					for(disp_loop = 0;disp_loop < speed;disp_loop ++)DISPLAY_ASC(DispBuffer,color);
				}			
			}
	}
}

void DISP_HANZI(const unsigned char gImage_dat_dest[][16],uint8_t datlen,uint8_t method,uint8_t speed,char color){

	uint16_t loop,loopa,loopb,loop_disp,num,nnum;		//字显周期，半字显周期，6个半字显周期 + 自定义补尾
	uint8_t gImage_dat_dest_buffer[2][16] = {0};
	
	if(method){
		
		for(num = 0;num < 4;num ++){
		
			memcpy(DispBuffer[2 * num],gImage_dat_dest[2 * num],16);
			memcpy(DispBuffer[2 * num + 1],gImage_dat_dest[2 * num + 1],16);
		}
		nnum = 4;
		switch(color){

			case 'R':	DISPLAY_R(DispBuffer);break;
			case 'G':	DISPLAY_G(DispBuffer);break;
			case 'X':	DISPLAY_RG(DispBuffer);break;
			default :	DISPLAY_R(DispBuffer);break;
		}
	}
	
	for(num = nnum;num < datlen;num ++){
	
		memcpy(gImage_dat_dest_buffer[0],gImage_dat_dest[num * 2],16);			//补尾前半字缓存
		memcpy(gImage_dat_dest_buffer[1],gImage_dat_dest[num * 2 + 1],16);	//补尾后半字缓存
		for(loop = 0;loop < 16;loop ++){					//单周期移位

			for(loopa = 0;loopa < 8;loopa ++){			//半字填充移位
				
				for(loopb = 0;loopb < 6;loopb ++){		//前三字移位
				
					DispBuffer[loopb][2 * loopa] *= 2;
					DispBuffer[loopb][2 * loopa] |= (DispBuffer[loopb][2 * loopa + 1] >> 7);
					
					DispBuffer[loopb][2 * loopa + 1] *= 2;
					DispBuffer[loopb][2 * loopa + 1] |= (DispBuffer[loopb + 2][2 * loopa] >> 7);
				}	

				DispBuffer[6][2 * loopa] *= 2;			//最后一字前半字上半移位
				DispBuffer[6][2 * loopa] |= (DispBuffer[6][2 * loopa + 1] >> 7);		

				DispBuffer[6][2 * loopa + 1] *= 2;	//最后一字后半字上半移位
				DispBuffer[6][2 * loopa + 1] |= (gImage_dat_dest_buffer[0][2 * loopa] >> 7);		

				gImage_dat_dest_buffer[0][2 * loopa] *= 2;	//最后一字后半字上半移位数据缓存处理
				gImage_dat_dest_buffer[0][2 * loopa] |= gImage_dat_dest_buffer[0][2 * loopa + 1] >> 7;	
				gImage_dat_dest_buffer[0][2 * loopa + 1] *= 2;			
				
				DispBuffer[7][2 * loopa] *= 2;			//最后一字后半字上半移位
				DispBuffer[7][2 * loopa] |= (DispBuffer[7][2 * loopa + 1] >> 7);

				DispBuffer[7][2 * loopa + 1] *= 2;	//最后一字后半字下半移位
				DispBuffer[7][2 * loopa + 1] |= (gImage_dat_dest_buffer[1][2 * loopa] >> 7);
				
				gImage_dat_dest_buffer[1][2 * loopa] *= 2;	////最后一字后半字下半移位数据缓存处理
				gImage_dat_dest_buffer[1][2 * loopa] |= gImage_dat_dest_buffer[1][2 * loopa + 1] >> 7;
				gImage_dat_dest_buffer[1][2 * loopa + 1] *= 2;
			}	
			for(loop_disp = 0;loop_disp < speed;loop_disp ++){
			
				switch(color){

					case 'R':	DISPLAY_R(DispBuffer);break;
					case 'G':	DISPLAY_G(DispBuffer);break;
					case 'X':	DISPLAY_RG(DispBuffer);break;
					default :	DISPLAY_R(DispBuffer);break;
				}
			}
		}
	}
}

void LEDArrayTest_Thread(const void *argument){

	while(1){
	
		if(FLG_CLO == 0)DiSP_ASC("Be the chenge you want to see in the world.        ",'X',4);
		else DISPLAY_R(DispBuffer);
	}
}

void LEDArrayTest(void){

	tid_LEDArrayTest_Thread = osThreadCreate(osThread(LEDArrayTest_Thread),NULL);
}
