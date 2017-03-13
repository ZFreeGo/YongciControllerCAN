#include "DevicdIO.h"
#include "../Header.h"

//输出状态LED1-LED6 Relay1 Relay7
uint8 OutState = 0;


/********************************************
*函数名： InitDeviceIO()
*形参：void
*返回值：void
*功能：初始化基本的IO端口
**********************************************/
void InitDeviceIO(void)
{
//    uint8 i = 0;
    ADPCFG = 0xFFFF; //模拟端口全部作为数字端口
    ClrWdt();
    IP595_DS_DIR = 0;
    IP595_SHCP_DIR = 0;
    IP595_STCP_DIR = 0;
   
    UpdateIndicateState(ON_RUN_LED, TURN_ON);
    
//    OutState = 0x01;
//    while(i++<30)
//    {
//        IP595_Out(OutState);
//        OutState = OutState << 1;
//        if (OutState == 0)
//        {
//             OutState = 0x04;
//        }
//        __delay_ms(100);
//    }
    //电流同步输出引脚
    OC1_T_DIR = 0;
    OC2_T_DIR = 0;
    OC1_T = 0;
    OC2_T = 0;
    //光纤/光耦同步控制输入
    SYNA_IN_DIR = 1;
    SYNB_IN_DIR = 1;

    //IGBT引脚
    //A
    DRIVER_A_DIR = 0;
    DRIVER_B_DIR = 0;
    DRIVER_C_DIR = 0;
    DRIVER_D_DIR = 0;
    RESET_CURRENT_A();


    
    
    //合分位状态监测
    HE_INPUT_DIR = 1;
    FEN_INPUT_DIR = 1;
    HE_WEI_INPUT_DIR = 1;
    FEN_WEI_INPUT_DIR= 1;
    YUAN_BEN_INPUT_DIR = 1;
    LOCK_INPUT_DIR = 1;
    HE_YUAN_INPUT_DIR = 1;
    FEN_YUAN_INPUT_DIR = 1;
    RESET_CURRENT_A();

     

   // InitInt0();//初始化外围中断

    
}

///********************************************
//*函数名： GetKeyValue()
//*形参：void
//*返回值：uint8--按键状态
//*功能：获取按键状态
//**********************************************/
//uint8 GetKeyValue(void)
//{
//    uint8 value = 0;
//    value = PORTD & 0x003F;
//    if (value != 0x003F)
//    {
//        Delay20ms(); //延时消除抖动
//        if(value == ((PORTD) & (0x003F)))
//        {
//            while(value == ((PORTD) & (0x003F)))
//            {//抬起检测
//                ClrWdt();
//            }
//            return value;
//        }
//    }
//    return 0;
//   
//}
/********************************************
*函数名：  Delay20ms()
*形参：void
*返回值：void
*功能：中间插入喂狗指令的延时函数
**********************************************/
void Delay20ms(void)
{
    uint8 i = 0;
    for ( i = 0;i < 20; i++)
    {
        ClrWdt();
         __delay_ms(1); //延时消除抖动
    };
}



void InitInt0()
{
    INTCON2bits.INT0EP = 1;//下降沿中断
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 0; //首先禁止中断
    //IPC0bits.INT0IP
    TRISBbits.TRISB7 = 1; //IN0 RB7
}

inline void TurnOnInt0()
{
     IFS0bits.INT0IF = 0;
     IEC0bits.INT0IE = 1;
}
inline void TurnOffInt0()
{
     IFS0bits.INT0IF = 0;
     IEC0bits.INT0IE = 0;
}


/**************************************************
 *函数名： IP595_Out()
 *功能：  IP595输出
 *形参：8bit输出状态
 *返回值：void
****************************************************/
void IP595_Out(uint8 state)
{
    uint8 i = 0;
    IP595_STCP = 0;
    for (i = 0; i < 8; i++)
    {
        if ((state & 0x80) == 0x80 )
        {
             IP595_DS = 1;
        }
        else
        {
            IP595_DS = 0;
        }
        //产生上升沿
         IP595_SHCP = 0;
         __delay_us(1);
         IP595_SHCP = 1;
          __delay_us(1);
        state = state << 1;
          
    }
    IP595_STCP = 0;
     __delay_us(1);
    IP595_STCP = 1;
     __delay_us(1);
}

/**************************************************
 *函数名： UpdateIndicateState()
 *功能：  更新LED状态,更新继电器状态
 *形参：uint8 port --从LED,Relay宏定义中选择，  uint8 state--开启/关闭 TURN_ON/TURN_OFF
 *TURN_ON 与ON组合为，ON之间可以用位或连接
 *TURN_OFF 与OFF组合为，OFF之间可以位与连接
 *返回值：void
****************************************************/
void UpdateIndicateState(uint8 port, uint8 state)
{
    //开启状态
    if (state == TURN_ON)
    {
        OutState |= port;
        IP595_Out( OutState);
    }
     if (state == TURN_OFF)
    {
        OutState &= port;
        IP595_Out( OutState);
    }
}