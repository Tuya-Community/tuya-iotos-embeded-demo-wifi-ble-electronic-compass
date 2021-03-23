/*
 * @file name: 
 * @Descripttion: 
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-03-17 10:43:08
 * @LastEditors: zgw
 * @LastEditTime: 2021-03-17 15:04:52
 */

#ifndef __QMC5883L_H__
#define __QMC5883L_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

typedef struct 
{
    UCHAR_T SDA_PIN;            ///< SDA pin
    UCHAR_T SCL_PIN;            ///< SCL pin 
}qmc5883l_init_t;

typedef enum
{
    no_ack = 0,
    ack
}Ack_NAck;

typedef struct 
{
	float x;
	float y;
	float z;
}float_xyz;

typedef struct 
{
	INT16_T x;
	INT16_T y;
	INT16_T z;
}i16_xyz;

typedef enum STATUS_VARIABLES
{
	NORMAL,
	NO_NEW_DATA,
	NEW_DATA_IS_READY,
	DATA_OVERFLOW,
	DATA_SKIPPED_FOR_READING
}_qmc5883l_status;

typedef enum MODE_VARIABLES
{
	MODE_CONTROL_STANDBY=0x00,
	MODE_CONTROL_CONTINUOUS=0x01
}_qmc5883l_MODE;

typedef enum ODR_VARIABLES
{
	OUTPUT_DATA_RATE_10HZ=0x00,
	OUTPUT_DATA_RATE_50HZ=0x04,
	OUTPUT_DATA_RATE_100HZ=0x08,
	OUTPUT_DATA_RATE_200HZ=0x0C
}_qmc5883l_ODR;

typedef enum RNG_VARIABLES
{
	FULL_SCALE_2G=0x00,
	FULL_SCALE_8G=0x10
}_qmc5883l_RNG;


typedef enum OSR_VARIABLES
{
	OVER_SAMPLE_RATIO_512=0x00,
	OVER_SAMPLE_RATIO_256=0x40,
	OVER_SAMPLE_RATIO_128=0x80,
	OVER_SAMPLE_RATIO_64=0xC0
}_qmc5883l_OSR;


typedef enum INTTERRUPT_VARIABLES
{
	INTERRUPT_DISABLE,
	INTERRUPT_ENABLE
}_qmc5883l_INT;

#define QMC5883L_ADDRESS  		0x1A

#define QMC5883L_DATA_READ_X_LSB	0x00
#define QMC5883L_DATA_READ_X_MSB	0x01
#define QMC5883L_DATA_READ_Y_LSB	0x02
#define QMC5883L_DATA_READ_Y_MSB	0x03
#define QMC5883L_DATA_READ_Z_LSB	0x04
#define QMC5883L_DATA_READ_Z_MSB	0x05
#define QMC5883L_STATUS		        0x06 // DOR | OVL | DRDY
#define QMC5883L_TEMP_READ_LSB		0x07
#define QMC5883L_TEMP_READ_MSB		0x08 
#define QMC5883L_CONFIG_1					0x09 // OSR | RNG | ODR | MODE
#define QMC5883L_CONFIG_2					0x0A // SOFT_RST | ROL_PNT | INT_ENB
#define QMC5883L_CONFIG_3					0x0B // SET/RESET Period FBR [7:0]
#define QMC5883L_ID								0x0D

#ifndef M_PI 
#define M_PI 3.14159265358979323846264338327950288f 
#endif 

#define QMC5883L_SCALE_FACTOR 		0.732421875f
#define QMC5883L_CONVERT_GAUSS_2G 	12000.0f
#define QMC5883L_CONVERT_GAUSS_8G 	3000.0f
#define QMC5883L_CONVERT_MICROTESLA 	100
#define QMC5883L_DECLINATION_ANGLE	93.67/1000  // radian, Tekirdag/Turkey


VOID qmc5883l_init(qmc5883l_init_t* param);
VOID qmc5883l_get_angle(float_xyz* data);

#endif