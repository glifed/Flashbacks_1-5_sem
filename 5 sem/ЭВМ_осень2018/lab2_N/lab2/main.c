#include <LPC23xx.H> /* ???????? LPC24xx */

#define STB 26 //Port1.26
#define CLK 27 //Port1.27
#define DIO 28 //Port1.28


void delay(void) 
{
	unsigned int i;
	for (i=0;i<0xfffff;i++){}
}

void tm1638_sendbyte(unsigned int x) 
{
	unsigned int i;
	IODIR1 |= (1<<DIO);//????????????? ??? DIO ?? ?????
	for(i = 0; i < 8; i++)
	{
		IOCLR1=(1<<CLK);
		delay();
		if (x&1) {IOSET1=(1<<DIO);}

	else {IOCLR1=(1<<DIO);}
	delay();
	x >>= 1;
	IOSET1=(1<<CLK);
	delay();
	}
}

void tm1638_sendcmd(unsigned int x)
{
	//????????????? ????????? ??????? ??????? ??????? STB
	IOSET1=(1<<STB);
	//????????????? ???? CLK,DIO,STB ?? ?????
	IODIR1 = (1<<CLK)|(1<<DIO)|(1<<STB);
	//????????????? ???????? ?????? ??????? ??????? STB
	IOCLR1=(1<<STB);
	tm1638_sendbyte(x);
}

unsigned int tm1638_receivebyte()
{
	unsigned int i;
	unsigned int x=0;
	IODIR1 &= ~(1<<DIO);//????????????? ??? DIO ?? ????
	for(i = 0; i < 32; i++)
	{
		IOCLR1=(1<<CLK);//?????? CLK ????????????? ? 0
		delay();//???????? 
		if (IOPIN1&(1<<DIO))
		{
			x |= (1<<i);
		}
		delay();//????????
		IOSET1=(1<<CLK);//?????? CLK ????????????? ? 1
		delay();			
	}
	return x;
}



void tm1638_setadr(unsigned int adr)
{
	//?????????? ????? ???????? LED ??????????
	tm1638_sendcmd(0xC0|adr);	
}

void tm1638_init()
{
	unsigned int i;
	//????????? ?????? ?????????
	tm1638_sendcmd(0x88);	
	//?????????? ????? ?????????: ?????????????
	tm1638_sendcmd(0x40);
	//?????????? ????? ???????? LED ??????????
	tm1638_setadr(0);
	//???????? ??? 
	for (i=0;i<=0xf;i++)
		tm1638_sendbyte(0);
	//?????????? ????? ?????????: ?????????????
	tm1638_sendcmd(0x44);
}



int main(void)
{
	int i;
	int iprev = 0;
	tm1638_init();
	while (1)
	{
		i = 0;
		tm1638_sendcmd(0x46); 
		i = tm1638_receivebyte();
		// button presed?
		// 1 - pressed 0 - free
		

		if ( (i & 1) ^ iprev )
		{
			if (i & 1) // 1 on 0 off
			{
				// fire led1 gaz - led2 nass led3
				tm1638_setadr(1); //????????????? ?????
				tm1638_sendbyte(1); //???? ??????
				
				tm1638_setadr(3); //????????????? ?????
				tm1638_sendbyte(1); //???? ??????
				delay();
				tm1638_setadr(3); //????????????? ?????
				tm1638_sendbyte(0); //???? ??????
				
				tm1638_setadr(5); //????????????? ?????
				tm1638_sendbyte(1); //???? ??????
			}
			else
			{
				tm1638_setadr(1); //????????????? ?????
				tm1638_sendbyte(0); //???? ??????
				
				tm1638_setadr(5); //????????????? ?????
				tm1638_sendbyte(0); //???? ??????
			}
			iprev = i & 1;
		}
		
		delay();
		
	}
	

}
