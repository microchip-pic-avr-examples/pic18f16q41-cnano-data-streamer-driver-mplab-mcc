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
#include "mcc_generated_files/system/system.h"

/*Header file included, since MCC components without an init function are not automatically declared as part of SYSTEM_Initialize()*/
#include "mcc_generated_files/data_streamer/data_streamer.h"

volatile uint8_t sendflag = 0; /*Global variable declared as volatile */
uint8_t count = 0;
uint16_t count16 = 0;
uint32_t count32 = 0;            
float    count_f = 1.5;


void Timer_Callback_100ms(void){
    LED_Toggle();    
    DebugIO_Toggle();
    
    count +=5;             /*Update variable values */
    count16 += 1000;
    count32 += 50000000;
    count_f += 1.5;
    sendflag = 1;         /* Variable values updated, a new Data Streamer frame can be sent. */
    
    //printf("Hello from the callback!");
}

int main(void)
{
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();       /* Enable the Global Interrupts  */
    
    Timer.TimeoutCallbackRegister(Timer_Callback_100ms);
    
    while(1){
        if(sendflag){
            variableWrite_SendFrame(count, count16, count32, count_f);
            sendflag = 0;
        }
    }    
}






