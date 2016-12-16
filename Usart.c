#include "usart.h"
#include <xc.h>
#include "RtuFrame.h"
#include  "Header.h"

extern boolean volatile ReciveErrorFlag;

/**************************************************
 *函数名： InitUART2()
 *功能:初始化UART2
 *形参：  unsigned int baud 波特率
 *返回值：void
****************************************************/
void InitUART2(unsigned int baud)
{
    float value = 0;
    // This is an EXAMPLE, so brutal typing goes into explaining all bit sets

    // The HPC16 board has a DB9 connector wired to UART2, so we will
    // be configuring this port only

    // configure U2MODE
    U2MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
    //U2MODEbits.notimplemented;	// Bit14
    U2MODEbits.USIDL = 0;	// Bit13 Continue in Idle
    U2MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
    U2MODEbits.LPBACK = 0;	// Bit6 No Loop Back
    U2MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
    U2MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
    U2MODEbits.STSEL = 0;	// Bit0 One Stop Bit

    // Load a value into Baud Rate Generator.  Example is for 9600.
    // See section 19.3.1 of datasheet.
    //  U2BRG = (Fcy/(16*BaudRate))-1
    //  U2BRG = (20M/(16*9600))-1
    //  U2BRG = 129
    value = (float)FCY /(float)(16*baud) - 1; //波特率 = FCY/(16 * (BRG + 1))
    U2BRG = 25;	//25-9600
    
    // Load all values in for U1STA SFR
    //U2STAbits.notimplemented = 0;	//Bit12
    U2STAbits.UTXBRK = 0;	//Bit11 Disabled
    U2STAbits.UTXEN = 0;	//Bit10 TX pins controlled by periph
    U2STAbits.UTXBF = 0;	//Bit9 *Read Only Bit*
    U2STAbits.TRMT = 0;	//Bit8 *Read Only bit*
    U2STAbits.URXISEL = 0;	//Bits6,7 Int. on character recieved
    U2STAbits.ADDEN = 0;	//Bit5 Address Detect Disabled
    U2STAbits.RIDLE = 0;	//Bit4 *Read Only Bit*
    U2STAbits.PERR = 0;		//Bit3 *Read Only Bit*
    U2STAbits.FERR = 0;		//Bit2 *Read Only Bit*
    U2STAbits.OERR = 0;		//Bit1 *Read Only Bit*
    U2STAbits.URXDA = 0;	//Bit0 *Read Only Bit*

    

    IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
    IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
    IEC1bits.U2RXIE = 1;	// 

    U2MODEbits.UARTEN = 1;	// And turn the peripheral on
    U2STAbits.UTXEN = 1;
    // I think I have the thing working now.
}

/********************************************
*函数名：InitPortsUART2()
*形参：void
*返回值：void
*功能：配置串口端口，尤其相对应的485使能发送端口
**********************************************/
void InitPortsUART2()
{ 
    RX_T = 1;
    TX_T = 0;
    RX_TX_MODE_T = 0;
    RX_TX_MODE = RX_MODE ; //默认为接收
}

void UsartInit(void)
{
     InitPortsUART2();
     InitUART2(9600);
    
}
void UsartSend(unsigned char abyte)
{
   // RX_TX_MODE = TX_MODE;--鉴于光耦响应时间，须有一定的延时
    U2TXREG = abyte;
    while(!U2STAbits.TRMT)
    {
      //  ClrWdt(); //2ms超时后,看门狗复位
    }
    
  //  RX_TX_MODE = RX_MODE;
}
void UsartRecive(unsigned char abyte)
{
}

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
    
    ClrWdt();
    IFS1bits.U2RXIF = 0;
    ReciveErrorFlag = FrameQueneIn(U2RXREG);
    
}
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void)
{
	IFS1bits.U2TXIF = 0;
}