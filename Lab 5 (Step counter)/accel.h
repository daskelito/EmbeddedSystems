#include <esp_pm.h>
#ifndef ACCEL_H_
#define ACCEL_H_

void initI2C(int sdapin, int sclpin);

void writeI2C(uint8_t address, uint8_t reg, uint8_t data);

void readI2C(uint8_t address, uint8_t reg, uint8_t *buffer, int len);

void axisValues();

#endif






