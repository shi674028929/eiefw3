/**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */

extern volatile u16 u16Led_Data[16][10];

static u8 u8LoopTime = 16;
static u8 u8SandData = 0;


void INT_CD4515(void)
{
		AT91C_BASE_PIOA->PIO_PER = (CD4515_A | CD4515_B | CD4515_C | CD4515_D | CD4515_INH | CD4515_STB );
		AT91C_BASE_PIOA->PIO_OER = (CD4515_A | CD4515_B | CD4515_C | CD4515_D | CD4515_INH | CD4515_STB );
		
		AT91C_BASE_PIOA->PIO_CODR = CD4515_STB;
		AT91C_BASE_PIOA->PIO_SODR = CD4515_INH;
}

void INT_MBI5026(void)
{
		AT91C_BASE_PIOA->PIO_PER = (MBI5026_CLK | MBI5026_SDI | MBI5026_LE | MBI5026_OE );
		AT91C_BASE_PIOA->PIO_OER = (MBI5026_CLK | MBI5026_SDI | MBI5026_LE | MBI5026_OE );
		
		AT91C_BASE_PIOA->PIO_CODR = MBI5026_CLK;
		AT91C_BASE_PIOA->PIO_CODR = MBI5026_SDI;
		AT91C_BASE_PIOA->PIO_CODR = MBI5026_LE;
		AT91C_BASE_PIOA->PIO_SODR = MBI5026_OE;
}



static void Delay(u8 u8DelayTime)
{
		for(u8 u8a = 0 ; u8a <u8DelayTime ; u8a++ )
		{	  	
		}
}


 static void Spi_Sand_Data(u8 u8Column_data)
{
	 static u8 u8i=0;
	 static u8 u8j=0;
	 static u8 u16data;
	 
	 for(u8i = 10; u8i>=1 ; u8i--)
	 {
		 	u16data = u16Led_Data[u8Column_data][u8i-1];
			
			for(u8j = 0; u8j <8; u8j++)
			{
					AT91C_BASE_PIOA->PIO_CODR = MBI5026_CLK;
					
				  if(0x01 & u16data)
				  {
						  AT91C_BASE_PIOA->PIO_SODR = MBI5026_SDI;
				  }
					else
					{
							AT91C_BASE_PIOA->PIO_CODR = MBI5026_SDI;
					}
					
					Delay(5);					
					AT91C_BASE_PIOA->PIO_SODR = MBI5026_CLK;
					
					Delay(5);	
					u16data= u16data>>1;
			}
	 }
}


static void CD4515_OUT_Port(u8 u8LineSelected )
{
		AT91C_BASE_PIOA->PIO_SODR = CD4515_STB;
		AT91C_BASE_PIOA->PIO_SODR = CD4515_INH;

		switch(u8LineSelected)
		{
			case 0:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 1:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 2:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 3:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 4:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 5:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 6:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 7:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_D;		
					break;
			}
			case 8:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}
			case 9:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}
			case 10:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}
			case 11:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}
			case 12:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}
			case 13:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_CODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}
			case 14:
			{
					AT91C_BASE_PIOA->PIO_CODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}
			case 15:
			{
					AT91C_BASE_PIOA->PIO_SODR = CD4515_A;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_B;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_C;
					AT91C_BASE_PIOA->PIO_SODR = CD4515_D;		
					break;
			}			
		}

		AT91C_BASE_PIOA->PIO_CODR = CD4515_INH;
	
}

void LED_OPEN(void )
{
		static u8 u8i;
		for(u8i=0 ; u8i<16 ; u8i++)
		{
				Spi_Sand_Data(u8i);
				AT91C_BASE_PIOA->PIO_SODR = MBI5026_OE;
				Delay(5);	
				
				AT91C_BASE_PIOA->PIO_SODR = MBI5026_LE;
				Delay(5);	
				
				AT91C_BASE_PIOA->PIO_CODR = MBI5026_LE;
				Delay(5);	
				
				CD4515_OUT_Port(u8i);
				AT91C_BASE_PIOA->PIO_CODR = MBI5026_OE;
				Delay(5);	
		}
}
