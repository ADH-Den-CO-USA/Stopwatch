/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/
APP_DATA appData;
APP_USART_DATA appUSARTData;
char *appStrTx;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void ISR_Callback_1 ( uintptr_t context, uint32_t currTick )
   {
//   PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_0);
    appData.upTimer ++;
   }
/* TODO:  Add any necessary callback functions.
*/

/*******************************************************************************
 * BUTTON 1 starts and stops the stopwatch and changes the state of the running
 * LED to reflect the status of the stopwatch.  The LED is on when running, and
 * off when stopped.  Additionally, information to this effect is sent via
 * USART.  
 * 
 * This is done as soon as the button #1 press is sensed, as a stopwatch is 
 * expected to be immediately responsive.
 ******************************************************************************/
void APP_Button1_Pressed(void) {
    if (! appData.isCounting) {
        // Start or Restart the timer
        char str[50];
        appData.upTimer = appData.timePaused;
        SYS_PORTS_PinSet(APP_LED_PORT_ID, APP_LED_PORT_CHANNEL, APP_LED1_PIN);
        sprintf(str, "START: %0.3f seconds\n", (float) appData.timePaused / 1000);
        APP_USART_Print(str);
        appUSARTData.transmitIndex = 0;
        appData.isCounting = true;
    } 
    else {
        // Pause the timer
        char str[50];
        appData.timePaused = appData.upTimer;
        sprintf(str, "STOP:  %0.3f seconds\n", (float) appData.timePaused / 1000);
        APP_USART_Print(str);
        appUSARTData.transmitIndex = 0;
        appData.isCounting = false;
        SYS_PORTS_PinClear(APP_LED_PORT_ID, APP_LED_PORT_CHANNEL, APP_LED1_PIN);
    }
}

/*******************************************************************************
 * Button 1 release after being pressed is unimportant and thus the callback is
 * null.  This function is commented out merely as a placeholder for documen-
 * tational purposes.
 ******************************************************************************/
//void APP_Button1_Released() {
//    
//}

/*******************************************************************************
 * For button 2 the program stops the stopwatch if running, and in this
 * case displays the final count via USART. 
 ******************************************************************************/
void APP_Button2_Pressed() {
   SYS_PORTS_PinClear(APP_LED_PORT_ID, APP_LED_PORT_CHANNEL, APP_LED1_PIN);
   if (appData.isCounting) {
      char str[50];
      appData.timePaused = appData.upTimer;
      sprintf(str, "STOP:  %0.3f seconds\n", (float) appData.timePaused / 1000);
      APP_USART_Print(str);
      appUSARTData.transmitIndex = 0;
      appData.isCounting = false;
   }
   // if the timer is running, then stop it now and send the final count via
   // USART, else if the timer was not running, then just exit the function in
   // wait for the button to be released.
}

/*******************************************************************************
 * The program displays "RESET" on a new line via USART.  This is handled in the
 * button RELEASED callback function, as it is not necessary to reset in such
 * short order as stopping the counter must be.  The count is returned to zero
 * at this point.
 ******************************************************************************/
void APP_Button2_Released() {
    appData.upTimer = 0;
    appData.timePaused = 0;
    APP_USART_Print("RESET: 0.000 seconds\n\n");
    appUSARTData.transmitIndex = 0;
    appData.isCounting = false;
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    // The code below is to overcome a bug in the Harmony configurator which causes this data to be lost after 
    // reopening/reloading the configuration data from the mhc file.  The configurator reverts back to analog
    // for all the pins each time you reopen the configurator.  Then when you rebuild, the configurator
    // builds an incorrect version of system_config.h
    //
    // To overcome this bug you can either remember to set the pins every time before you generate code from within
    // the configurator, or you can simply ignore its generated code for the ADP1CFG register and set up the pins
    // you want to use as seen below.
    SYS_PORTS_PinModeSelect(APP_LED_PORT_ID, PORTS_ANALOG_PIN_10, PORTS_PIN_MODE_DIGITAL); // connected to LD1
    SYS_PORTS_PinModeSelect(APP_LED_PORT_ID, PORTS_ANALOG_PIN_11, PORTS_PIN_MODE_DIGITAL); // connected to LD2
    SYS_PORTS_PinModeSelect(APP_LED_PORT_ID, PORTS_ANALOG_PIN_12, PORTS_PIN_MODE_DIGITAL); // connected to LD3
    SYS_PORTS_PinModeSelect(APP_LED_PORT_ID, PORTS_ANALOG_PIN_13, PORTS_PIN_MODE_DIGITAL); // connected to LD4

}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            SYS_TMR_CallbackPeriodic(1, 0, ISR_Callback_1);

            SYS_PORTS_PinClear(APP_LED_PORT_ID, APP_LED_PORT_CHANNEL, APP_LED1_PIN);
            SYS_PORTS_PinClear(APP_LED_PORT_ID, APP_LED_PORT_CHANNEL, APP_LED2_PIN);
            SYS_PORTS_PinClear(APP_LED_PORT_ID, APP_LED_PORT_CHANNEL, APP_LED3_PIN);
            SYS_PORTS_PinClear(APP_LED_PORT_ID, APP_LED_PORT_CHANNEL, APP_LED4_PIN);

            appData.upTimer = 0; // no time has elapsed yet
            appData.timePaused = 0;

            appData.buttonData[0].buttonCallbackPressed = APP_Button1_Pressed;
            appData.buttonData[0].buttonCallbackReleased = NULL;
            appData.buttonData[0].buttonPollState = APP_BTN_STATE_INIT;
            appData.buttonData[0].buttonStatus = false;

            appData.buttonData[1].buttonCallbackPressed = APP_Button2_Pressed;
            appData.buttonData[1].buttonCallbackReleased = APP_Button2_Released;
            appData.buttonData[1].buttonPollState = APP_BTN_STATE_INIT;
            appData.buttonData[1].buttonStatus = false;
            
            appData.isCounting = false;

//            sprintf(appUSARTData.strTransmitBuffer, "\n----STOPWATCH---\nBTN1: Start/Stop\nBTN2: Reset\n");
            appUSARTData.state = APP_USART_STATE_IDLE;
            APP_USART_Print("\n----STOPWATCH---\nBTN1: Start/Stop\nBTN2: Reset\n");
            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            APP_USART_Tasks();
            APP_Button_Tasks();
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            appData.state = APP_STATE_INIT;
            break;
        }
    }
}


/******************************************************************************
 * USART Print
 ******************************************************************************/
void * APP_USART_Print(char *str) {
   if(NULL != appStrTx) {
     free(appStrTx); // free the existing memory
     appStrTx = NULL;
   } 
   appStrTx = calloc(strlen(str), sizeof(char)); // allocate space for the string to send
//   appStrTx = malloc(strlen(str)); // allocate space for the string to send
   if(appStrTx == NULL) return appStrTx; // if something went wrong return
   strcpy(appStrTx, str); // save the string to transmit in the allocated space
   appUSARTData.state = APP_USART_STATE_TX;  //update the state machine state variable
   return appStrTx; // SUCCESS!
}

/******************************************************************************
 * USART Task handler
 ******************************************************************************/
void APP_USART_Tasks(void) {

   /* Check the application's current state. */
   switch (appUSARTData.state) {
      case APP_USART_STATE_IDLE:
         break;
      case APP_USART_STATE_TX: // USART transmit state
         // make sure the transmit buffer is not full before trying to write byte 
         if (!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART0_TransferStatus())) {
            // appUSARTData.chByteTx = appUSARTData.strTransmitBuffer[appUSARTData.transmitIndex];
//            appUSARTData.chByteTx = appStrTx[appUSARTData.transmitIndex];
            if (appStrTx[appUSARTData.transmitIndex] != '\0') {
               DRV_USART0_WriteByte(appStrTx[appUSARTData.transmitIndex]); // send modified byte
               appUSARTData.transmitIndex++; // point to the next byte
            } else {
               appUSARTData.state = APP_USART_STATE_IDLE; // SUCCESS!!
            }
         }
         break;
      default:
         appUSARTData.state = APP_USART_STATE_IDLE;
   } 
}

/*******************************************************************************
 End of File
 */
