/*
 * File:   mill8854_lab6_main_v001.c
 * Authors: Logan Miller and Andrew Forner
 *
 * Created on November 17, 2021, 9:39 PM
 */


#include "xc.h"
#include "LCD_Library.h"
#include "stdio.h"
#include "ASMfinalproject.h"
#include "LEDStripLib.h"
#define OFF 0
#define ON 1
#define BLUE 1
#define RED 0
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
volatile unsigned int tug=80;
volatile unsigned int game=OFF;
/*Function: initPushButton1blue
 Uses PPS to set input capture pin for blue button and sets up input capture 1 interrupts*/
void initPushButton1blue(void) //Logan
{
    CNPU2bits.CN22PUE=1; //set pull up resistor for pin RB8
   __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8;  // Use Pin RP8 = "8", for Input Capture 1 (Table 10-2)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
   IC1CON = 0; // Turn off and reset internal state of IC1
   IC1CONbits.ICTMR = 1; // Use Timer 2 for capture source
   IC1CONbits.ICM = 0b011; // Turn on and capture every falling edge
   IFS0bits.IC1IF=0; //set interrupt flag to 0
   IEC0bits.IC1IE=1; //enable interrupt   
}
/*Function: initPushButton2red
 Uses PPS to set input capture pin for red button and set up input capture 2 interrupts*/
void initPushButton2red(void) //Logan
{
    CNPU2bits.CN24PUE=1; //set pull up resistor for pin RB6
   __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC2R = 6;  // Use Pin RP8 = "6", for Input Capture 2 (Table 10-2)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
   IC2CON = 0; // Turn off and reset internal state of IC1
   IC2CONbits.ICTMR = 1; // Use Timer 2 for capture source
   IC2CONbits.ICM = 0b011; // Turn on and capture every falling edge
   IFS0bits.IC2IF=0; //set interrupt flag to 0
   IEC0bits.IC2IE=1; //enable interrupt   
}
void __attribute__((interrupt, auto_psv)) _IC1Interrupt() //Logan
{
    _IC1IF=0;//clear flag
    tug+=1;//one closer to blue winning
    delay_ms(10);
}
void __attribute__((interrupt, auto_psv)) _IC2Interrupt() //Logan
{
    _IC2IF=0;//clear flag
    tug-=1;
    delay_ms(10);
}
void StartGame (void) //Both
{
    while(tug==80 && game==OFF)
    {
        
        lcd_setCursor(0,0);
        lcd_printStr("Press to");
        lcd_setCursor(1,2);
        lcd_printStr("Start");
    }
   
    game=ON;//StartGame function can no longer run unless the game finishes
}
void EndGame(int won)//Andrew 
{
    int i;
    int u;
    if (won == RED) {
            //write "Red wins"
        lcd_cmd(0b00000001); // Clear Display
        lcd_setCursor(0,0);
        lcd_printStr("Red");
        lcd_setCursor(1,3);
        lcd_printStr("Wins!");
            
            for (u=0; u<5; u++) {
                for (i=0; i<16; i++) {
                    writeColor(0, 255, 0);
                }
                delay_ms(1000);
                for (i=0; i<16; i++) {
                    writeColor(0, 0, 0);
                }
                delay_ms(1000);
            }
            game = OFF;
            tug = 80;
        }
    else if (won == BLUE) {
            //write "Blue Wins
        lcd_cmd(0b00000001); // Clear Display
        lcd_setCursor(0,0);
        lcd_printStr("Blue");
        lcd_setCursor(1,3);
        lcd_printStr("Wins!");
        
            for (u=0; u<5; u++) {
                for (i=0; i<16; i++) {
                    writeColor(0, 0, 255);
                }
                delay_ms(1000);
                for (i=0; i<16; i++) {
                    writeColor(0, 0, 0);
                }
                delay_ms(1000);
            }
            game = OFF;
            tug = 80;
        }
    writeLEDStrip(80);
    lcd_cmd(0b00000001); // Clear Display
    StartGame();
    lcd_cmd(0b00000001); // Clear Display
    lcd_startSequence();
    tug=80;//reset tug to be equal parts red,blue
    game=ON;//StartGame function can no longer run unless the game finishes
}
int main(void) //Both
{
    initPIC24();
    lcd_init();
    initPushButton1blue();
    initPushButton2red();
    writeLEDStrip(80);//half red, half blue
    StartGame();//wait until somebody clicks a button to start the game
    lcd_cmd(0b00000001);//clear lcd screen
    lcd_startSequence();//print "Ready Set Go!" on LCD
    tug=80;//reset tug to be equal parts red,blue
    while (1)
    {
        writeLEDStrip(tug);
        if (tug <= 0 ) {
            EndGame(RED);
        }
        else if (tug > 1000) {
            EndGame(RED);
        }
        else if (tug >= 160) {
            EndGame(BLUE);
        }
        
    }
    return 0;
}
