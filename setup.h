// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef setup_h
#define	setup_h

#include <xc.h> // include processor files - each processor file is guarded.  

// Function prototypes for tasks
void line_follow(void);
void ball_collect(void);
void ball_return(void);
//int nav_canyon(int previous_state);
void nav_canyon(void);
void transmit_sig(void);

// Function prototypes for setup and misc
void config_pins(void);
void config_ad(void);
void config_pwm(void);
void config_tmr1(void);
void initial_OCregisters(void);
void wheel_dir(void);
void config_OCinterrupt(void);

// some ball_collect function prototypes
void turn_right_collect(void);
void turn_left_collect(void);
void pause(void);
void collect(void);
void backup(void);

// ball_return function prototypes
void delay(void);
void turn_right_return(void);
void turn_left_return(void);

// nav_canyon function prototypes
void turn_right_canyon(void);
void turn_left_canyon(void);


#endif	/* XC_HEADER_TEMPLATE_H */

