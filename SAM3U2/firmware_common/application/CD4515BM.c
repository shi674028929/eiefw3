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

void INT_CD4515(void)
{
		AT91C_BASE_PIOA->PIO_PER = (CD4515_A | CD4515_B | CD4515_C | CD4515_D | CD4515_INH | CD4515_STB );
		AT91C_BASE_PIOA->PIO_OER = (CD4515_A | CD4515_B | CD4515_C | CD4515_D | CD4515_INH | CD4515_STB );
		
		AT91C_BASE_PIOA->PIO_CODR = CD4515_STB;
		AT91C_BASE_PIOA->PIO_SODR = CD4515_INH;
}

static void Delay(u8 u8DelayTime)
{
  	for(u8 u8a = 0 ; u8a <u8DelayTime ; u8a++ )
	{	  	
	}
}

static void CD4515_OUT_Port(u8 u8Selected)
{
		AT91C_BASE_PIOA->PIO_SODR = CD4515_STB;
		
		switch(u8Selected)
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
}