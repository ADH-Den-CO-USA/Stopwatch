/* 
 * File:   app_system_config.h
 * Author: Dad
 *
 * Created on May 29, 2016, 6:58 PM
 */

#ifndef APP_SYSTEM_CONFIG_H
#define	APP_SYSTEM_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    // *****************************************************************************
// *****************************************************************************
// Section: Application Configurations that are specific to a certain system
//    configuration.
// *****************************************************************************
// *****************************************************************************

/*** Application Instance 0 Configuration ***/
#define APP_LED_PORT_ID         PORTS_ID_0
#define APP_BUTTON_PORT_ID      PORTS_ID_0
#define APP_LED_PORT_CHANNEL    PORT_CHANNEL_B
#define APP_BUTTON_PORT_CHANNEL PORT_CHANNEL_A
#define APP_LED1_PIN            PORTS_BIT_POS_10
#define APP_LED2_PIN            PORTS_BIT_POS_11
#define APP_LED3_PIN            PORTS_BIT_POS_12
#define APP_LED4_PIN            PORTS_BIT_POS_13
#define APP_BUTTON1_PIN         PORTS_BIT_POS_6
#define APP_BUTTON2_PIN         PORTS_BIT_POS_7

#ifdef	__cplusplus
}
#endif

#endif	/* APP_SYSTEM_CONFIG_H */

