/*
 * @file name: 
 * @Descripttion: 
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-03-17 10:42:50
 * @LastEditors: zgw
 * @LastEditTime: 2021-03-19 11:05:47
 */

#include "QMC5883L.h"
#include "soc_i2c.h"
#include "math.h"   


STATIC int __iic_start(VOID)
{
    vI2CSDASet();
    vI2CSCLSet();

    vI2CDelay(5);

    vI2CSDAReset();

    vI2CDelay(5);

    vI2CSCLReset();

    return 0;
}


STATIC int __iic_stop(VOID)
{   
    vI2CSCLReset();
    vI2CSDAReset();

    vI2CDelay(5);
    
    vI2CSCLSet();
    vI2CSDASet();

    vI2CDelay(5);

    return 0;     
}


STATIC VOID __iic_send_ack(VOID)
{   
    vI2CSCLReset();
        
    vI2CSDAReset();

    vI2CDelay(2);
    vI2CSCLSet();

    vI2CDelay(2);

    vI2CSCLReset();
}

STATIC VOID __iic_send_nack(VOID)
{   
    vI2CSCLReset();
        
    vI2CSDASet();

    vI2CDelay(2);
    vI2CSCLSet();

    vI2CDelay(2);

    vI2CSCLReset();
}

STATIC UINT8_T __iic_recv_ack(VOID)
{   
    UINT8_T ucErrTime=0;
    UINT8_T recv_ack;
    vI2CSDARelease();
    vI2CSCLSet();
    vI2CDelay(5);
    
    while(ucI2CSDAInputRead()) {
        ucErrTime++;
        if(ucErrTime>250) {
			__iic_stop();
            PR_NOTICE("-------iic ack error-----");
			return 1;
		}
    }

    
    vI2CSCLReset();

    return 0;
}

STATIC VOID __iic_send_byte(UCHAR_T sendbyte)
{
    UCHAR_T i = 0;
    vI2CSCLReset;
    for(i = 0x80; i > 0; i >>= 1)
    {
        if((sendbyte & i) == 0) {
            vI2CSDAReset();
        } else {
            vI2CSDASet();
        }
        vI2CDelay(2);   
        vI2CSCLSet();
        vI2CDelay(2); //vI2CDelay(5);
        vI2CSCLReset();
        vI2CDelay(2);
    }
}

STATIC UINT8_T __iic_read_byte(UCHAR_T ack)
{
    UCHAR_T i = 0;
    
    UCHAR_T readbyte = 0;
    vI2CSDARelease();
    for(i = 0x80; i > 0; i >>= 1)
    {
        vI2CSCLReset();
        vI2CDelay(2);
        vI2CSCLSet();
        if(ucI2CSDAInputRead()) {
            readbyte |= i;    
        }
        vI2CDelay(2);
    }
    if(!ack) {
        __iic_send_nack(); 
    }else {
        __iic_send_ack();
    }
    
    return readbyte;    
}


VOID qmc5883l_send_cmd(UINT8_T drv_addr, UINT16_T write_addr, UINT8_T data)
{   
    __iic_start();

    __iic_send_byte(drv_addr);  //发生写命令
    __iic_recv_ack();
    __iic_send_byte(write_addr);
    __iic_recv_ack();
    __iic_send_byte(data);
    __iic_recv_ack();  

    __iic_stop();
    vI2CDelay(2);
}

UINT8_T qmc5883l_read_mul_byte(UINT8_T drv_addr, UINT16_T read_addr, UINT8_T *param, UINT16_T byte_num)
{
    UINT8_T ret = 0;

    __iic_start();
    __iic_send_byte(drv_addr);
    __iic_recv_ack();
    __iic_send_byte(read_addr);
    __iic_recv_ack();    

    __iic_start();
    __iic_send_byte(drv_addr + 1);
    __iic_recv_ack();   

    while (byte_num)
    {
        if(byte_num == 1) {
            *param = __iic_read_byte(0);   
        }else {
            *param = __iic_read_byte(1);
        }
        param++;
        byte_num--;
    }
    __iic_stop();

    return ret;
}

VOID qmc5883l_init(qmc5883l_init_t* param)
{
    UINT8_T config = 0;
    UINT8_T ret = 0;

    i2c_pin_t i2c_config = {
        .ucSDA_IO = param ->SDA_PIN,
        .ucSCL_IO = param ->SCL_PIN,
    };
    
    opSocI2CInit(&i2c_config);          /* SDA&SCL GPIO INIT */
    vI2CDelay(1000);
    
    qmc5883l_send_cmd(QMC5883L_ADDRESS, 0x0B, 0x01);
    qmc5883l_send_cmd(QMC5883L_ADDRESS, 0x20, 0x40);
    qmc5883l_send_cmd(QMC5883L_ADDRESS, 0x21, 0x01);

    config = OVER_SAMPLE_RATIO_512|FULL_SCALE_2G|OUTPUT_DATA_RATE_10HZ|MODE_CONTROL_CONTINUOUS;
    qmc5883l_send_cmd(QMC5883L_ADDRESS, 0x09, config);

    vI2CDelay(50);
}

VOID qmc5883l_get_angle(float_xyz* data)
{
    UINT8_T mag[6],temp;
    i16_xyz mag_data;
    qmc5883l_read_mul_byte(QMC5883L_ADDRESS,0x06,&temp,1);

    qmc5883l_read_mul_byte(QMC5883L_ADDRESS,0x00,mag,6);

    mag_data.x = (INT16_T)(mag[1] << 8 |  mag[0]);
    mag_data.y = (INT16_T)(mag[3] << 8 |  mag[2]);
    mag_data.z = (INT16_T)(mag[5] << 8 |  mag[4]);

    data->x = atan2((double)mag_data.y,(double)mag_data.x)*573 + 1800;

    
}