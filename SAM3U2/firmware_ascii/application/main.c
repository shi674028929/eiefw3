/*!**********************************************************************************************************************
@file main.c                                                                
@brief Main system file for the EiE firmware.  
***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32SystemTime1ms = 0;                   /*!< @brief Global system time incremented every ms, max 2^32 (~49 days) */
volatile u32 G_u32SystemTime1s = 0;                    /*!< @brief Global system time incremented every second, max 2^32 (~136 years) */
volatile u32 G_u32SystemFlags = 0;                     /*!< @brief Global system flags */
volatile u32 G_u32ApplicationFlags = 0;                /*!< @brief Global system application flags: set when application is successfully initialized */

/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Main_" and be declared as static.
***********************************************************************************************************************/


/*!**********************************************************************************************************************
@fn void main(void)
@brief Main program where all tasks are initialized and executed.

Main has two sections:

1. Initialization which is run once on power-up or reset.  All drivers and applications are setup here without timing
contraints but must complete execution regardless of success or failure of starting the application. 

2. Super loop which runs infinitely giving processor time to each application.  The total loop time should not exceed
1ms of execution time counting all application execution.  SystemSleep() will execute to complete the remaining time in
the 1ms period.
***********************************************************************************************************************/

void main(void)
{
  G_u32SystemFlags |= _SYSTEM_INITIALIZING;

  /* Low level initialization */
  WatchDogSetup(); /* During development, does not reset processor if timeout */
  GpioSetup();
  ClockSetup();
  InterruptSetup();
  SysTickSetup();

  /* Driver initialization */
  MessagingInitialize();
  UartInitialize();
  DebugInitialize();

  /* Debug messages through DebugPrintf() are available from here */
  ButtonInitialize();
  TimerInitialize();  
  SspInitialize();
  TWIInitialize();
  Adc12Initialize();
  
  LcdInitialize();
  LedInitialize();
  SdCardInitialize();


  /* Application initialization */
  UserApp1Initialize();
  UserApp2Initialize();
  UserApp3Initialize();

  
  /* Exit initialization */
  SystemStatusReport();
  G_u32SystemFlags &= ~_SYSTEM_INITIALIZING;
    
  /* Super loop */  
  while(1)
  {
    WATCHDOG_BONE();
    SystemTimeCheck();
    
    /* Drivers */
    LedUpdate();
    ButtonRunActiveState();
    UartRunActiveState();
    TimerRunActiveState(); 
    SspRunActiveState();
    TWIRunActiveState();
    Adc12RunActiveState();
    MessagingRunActiveState();
    DebugRunActiveState();
    LcdRunActiveState();
    SdCardRunActiveState();

    /* Applications */
    UserApp1RunActiveState();
    UserApp2RunActiveState();
    UserApp3RunActiveState();
    
    HEARTBEAT_OFF();

    /* System sleep until next Systick */
    SystemSleep();

    HEARTBEAT_ON();
    
  } /* end while(1) main super loop */
  
} /* end main() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
