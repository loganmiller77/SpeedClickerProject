/* 
 * File:   LCD_Library.h
 * Author: logan
 *
 * Created on December 2, 2021, 11:38 AM
 */

#ifndef LCD_LIBRARY_H
#define	LCD_LIBRARY_H

#ifdef	__cplusplus
extern "C" {
#endif
    void initPIC24(void);
    void delay_ms(unsigned int ms);
    void lcd_cmd(char Package);
    void lcd_init(void);
    void lcd_setCursor(int x,int y);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char s[]);



#ifdef	__cplusplus
}
#endif

#endif	/* LCD_LIBRARY_H */

