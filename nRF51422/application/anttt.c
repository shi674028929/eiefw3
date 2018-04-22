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
	NRF_SPI0->ENABLE = SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos;
	NRF_SPI0->PSELSCK = 11;
	NRF_SPI0->PSELMISO = 12;
	NRF_SPI0->PSELMOSI = 13;
	NRF_SPI0->CONFIG = 0;
	NRF_SPI0->FREQUENCY = SPI_FREQUENCY_FREQUENCY_K125 << SPI_FREQUENCY_FREQUENCY_Pos;
	NRF_SPI0->POWER = SPI_POWER_POWER_Disabled << SPI_POWER_POWER_Pos;
	NRF_SPI0->INTENSET = SPI_INTENSET_READY_Enabled << SPI_INTENSET_READY_Pos;
	NRF_SPI0->INTENCLR = SPI_INTENCLR_READY_Enabled << SPI_INTENCLR_READY_Pos;
	
	NRF_GPIO->OUTCLR = P0_10_SPI_CS;
	
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
  	static bool bStart_Synchronous = 0;
  
  	if(NRF_GPIO->IN & 0x00000300 == 0x00000300  )
	{
	  	bStart_Synchronous = 1;
	}
	
	if(bStart_Synchronous)
	{
	  	NRF_GPIO->OUTSET = P0_10_SPI_CS;
		
		Delay(3);
		
		NRF_GPIO->OUTCLR = P0_10_SPI_CS;
		
		bStart_Synchronous = 0;
	}    
} 



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
