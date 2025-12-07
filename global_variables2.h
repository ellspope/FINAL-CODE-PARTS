// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef global_variables_h
#define	global_variables_h

#include <xc.h> // include processor files - each processor file is guarded.  

// States for which task the robot should be performing
typedef enum {LINEFOLLOW, COLLECT, RETURN, CANYON, TRANSMIT} state;
//extern state current_state;

extern int ball_counter;
extern int return_counter;
extern int QRDthreshold;

extern int x_right;
extern int x_front;

#endif	/* XC_HEADER_TEMPLATE_H */
