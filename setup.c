#include "xc.h"
#include "setup.h"
#include "global_variables.h"

void config_pins(void) {
    // Pin 3 determines direction of left motor
    _ANSA1 = 0; //pin 3 is digital
    _TRISA1 = 0; //pin 3 is an output
    
    // Pin 13 determines direction of right motor
    //pin 13 defaults to digital, it cannot do analog, so we don't have to configure that part of it
    _TRISB9 = 0; //pin 13 is an output
    
    // Configure pin 16 (RB13) as analog input for QRD1
    _TRISB13 = 1;
    _ANSB13 = 1;
    
    // Configure pin 17 (RB14) as analog input for QRD2
    _TRISB14 = 1;
    _ANSB14 = 1;
    
    // Configure pin 18 (RB15) as analog input for QRD3
    _TRISB15 = 1;
    _ANSB15 = 1;
    
    // Configure pin 8 (RA3) as analog input for IR diode
    _TRISB2 = 1;
    _ANSB2 = 1;
    
    // Pin 6 is our debug LED
    _TRISB2 = 0; // output
    _ANSB2 = 0; // digital
    
}

void config_ad(void){
    
    _ADON = 0;          // AD1CON1<15> -- Turn off A/D during config
    
    // Clear all A/D registers
    AD1CON1 = 0; 
    AD1CON2 = 0; 
    AD1CON3 = 0; 
    AD1CON5 = 0; 
    AD1CSSL = 0; 
    AD1CSSH = 0; 
    
    // AD1CON1 register
    _ADSIDL = 0;    // AD1CON1<13> -- A/D continues while in idle mode
    _MODE12 = 1;    // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;      // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;      // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;      // AD1CON1<2> -- Auto sampling

    // AD1CON2 register
    _PVCFG = 0;     // AD1CON2<15:14> -- Use VDD as positive ref voltage
    _NVCFG = 0;     // AD1CON2<13> -- Use VSS as negative ref voltage
    _BUFREGEN = 1;  // AD1CON2<11> -- Result appears in buffer
                    // location corresponding to channel, e.g., AN12
                    // results appear in ADC1BUF12
    _CSCNA = 1;     // AD1CON2<10> -- Does not scan inputs specified in AD1CSSx
                    // registers
    _SMPI = 4;      // AD1CON2<6:2> -- Results sent to buffer after n conversion
                    // For example, if you are sampling 4 channels, you
                    // should have _SMPI = 3;
    _ALTS = 0;      // AD1CON2<0> -- Sample MUXA only

    // AD1CON3 register
    _ADRC = 0;      // AD1CON3<15> -- Use system clock
    _SAMC = 1;      // AD1CON3<12:8> -- Auto sample every A/D period TAD
    _ADCS = 0;      // AD1CON3<7:0> -- A/D period TAD = TCY
    
    // AD1CHS register
    _CH0NA = 0;     // AD1CHS<7:5> -- Measure voltages relative to VSS
    _CH0SA = 14;    // AD1CHS<4:0> -- Use AN2 (pin 4) as positive input
    
    // AD1CSSL register
    _CSS11 = 1; // scan pin 16
    _CSS10 = 1; // scan pin 17
    _CSS9 = 1; //scan pin 18
    _CSS0 = 1; //scan pin 2
    _CSS14 = 1; //scan pin 8
    
    _ADON = 1;      // AD1CON1<15> -- Turn on A/D
}

void config_pwm(void) {
    // Configure PWM for pin 14 (LEFT motor)
    OC1CON1 = 0;                // Clear all bits of OC1CON1
    OC1CON2 = 0;                // Clear all bits of OC1CON2
    OC1CON1bits.OCTSEL = 0b111; // System clock as timing source
    OC1CON2bits.SYNCSEL = 0x1F; // Self-synchronization
    OC1CON2bits.OCTRIG = 0;     // Synchronization mode
    OC1CON1bits.OCM = 0b110;    // Edge-aligned PWM mode
    
    // Configure PWM for pin 4 (RIGHT motor)
    OC2CON1 = 0;                // Clear all bits of OC1CON1
    OC2CON2 = 0;                // Clear all bits of OC1CON2
    OC2CON1bits.OCTSEL = 0b111; // System clock as timing source
    OC2CON2bits.SYNCSEL = 0x1F; // Self-synchronization
    OC2CON2bits.OCTRIG = 0;     // Synchronization mode
    OC2CON1bits.OCM = 0b110;    // Edge-aligned PWM mode
}

void config_tmr1(void) {
    _TON = 1;       // Turn Timer1 on
    _TCKPS = 0b10; // 1:64 prescaler
    _TCS = 0;       // Internal clock source (FOSC/2)
    TMR1 = 0;       // Reset Timer1
}

void initial_OCregisters(void) {
    OC1RS = 0; 
    OC1R = OC1RS * 0.5; // 50% duty cycle
    OC2RS = 0;
    OC2R = OC1RS * 0.5; // 50% duty cycle
}

void wheel_dir(void) {
    _LATA1 = 1; // LEFT (pin 3) clockwise
    _LATB9 = 1; //  RIGHT (pin 13) clockwise
}
