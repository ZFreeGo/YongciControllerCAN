/***********************************************
*Copyright(c) 2016,FreeGo
*保留所有权利
*文件名称:Main.c
*文件标识:
*创建日期： 2016年12月23日 
*摘要:

*当前版本:1.0
*作者: FreeGo
*取代版本:
*作者:
*完成时间:
************************************************************/
#include "xc.h"
#include "../Driver/AdcSample.h"
#include "DeviceParameter.h"
/********************************************
*函数名：  GetCapVoltage()
*形参：void
*返回值：uint16--电容电压ADC值
*功能：软件启动转换，获取ADC值.
**********************************************/
uint16 GetCapVoltage(void)
{
    return  SoftSampleOnce();
    
}

/********************************************
*函数名：  GetCapVolatageState()
*形参：void
*返回值：uint16 --电压状态，大于最小值为0xAAAA
*功能：获取电压状态.
**********************************************/
uint16 GetCapVolatageState(void)
{
    uint16 currentValue = 0;
    currentValue =  GetCapVoltage();
    if ( currentValue  >= LOW_VOLTAGE_ADC)
    {
        return 0xAAAA;
    }
    else
    {
        return 0;
    }
}


