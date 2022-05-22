/*
 * File:   LEDStripLib.c
 * Author: Logan Miller
 *
 * Created on December 9, 2021, 12:06 PM
 */


#include "xc.h"
#include "ASMfinalproject.h"
void writeColor(int g, int r, int b) //Andrew  
{
    int checker=128 , u=0;
    for(u=0; u<8; u++) {
        if ((g&checker)==checker) {
            write_1();
            checker = checker/2;
        }
        else {
            write_0();
            checker=checker/2;
        }
    }
    checker=128;
    for(u=0; u<8; u++) {
        if ((r&checker)==checker) {
            write_1();
            checker = checker/2;
        }
        else {
            write_0();
            checker=checker/2;
        }
    }
    checker=128;
    for(u=0; u<8; u++) {
        if ((b&checker)==checker) {
            write_1();
            checker = checker/2;
        }
        else {
            write_0();
            checker=checker/2;
        }
    }
}
void writeLEDStrip(unsigned int count)//Andrew
{
        int i;
        int u;
        u = count/10;
        for (i=0; i<u; i++) {
            writeColor(0,0,255);
        }
        writeColor(0, (255-(28*(count % 10))), 28*(count % 10));
        for (u; (u+1)<16; u++) {
            writeColor(0,255,0);
        }
        wait_100microseconds();
}

