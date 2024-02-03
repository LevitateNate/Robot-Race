/*===================================CPEG222====================================
 * Program:      ROBOT RACE
 * Authors:     Griffin, TImothy
 * Date:        11/12/2023
 * This is a guide that you can use to write your project 4 code
==============================================================================*/
/*-------------- Board system settings. PLEASE DO NOT MODIFY THIS PART ----------*/
#ifndef _SUPPRESS_PLIB_WARNING          //suppress the plib warning during compiling
#define _SUPPRESS_PLIB_WARNING
#endif
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
/*----------------------------------------------------------------------------*/
#define SYS_FREQ (80000000L) // 80MHz system clock
#define _80Mhz_ (80000000L)
#define LOOPS_NEEDED_TO_DELAY_ONE_MS_AT_80MHz 1426
#define LOOPS_NEEDED_TO_DELAY_ONE_MS (LOOPS_NEEDED_TO_DELAY_ONE_MS_AT_80MHz * (SYS_FREQ / _80Mhz_))
#define CORE_TICK_PERIOD (SYS_FREQ/20) //100ms core tick DO I NEED THIS??

#define TRUE 1
#define FALSE 0


// Libraries
#include <string.h>
#include <xc.h>   //Microchip XC processor header which links to the PIC32MX370512L header
#include <stdio.h>  // need this for sprintf
#include <sys/attribs.h>
#include "config.h" // Basys MX3 configuration header
#include "led.h"
#include "ssd.h"
#include "lcd.h"
#include "swt.h"

// BUTTONS
#define SW0 PORTFbits.RF3
#define SW1 PORTFbits.RF5
#define SW2 PORTFbits.RF4
#define SW3 PORTDbits.RD15
#define SW4 PORTDbits.RD14
#define SW5 PORTBbits.RB11
#define SW6 PORTBbits.RB10
#define SW7 PORTBbits.RB9
#define led0 LATAbits.LATA0
#define led1 LATAbits.LATA1
#define led2 LATAbits.LATA2
#define led3 LATAbits.LATA3
#define led4 LATAbits.LATA4
#define led5 LATAbits.LATA5
#define led6 LATAbits.LATA6
#define led7 LATAbits.LATA7
#define btnr PORTBbits.RB8
#define btnc PORTFbits.RF0

// Function Declarations
void initializePorts();
void pwmConfig();
//void activateServo();

//GLOBAL DECLERATIONS
int LM = 0;
int RM = 0;
int count = 0;///to
int left_motor_status = 0;
int right_motor_status = 0;
int timer_on = FALSE;
int timer = 0;
int dig1 = 31;
int dig2 = 31;
int dig3 = 31;
int dig4 = 31;
int dig5 = 0;
int IR1=0;//1st left sensor
int IR2=0;//2nd  left sensor
int IR3=0;//3rd sensor from right
int IR4=0;// 4th sensor on right


/* else if (left_motor_status == 2){
       OC4RS = (int) PR3 / 10; //left forward or reverse
    }
    
    if (right_motor_status == 2) {
        OC5RS = (int) PR3 / 10; //right reverse
 */






int stops = 0;

int main(void) {
    initializePorts();
    pwmConfig();
//    activateServo();
    
    LCD_WriteStringAtPos("Clanker",0,0);
    LCD_WriteStringAtPos("STP          STP",1,0);
    //SSD_WriteDigits(dig1, dig2, dig3, dig4, 0,dig5, 0, 0);
    OC4RS = 0;
    OC5RS = 0;
    Timer2Setup();
    timer_on = TRUE;
        SSD_WriteDigits(dig1, dig2, dig3, dig4, 0,dig5, 0, 0);
        int micVal = 0;
    while (TRUE) {
        while(count < 2){
            unsigned int micVal = MIC_Val();
            if(micVal > 950){
                delay_ms(200);
                count ++;
                if (count ==2){
                    activateServo();
                    RM = 2;
                    LM = 2;
                    delay_ms(200);
                    
                }
            }
        }
        //activateServo();
        //code to satisfy the midstage demo requirement of demonstrating that we receive data from the infared sensors
        // this displays the number of the 
        //PMODS_GetValue(0, x) is the binary output value of an infared sensor "x" 1 through 4. the 0 signifies from PmodA not B
        if (PMODS_GetValue(0, 7)) {LCD_WriteStringAtPos("1", 1, 9); IR1 = 1;}
        if (PMODS_GetValue(0, 8)) {LCD_WriteStringAtPos("1", 1, 8); IR2 = 1;}
        if (PMODS_GetValue(0, 9)) {LCD_WriteStringAtPos("1", 1, 7); IR3 = 1;}
        if (PMODS_GetValue(0, 10)) {LCD_WriteStringAtPos("1", 1, 6); IR4 = 1;}

// this reverts the lcd segments to an empty digit when they are not detecting light
        if (PMODS_GetValue(0, 7) == 0) {LCD_WriteStringAtPos("0", 1, 9); IR1 = 0;}
        if (PMODS_GetValue(0, 8) == 0) {LCD_WriteStringAtPos("0", 1, 8); IR2 = 0;}
        if (PMODS_GetValue(0, 9) == 0) {LCD_WriteStringAtPos("0", 1, 7); IR3 = 0;}
        if (PMODS_GetValue(0, 10) == 0) {LCD_WriteStringAtPos("0", 1, 6); IR4 = 0;}
        
           // SSD_WriteDigits(dig1, dig2, dig3, dig4, 0,dig5, 0, 0);
            
        if(SW6){
            timer_on=TRUE;
            SSD_WriteDigits(20, 24, dig3, dig4, 0,dig5, 0, 0);
            OC4RS = (int) PR3 / 40/3; //left motor reverse
             OC5RS = (int) PR3 ; //right motor stop
            
        } if(SW5){
            OC4RS = (int) PR3 /20;//left motor backwards
            OC5RS = (int) PR3 /10;// right motor backwards
        } if (SW4){
            OC4RS = (int) PR3 / 10;//left motor forward
            OC5RS = (int) PR3 / 20 ;//right motor forward
        } if (SW3){ OC4RS = (int) PR3 / 40/3;//left motor backwards
                  OC5RS = (int) PR3 / 40/3;// right motor forward ###    
                    
        }
        
        
        

// ------------------------------------------------------------
        
        
    if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 1 )
    {
            //left_motor_status = 2;
           // right_motor_status = 1;
           RM=2;
           LM=2;//forward
            OC5RS = (int) PR3 / 20; //forward
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
       
        
        
        
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 1 )
    {
            //left_motor_status = 2;
           // right_motor_status = 1;
           LM=2; //forward
           LM=2; //forward
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
             LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        }
        
        
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 1 )
    {
            //left_motor_status = 2;
           // right_motor_status = 1;
           LM=2; //forward
            RM=2; //forward
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 1 )
    {       //OC4RS = 0;
            //OC5RS = 0;
            LCD_WriteStringAtPos("RAT", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            //delay_ms(30);
            
            
            LM=2; //forward
            RM=0;
            //delay_ms(40);
                    }
        
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0 )
    {       //OC4RS = 0;
            //OC5RS = 0;
            LCD_WriteStringAtPos("RAT", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            //delay_ms(30);
            
            
            LM = 0;
             RM=2; //forward
            //delay_ms(40);
                    }
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 1)
        {
            LM=2;//forward
            RM= 1;
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("RET", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 1)
        {
            LM=2;//maybe 20
            RM=1;// maybe 20
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("RET", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
        {
            LM = 1;//maybe 20
            RM=2;// maybe 20
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("RET", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 0)
        {
            LM = 1;//maybe 20
            RM=2;// maybe 20
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("RET", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 1)
        {
            LM=0;
            
            RM=0;
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("FWK", 1, 0);
            LCD_WriteStringAtPos("FWK", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
        }
        
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
        {
            if (stops==1){
            LM=0;// forwarf or reve
            
            RM=0;//rev
            }
            
            if (stops==0){
            LM=2;// forwarf or reve
            
            RM=2;//rev
            stops=1;
            delay_ms(120);        
            }
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 0;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            
                  
        
            }
        }
           
        /*
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0 && stops==1)
        {
            RM=0;
           LM=0;
                    timer = 0;;
            //left_motor_status = 0;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("STP", 1, 0);
            LCD_WriteStringAtPos("STP", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 0;
        }
      */   
        
    
     
    if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
    {
        OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            left_motor_status = 2;
            right_motor_status = 1;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        }
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 0)
    {
        OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        }
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
            
    {       RM=0;
            LM=0;
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        } 
        if (PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 0){
            LM=0; //forward
            RM=0; //forward
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            
        }
        
        
       
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 1){
            RM=0;
            LM=0;
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            
        }
        

        
        
        
        
        

        
        

        // Activate servo based on SW6
        //activateServo();
    

    return 0;
}



void Timer2Setup(){
    PR2 = 2450; // prescaler
    TMR2 = 0;
    T2CONbits.TCKPS = 2; // timer prescaler
    T2CONbits.TGATE = 0; // not gated inpute,, default
    T2CONbits.TCS = 0; // PBCLK Input,,default
    T2CONbits.ON = 1;
    
    IPC2bits.T2IP = 6; //priority
    IPC2bits.T2IS = 3;//subpriority
    IFS0bits.T2IF = 0; //clear interrupt flag
    IEC0bits.T2IE = 1;
    macro_enable_interrupts();
    
}




// Initialize ports on board
void initializePorts() {
    DDPCONbits.JTAGEN = 0;// WHAT DOES THIS DO?
    LED_Init();
    LCD_Init();
    SSD_Init();
    SWT_Init();
    SRV_Init(); // DOES THIS INITIATE THE SERVOS????----- YES
    MIC_Init();
    RGBLED_Init();
    PMODS_InitPin(0,7,1,0,0);   //Griffin- I changed this to A instead of B by making first parameter 0. Initializes PMod port A segments 1-4 as inputs
    PMODS_InitPin(0,8,1,0,0);
    PMODS_InitPin(0,9,1,0,0);
    PMODS_InitPin(0,10,1,0,0);
    ADC_Init();
    
    /*RED = 255;                  //Sets the RGBLED to RED
    GRN = BLU = 0;
    RGBLED_SetValue(RED,GRN,BLU);
     */
    
}




// Software delay function
void delay_ms(int ms) {
    // delays the program for ms milliseconds
    int i;
    for (i = 0; i < ms* LOOPS_NEEDED_TO_DELAY_ONE_MS; i++) {
         
        
    }
}



void pwmConfig() {
    // configure Timer 1
    PR3 = (int)(((float)(10000000)/(16*5))+0.5)*0.2;
    T3CONbits.TCKPS = 3; // Timer Prescaler - Set the appropriate value based on your requirements. ASK TA IF RIGHT
    T3CONbits.TGATE = 0; // not gated input (the default)
    T3CONbits.TCS = 0; // PBCLK input (the default)
    T3CONbits.ON = 1;  // Turn on Timer
    TMR3 = 0; // Set Timer 1 to 0
    
    IPC3bits.T3IP = 6;  // priority
    IPC3bits.T3IS = 3;  // subpriority
    IFS0bits.T3IF = 0; // clear interrupt flag
    IEC0bits.T3IE = 1; // enable interrupt
    macro_enable_interrupts();
    
    // Configure Output Compare Module 4 (OC4)
    OC4CONbits.ON = 1;
    OC4CONbits.OCM = 6; // PWM mode on OC4; Fault pin is disabled
    OC4CONbits.OCTSEL = 1;    // Select Timer 1 to use as a clock source
    OC4CONbits.ON = 1;       // Start the OC4 module
    OC5CONbits.ON=0;
    OC5CONbits.OCM=6;
    OC5CONbits.OCTSEL=1;
    OC5CONbits.ON=1;
    OC4RS = PR3/20;
    OC4R = OC4RS;
    OC5RS = PR3/20;
    OC5RS= OC5R;
            
    
    
    // Configure Output Compare Module 5 (OC5) - Add similar configurations as OC4
    
    // Set up pin directions and mappings
    TRISBbits.TRISB8 = 0; // set servo 0 as output
    TRISAbits.TRISA15 = 0; // set servo 1 as output
    ANSELBbits.ANSB8 = 0; // set servo 0 as digital
    
    // Map OC4 and OC5 to respective pins
    RPB8R = 0x0B; // connect Servo 0 to OC5
    RPA15R = 0x0B;// connect Servo 1 to OC4
    
    // Set up additional timers here if necessary
}

// ISR's are here if you need them. Don't forget to set them up!
void __ISR(_TIMER_2_VECTOR) Timer2ISR(void) {// i changed the code
    IEC0bits.T2IE = 0; // disable interrupt
    if(timer_on == TRUE && LM != 0 || RM != 0) {
        if(timer < 999900) {
            timer++;
            if (timer % 100 == 0){
                dig1 = (timer / 100) % 10;
                dig2 = (timer / 1000) % 10;
                dig3 = (timer / 10000) % 10;
                dig4 = (timer / 100000) % 10;
                dig5 = 1;
                if (dig1 == 0 && dig2 == 0 && dig3 == 0 && dig4 == 0){
                    dig1 = 31;
                }
                if (dig2 == 0 && dig3 == 0 && dig4 == 0){
                    dig2 = 31;
                }
                if (dig3 == 0 && dig4 == 0){
                    dig3 = 31;
                }
                if (dig4 == 0){
                    dig4 =31;
                }
                SSD_WriteDigits(dig1, dig2, dig3, dig4, 0, dig5, 0, 0);
            }
        }
        
        
    }
    else{
        timer=0;
    }
    
    IFS0bits.T2IF = 0; // clear interrupt flag
    IEC0bits.T2IE = 1; // enable interrupt    I TURNED THESE OFF SINCE IM USING TIMER TWP, MAY CAUSE PROBLEMS
}

void __ISR(_TIMER_3_VECTOR) Timer3ISR(void) {
    //IEC0bits.T3IE = 0; // disable interrupt
    
    IFS0bits.T3IF = 0; // clear interrupt flag
   // IEC0bits.T3IE = 1; // enable interrupt
    
    if (LM == 2){
       OC4RS = (int) PR3 / 10; //LEFT MOTOR FORWARD FINAL
    }
    else if ( LM == 1 )//LEFT MOTOR REVERSE FINAL
    {
        OC4RS = (int) PR3 /40/3; 
       
    }
    else if (LM == 0){
       OC4RS = (int) PR3; //LEFT MOTOR STOP FINAL
    }
    
    if (RM == 2) {
        OC5RS = (int) PR3 / 20; //RIGHT MOTOR FORWARD FINAL
    }
    else if (RM == 1) { // RIGHT MOTOR REVERSE FINAL
        OC5RS = (int) PR3 / 10;
       
    }
    else if (RM == 0) {
        OC5RS = (int) PR3; //RIGHT MOTOR STOP FINAL
    }
    
    
}



void activateServo(){
    
    SSD_WriteDigits(dig1,dig2,dig3,dig4,0,dig5,0,0);
    /*if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 1 )
    {
            //left_motor_status = 2;
           // right_motor_status = 1;
           OC4RS = (int) PR3 / 10;
            OC5RS = (int) PR3 / 20;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 1 )
    {      OC4RS = (int) PR3 / 10;
            OC5RS = (int) PR3 / 20;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            delay_ms(60);
            OC4RS = (int) PR3 / 10;
            OC5RS = (int) PR3 / 10;
            delay_ms(40);
                    }
        
        
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 1)
        {
            OC4RS = (int) PR3 / 10;//maybe 20
            OC5RS = (int) PR3 / 10;// maybe 20
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("REV", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        }
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 1)
        {
            OC4RS = (int) PR3 / 10;
            
            OC4RS = (int) PR3 / 20;
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("REV", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
        }
        
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0 && var ==0)
        {
            OC4RS = (int) PR3 / 10;// forwarf or reve
            
            OC4RS = (int) PR3 / 20;//rev
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("REV", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 0;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            var++;
                    delay_ms(120);
            
        
            }
        
           
        
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0 &&var != 0)
        {
            OC4RS = 0;
            OC5RS = 0;
                    timer = 0;;
            //left_motor_status = 0;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("STP", 1, 0);
            LCD_WriteStringAtPos("STP", 1, 13);
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 0;
        }
         if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
         {
             OC4RS = (int) PR3 / 10;
            OC5RS = (int) PR3 / 20;
           // left_motor_status = 1;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 1;
            LATAbits.LATA6 = 1;
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 0;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            delay_ms(30);
            OC5RS = (int) PR3 / 20;
            OC4RS = (int) PR3 / 20;
                    delay_ms(40);
            
        }
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 0)
        {
            OC4RS = (int) PR3 / 10;
            OC5RS = (int) PR3 / 20;
            //left_motor_status = 2;
            //right_motor_status = 2;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA0 = 1;
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
        } 
    if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 1)
    {
            //left_motor_status = 1;
            //right_motor_status = 1;
            OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("REV", 1, 13);
            LATAbits.LATA7 = 1;
            LATAbits.LATA6 = 1;
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 0;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        } 
     if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
     {
         OC4RS = (int) PR3 / 10;
            OC5RS = (int) PR3 / 20  ;
            //left_motor_status = 2;
           // right_motor_status = 1;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 1;
            LATAbits.LATA6 = 1;
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 0;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        } 
    if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
    {
        OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            left_motor_status = 2;
            right_motor_status = 1;
            LCD_WriteStringAtPos("REV", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        }
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 0)
    {
        OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        }
        if(PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 0)
            
    {       OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        } 
        if (PMODS_GetValue(0,7)==0 && PMODS_GetValue(0,8)==0 && PMODS_GetValue(0,9)==1 && PMODS_GetValue(0,10) == 0){
            OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            
        }
        if(PMODS_GetValue(0,7)==1 && PMODS_GetValue(0,8)==1 && PMODS_GetValue(0,9)==0 && PMODS_GetValue(0,10) == 1){
            OC4RS = (int) PR3 / 20;
            OC5RS = (int) PR3 / 10;
            //left_motor_status = 2;
            //right_motor_status = 1;
            LCD_WriteStringAtPos("FWD", 1, 0);
            LCD_WriteStringAtPos("FWD", 1, 13);
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 0;
            LATAbits.LATA5 = 1;
            LATAbits.LATA4 = 1;
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
        }
}
            
        
    
    /*
        if(SW0 && !SW1){    //Right Motor FWD CW
            LCD_WriteStringAtPos("FWD",1,13);
            led3=led2=1;
            led1=led0=0;
            right_motor_status = 1 ;
             OC5RS = (int) PR3 / 20;
            
          // OC5RS = (int)(PR3 /23 ); // move forward
            
            
            //SRV_SetPulseMicroseconds1(300);
        }
        else if(!SW0 && SW1){   //Right Motor CCW
            LCD_WriteStringAtPos("REV",1,13);
            led1=led0=1;
            led3=led2=0;
            right_motor_status = 2;
           OC5RS = (int) PR3 / 10; //right reverse
            
            //SRV_SetPulseMicroseconds1(540);
        }
        else if(SW0 ^ !SW1){    //Right Motor STP
            LCD_WriteStringAtPos("STP",1,13);
            led0=led1=led2=led3=0;
            right_motor_status = 0;
            OC5RS = 0;
            //OC5RS = (int)(PR3 / (40.0 / 3.0));


            //OC5RS = 0;
            //SRV_SetPulseMicroseconds1(0);
        }

        if(SW6 && !SW7){
            LCD_WriteStringAtPos("FWD",1,0);   //Left Motor CCW
            led7=led6=0;
            led5=led4=1;
            left_motor_status = 1;
            OC4RS = (int) PR3 / 10; //left forward
            //SRV_SetPulseMicroseconds1(540);
        }
        else if(!SW6 && SW7){
            LCD_WriteStringAtPos("REV",1,0); //right motor  //Griffin- i changed the leds to 4 through 7 here Left Motor CW
            led5=led4=0;
            led7=led6=1;
            left_motor_status = 2;
            OC4RS = (int) PR3 / 20; //left reverse
          
            
            //SRV_SetPulseMicroseconds1(300);
        }
        else if(SW6 ^ !SW7){
            LCD_WriteStringAtPos("STP",1,0);   //Left Motor STP
            led4=led5=led6=led7=0;
            left_motor_status = 0;
            OC4RS = 0;
            
            
           // OC4RS = (int)(PR3 / (40.0 / 3.0)); makes the motors go


        }
         */
            
            
            //SRV_SetPulseMicroseconds1(0);
        

        // Activate servo based on SW6
        //activateServo();
    }
     


/*
void activateServo(){
    SSD_WriteDigits(dig1,dig2,dig3,dig4,0,dig5,0,0);
if(SW6 && !SW1){    //Right Motor FWD CW
            LCD_WriteStringAtPos("FWD",1,13);
            led3=led2=1;
            led1=led0=0;
            right_motor_status = 1 ;
          // OC5RS = (int)(PR3 /23 ); // move forward
            
            
            //SRV_SetPulseMicroseconds1(300);
        }
        else if(!SW0 && SW1){   //Right Motor CCW
            LCD_WriteStringAtPos("REV",1,13);
            led1=led0=1;
            led3=led2=0;
            right_motor_status = 2;
           // OC5RS = (int)(PR3 / 6);
            
            //SRV_SetPulseMicroseconds1(540);
        }
        else if(SW0 ^ !SW1){    //Right Motor STP
            LCD_WriteStringAtPos("STP",1,13);
            led0=led1=led2=led3=0;
            right_motor_status = 0;
           // OC5RS = (int)(PR340/3);
            //OC5RS = 0;
            //SRV_SetPulseMicroseconds1(0);
        }

        if(SW6 && !SW7){
            LCD_WriteStringAtPos("FWD",1,0);   //Left Motor CCW
            led7=led6=0;
            led5=led4=1;
            left_motor_status = 1;
          //  OC4RS = (int)(PR3 / 10);
            //SRV_SetPulseMicroseconds1(540);
        }
        else if(!SW6 && SW7){
            LCD_WriteStringAtPos("REV",1,0); //right motor  //Griffin- i changed the leds to 4 through 7 here Left Motor CW
            led5=led4=0;
            led7=led6=1;
            left_motor_status = 2;
          //  OC4RS = (int)(PR3 / 20);
            
            //SRV_SetPulseMicroseconds1(300);
        }
        else if(SW6 ^ !SW7){
            LCD_WriteStringAtPos("STP",1,0);   //Left Motor STP
            led4=led5=led6=led7=0;
            left_motor_status = 0;
         //  OC5RS = (int)(PR3 /(40/3);]
           //   OC4RS = 0;
            
            
            //SRV_SetPulseMicroseconds1(0);
        }

        // Activate servo based on SW6
        //activateServo();
    }
 */


   





