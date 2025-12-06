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

// variables for ball return
int steps = 0;
int time = 31250;
int ball_color = 0;
int temp_time = 0;

// variables for canyon sonar
int x_right;
int x_front;

void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void)
{
    _OC1IF = 0;
     steps ++ ;
    // this function is called
}

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
        else if (ADC1BUF11 <= QRDthreshold && ADC1BUF10 >= 1240 && ADC1BUF9 >= 1240 || ADC1BUF11 <= QRDthreshold && ADC1BUF10 <= QRDthreshold && ADC1BUF9 >= 1240 ) // if QRD1 (left) sees white
        {
            // Right wheel on, left wheel slower
            OC1RS = slow ; // left slower
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = fast ; // right faster
            OC2R = OC2RS * 0.5; // 50% duty cycle
        }
        else if (ADC1BUF9 <= QRDthreshold && ADC1BUF10 >= 1240 && ADC1BUF11 >= 1240 || ADC1BUF9 <= QRDthreshold && ADC1BUF10 <= QRDthreshold && ADC1BUF11 >= 1240) // if QRD3 (right) sees white
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
        else if ((ADC1BUF14 >= 1500) ) // if IR diode senses ball collect 1861
        {
            _LATB2 = 1;
            OC1RS = 0 ; // left slower
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = 0 ; // right faster
            OC2R = OC2RS * 0.5; // 50% duty cycle
            return;
            
//            current_state = COLLECT;
        }
        
        else {
            // If all 3 QRDs see black, go straight slowly
            OC1RS = slow ; // left slower
            OC2RS = slow ; // right slower
            OC1R = OC1RS * 0.5; // 0% duty cycle > left off
            OC2R = OC2RS * 0.5; // 0% duty cycle > right off
        }
        
        if (ball_counter == 1 && return_counter == 0) {
            if (ADC1BUF0 <= 2400) {
                ball_color = 1;
                _LATB2 = 1;
            }
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
    wheel_dir();      

}

void turn_right_collect(void) {
    TMR1 = 0;
    _LATA0 = 0;
    _LATA1 = 1; // LEFT (pin 3) forward
    _LATB9 = 0; //  RIGHT (pin 13) backward
    while (TMR1 <= 0.60*sec) { // turn right
                OC1RS = reg * 1.25 ; // left slower
                OC1R = OC1RS * 0.5; // 50% duty cycle
                OC2RS = reg * 1.25; // right faster
                OC2R = OC2RS * 0.5; // 50% duty cycle
            }
}

void turn_left_collect(void) {
    TMR1 = 0;
    _LATA0 = 0;
    _LATA1 = 0; // LEFT (pin 3) backward
    _LATB9 = 1; //  RIGHT (pin 13) forward
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
    _LATA1 = 1; // LEFT (pin 3) forward
    _LATB9 = 1; //  RIGHT (pin 13) forward
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
    _LATA1 = 0; // LEFT (pin 3) backward
    _LATB9 = 0; //  RIGHT (pin 13) backward
    while (TMR1 < dist) {
            OC1RS = reg * 1.25; // left on
            OC1R = OC1RS * 0.5; // 50% duty cycle
            OC2RS = reg * 1.25; // right on
            OC2R = OC2RS * 0.5; // 50% duty cycle
    } 
}

void ball_return(void) 
{
    //if ((TMR1 < 3906.25*2)) { // If Timer1 < 2s
//            if (ADC1BUF0 <= 2400) { // if ball is white
//                _LATB2 = 1;
//                ball_color = 1;
//            }
//            else { // ball is black
//            }
        //}
    //else {
        if (ball_color == 0){ // If ball is black
            turn_right_return(); // turn right 90 deg
            while (OC3R > 175) {
                if ((TMR1-temp_time)> 400){
                    //OC3R = 125; // 0 deg
                    temp_time = TMR1;
                        OC3R -= 5;
                }
            }
            turn_left_return();
            OC3R = 425;
            wheel_dir(); // reset wheels to forward
        }
        else if (ball_color == 1){ // If ball is white
            turn_left_return();
            while (OC3R > 175) {
                if ((TMR1-temp_time)> 400){
                    //OC3R = 125; // 0 deg
                    temp_time = TMR1;
                        OC3R -= 5;
                }
            }
            turn_right_return();
            OC3R = 425;
            wheel_dir(); // reset wheels to forward
        }
    //}
}

void delay(void) {
    TMR1 = 0;
    while (TMR1 < time/2) {
        OC1R = 0;
        OC2R = 0;
    }
}

void turn_right_return(void) {
    //steps = 0;
    OC1R = 0; // 0% duty cycle > left off
    OC2R = 0; // 0% duty cycle > right off
    delay();
    steps = 0;
    while(steps <= 138.5 * 2 *2) {
            _LATA1 = 1; //  LEFT forward
            _LATB9 = 0; //  RIGHT backward
            OC1RS = 1249 * 0.5;
            OC1R = OC1RS * 0.5;
            OC2RS = 1249 * 0.5;
            OC2R = OC1RS * 0.5;
             
    }
    OC1R = 0;
    OC2R = 0;
           
             
}

void turn_left_return(void) {
    //steps = 0;
    OC1R = 0; // 0% duty cycle > left off
    OC2R = 0; // 0% duty cycle > right off
    delay();
    steps = 0;
    while(steps <= 138.5 * 2 *2) {
            _LATA1 = 0; //  LEFT backward
            _LATB9 = 1; //  RIGHT forward
            OC1RS = 1249 * 0.5;
            OC1R = OC1RS * 0.5;
            OC2RS = 1249 * 0.5;
            OC2R = OC1RS * 0.5;
             
    }
    OC1R = 0;
    OC2R = 0;

}

void nav_canyon (void)
{
    if (ADC1BUF4 <= 2150) { // 1861=threshold for 1.5 V; no wall
            // Go straight
            _LATA1 = 1; //  LEFT forward
            _LATB9 = 1; //  RIGHT forward
        } 
        else if (ADC1BUF4 >= 2150 && ADC1BUF13 >= 1000) { // If front sees wall and right sees wall
            //_LATB4 = 0;
            turn_left_canyon();
        }
        else if (ADC1BUF4 >= 2150 && ADC1BUF13 <= 1000) { // If front sees wall and right does NOT
            //_LATB4 = 0;
            turn_right_canyon();
        }
    if (ADC1BUF10 <= QRDthreshold && ADC1BUF11 <= QRDthreshold || ADC1BUF9 <= QRDthreshold && ADC1BUF10 <= QRDthreshold) {
        //canyon_counter = 1;
        if (ADC1BUF13 <= 1050) { // right sharp doesn't see wall {
            TMR1 = 0;
            
            while (TMR1 <= 2200/2) // go straight for a little bit 
            {
                OC1RS = reg ; // left slower
                OC1R = OC1RS * 0.5; // 50% duty cycle
                OC2RS = reg ; // right faster
                OC2R = OC2RS * 0.5; // 50% duty cycle
            }
            TMR1 = 0;
            turn_right_canyon();
            wheel_dir();
            current_state = LINEFOLLOW;
        }
        else {
            TMR1 = 0;
            
            while (TMR1 <= 2200/2) // go straight for a little bit 
            {
                OC1RS = reg ; // left slower
                OC1R = OC1RS * 0.5; // 50% duty cycle
                OC2RS = reg ; // right faster
                OC2R = OC2RS * 0.5; // 50% duty cycle
            }
            TMR1 = 0;
            turn_left_canyon();
            wheel_dir();
             current_state = LINEFOLLOW;
        }

    }
                
                
}

void turn_right_canyon(void) {
    steps = 0;
    while(steps <= 138.5 * 2 *2) {
            _LATA1 = 1; //  LEFT forward
            _LATB9 = 0; //  RIGHT backward
            OC1RS = 1249 * 0.5;
            OC1R = OC1RS * 0.5;
            OC2RS = 1249 * 0.5;
            OC2R = OC1RS * 0.5;
             
    }
}

void turn_left_canyon(void) {
    steps = 0;
    while(steps <= 138.5 * 2 *2) {
            _LATA1 = 0; //  LEFT backward
            _LATB9 = 1; //  RIGHT forward
            OC1RS = 1249 * 0.5;
            OC1R = OC1RS * 0.5;
            OC2RS = 1249 * 0.5;
            OC2R = OC1RS * 0.5;
    }
}
