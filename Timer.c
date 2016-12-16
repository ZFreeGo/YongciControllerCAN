#define FCY 4e6 
#include "Timer.h"
#include <xc.h>
#include "tydef.h"

uint16 TPRCount = 0;

/********************************************
*函数名：Init_Timer1()
*形参：uint16 ms
*返回值：void
*功能：TIMER1 定时器设置 用于永磁合分闸计时
**********************************************/
//用于永磁合分闸计时
void Init_Timer1( unsigned int  ms)
{
     ClrWdt();
    T1CON = 0;
    IFS0bits.T1IF = 0;
    IPC0bits.T1IP = 4; 

    T1CONbits.TCKPS = 0b10; //1:64
    T1CONbits.TCS = 0;
    T1CONbits.TGATE = 0;

    //INTCON1bits.NSTDIS = 1;
    IFS0bits.T1IF = 0;
    
    TMR1 = 0;
    PR1 = (unsigned int)((float)FCY/1000.00/64.0*(float)ms)-1;
} 
inline void StartTimer1(void)
{
    IEC0bits.T1IE = 1; 
    T1CONbits.TON = 1;
}
inline  void ResetTimer1(void)
{
    T1CON = 0;
    TMR1 = 0; 
    PR1 = 0;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0;
}

/********************************************
*函数名：SetTimer2()
*形参：uint16 ms
*返回值：void
*功能：TIMER2 定时器设置 —— 用于通讯超时检测
**********************************************/
 void SetTimer2(uint16 ms)
{
      ClrWdt();
    IPC1bits.T2IP = 0;//最高的优先级
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 0b11; //1:256
    T2CONbits.TCS = 0; //Fcy = Fosc/4
    T2CONbits.TGATE = 0;

    //INTCON1bits.NSTDIS = 1;// 允许嵌套
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 0;
    TMR2 = 0;
    //PR2 = (uint16)((float)ms * MS_COUNT - 1);
    PR2 = (unsigned int)((float)FCY/1000.00/256.0*(float)ms)-1;
    TPRCount = PR2;
    T2CONbits.TON = 0;

}
inline void StartTimer2(void)
{
    IFS0bits.T2IF = 0;
    TMR2 = 0;
    PR2 =  TPRCount;
    T2CONbits.TON = 1;
}
inline void StopTimer2(void)
{
    T2CONbits.TON = 0;
    IFS0bits.T2IF = 0;
}