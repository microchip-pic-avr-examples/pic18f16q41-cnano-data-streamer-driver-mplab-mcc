/**
  TMR0 Generated Driver File
 
  @Company
    Microchip Technology Inc.
 
  @File Name
    tmr0.c
 
  @Summary
    This is the generated driver implementation file for the TMR0 driver
 
  @Description
    This source file provides APIs for driver for TMR0.
    Generation Information :
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 v2.31
        MPLAB             :  MPLAB X v5.45
*/

/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/

#include <xc.h>
#include "../tmr0.h"


const struct TMR_INTERFACE Timer = {
    .Initialize = Timer_Initialize,
    .Start = Timer_Start,
    .Stop = Timer_Stop,
    .PeriodCountSet = Timer_Write,
    .TimeoutCallbackRegister = Timer_OverflowCallbackRegister,
    .Tasks = NULL
};

static void (*Timer_OverflowCallback)(void);
static void Timer_DefaultOverflowCallback(void);

void Timer_Initialize(void)
{
    //TMR0H 192; 
    TMR0H = 0xC0;

    //TMR0L 0; 
    TMR0L = 0x0;

    //T0CS LFINTOSC; T0CKPS 1:16; T0ASYNC synchronised; 
    T0CON1 = 0x84;


    //Set default callback for TMR0 overflow interrupt
    Timer_OverflowCallbackRegister(Timer_DefaultOverflowCallback);

    //Clear Interrupt flag before enabling the interrupt
    PIR3bits.TMR0IF = 0;
	
    //Enable TMR0 interrupt.
    PIE3bits.TMR0IE = 1;
	
    //T0OUTPS 1:1; T0EN enabled; T016BIT 8-bit; 
    T0CON0 = 0x80;
}

void Timer_Start(void)
{
    T0CON0bits.T0EN = 1;
}

void Timer_Stop(void)
{
    T0CON0bits.T0EN = 0;
}

uint8_t Timer_Read(void)
{
    uint8_t readVal;

    //Read TMR0 register, low byte only
    readVal = TMR0L;

    return readVal;
}

void Timer_Write(size_t timerVal)
{
    //Write to TMR0 register, low byte only
    TMR0L = (uint8_t)timerVal;
 }

void Timer_Reload(uint8_t periodVal)
{
   //Write to TMR0 register, high byte only
   TMR0H = periodVal;
}

void Timer_OverflowISR(void)
{
    //Clear the TMR0 interrupt flag
    PIR3bits.TMR0IF = 0;
    if(Timer_OverflowCallback)
    {
        Timer_OverflowCallback();
    }
}

void Timer_OverflowCallbackRegister(void (* CallbackHandler)(void))
{
    Timer_OverflowCallback = CallbackHandler;
}

static void Timer_DefaultOverflowCallback(void)
{
    //Add your interrupt code here or
    //Use Timer_OverflowCallbackRegister function to use Custom ISR
}

