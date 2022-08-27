
#include <at89c5131.h>
#include "lcd.h"																				//Driver for interfacing lcd 
#include "mcp3008.h"																		//Driver for interfacing ADC ic MCP3008

char adc_ip_data_ascii[6]={0,0,0,'\0'};							//string array for saving ascii of input sample
unsigned char display_msg3[]={0,0,'.',0,'\0'};        //"xxx °C", Display msg on 2nd line of lcd
unsigned char display_msg4[]={0,0,'.',0,'\0'};
unsigned char display_msg5[]={0,0,'.',0,'\0'};
unsigned char display_msgL2[]={0,0,'.',0,' ',223,'C','\0'};
code unsigned char display_msg6[]="AvgT.: ";						//Display msg on 2nd line of lcd

unsigned int var;
float temp[10];
float avg=0.0;
int i,j;
unsigned int time=0;
void print();
void tone();

void main(void)
{
	unsigned int adc_data=0;
	for(i=0;i<10;i++)
	{
		temp[i]=30.0;
	}
	spi_init();																					
	adc_init();
  lcd_init();
	while(i>-1)
	{		
		for (i=0;i<10;i++)
		{
			var = adc(7);
			temp[i]=  (var*0.3229*10.0);
			if(time>9)
			{
				for (j=0;j<8;j++)
				{
					temp[j]=temp[j+1];
				}
				var = adc(7);
				temp[9]=  (var*0.3229*10.0);
			}
			if((time<9) && (time>1))
			{
				int_to_string(temp[i],adc_ip_data_ascii);
				display_msg5[0] = adc_ip_data_ascii[2];
				display_msg5[1] = adc_ip_data_ascii[3];
				display_msg5[3] = adc_ip_data_ascii[4];
				int_to_string(temp[i-1],adc_ip_data_ascii);
				display_msg4[0] = adc_ip_data_ascii[2];
				display_msg4[1] = adc_ip_data_ascii[3];
				display_msg4[3] = adc_ip_data_ascii[4];
				int_to_string(temp[i-2],adc_ip_data_ascii);
				display_msg3[0] = adc_ip_data_ascii[2];
				display_msg3[1] = adc_ip_data_ascii[3];
				display_msg3[3] = adc_ip_data_ascii[4];
			}
			else if(time>9)
			{
				int_to_string(temp[9],adc_ip_data_ascii);
				display_msg5[0] = adc_ip_data_ascii[2];
				display_msg5[1] = adc_ip_data_ascii[3];
				display_msg5[3] = adc_ip_data_ascii[4];
				int_to_string(temp[8],adc_ip_data_ascii);
				display_msg4[0] = adc_ip_data_ascii[2];
				display_msg4[1] = adc_ip_data_ascii[3];
				display_msg4[3] = adc_ip_data_ascii[4];
				int_to_string(temp[7],adc_ip_data_ascii);
				display_msg3[0] = adc_ip_data_ascii[2];
				display_msg3[1] = adc_ip_data_ascii[3];
				display_msg3[3] = adc_ip_data_ascii[4];
			}
			else
			{
				int_to_string(temp[i],adc_ip_data_ascii);
				display_msg5[0] = adc_ip_data_ascii[2];
				display_msg5[1] = adc_ip_data_ascii[3];
				display_msg5[3] = adc_ip_data_ascii[4];
				int_to_string(temp[0],adc_ip_data_ascii);
				display_msg4[0] = adc_ip_data_ascii[2];
				display_msg4[1] = adc_ip_data_ascii[3];
				display_msg4[3] = adc_ip_data_ascii[4];
				int_to_string(temp[0],adc_ip_data_ascii);
				display_msg3[0] = adc_ip_data_ascii[2];
				display_msg3[1] = adc_ip_data_ascii[3];
				display_msg3[3] = adc_ip_data_ascii[4];
			}
			for(j=0;j<10;j++)
			{
				avg+=temp[j]; //Average temperature
			}
			avg/=10;
			int_to_string(avg,adc_ip_data_ascii);
			display_msgL2[0] = adc_ip_data_ascii[2];
			display_msgL2[1] = adc_ip_data_ascii[3];
			display_msgL2[3] = adc_ip_data_ascii[4];
			print();
			/*if((temp[9]>avg+2)||(temp[9]<avg-2))
			{
				tone();
			}*/
			msdelay(1000);
			time++;
		}//end for
		i=0;
	}//end while
}//end method
void tone()
{
	while(1)//Output at P2.0
	{
		P3 |= 0x01;
		msdelay(1);
		P3	 |= 0x00;
		msdelay(1);
		for(i=0;i<4;i++)
		{
			P1_4=1;
			P1_5=1;
			P1_6=1;
			P1_7=1;
			msdelay(500);
			P1_4=0;
			P1_5=0;
			P1_6=0;
			P1_7=0;
			msdelay(500);
		
		}
	}
}

void print()
{
	lcd_cmd(0x80);	
	lcd_write_string(display_msg3);
	lcd_cmd(0x86);	
	lcd_write_string(display_msg4);
	lcd_cmd(0x8c);	
	lcd_write_string(display_msg5);
	lcd_cmd(0xc0);	
	lcd_write_string(display_msg6);
	lcd_cmd(0xc7);	
	lcd_write_string(display_msgL2);
}