/* 
 * File:   Usart.h
 * Author: LiDehai
 *
 * Created on 2014年12月3日, 上午7:22
 */

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif


#define TX   LATFbits.LATF5
#define TX_T TRISFbits.TRISF5
#define RX   PORTFbits.RF4
#define RX_T TRISFbits.TRISF4

#define RX_TX_MODE  LATFbits.LATF6  //0-接收模式， 1-发送模式
#define RX_TX_MODE_T TRISFbits.TRISF6
#define RX_MODE 0
#define TX_MODE 1
    
#define ON_UART_INT()  {IEC1bits.U2RXIE = 1;	} //应该配对使用
#define OFF_UART_INT()  {IEC1bits.U2RXIE = 0;	}    
    
void UsartInit();
void UsartSend(unsigned char abyte);
void UsartRecive(unsigned char abyte);



#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

