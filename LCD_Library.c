/*
 * File:   LCD_Library.c
 * Author: Logan Miller and Andrew Forness
 *
 * Created on December 2, 2021, 11:35 AM
 */

#include "xc.h"


void delay_ms(unsigned int ms) {
    while(ms-- > 0) {
        asm("repeat #15998");
        asm("nop"); 
    }
}

void lcd_cmd(char Package) {
    I2C2CONbits.SEN = 1;   //start condition
    while (I2C2CONbits.SEN == 1) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; //slave address and R/nW bit
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000; //control byte
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package;
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1) ;
}

void lcd_init(void) {
    I2C2BRG = 157;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1;
    
    delay_ms(40);
    
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    delay_ms(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    delay_ms(2);

}

void lcd_setCursor(int x, int y) {
    char Cursor = 0x0000; //initialize cursor as empty
    
    if (x==1){
        Cursor = 0b01000000; //to say which row the character should be displayed on
    }
    
    Cursor |= y; //to say which column the character should be displayed on
    Cursor |= 0b10000000; //"set ddram address" command
    lcd_cmd(Cursor);
}

void lcd_printChar(char myChar) {
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000;
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar;
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1) ;
}

void lcd_printStr(const char s[]) {
    int i=0;
    
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    while ((s[i+1]) != '\0') {
        I2C2TRN = 0b11000000;
        while (IFS3bits.MI2C2IF == 0) ;
        IFS3bits.MI2C2IF = 0;
        I2C2TRN = s[i];
        while (IFS3bits.MI2C2IF == 0) ;
        IFS3bits.MI2C2IF = 0;
        i++;
    }
    I2C2TRN = 0b01000000;
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = s[i];
    while (IFS3bits.MI2C2IF == 0) ;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1) ;
}
/*Function: initPIC24
 initializes the PIC24 clock and pins*/
void initPIC24(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xFFFF; //makes pins digital mode
    TRISA=0xFFFE;
    LATA=0x0000;
}
void lcd_startSequence(void) //Logan
{
    lcd_setCursor(0,0);
    
    lcd_printStr("  Ready");
       delay_ms(3000);
       
       lcd_setCursor(1,0);
       lcd_printStr("Set");
       delay_ms(3000);
       
       lcd_setCursor(1,5);
       lcd_printStr("Go!");
}
