#include "ev.h"
#include "log.h"
#include "linux_uart.h"
#include "wit_c_sdk.h"

#include "navi-config.h"

#include "wt_jy901.h"

wt_jy901_data_t g_jy901_data; // global jy901 data

static HARDWARE_UART jy901_uart;
static ev_io uart_watcher;

static void jy901_uart_write(uint8_t *p_ucData, uint32_t uiLen)
{
    navi_uart_write(jy901_uart, p_ucData, uiLen);
}

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
            g_jy901_data.Angle[2] = sReg[Roll] / 32768.0f * 180.0f;
            break;
        case Pitch:
            g_jy901_data.Angle[2] = sReg[Pitch] / 32768.0f * 180.0f;
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

static void uart_read_cb (EV_P_ ev_io *w, int revents)
{
    log_debug("uart EV_READ event");
    uint8_t data;
    while (read(w->fd, &data, 1) == 1)
    {
        WitSerialDataIn(data);
    }
}

int navi_jy901_init(void)
{
    // interface init
    if (0 != navi_uart_begin(&jy901_uart, SUB_NAVI_CONFIG_JY901_UART_PATH))
    {
        return -1;
    }
    if (0 != navi_uart_setBaudrate(&jy901_uart, SUB_NAVI_CONFIG_JY901_UART_BAUDRATE))
    {
        return -1;
    }

    // wit sdk init
    if (WIT_HAL_OK != WitInit(WIT_PROTOCOL_NORMAL, 0x50)) // 0x50 is 7 bit address of jy901
    {
        return -1;
    }
    WitRegisterCallBack(jy901_data_update);
    WitSerialWriteRegister(jy901_uart_write); // register uart write callback

    EV_P = EV_DEFAULT;
    ev_io_init(&uart_watcher, uart_read_cb, jy901_uart.fd, EV_READ);
    ev_io_start(EV_A_ &uart_watcher);
    log_info("jy901 data watcher on uart %s.", SUB_NAVI_CONFIG_JY901_UART_PATH);
    return 0;
}
