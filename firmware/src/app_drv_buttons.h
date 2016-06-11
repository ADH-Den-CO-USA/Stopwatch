/* 
 * File:   APP_DRV_BUTTONS.h
 * Author: Dad
 *
 * Created on May 29, 2016, 10:25 PM
 */

#ifndef APP_DRV_BUTTONS_H
#define	APP_DRV_BUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Button Driver States
 ******************************************************************************/
typedef enum{
	APP_BTN_STATE_INIT=0,
	APP_BTN_STATE_UP,
    APP_BUTTON_STATE_DOWN,
} APP_BTN_STATES;

/*******************************************************************************
 * Button Driver Data
 ******************************************************************************/
typedef struct {
    bool            buttonInput; // raw data read (not de-bounced)
    APP_BTN_STATES  buttonPollState;
    bool            buttonStatus; // de-bounced signal input 
    void            (* buttonCallbackPressed)(); //pointer to function that does
    void            (* buttonCallbackReleased)(); // not return any value
    int             buttonOnCounter;
    int             buttonOffCounter;
} APP_BUTTON_DATA;

/*******************************************************************************
 * Button driver prototypes
 ******************************************************************************/
void APP_ButtonStateMachine (APP_BUTTON_DATA * buttonData);

/*******************************************************************************
 * 
 ******************************************************************************/
void APP_ButtonDebounce     (APP_BUTTON_DATA * buttonData);

void APP_Button_Tasks(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_DRV_BUTTONS_H */

