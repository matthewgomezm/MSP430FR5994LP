
#include "driverlib.h"
#include <stdint.h>

//INLAB CODE

#define DELAYTIME 500000
typedef enum LEDSTATES {ALL_ON, ALL_OFF, RED_ON, RED_OFF, GREEN_ON, GREEN_OFF} LEDSTATES; 


void configIO(void);
void myLED(LEDSTATES);
void myDelay(const uint32_t);
void displayCount(uint8_t);

void main (void)
{
    //declaring local variables
    LEDSTATES color;
    uint8_t currentCount;
    uint8_t PBS1;
    uint8_t PBS2;


    //watchdog 
    WDT_A_hold(WDT_A_BASE);

    //calling config IO
    configIO();    
    
    //initial testing before experiment
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
    __delay_cycles(DELAYTIME);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
    __delay_cycles(DELAYTIME);

    //working loop. currentCount dictates the state of the up/down counter
    currentCount = 0;
    while(1)
    {
        PBS1 = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6);
        PBS2 = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5);

        if (PBS1 == GPIO_INPUT_PIN_LOW) // UP
        {
            if (currentCount == 3) currentCount = 0;
            else currentCount++;
            
        }
        else if (PBS2 == GPIO_INPUT_PIN_LOW) // DOWN
        {
            if (currentCount == 0) currentCount = 3;
            else currentCount--;
        }

        //////

        displayCount(currentCount); // <=======================
        __delay_cycles(DELAYTIME);


    }

}



/*
    configIO
    Function that configures the LEDs for output and PBs for input
    Input: N/A 
    Return Values: None
    Author: Matthew Gomez Morales
    Date Written: 9/16/25
*/

void configIO(void)
{
    //configuring both LEDs as outputs
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); //LED 1
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1); //LED 2

    //turning both LEDs OFF
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // LED 1
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1); // LED 2

    //configuring pushbuttons as inputs with the pullup resistor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN6); //PBS 1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5); //PBS 2

    //activating configuration
    PMM_unlockLPM5();
}

/*
    myLED
    Function to turn ON and OFF individual LEDs on the LP
    Inputs: color - LED to turn off or on
    Return Values: None
    Author: Matthew Gomez Morales
    Date Written: 9/16/25
*/

void myLED(LEDSTATES color)
{
    switch(color)
    {
        case ALL_ON:
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
            break;
        case ALL_OFF:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
            break;
        case RED_ON:
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            break;
        case RED_OFF:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            break;
        case GREEN_ON:
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
            break;
        case GREEN_OFF:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
            break;
        default:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
    }
}


/*
    displayCount
    Displays the current count on the LEDs
    Inputs: currentCount
    Return Values: None
    Author: Matthew Gomez Morales
    Date Written: 9/16/25

*/

void displayCount(uint8_t currentCount)
{

    uint8_t vTwos, vOnes;
    vTwos = currentCount / 2;
    vOnes = currentCount % 2;

    if(vTwos == 1) myLED(RED_ON); else myLED(RED_OFF);
    if(vOnes == 1) myLED(GREEN_ON); else myLED(GREEN_OFF);
}

