#include "accel.h"
#include <math.h>
#include <esp_pm.h>
#include <driver/i2c.h>
#include "circular_buffer.h"
#include "pins.h"
#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>

#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_SMPLRT_DIV 0x19

uint8_t getAcc;
int16_t totalAcc;
struct circularBuffer cBuffer;
volatile i2c_cmd_handle_t cmd;
volatile esp_err_t res;

/*
* Initialises I2C bus, uses controller 0
*/
void initI2C(int sdapin, int sclpin)
{
    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = sdapin;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_io_num = sclpin;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = 100000;

    //controller 0
    esp_err_t res = i2c_param_config(I2C_NUM_0, &config);
    ESP_ERROR_CHECK(res);

    //install driver
    res = i2c_driver_install(I2C_NUM_0, config.mode, 0, 0, 0);
    ESP_ERROR_CHECK(res);
    writeI2C(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00); // power mode awake

    writeI2C(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 250); // divide by 250 to get 32 *** (K?) Hz
}

/**
* Writes one byte into a register
*/
void writeI2C(uint8_t address, uint8_t reg, uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, 1);

    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, reg, 1);
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, data, 1);
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
}

/**
* Reads len bytes and places them into a buffer, buffer must be pre-allocated
*/
void readI2C(uint8_t address, uint8_t reg, uint8_t *buffer, int len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, 1);

    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, reg, 1);
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_READ, 1);

    ESP_ERROR_CHECK(res);
    res = i2c_master_read(cmd, buffer, len, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
}

void axisValues()
{
    uint32_t result;
    volatile int z = 0;
    volatile int x = 0;
    volatile int y = 0;
    readI2C(MPU6050_ADDR, 0x3c, &getAcc, 1);
    totalAcc = getAcc;
    readI2C(MPU6050_ADDR, 0x3b, &getAcc, 1);
    // combine high and low registers into a signed integer
    totalAcc |= ((int16_t)getAcc) << 8;  //8-bit to 16-bit convertion by bit shifting
    x = totalAcc;

    readI2C(MPU6050_ADDR, 0x3e, &getAcc, 1);
    totalAcc = getAcc;
    readI2C(MPU6050_ADDR, 0x3d, &getAcc, 1);
    // combine high and low registers into a signed integer
    totalAcc |= ((int16_t)getAcc) << 8;
    y = totalAcc;

    readI2C(MPU6050_ADDR, 0x40, &getAcc, 1);
    totalAcc = getAcc;
    readI2C(MPU6050_ADDR, 0x3f, &getAcc, 1);
    // combine high and low registers into a signed integer
    totalAcc |= ((int16_t)getAcc) << 8;
    z = totalAcc;

    result = (uint32_t)sqrt((x * x) + (y * y) + (z * z));
    addElement(&cBuffer, result);
}
