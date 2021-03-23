/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: sht21.h
 * @Description: SHT21 IIC drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 2021-03-17 17:29:32
 */

#include "app_control.h"
#include "tuya_gpio.h"
#include "tuya_uart.h"
#include "BkDriverUart.h"
#include "sys_timer.h"
#include "uni_time.h"
#include "app_pwm.h"
#include "QMC5883L.h"
/***********************************************************
*************************types define***********************
***********************************************************/
typedef enum
{
    LOW = 0,
    HIGH,
}default_level;

APP_CTRL_DATA_T app_ctrl_data = {0};
/***********************************************************
*************************IO control device define***********
***********************************************************/


/***********************************************************
*************************about adc init*********************
***********************************************************/


/***********************************************************
*************************about iic init*********************
***********************************************************/
#define IIC_SDA_PIN                         (6)
#define IIC_SCL_PIN                         (7)

STATIC qmc5883l_init_t qmc5883l_init_param = {IIC_SDA_PIN, IIC_SCL_PIN};

float_xyz mag_angle;

/***********************************************************
*************************function***************************
***********************************************************/

STATIC VOID __ctrl_gpio_init(CONST TY_GPIO_PORT_E port, CONST BOOL_T high)
{   
    //Set IO port as output mode
    tuya_gpio_inout_set(port, FALSE);
    //Set IO port level
    tuya_gpio_write(port, high);
}


VOID app_device_init(VOID)
{
    INT_T op_ret = 0;
    tuya_hal_system_sleep(1000);
    qmc5883l_init(&qmc5883l_init_param);
    tuya_hal_system_sleep(1000);
    qmc5883l_get_angle(&mag_angle);
}


VOID app_ctrl_handle(VOID)
{
    qmc5883l_get_angle(&mag_angle);
    PR_NOTICE("-----------angle = %f----------",mag_angle.x);
}

VOID app_ctrl_all_off(VOID)
{   

}

 