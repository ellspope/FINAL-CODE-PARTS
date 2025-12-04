//*********************************************************
//In this file we'll include any of the task functions, and basically
//all other functions that aren't configuration functions
//*********************************************************

#include "xc.h"
#include "setup.h"
#include "global_variables.h"

// Task states initialized in main file. This passes them in to this actions file
extern state current_state;

int fast = 1249 * 0.5 * 0.5 * 1.25 * 0.5 * 0.5;
int reg =  1249 * 0.5 * 0.5 * 1.25 * 0.5; // 1249 is for PWM frequency of 200 Hz (motor speed of 1 rev/s, for full-step)
int slow = 1249 * 0.5 * 0.5 * 1.25 * 3 * 0.5;
int QRDthreshold = 2400;

// variables for ball collect
int sec = 3906;
int dist = 3906 * 0.85;

void line_follow(void)
{
        if (ADC1BUF10 <= QRDthreshold && ADC1BUF11 >= 1240 && ADC1BUF9 >= 1240) // if QRD2 (middle) sees white
        {
            // Both wheels on
            OC1RS = reg ; // left on
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = reg ; // right on
            OC2R = OC2RS * 0.5; // 50% duty cycle
        }
        else if (ADC1BUF11 <= QRDthreshold && ADC1BUF10 >= 1240 && ADC1BUF9 >= 1240) // if QRD1 (left) sees white
        {
            // Right wheel on, left wheel slower
            OC1RS = slow ; // left slower
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = fast ; // right faster
            OC2R = OC2RS * 0.5; // 50% duty cycle
        }
        else if (ADC1BUF9 <= QRDthreshold && ADC1BUF10 >= 1240 && ADC1BUF11 >= 1240) // if QRD3 (right) sees white
        {
            // Left wheel on, right wheel slower
            OC1RS = fast ; // left faster
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = slow ; // right slower
            OC2R = OC2RS * 0.5; // 50% duty cycle
        }
        else if (ADC1BUF9 <= QRDthreshold && ADC1BUF11 <= QRDthreshold && ADC1BUF10 <= QRDthreshold ) // if all QRD'S see white
        {
            TMR1 = 0;
            
            while (TMR1 <= 1953/2) // go straight for a little bit 
            {
                OC1RS = reg ; // left slower
                OC1R = OC1RS * 0.5; // 50% duty cycle
                OC2RS = reg ; // right faster
                OC2R = OC2RS * 0.5; // 50% duty cycle
            }
            TMR1 = 0;
            
            while (TMR1 <= 1953) { // turn left
                _LATA1 = 0;
                OC1RS = reg ; // left slower
                OC1R = OC1RS * 0.5; // 50% duty cycle
                OC2RS = reg ; // right faster
                OC2R = OC2RS * 0.5; // 50% duty cycle
            }
            _LATA1 = 1;
        }
        else {
            // If all 3 QRDs see black, go straight slowly
            OC1RS = slow ; // left slower
            OC2RS = slow ; // right slower
            OC1R = OC1RS * 0.5; // 0% duty cycle > left off
            OC2R = OC2RS * 0.5; // 0% duty cycle > right off
        }
}

void ball_collect(void)
{
    turn_right_collect();
    pause();
    collect();
    pause();
    backup();
    pause();
    turn_left_collect();
    pause();

}

void turn_right_collect(void) {
    TMR1 = 0;
    _LATA0 = 0;
    _LATA1 = 1; // LEFT (pin 3) clockwise
    _LATB9 = 0; //  RIGHT (pin 13) clockwise
    while (TMR1 <= 0.65*sec) { // turn right
                OC1RS = reg * 1.25 ; // left slower
                OC1R = OC1RS * 0.5; // 50% duty cycle
                OC2RS = reg * 1.25; // right faster
                OC2R = OC2RS * 0.5; // 50% duty cycle
            }
}

void turn_left_collect(void) {
    TMR1 = 0;
    _LATA0 = 0;
    _LATA1 = 0; // LEFT (pin 3) clockwise
    _LATB9 = 1; //  RIGHT (pin 13) clockwise
    while (TMR1 <= 0.57*sec) { // turn right
                OC1RS = reg * 1.25 ; // left slower
                OC1R = OC1RS * 0.5; // 50% duty cycle
                OC2RS = reg * 1.25; // right faster
                OC2R = OC2RS * 0.5; // 50% duty cycle
            }
}

void pause(void) {
    TMR1 = 0;
    _LATA0 = 0;
    while (TMR1 < sec) {
        OC1RS = 0;
        OC2RS = 0;
    }
}

void collect(void) {
    TMR1 = 0;
    _LATA0 = 0;
    _LATA1 = 1; // LEFT (pin 3) clockwise
    _LATB9 = 1; //  RIGHT (pin 13) clockwise
    while (TMR1 < dist) {
            OC1RS = reg * 1.25; // left on
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = reg * 1.25; // right on
            OC2R = OC2RS * 0.5; // 50% duty cycle
    }
}

void backup(void) {
   TMR1 = 0;
    _LATA0 = 0;
    _LATA1 = 0; // LEFT (pin 3) clockwise
    _LATB9 = 0; //  RIGHT (pin 13) clockwise
    while (TMR1 < dist) {
            OC1RS = reg * 1.25; // left on
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = reg * 1.25; // right on
            OC2R = OC2RS * 0.5; // 50% duty cycle
    } 
}
