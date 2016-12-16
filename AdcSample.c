/***********************************************
*Copyright(c) 2015,FreeGo
*保留所有权利
*文件名称:AdcSample.c
*文件标识:
*创建日期： 2015年4月23日
*摘要:

*2015/4/23:开始编码;此文件为配置ADC，以便测量电容电压
*当前版本:1.0
*作者: FreeGo
*取代版本:
*作者:
*完成时间:
************************************************************/
#include "Header.h"
#include "AdcSample.h"


/********************************************
*函数名：AdcInit()
*形参：void
*返回值：void
*功能：初始化AN0 为软件触发采样
**********************************************/
void AdcInit(void)
{
    ADCON1bits.ADON = 0;//关闭ADC
    
    ADCON1bits.FORM   = 0;     //整数

    ADCON1bits.SSRC   = 0;		// 清零 SAMP 位时结束采样并启动转换
 
    ADCON1bits.ASAM   = 0;		// ADC Sample Control: Sampling begins immediately after conversion
   
    
    
    ADCON2bits.CSCNA = 0;		// Scan Input Selections for CH0+ during Sample A bit

    ADCON2bits.SMPI    = (NUM_CHS2SCAN-1);	// 1 ADC Channel is scanned

                                                            // ADC Conversion Time for 10-bit Tc=12*Tab = 19.2us
    ADCON2bits.VCFG = 0; //参考电源

    ADCON3bits.ADRC = 0;		// ADC Clock is derived from Systems Clock
    ADCON3bits.ADCS = 3;		// ADC Conversion Clock Tad=Tcy/2*(ADCS+1)= (1/4M/2)*12 = 
                                                            // ADC Conversion Time for 10-bit Tc=12*Tab = 19.2us
    ADCHSbits.CH0SA = 0;//AN0为同相输入
    
    ADCSSL = 0x0000;
    ADCSSLbits.CSSL0 =1;			// Enable AN6 for channel scan
   
    ADPCFG=0xFFFF;
   
    ADPCFGbits.PCFG0 = 0;		// AN0 as Analog Input
    
    TRISBbits.TRISB0 = 1; //AN0

    
    IFS0bits.ADIF = 0;			// Clear the A/D interrupt flag bit
    IEC0bits.ADIE = 0;			// Disable A/D interrupt
    ADCON1bits.ADON = 0;		// Turn on the A/D converter
    INTCON1bits.NSTDIS = 1; //不允许中断嵌套
   

}
/********************************************
*函数名： SoftSampleOnce()
*形参：void
*返回值：uint16
*功能：软件启动转换，获取ADC值.
**********************************************/
uint16  SoftSampleOnce(void)
{
    uint16 adcdata =0;
    ADCON1bits.ADON = 1; //启动转换
    ADCON1bits.SAMP = 1;
    ClrWdt(); 
    __delay_us(20);
    ADCON1bits.SAMP = 0;
     ClrWdt(); 
     //2ms 若不能完成，说明转换异常，触发看门狗复位
    while(!ADCON1bits.DONE)
    {
        //ClrWdt(); 
    }
    ClrWdt(); 
    adcdata =  ADCBUF0;
    ADCON1bits.ADON = 0;
    return adcdata;
}

