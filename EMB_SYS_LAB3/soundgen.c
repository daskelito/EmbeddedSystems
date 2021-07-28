#include <stdio.h>
#include "driver/dac.h"
#include <esp_task_wdt.h>
#include "driver/adc.h"
#include "soc/syscon_reg.h"
#include "soundgen.h"
#include "soc/sens_reg.h"

//Setup and start of the DAC
int startSound(int freq)
{
    int inputSound = (freq * 65536) / (8000000);
    REG_SET_FIELD(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN, 1 );
    REG_SET_FIELD(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1, 1);    
    REG_SET_FIELD(SENS_SAR_DAC_CTRL1_REG, SENS_SW_FSTEP, inputSound);
    REG_SET_FIELD(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 10 );
    REG_SET_FIELD(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE1, 0);
    REG_SET_FIELD(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC1, 0);
    dac_output_enable(DAC_CHANNEL_1);
    return (inputSound * 122);
}

//Switches of the DAC
void stopSound()
{
    dac_output_disable(DAC_CHANNEL_1);
}