#include <unistd.h>

#include "log.h"
#include "wit_c_sdk.h"

#include "navi-config.h"

#include "wt_jy901.h"

wt_jy901_data_t g_jy901_data; // global jy901 data

static void jy901_data_update(uint32_t uiReg, uint32_t uiRegNum)
{
    log_debug("jy901_data_update triggered!");
    for (int i = 0; i < uiRegNum; i++)
    {
        switch (uiReg)
        {
        case AX:
            g_jy901_data.Acc[0] = sReg[AX] / 32768.0f * 16.0f;
            break;
        case AY:
            g_jy901_data.Acc[1] = sReg[AY] / 32768.0f * 16.0f;
            break;
        case AZ:
            g_jy901_data.Acc[2] = sReg[AZ] / 32768.0f * 16.0f;
            break;
        case GX:
            g_jy901_data.Gyro[0] = sReg[GX] / 32768.0f * 2000.0f;
            break;
        case GY:
            g_jy901_data.Gyro[1] = sReg[GY] / 32768.0f * 2000.0f;
            break;
        case GZ:
            g_jy901_data.Gyro[2] = sReg[GZ] / 32768.0f * 2000.0f;
            break;
        case HX:
            break;
        case HY:
            break;
        case HZ:
            break;
        case Roll:
            g_jy901_data.Angle[0] = sReg[Roll] / 32768.0f * 180.0f;
            break;
        case Pitch:
            g_jy901_data.Angle[1] = sReg[Pitch] / 32768.0f * 180.0f;
            break;
        case Yaw:
            g_jy901_data.Angle[2] = sReg[Yaw] / 32768.0f * 180.0f;
            break;
        default:
            break;
        }
        uiReg++;
    }
}

static void jy901_delay_ms(uint16_t ucMs)
{
    usleep(ucMs * 1000);
}


#if SUB_NAVI_CONFIG_JY901_USE_IIC

#include "wiringPiI2C.h"

static int jy901_iic;

static int32_t jy901_i2c_read(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen)
{
    if (wiringPiI2CReadBlockData(jy901_iic, ucReg, p_ucVal, uiLen) < 0)
    {
        return WIT_HAL_ERROR;
    }
    return WIT_HAL_OK;
}

static int32_t jy901_i2c_write(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen)
{
    if (wiringPiI2CWriteBlockData(jy901_iic, ucReg, p_ucVal, uiLen) < 0)
    {
        return WIT_HAL_ERROR;
    }
    return WIT_HAL_OK; // 1 on success, 0 on failure
}

navi_ret_t navi_jy901_init(void)
{
    jy901_iic = wiringPiI2CSetupInterface(SUB_NAVI_CONFIG_JY901_IIC_PATH, SUB_NAVI_CONFIG_JY901_IIC_ADDR);
    if (jy901_iic < 0)
    {
        return NAVI_RET_FAIL;
    }

    // wit sdk init
    if (WIT_HAL_OK != WitInit(WIT_PROTOCOL_I2C, SUB_NAVI_CONFIG_JY901_IIC_ADDR)) // 0x50 is 7 bit iic address of jy901
    {
        return NAVI_RET_FAIL;
    }
    WitDelayMsRegister(jy901_delay_ms);
    WitRegisterCallBack(jy901_data_update);
    WitI2cFuncRegister(jy901_i2c_write, jy901_i2c_read); // register iic read/write callback
    return NAVI_RET_SUCCESS;
}


#else

#include "ev.h"
#include "wiringSerial.h"

static int jy901_uart;
static ev_io uart_watcher;

static void jy901_uart_write(uint8_t *p_ucData, uint32_t uiLen)
{
    write(jy901_uart, p_ucData, uiLen);
}

static void uart_read_cb (EV_P_ ev_io *w, int revents)
{
    log_debug("uart EV_READ event");
    uint8_t data;
    while (read(w->fd, &data, 1) == 1)
    {
        WitSerialDataIn(data);
    }
}

navi_ret_t navi_jy901_init(void)
{
    // interface init
    jy901_uart = serialOpen(SUB_NAVI_CONFIG_JY901_UART_PATH, SUB_NAVI_CONFIG_JY901_UART_BAUDRATE);
    if (jy901_uart < 0)
    {
        log_error("jy901 uart %s open failed.", SUB_NAVI_CONFIG_JY901_UART_PATH);
        return NAVI_RET_FAIL;
    }

    // wit sdk init
    if (WIT_HAL_OK != WitInit(WIT_PROTOCOL_NORMAL, 0x50)) // 0x50 is 7 bit iic address of jy901
    {
        log_error("wit sdk init failed.");
        return NAVI_RET_FAIL;
    }
    WitDelayMsRegister(jy901_delay_ms);
    WitRegisterCallBack(jy901_data_update);
    WitSerialWriteRegister(jy901_uart_write); // register uart write callback

    EV_P = EV_DEFAULT;
    ev_io_init(&uart_watcher, uart_read_cb, jy901_uart, EV_READ);
    ev_io_start(EV_A_ &uart_watcher);
    log_info("jy901 data watcher on uart %s.", SUB_NAVI_CONFIG_JY901_UART_PATH);
    return NAVI_RET_SUCCESS;
}


#endif
