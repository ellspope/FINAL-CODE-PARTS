#pragma config FNOSC = LPFRC //choose the internal 500 kHz oscillator
#pragma config OSCIOFNC = OFF   // Turn off clock output on pin 8
#pragma config SOSCSRC = DIG    // Turn off secondary oscillator on pins 9&10
#include "xc.h"
#include "setup.h"
#include "global_variables.h"

int ball_counter = 0;

int main(void) {
    // Post-scale oscillator
    _RCDIV = 0b000;             // Divide-by-1 post-scaler

    // Configure pins, ADC, PWM, Timer1
    config_pins();
    config_ad(); 
    config_pwm();
    config_tmr1();
    
    initial_OCregisters(); // Initialize OC1RS and OC1R
    wheel_dir(); // Set wheel direction
    
    //initialize state here
    state current_state = LINEFOLLOW;
    
    while (1) 
    {
        switch(current_state) 
        {
            case LINEFOLLOW: // If current_state == LINEFOLLOW
                
                line_follow();
                if (ADC1BUF14 >= 1500 && ball_counter ==0) {
                    current_state = COLLECT;
                    
                }
                break;
            case COLLECT: // If current_state == COLLECT
                ball_collect();
                _LATB2 = 0;
                ball_counter = 1;
                current_state = LINEFOLLOW;
                break;
//            case RETURN: // If current_state == RETURN
//                ball_return();
//                break;
//            case CANYON: // If current_state == CANYON
//                nav_canyon();
//                break;
//            case TRANSMIT: // If current_state == TRANSMIT
//                transmit_sig();
//                break;
        }
    }
    
    return 0;
}
