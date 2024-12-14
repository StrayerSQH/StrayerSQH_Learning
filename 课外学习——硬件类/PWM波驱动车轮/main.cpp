/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti/driverlib/dl_adc12.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti_msp_dl_config.h"
#include <random>
#include <stdio.h>

#define Frontleft 3200
#define Frontright 3200

uint32_t PWMADutyA;
uint32_t PWMBDutyB;
uint32_t pwmPeriod;

int setDuty(float dutyA, float dutyB)
{
    

    PWMADutyA = Frontleft * (1 - dutyA);
    PWMBDutyB = Frontright * (1 - dutyB);

    DL_TimerG_setCaptureCompareValue(PWM_INST, PWMADutyA, DL_TIMER_CC_0_INDEX);
    DL_TimerG_setCaptureCompareValue(PWM_INST, PWMBDutyB, DL_TIMER_CC_1_INDEX);
    pwmPeriod =3200 - DL_Timer_getCaptureCompareValue(CAPTURE_E1A_PWM_INST, DL_TIMER_CC_0_INDEX);

    return pwmPeriod;
}

int main(void)
{
    SYSCFG_DL_init();

    //初始化ADC
    NVIC_EnableIRQ(LIGHTSENSORS_INST_INT_IRQN);

    //初始化PWM波
    DL_TimerG_startCounter(PWM_INST);
    //初始化电机编码器返回波
    DL_TimerG_startCounter(CAPTURE_E1A_PWM_INST);

    //初始化状态灯、AIN、BIN值，使左右轮前进
    DL_GPIO_setPins(ABIN_PIN_AIN1_PIN_PORT, ABIN_PIN_AIN1_PIN_PIN);
    DL_GPIO_clearPins(ABIN_PIN_AIN2_PIN_PORT, ABIN_PIN_AIN2_PIN_PIN);
    DL_GPIO_setPins(ABIN_PIN_BIN1_PIN_PORT, ABIN_PIN_BIN1_PIN_PIN);
    DL_GPIO_clearPins(ABIN_PIN_BIN2_PIN_PORT, ABIN_PIN_BIN2_PIN_PIN);

    //定义变量
    int lightNum = 0;

    while (1) 
    {
        DL_ADC12_startConversion(LIGHTSENSORS_INST);

        int a = setDuty(0.2, 0.2);

        lightNum = DL_ADC12_getMemResult(LIGHTSENSORS_INST, DL_ADC12_MEM_IDX_2);
        if(lightNum < 500)
        {
           DL_GPIO_setPins(LEDS_PORT, LEDS_LED_STATE_PIN);
        }
        DL_ADC12_enableConversions(LIGHTSENSORS_INST);
    }
}

