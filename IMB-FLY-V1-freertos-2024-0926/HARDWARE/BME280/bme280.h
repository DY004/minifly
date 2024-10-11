#ifndef __BME280_H
#define __BME280_H
#include "sys.h"
#include "stdbool.h"

#define BME280_ADDR						(0x76)
#define BME280_DEFAULT_CHIP_ID			(0x60)

#define BME280_CHIP_ID					(0xD0)                                 /* Chip ID Register */
#define BME280_RST_REG					(0xE0)                                 /* Softreset Register */
#define BME280_CTRL_HUM                 (0xF2)                                 /* Ctrl Humidity Register */
#define BME280_STAT_REG					(0xF3)                                 /* Status Register */
#define BME280_CTRL_MEAS_REG			(0xF4)                                 /* Ctrl Measure Register */
#define BME280_CONFIG_REG				(0xF5)                                 /* Configuration Register */
#define BME280_PRESSURE_MSB_REG			(0xF7)                                 /* Pressure MSB Register */
#define BME280_PRESSURE_LSB_REG			(0xF8)                                 /* Pressure LSB Register */
#define BME280_PRESSURE_XLSB_REG		(0xF9)                                 /* Pressure XLSB Register */
#define BME280_TEMPERATURE_MSB_REG		(0xFA)                                 /* Temperature MSB Reg */
#define BME280_TEMPERATURE_LSB_REG		(0xFB)                                 /* Temperature MSB Reg */
#define BME280_TEMPERATURE_XLSB_REG		(0xFC)                                 /* Temperature XLSB Reg */
#define BME280_HUMIDITY_MSB_REG			(0xFD)                                 /* Humidity MSB Reg */
#define BME280_HUMIDITY_LSB_REG		    (0xFE)                                 /* Humidity LSB Reg */

#define BME280_SLEEP_MODE				(0x00)
#define BME280_FORCED_MODE				(0x01)
#define BME280_NORMAL_MODE				(0x03)

#define BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG             (0x88)
#define BME280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH       (32)
#define BME280_DATA_FRAME_SIZE			(8)

#define BME280_OVERSAMP_SKIPPED			(0x00)
#define BME280_OVERSAMP_1X				(0x01)
#define BME280_OVERSAMP_2X				(0x02)
#define BME280_OVERSAMP_4X				(0x03)
#define BME280_OVERSAMP_8X				(0x04)
#define BME280_OVERSAMP_16X				(0x05)

typedef struct 
{
    u16 dig_T1;                                                                /* calibration T1 data */
    s16 dig_T2;                                                                /* calibration T2 data */
    s16 dig_T3;                                                                /* calibration T3 data */
    u16 dig_P1;                                                                /* calibration P1 data */
    s16 dig_P2;                                                                /* calibration P2 data */
    s16 dig_P3;                                                                /* calibration P3 data */
    s16 dig_P4;                                                                /* calibration P4 data */
    s16 dig_P5;                                                                /* calibration P5 data */
    s16 dig_P6;                                                                /* calibration P6 data */
    s16 dig_P7;                                                                /* calibration P7 data */
    s16 dig_P8;                                                                /* calibration P8 data */
    s16 dig_P9;                                                                /* calibration P9 data */
	u8  dig_H1;                                                                /* calibration H1 data */
	s16 dig_H2;                                                                /* calibration H2 data */
	u8  dig_H3;                                  							   /* calibration H3 data */
	s16 dig_H4;                                                                /* calibration H4 data */
	s16 dig_H5;                                                                /* calibration H5 data */
	u8  dig_H6;                                                                /* calibration H6 data */
    s32 t_fine;                                                                /* calibration t_fine data */
} bme280Calib;

void bme280Init(void);
void bme280GetData(float* pressure, float* temperature,float* humidity,float* asl);

#endif


