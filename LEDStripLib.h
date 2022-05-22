/* 
 * File:   LEDStripLib.h
 * Author: Logan Miller
 *
 * Created on December 9, 2021, 12:12 PM
 */

#ifndef LEDSTRIPLIB_H
#define	LEDSTRIPLIB_H

#ifdef	__cplusplus
extern "C" {
#endif
    void writeColor(int g, int r, int b);
    void writeLEDStrip(unsigned int count);



#ifdef	__cplusplus
}
#endif

#endif	/* LEDSTRIPLIB_H */

