/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                    /* From debug.c */
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/
static SspConfigurationType Spi_Slave_Configuration;
static SspConfigurationType* psSpi_Slave_Configuration;

static SspPeripheralType* psSlave_RequestedSsp; 
static u8 au8RxBuffer[100] ;
static u8* pu8RxNextByte ;
static u8 au8DebugGrid[][51]=
{
    "012345678901234567890123456789012345678901234567\n\r",
	"1               |               |               \n\r",
	"2               |               |               \n\r",
	"3               |               |               \n\r",
	"4       1       |       2       |       3       \n\r",
	"5               |               |               \n\r",
	"6               |               |               \n\r",
	"7---------------|---------------|---------------\n\r",
	"8               |               |               \n\r",
	"9               |               |               \n\r",
	"0               |               |               \n\r",
	"1       4       |       5       |       6       \n\r",
	"2               |               |               \n\r",
	"3               |               |               \n\r",
	"4---------------|---------------|---------------\n\r",
	"5               |               |               \n\r",
	"6               |               |               \n\r",
	"7               |               |               \n\r",
	"8       7       |       8       |       9       \n\r",
	"9               |               |               \n\r",
	"0               |               |               \n\r"
};


 

  
  
/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
static void DebugPrintfGrid (void)
{
  	for(u8 i = sizeof(au8DebugGrid) / 51, *pu8Point = au8DebugGrid[0]; i; i--, pu8Point += 51)
	{
		DebugPrintf(pu8Point);
	}
}

static void UserApp1SlaveTxFlowCallback (void)
{
//  	for(u16 i=0;i<100;i++)
//	{
//	  	for(u16 b=0;b<100;b++);
//	}
  	AT91C_BASE_PIOB->PIO_CODR = PB_24_ANT_SRDY;
}

static void UserApp1SlaveRxFlowCallback (void)
{
  	static u8 u8Rx;
	
	u8Rx = AT91C_BASE_US2->US_RHR;
	
	switch(u8Rx)
	{
		case 0x11:
		{
			au8DebugGrid[4][8] = 'O';
			DebugPrintfGrid();
			break;
		}
		  
		case 0x12:
		{
			au8DebugGrid[4][24] = 'O';
			DebugPrintfGrid();
			break;
		}

		case 0x13:
		{
			au8DebugGrid[4][40] = 'O';
			DebugPrintfGrid();
			break;
		}

		case 0x21:
		{
			au8DebugGrid[11][8] = 'O';
			DebugPrintfGrid();
			break;
		}

		case 0x22:
		{
			au8DebugGrid[11][24] = 'O';
			DebugPrintfGrid();
			break;
		}

		case 0x23:
		{
			au8DebugGrid[11][40] = 'O';
			DebugPrintfGrid();
			break;
		}

		case 0x31:
		{
			au8DebugGrid[18][8] = 'O';
			DebugPrintfGrid();
			break;
		}

		case 0x32:
		{
			au8DebugGrid[18][24] = 'O';
			DebugPrintfGrid();
			break;
		}

		case 0x33:
		{
			au8DebugGrid[18][40] = 'O';
			DebugPrintfGrid();
			break;
		}

		default:
		{
			DebugPrintf("\n\rInput command error!\n\r");
			break;
		} 
	}		
}



/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  	DebugPrintfGrid();
  /*	for(u8 i = sizeof(au8DebugGrid) / 51, *pu8Point = au8DebugGrid[0]; i; i--, pu8Point += 51)
	{
		DebugPrintf(pu8Point);
	}*/
		
  	AT91C_BASE_PIOB->PIO_PER |= ( PB_23_ANT_MRDY | PB_24_ANT_SRDY );  
	AT91C_BASE_PIOB->PIO_OER |= ( PB_23_ANT_MRDY | PB_24_ANT_SRDY );
	
  	AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY;
	AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
	
  	Spi_Slave_Configuration.SspPeripheral = USART2;
	Spi_Slave_Configuration.pCsGpioAddress = AT91C_BASE_PIOB;
	Spi_Slave_Configuration.u32CsPin = 1<<22;
	Spi_Slave_Configuration.eBitOrder = 	LSB_FIRST;
	Spi_Slave_Configuration.eSspMode = SPI_SLAVE_FLOW_CONTROL;
	Spi_Slave_Configuration.fnSlaveTxFlowCallback = UserApp1SlaveTxFlowCallback;
	Spi_Slave_Configuration.fnSlaveRxFlowCallback = UserApp1SlaveRxFlowCallback;
	Spi_Slave_Configuration.pu8RxBufferAddress = au8RxBuffer;
	Spi_Slave_Configuration.ppu8RxNextByte = &pu8RxNextByte;
	Spi_Slave_Configuration.u16RxBufferSize = 64;
	
	psSpi_Slave_Configuration = &Spi_Slave_Configuration;
	
	psSlave_RequestedSsp = SspRequest(psSpi_Slave_Configuration);
	
	
	
	if(psSlave_RequestedSsp == NULL)
	{
	  	LedOn(RED);	
		LedOff(GREEN);
	}
	else
	{
	  	LedOn(GREEN);		
		LedOff(RED);	
	}
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();
} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
  	static u8 au8DebugScanf[3];
	static u8 u8TXData;
	u8* pu8Point2 = NULL;
	static bool bTXReady = FALSE;
	
	if(DebugScanf(au8DebugScanf)!= NULL)
	{
	  	AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
	  	bTXReady = TRUE;
	  	switch(au8DebugScanf[0])
		{
			case '1':
			{
				au8DebugGrid[4][8] = 'X';
				u8TXData = 0x11;
				DebugPrintfGrid();
				break;
			}
			case '2':
			{
				au8DebugGrid[4][24] = 'X';
				u8TXData = 0x12;
				DebugPrintfGrid();
				break;
			}

			case '3':
			{
				au8DebugGrid[4][40] = 'X';
				u8TXData = 0x13;
				DebugPrintfGrid();
				break;
			}

			case '4':
			{
				au8DebugGrid[11][8] = 'X';
				u8TXData = 0x21;
				DebugPrintfGrid();
				break;
			}

			case '5':
			{
				au8DebugGrid[11][24] = 'X';
				u8TXData = 0x22;
				DebugPrintfGrid();
				break;
			}

			case '6':
			{
				au8DebugGrid[11][40] = 'X';
				u8TXData = 0x23;
				DebugPrintfGrid();
				break;
			}

			case '7':
			{
				au8DebugGrid[18][8] = 'X';
				u8TXData = 0x31;
				DebugPrintfGrid();
				break;
			}

			case '8':
			{
				au8DebugGrid[18][24] = 'X';
				u8TXData = 0x32;
				DebugPrintfGrid();
				break;
			}

			case '9':
			{
				au8DebugGrid[18][40] = 'X';
				u8TXData = 0x33;
				DebugPrintfGrid();
				break;
			}

			default:
			{
				DebugPrintf("\n\rInput command error!\n\rYour Input:");
				break;
			}
		}		  			
	}
	
	if(bTXReady)
	{
	  SspWriteByte(psSlave_RequestedSsp,u8TXData);
	  //AT91C_BASE_US2->US_THR = u8TXData;
	  
				bTXReady = FALSE;
		
	}
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
