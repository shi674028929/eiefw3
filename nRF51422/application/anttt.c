/**********************************************************************************************************************
File: anttt.c                                                                

Description:
Implements TIC-TAC-TOE using data input from ANT or BLE.



**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_xxAnttt"
***********************************************************************************************************************/
/* New variables */
u32 G_u32AntttFlags;                                     /* Global state flags */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
extern volatile u32 G_u32BPEngenuicsFlags;             /* From bleperipheral_engenuics.c  */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Anttt_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Anttt_pfnStateMachine;              /* The application state machine function pointer */

u8 au8SpiTxdBuffer[20];
u8 u8SpiTxcount=0;
u8 u8SpiTxLength=0;
bool bSpiTxEn=0;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
static void Delay(u8 u8DelayTime)
{
  	for(u8 u8a = 0 ; u8a <u8DelayTime ; u8a++ )
	{	  	
	}
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: AntttInitialize

Description:
Initializes the State Machine and its variables.

Requires:

Promises:
*/
void AntttInitialize(void)
{
	//		configuration of the SPI0 //
		NRF_SPI0->PSELSCK = 11;
		NRF_SPI0->PSELMISO = 12;
		NRF_SPI0->PSELMOSI = 13;
		NRF_SPI0->CONFIG = 0x3;
		NRF_SPI0->FREQUENCY = SPI_FREQUENCY_FREQUENCY_K500 << SPI_FREQUENCY_FREQUENCY_Pos;			
		NRF_TWI0->ENABLE = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos;
		NRF_SPI0->ENABLE = SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos;
		NRF_SPI0->EVENTS_READY = 0;
		
		Anttt_pfnStateMachine = AntttSM_Idle;
  
} /* end AntttInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function AntttRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void AntttRunActiveState(void)
{
  Anttt_pfnStateMachine();

} /* end AntttRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------------------------*/
/* State Machine definitions                                                                                          */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
State: AntttSM_Idle
*/
static void AntttSM_Idle(void)
{
		static bool bRXData = 1;
		static u16 u16count;
		static u8 u8byte;
		static u8 u8TXtoBle;
		static bool bFirst = 1;
	
		//	Enter when data sent by bluetooth is received and determined to be valid values	//
		if(bSpiTxEn==1)
		{
				NRF_GPIO->OUTSET = P0_26_LED_BLU;  //	the blue lights//
				NRF_GPIO->OUTCLR = P0_10_SPI_CS;		//	Pull down the CS and start communicating //

				for(u16 i = 0 ; i <100 ; i++ )   // 	time delay //
				{
					for(u16 j = 0 ; j <1000 ; j++ );
				}
				
				if(NRF_SPI0->EVENTS_READY==1)
				{
					u8byte=NRF_SPI0->RXD;
					NRF_SPI0->EVENTS_READY=0;
				}
				
				if(u8SpiTxcount<u8SpiTxLength)
				{
						NRF_SPI0->TXD = au8SpiTxdBuffer[u8SpiTxcount]; 		// send data to SAM3U //
						u8SpiTxcount++;
				}
				else
				{
						u8SpiTxcount=0;
						bSpiTxEn=0;
						
						for(u16 i = 0 ; i <50 ; i++ )
						{
							for(u16 j = 0 ; j <100 ; j++ );
						}
						
						NRF_GPIO->OUTSET = P0_10_SPI_CS;
						NRF_GPIO->OUTCLR = P0_26_LED_BLU;
				}		 
		}
		
		//		Determine whether SAM3U is sending data//
		if((NRF_GPIO->IN & 0x00000200) == 0x0 )			// Determine the SRDY//
		{
				for(u16 i = 0 ; i <10 ; i++ )
				{
					for(u16 j = 0 ; j <100 ; j++ );
				}
				
				NRF_GPIO->OUTCLR = P0_10_SPI_CS;
				
				for(u16 i = 0 ; i <10 ; i++ )
				{
					for(u16 j = 0 ; j <100 ; j++ );
				}
				
				if(NRF_SPI0->EVENTS_READY==1)
				{
						u8byte=NRF_SPI0->RXD;					
						NRF_SPI0->EVENTS_READY=0;								
				}
				
				NRF_SPI0->TXD = 0x88;		// send 0x88 to indicate to SAM3U that it has received the message //
				
				for(u16 i = 0 ; i <10 ; i++ )
				{
					for(u16 j = 0 ; j <100 ; j++ );
				}
				
				switch(u8byte)
				{
						case 0x11:
						{
								u8TXtoBle = 1;
								break;
						}

						case 0x12:
						{
								u8TXtoBle = 2;
								break;
						}
						
						case 0x13:
						{
								u8TXtoBle = 3;
								break;
						}
						
						case 0x21:
						{
								u8TXtoBle = 4;
								break;
						}
						
						case 0x22:
						{
								u8TXtoBle = 5;
								break;
						}
						
						case 0x23:
						{
								u8TXtoBle = 6;
								break;
						}
						
						case 0x31:
						{
								u8TXtoBle = 7;
								break;
						}
						
						case 0x32:
						{
								u8TXtoBle = 8;
								break;
						}
						
						case 0x33:
						{
								u8TXtoBle = 9;
								break;
						}
				}

				BPEngenuicsSendData(&u8TXtoBle, sizeof(u8TXtoBle));		//	send data to bluetooth //
				
		}
}
	
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
