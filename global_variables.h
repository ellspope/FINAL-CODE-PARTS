// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef global_variables_h
#define	global_variables_h

#include <xc.h> // include processor files - each processor file is guarded.  

// States for which task the robot should be performing
typedef enum {LINEFOLLOW, COLLECT, RETURN, CANYON, TRANSMIT} state;

#endif	/* XC_HEADER_TEMPLATE_H */
