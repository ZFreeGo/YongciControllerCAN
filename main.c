/***********************************************
*Copyright(c) 2015,FreeGo
*保留所有权利
*文件名称:Main.c
*文件标识:
 * 大改日期：2015/7/13
*创建日期： 2015年4月23日 
*摘要:
 * 2016/12/15:更换调试器后调试，调试驱动可用
 * 2016/12/3:将程序修改为适应新硬件版本，修改相当一部分端口
 * 2015/11/29:屏蔽通讯无效果。改为11ms软件检测.
 * 2015/10/30:添加写EEPROM超时复位，超过20ms停止等待跳出。
 * 
 * 2015/10/25：取消CO时候的电压检测
 *      
 * 2015/10/22:添加合分闸锁，只有在执行命令后同时上锁，才能执行动作。
 *            添加EEPROM计数，并设置下载程序保留EEPROM
 * 2015/10/21：测试版本，测试合分闸时序.
 * 2015/10/19:注意第二次做板晶振由16M改为4M，晶振模式为XT，4倍频。
 *             合闸时间50ms，分闸时间30ms
 * 2015/10/15: 进一步优化接收程序，试图消灭接收机问题。
 * 2015/10/14:
 *  调试完毕ADC转换 
 *  修正闭锁功能。
 * 2015/10/13:减少非按钮等待时间，由20ms改为2ms。取消对命令重复执行结构.
 *             添加防跳与闭锁功能.——有待测试.
 * 2015/8/12：添加抬起检测，本地控制.
 * 2015/8/10:完善远本转换。
 * 2015/8/6:添加合闸分闸闭锁功能，一次按钮动作只能进行一次操作。
 * 2015/8/3:添加看门狗1:1 1：1 2ms以内喂狗
 * 2015/8/2:发现通讯不能正常接收，经跟踪发现系循环延时所致，遂更改循环优先级。
 * 2015/7/3: 改为F4011 ,外部晶振16M，主频4M
 * 
 * 2015/7/2:完善生成帧的CRC校验。增大发送延时.
 * 2015/7/1:发现上位机不能修改分闸时间需要进一步确认问题所在.
 * 2015/6/17: 添加动作中断控制。合分闸时关闭通讯与外部中断。
 * 添加等待状态。等待中断接收分合闸指令。
*2015/4/27:改FOSC为PLL16，M = FOSC/4 = 16M
*2015/4/23:开始编码；基于外部晶振4M，PLL4
*当前版本:1.0
*作者: FreeGo
*取代版本:
*作者:
*完成时间:
************************************************************/
#include "Header.h"


// DSPIC30F4011Configuration Bit Settings

// 'C' source line config statements
// FOSC
#pragma config FPR = XT_PLL4               //HS->XT_PLL4        // Primary Oscillator Mode (HS)
#pragma config FOS = PRI              // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_ON   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)



// FWDT
#pragma config FWPSB = WDTPSB_1         // WDT Prescaler B (1:1)
#pragma config FWPSA = WDTPSA_1         // WDT Prescaler A (1:1)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Enabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)




frameRtu sendFrame, recvFrame;
uint16 data = 0;
int main()
{
    __delay_ms(100);
    uint16 cn = 0;
   
    
    InitDeviceIO(); //IO初始化 首先禁止中断
     AdcInit(); //ADC采样初始化
 

    ClrWdt(); 
    UsartInit(); //串口初始化 9600bps 785cycs 上
 
    ClrWdt(); 
    
    RX_TX_MODE = TX_MODE; //串口发送
    //while(1)
    //{
       ClrWdt(); 
       // __delay_us(50);
        UsartSend(0x55);
        // UsartSend(0xAA);
         //UsartSend(0xBB);
   // }
    
    RX_TX_MODE = RX_MODE;
    ClrWdt(); //204cys 

    

   // SetTimer2(100);//用于超时检测
    ClrWdt(); //452cycs
    ReciveFrameDataInit(); //接收帧初始化
    sendFrame.address =  LOCAL_ADDRESS; //本机接收地址处理
    ClrWdt(); //21cys

    YongciFirstInit();
    ClrWdt(); //33cys
    //低功耗配置
   // PMD1 = 0x079F;
    //PMD2 = 0xFFFF;
    cn = 0;
    //延时3s判断启动
    while(cn++ <3000)
    {
        __delay_ms(1);
        ClrWdt();
    }
    

  
    while(0xFFFF)
    {
        YongciMainTask();
    }
       
   

    
}

  